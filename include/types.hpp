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

#ifndef CPGL_TYPES_HPP_
#define CPGL_TYPES_HPP_
#include <GL/gl.h>
#include <boost/shared_ptr.hpp>
#include "yaml-cpp/yaml.h"
extern "C" {
    #include "loadobj.h"
}

namespace CPGL {
    namespace core {
        class BaseElement;
        typedef BaseElement*(*factory_t)(const YAML::Node&, const BaseElement* const);
        class Window;
        typedef boost::shared_ptr<Window> window_t;
        typedef void(*window_handle_callback_t)(window_t);
    }
}

#endif
