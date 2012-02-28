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

#ifndef CPGL_GLUT_HPP_
#define CPGL_GLUT_HPP_
#include <GL/glut.h>
#include "types.hpp"
#include "yaml-cpp/yaml.h"

namespace CPGL {
    namespace glut {
        /**
         * Start a new thread with glut
         */
        void init(int& argc, char* argv[], void(*)(core::window_handle_callback_t), core::window_handle_callback_t wincb = NULL);

        /**
         * Register a new figure to be created and drawn
         */
        core::window_t create_window(const YAML::Node&);

        /**
         * Set the title of a glut window
         */
        void set_window_title(const int wn, const std::string name);

        void redisplay();

        void wait();
    }
}

#endif
