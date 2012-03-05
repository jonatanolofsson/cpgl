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

#include "yaml-cpp/yaml.h"
#include "tools.hpp"
#include "GL_utilities.h"
#include <iostream>

namespace CPGL {
    extern YAML::Node config;
    namespace tools {
        GLuint load_shaders(const std::string module, const std::string vs, const std::string fs) {
            std::string path =
                config["directories"]["root"].as<std::string>("")
                + config["directories"]["element_files"].as<std::string>("")
                + module + "/"
                + config["directories"]["shaders"].as<std::string>("");

            std::cout << "Loading shaders: " <<  path << std::endl;
            return loadShaders(
                (path + vs).c_str(),
                (path + fs).c_str()
            );
        }

        Model* load_model(
            const std::string module,
            const std::string name,
            const GLuint program,
            const std::string vertexVariableName,
            const std::string normalVariableName,
            const std::string texCoordVariableName)
        {
            std::string path =
                config["directories"]["root"].as<std::string>("")
                + config["directories"]["element_files"].as<std::string>("")
                + module + "/"
                + config["directories"]["models"].as<std::string>("")
                + name;
                std::cout << "Loading model: " <<  path << std::endl;

            return LoadModelPlus(
                const_cast<char*>(path.c_str()),
                program,
                const_cast<char*>(vertexVariableName.c_str()),
                const_cast<char*>(normalVariableName.c_str()),
                const_cast<char*>(texCoordVariableName.c_str())
            );
        }

        GLuint load_texture(const std::string module, const std::string texture) {
            GLuint tex;
            std::string path =
                config["directories"]["root"].as<std::string>("")
                + config["directories"]["element_files"].as<std::string>("")
                + module + "/"
                + config["directories"]["textures"].as<std::string>("")
                + texture;
            std::cout << "Loading texture: " <<  path << std::endl;

            LoadTGATextureSimple(const_cast<char*>(path.c_str()), &tex);
            return tex;
        }

        TextureData load_texture_struct(const std::string module, const std::string texture) {
            TextureData tex;
            std::string path =
                config["directories"]["root"].as<std::string>("")
                + config["directories"]["element_files"].as<std::string>("")
                + module + "/"
                + config["directories"]["textures"].as<std::string>("")
                + texture;
            std::cout << "Loading texture struct: " <<  path << std::endl;

            LoadTGATexture(const_cast<char*>(path.c_str()), &tex);
            return tex;
        }

        void generate_mipmaps(GLuint tex) {
            glBindTexture(GL_TEXTURE_2D, tex);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        void print_error(const std::string fn) {
            printError(fn.c_str());
        }
    }
}
