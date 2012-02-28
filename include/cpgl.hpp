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

#ifndef CPGL_HPP_
#define CPGL_HPP_

#include <GL/glut.h>
#include <GL/gl.h>
#include "yaml-cpp/yaml.h"
#include "types.hpp"

namespace CPGL {
    namespace core {
        void set_window_title(const int wn, const std::string name);
        factory_t get_factory(const std::string t);
    }

    void init(int& argc, char* argv[], const YAML::Node& c, core::window_handle_callback_t wincb = NULL);

    void wait();

    core::window_t a_whole_new_world(const YAML::Node& c);
}

#include "BaseElement.hpp"
#include "tools.hpp"

#endif
