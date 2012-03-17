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

#ifndef CPGL_OPENCL_HPP_
#define CPGL_OPENCL_HPP_

#if defined __APPLE__ || defined(MACOSX)
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
#endif

namespace CPGL {
    namespace CL {
        class CLHost {
            public:
                CLHost();
                CLHost(std::string);
                ~CLHost();

                void load_program(const std::string relative_path, const char* args = "");
                friend class Kernel;
            private:
                void create_context();
                cl_platform_id platform;
                cl_device_id* devices;
                cl_uint numDevices;
                cl_uint deviceUsed;
                cl_context context;
                cl_program program;

                cl_int error_code;

                void buildExecutable(const char*);
        };

        class Kernel {
            private:
                CLHost *host;
                Kernel();

            public:
                Kernel(CLHost* host, const std::string kernel_name);
                ~Kernel();
                size_t globalSize[2];

                cl_int err;

                cl_command_queue command_queue;
                cl_kernel kernel;
                cl_event event;

                template<int N, typename T>
                cl_int set_arg(T& data) {
                    err = clSetKernelArg(kernel, N, sizeof(T), &data);
                    return err;
                }

                void wait();
                void run();
        };
    }
}
#endif
