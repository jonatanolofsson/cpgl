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

#ifndef CPGL_WINDOW_HPP_
#define CPGL_WINDOW_HPP_

#include "glut.hpp"
#include "yaml-cpp/yaml.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "BaseElement.hpp"

namespace CPGL {
    namespace core {
        class Window;
        typedef boost::shared_ptr<Window> window_t;
        class Window : public boost::enable_shared_from_this<Window>, boost::noncopyable, public BaseElement {
            public:
                int window_id;

                Window(const int id, const YAML::Node c) : BaseElement(c), window_id(id) {
                    set_projection(
                        c["near"].as<float>(1.0),
                        c["far"].as<float>(80.0),
                        c["right"].as<float>(0.5),
                        c["left"].as<float>(-0.5),
                        c["top"].as<float>(0.5),
                        c["bottom"].as<float>(-0.5)
                    );
                    BaseElement::id = "Viewframe";

                    glClearColor(0.2,0.2,0.5,0);
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_TEXTURE_2D);
                }

                //~ ~Window() {
                    //~ glut::DestroyWindow(window_id);
                //~ }


                typedef std::map<std::string, BaseElement*> element_map;
                element_map elements;
                BaseElement* get(std::string id) {
                    if(elements.find(id) != elements.end()) {
                        return elements[id];
                    }
                    return (NULL);
                }

                void register_element(std::string id, BaseElement* ptr) {
                    assert(elements[id] == NULL); // No previous element with same id
                    elements[id] = ptr;
                }

                void unregister_element(std::string id) {
                    elements.erase(id);
                }

                void set_window_name(const std::string name) {
                    config["window_name"] = name;
                    if(window_id) glut::set_window_title(window_id, name);
                }

                virtual bool reshape(int, int) {glut::redisplay();return false;}
                virtual bool mouse(int, int, int, int) {return false;}
                virtual bool motion(int, int) {return false;}
                virtual bool passivemotion(int,int) {return false;}
                virtual bool keyboard(unsigned char, int, int) {return false;}
                void draw(){};
        };
    }
}

#endif
