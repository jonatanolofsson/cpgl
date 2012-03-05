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
                Matrix<float, 3, Dynamic> directional_light;
                Matrix<float, 4, Dynamic> directional_light_color;
                Matrix<float, 3, Dynamic> positional_light;
                Matrix<float, 4, Dynamic> positional_light_color;
                Matrix<float, 4, Dynamic> ambient_light_color;
                int window_id;

                Window(const int id, const YAML::Node c);

                int add_positional_light(Vector3f light_pos, Vector4f color, int n = -1);
                int add_directional_light(Vector3f light_dir, Vector4f color, int n = -1);
                int add_ambient_light(Vector4f color, int n = -1);
                void upload_light(GLuint program, const std::string aname, const std::string ddir = "", const std::string dcolor = "", const std::string ppos = "", const std::string pcolor = "");

                void set_window_name(const std::string name);

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
