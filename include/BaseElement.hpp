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

#ifndef CPGL_BASEELEMENT_HPP_
#define CPGL_BASEELEMENT_HPP_

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <string>
#include <list>
#include "cpgl.hpp"

namespace CPGL {
    namespace core {
        using namespace Eigen;
        class BaseElement;
        typedef std::map<std::string, BaseElement*> element_map;
        typedef std::list<BaseElement*> basemap;
        class BaseElement {
            public:
                Transform<float, 3, Projective> base;
                Transform<float, 3, Projective> base_cache;
                bool base_cached;
                YAML::Node config;

                std::string id;
                basemap children;
                element_map elements;
                BaseElement* parent;

                void register_child(const YAML::Node c);
                void register_children(const YAML::Node& c);

                BaseElement(const YAML::Node c, BaseElement* p = NULL);
                virtual ~BaseElement();

                BaseElement* get(std::string id);
                void register_element(std::string id, BaseElement* ptr);
                void unregister_element(std::string id);

                void look_at(const Vector3f pos, const Vector3f lookAt, const Vector3f up);

                void set_projection(
                    const float near = 1.0,
                    const float far = 80.0,
                    const float right = 0.5,
                    const float left = -0.5,
                    const float top = 0.5,
                    const float bottom = -0.5);

                Transform<float, 3, Projective> get_full_base();
                Transform<float, 3, Projective> get_base();
                float* get_projection();
                Transform<float, 3, Projective>& get_projection_matrix();

                virtual void draw() = 0;

                void DRAW();

                virtual bool reshape(int, int) {return false;}
                virtual bool mouse(int, int, int, int) {return false;}
                virtual bool motion(int, int) {return false;}
                virtual bool passivemotion(int,int) {return false;}
                virtual bool keyboard(unsigned char, int, int) {return false;}

                virtual bool RESHAPE(int w, int h);
                virtual bool MOUSE(int btn, int state, int x, int y);
                virtual bool MOTION(int x, int y);
                virtual bool PASSIVEMOTION(int x,int y);
                virtual bool KEYBOARD(unsigned char key, int x, int y);
        };
    }
}

#endif
