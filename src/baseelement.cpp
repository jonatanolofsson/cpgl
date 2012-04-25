/**
 * Copyright 2011, 2012 Jonatan Olofsson
 *
 * This file is part of C++ GL Framework (CPGL).
 *
 * CPGL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CPGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CPGL.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BaseElement.hpp"


namespace CPGL {
    namespace core {
        bool invalidate_cache = false;
        using namespace Eigen;
        void BaseElement::register_child(const YAML::Node c) {
            children.push_back(get_factory(c["type"].as<std::string>())(c, this));
        }

        void BaseElement::register_children(const YAML::Node& c) {
            for(YAML::const_iterator it = c.begin(); it != c.end(); ++it) {
                register_child(*it);
            }
        }

        BaseElement::BaseElement(const YAML::Node c, BaseElement* p) : base_cached(false), config(c), parent(p) {
            base.setIdentity();
            if(config["id"]) {
                id = config["id"].as<std::string>();
                if(parent) {
                    parent->register_element(id, this);
                }
            }
            if(config["children"]) {
                register_children(config["children"]);
            }
        }

        BaseElement::~BaseElement() {
            children.clear();
        }

        BaseElement* BaseElement::get(std::string eid) {
            element_map::iterator it = elements.find(eid);
            if(it != elements.end()) {
                return it->second;
            } else if(parent != NULL) {
                return parent->get(eid);
            }
            return (NULL);
        }

        void BaseElement::register_element(std::string eid, BaseElement* ptr) {
            assert(elements.find(eid) == elements.end()); // No previous element with same id
            elements.insert(element_map::value_type(eid, ptr));
            if(parent) parent->register_element(eid, ptr);
        }

        void BaseElement::unregister_element(std::string eid) {
            elements.erase(eid);
            if(parent) parent->unregister_element(eid);
        }

        void BaseElement::look_at(const Vector3f pos, const Vector3f lookAt, const Vector3f up) {
            Vector3f n = (pos-lookAt).normalized();
            Vector3f u = (up.cross(n)).normalized();
            Vector3f v = n.cross(u);
            base.linear().row(0) = u;
            base.linear().row(1) = v;
            base.linear().row(2) = n;
            base.matrix().row(3) << 0,0,0,1;
            base.translation() = -base.linear()*pos;
        }

        void BaseElement::set_projection(
            const float near,
            const float far,
            const float right,
            const float left,
            const float top,
            const float bottom)
        {
            base.matrix() << 2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                0.0f, 0.0f, -1.0f, 0.0f;
        }

        Transform<float, 3, Projective> BaseElement::get_full_base() {
            return get_projection_matrix() * get_base();
        }

        Transform<float, 3, Projective> BaseElement::get_base() {
            if(base_cached && !invalidate_cache) return base_cache;
            base_cache = (parent->parent == NULL) ? base : parent->get_base() * base;
            return base_cache;
        }

        float* BaseElement::get_projection() {
            return get_projection_matrix().data();
        }

        Transform<float, 3, Projective>& BaseElement::get_projection_matrix() {
            return (parent == NULL) ? base : parent->get_projection_matrix();
        }

        void BaseElement::DRAW() {
            if(!parent) invalidate_cache = false;
            draw();
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                (*it)->DRAW();
            }
            if(!parent) invalidate_cache = true;
        }

        bool BaseElement::RESHAPE(int w, int h) {
            if(reshape(w,h)) return true;
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                if((*it)->RESHAPE(w,h)) return true;
            }
            return false;
        }
        bool BaseElement::MOUSE(int btn, int state, int x, int y) {
            if(mouse(btn,state,x,y)) return true;
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                if((*it)->MOUSE(btn,state,x,y)) return true;
            }
            return false;
        }
        bool BaseElement::MOTION(int x, int y) {
            if(motion(x,y)) return true;
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                if((*it)->MOTION(x,y)) return true;
            }
            return false;
        }
        bool BaseElement::PASSIVEMOTION(int x,int y) {
            if(passivemotion(x,y)) return true;
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                if((*it)->PASSIVEMOTION(x,y)) return true;
            }
            return false;
        }
        bool BaseElement::KEYBOARD(unsigned char key, int x, int y) {
            if(keyboard(key,x,y)) return true;
            for(basemap::iterator it = children.begin(); it != children.end(); ++it) {
                if((*it)->KEYBOARD(key,x,y)) return true;
            }
            return false;
        }
    }
}
