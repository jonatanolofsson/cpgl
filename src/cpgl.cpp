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

#include <GL/glut.h>
#include <map>
#include "glut.hpp"
#include "types.hpp"
#include <dlfcn.h>
#include "yaml-cpp/yaml.h"
#include "Window.hpp"

namespace CPGL {
    using namespace core;
    YAML::Node config;

    core::window_t a_whole_new_world(const YAML::Node& c) {
        return glut::create_window(c);
    }

    void start(window_handle_callback_t wincb) {
        if(config["window"]) {
            window_t w(CPGL::a_whole_new_world(config["window"]));
            if(wincb != NULL) wincb(w);
        }
    }

    void init(int& argc, char* argv[], const YAML::Node& c, window_handle_callback_t wincb) {
        config = c;
        glut::init(argc, argv, start, wincb);
    }

    void wait() {
        glut::wait();
    }

    namespace core {
        typedef std::map<std::string, void*> dlib_map;
        typedef std::map<std::string, factory_t> factory_map;
        dlib_map libraries;
        factory_map factories;

        factory_t get_factory(const std::string t) {
            if(factories[t]) {
                return factories[t];
            }
            else {
                void* dlib;
                std::string module_path =
                    config["directories"]["root"].as<std::string>("") +
                    config["directories"]["element_objects"].as<std::string>("") + t + ".so";
                std::cout << "Adding module: " << module_path << std::endl;
                dlib = dlopen(module_path.c_str(), RTLD_LAZY);
                if(dlib == NULL) {
                    std::cerr << "Failed to link element: " << "(" << dlerror() << ")" << std::endl;
                    return (NULL); //FIXME: Throw exception?
                }

                libraries.insert(dlib_map::value_type(t,dlib));
                void* factory = dlsym(dlib, "factory");
                if(factory == NULL) {
                    std::cerr << "No factory in " << t << ": " << "(" << dlerror() << ")" << std::endl;
                    return (NULL); //FIXME: Throw exception?
                }
                factories.insert(factory_map::value_type(t,(factory_t)factory));
                return (factory_t)factory;
            }
        }
    }
}
