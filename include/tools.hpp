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

#ifndef CPGL_TOOLS_HPP_
#define CPGL_TOOLS_HPP_

#include <GL/gl.h>
#include "types.hpp"
extern "C" {
    // This is the best library in the world
    #include "LoadTGA2.h"
    #ifdef bool
        #undef bool
    #endif
    #ifdef true
        #undef true
    #endif
    #ifdef false
        #undef false
    #endif
}
namespace CPGL {
    namespace tools {
        GLuint load_shaders(const std::string module,const std::string vs, const std::string fs);

        Model* load_model(
            const std::string module,
            const std::string name,
            const GLuint program,
            const std::string vertexVariableName,
            const std::string normalVariableName,
            const std::string texCoordVariableName);

        GLuint load_texture(const std::string module, const std::string texture);
        TextureData load_texture_struct(const std::string module, const std::string texture);
        void generate_mipmaps(GLuint tex);
        void print_error(const std::string);
    }
}

#endif
