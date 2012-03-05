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

#include "Window.hpp"

namespace CPGL {
    namespace core {
        Window::Window(const int id, const YAML::Node c) : BaseElement(c), window_id(id) {
            set_projection(
                c["near"].as<float>(1.0),
                c["far"].as<float>(80.0),
                c["right"].as<float>(0.5),
                c["left"].as<float>(-0.5),
                c["top"].as<float>(0.5),
                c["bottom"].as<float>(-0.5)
            );

            glClearColor(0.2,0.2,0.5,0);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
        }

        int Window::add_positional_light(Vector3f light_pos, Vector4f color, int n) {
            if(n < 0) {
                n = positional_light.cols();
                positional_light.conservativeResize(3, n+1);
                positional_light_color.conservativeResize(4, n+1);
            }
            positional_light.col(n) = light_pos;
            positional_light_color.col(n) = color;
            return n;
        }

        int Window::add_directional_light(Vector3f light_dir, Vector4f color, int n) {
            if(n < 0) {
                n = directional_light.cols();
                directional_light.conservativeResize(3, n+1);
                directional_light_color.conservativeResize(4, n+1);
            }
            directional_light.col(n) = light_dir;
            directional_light_color.col(n) = color;
            return n;
        }

        int Window::add_ambient_light(Vector4f color, int n) {
            if(n < 0) {
                int n = ambient_light_color.cols();
                ambient_light_color.conservativeResize(4, n+1);
            }
            ambient_light_color.col(n) = color;
            return n;
        }

        void Window::upload_light(GLuint program, const std::string aname, const std::string ddir, const std::string dcolor, const std::string ppos, const std::string pcolor) {
            if(aname != "") {
                glUniform3fv(glGetUniformLocation(program, aname.c_str()), ambient_light_color.cols(), ambient_light_color.data());
            }

            if(ddir != "") {
                glUniform3fv(glGetUniformLocation(program, ddir.c_str()), directional_light.cols(), directional_light.data());
            }
            if(dcolor != "") {
                glUniform4fv(glGetUniformLocation(program, dcolor.c_str()), directional_light_color.cols(), directional_light_color.data());
            }

            if(ppos != "") {
                glUniform3fv(glGetUniformLocation(program, ppos.c_str()), positional_light.cols(), positional_light.data());
            }
            if(pcolor != "") {
                glUniform4fv(glGetUniformLocation(program, pcolor.c_str()), positional_light_color.cols(), positional_light_color.data());
            }
        }

        void Window::set_window_name(const std::string name) {
            config["window_name"] = name;
            if(window_id) glut::set_window_title(window_id, name);
        }
    }
}
