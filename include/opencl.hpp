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
#include <GL/gl.h>
#include <cstring>
#include <vector>
#include <iostream>

namespace CPGL {
    namespace CL {
        // Helper function to get error string from a code
        const std::string error_string(cl_int error) {
            static const std::vector<std::string> error_strings = {
                {"CL_SUCCESS"},
                {"CL_DEVICE_NOT_FOUND"},
                {"CL_DEVICE_NOT_AVAILABLE"},
                {"CL_COMPILER_NOT_AVAILABLE"},
                {"CL_MEM_OBJECT_ALLOCATION_FAILURE"},
                {"CL_OUT_OF_RESOURCES"},
                {"CL_OUT_OF_HOST_MEMORY"},
                {"CL_PROFILING_INFO_NOT_AVAILABLE"},
                {"CL_MEM_COPY_OVERLAP"},
                {"CL_IMAGE_FORMAT_MISMATCH"},
                {"CL_IMAGE_FORMAT_NOT_SUPPORTED"},
                {"CL_BUILD_PROGRAM_FAILURE"},
                {"CL_MAP_FAILURE"},
                {""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},
                {"CL_INVALID_VALUE"},
                {"CL_INVALID_DEVICE_TYPE"},
                {"CL_INVALID_PLATFORM"},
                {"CL_INVALID_DEVICE"},
                {"CL_INVALID_CONTEXT"},
                {"CL_INVALID_QUEUE_PROPERTIES"},
                {"CL_INVALID_COMMAND_QUEUE"},
                {"CL_INVALID_HOST_PTR"},
                {"CL_INVALID_MEM_OBJECT"},
                {"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"},
                {"CL_INVALID_IMAGE_SIZE"},
                {"CL_INVALID_SAMPLER"},
                {"CL_INVALID_BINARY"},
                {"CL_INVALID_BUILD_OPTIONS"},
                {"CL_INVALID_PROGRAM"},
                {"CL_INVALID_PROGRAM_EXECUTABLE"},
                {"CL_INVALID_Kernel_NAME"},
                {"CL_INVALID_Kernel_DEFINITION"},
                {"CL_INVALID_Kernel"},
                {"CL_INVALID_ARG_INDEX"},
                {"CL_INVALID_ARG_VALUE"},
                {"CL_INVALID_ARG_SIZE"},
                {"CL_INVALID_Kernel_ARGS"},
                {"CL_INVALID_WORK_DIMENSION"},
                {"CL_INVALID_WORK_GROUP_SIZE"},
                {"CL_INVALID_WORK_ITEM_SIZE"},
                {"CL_INVALID_GLOBAL_OFFSET"},
                {"CL_INVALID_EVENT_WAIT_LIST"},
                {"CL_INVALID_EVENT"},
                {"CL_INVALID_OPERATION"},
                {"CL_INVALID_GL_OBJECT"},
                {"CL_INVALID_BUFFER_SIZE"},
                {"CL_INVALID_MIP_LEVEL"},
                {"CL_INVALID_GLOBAL_WORK_SIZE"}
            };
            return error_strings[-error];
        }
        template<int WORK_DIM>
        class Kernel;
        class Host {
            public:
                Host();
                Host(const std::string);
                ~Host();

                void load_program(const std::string relative_path, const char* args = "");
                template<int WORK_DIM>
                Kernel<WORK_DIM>* load_kernel(const std::string kernel_name) {
                    return new Kernel<WORK_DIM>(this, kernel_name);
                }

                void create_context();
                cl_platform_id platform;
                cl_device_id* devices;
                cl_uint numDevices;
                cl_uint deviceUsed;
                cl_context context;
                cl_program program;

                cl_int error_code;

                void buildExecutable(const char*);

                cl_mem bind_gl_vbo(GLuint buffer_id, cl_mem_flags flags = CL_MEM_WRITE_ONLY) {
                    cl_int err;
                    cl_mem m = clCreateFromGLBuffer(context, flags, buffer_id, &err);
                    std::cout << "Binding buffer " << buffer_id << ": " << error_string(err) << std::endl;
                    return m;
                }
        };

        template<int WORK_DIM>
        class Kernel {
            private:
                Host* host;

            public:
                size_t global_work_size[WORK_DIM];
                size_t local_work_size[WORK_DIM];
                size_t globalSize[2];
                cl_int err;
                cl_command_queue command_queue;
                cl_event event;
                cl_kernel kernel;

                void set_work_size(const size_t* global_work_size_, const size_t* local_work_size_) {
                    std::memcpy(global_work_size, global_work_size_, sizeof(global_work_size));
                    std::memcpy(local_work_size, local_work_size_, sizeof(local_work_size));
                }

                Kernel(Host* host, const std::string kernel_name, const size_t* global_work_size_, const size_t* local_work_size_) {
                    set_work_size(global_work_size_, local_work_size_);
                    this->host = host;

                    // Create the command queue we will use to execute OpenCL commands
                    command_queue = clCreateCommandQueue(host->context, host->devices[host->deviceUsed], NULL, &err);
                    std::cout << "clCreateCommandQueue: " << error_string(err) << std::endl;

                    // Initialize our kernel from the program
                        kernel = clCreateKernel(host->program, kernel_name.c_str(), &err);
                    std::cout << "clCreateKernel: " << error_string(err) << std::endl;
                }

                Kernel(Host* host, const std::string kernel_name) {
                    this->host = host;

                    // Create the command queue we will use to execute OpenCL commands
                    command_queue = clCreateCommandQueue(host->context, host->devices[host->deviceUsed], NULL, &err);
                    std::cout << "clCreateCommandQueue: " << error_string(err) << std::endl;

                    // Initialize our kernel from the program
                    kernel = clCreateKernel(host->program, kernel_name.c_str(), &err);
                    std::cout << "clCreateKernel: " << error_string(err) << std::endl;
                }
                ~Kernel() {
                    std::cout << "Releasing OpenCL App Memory" << std::endl;
                    if(kernel) clReleaseKernel(kernel);
                    if(command_queue) clReleaseCommandQueue(command_queue);
                    // Release any and all cl resources
                    std::cout << "Done releasing OpenCL App Memory" << std::endl;
                }

                template<int N, typename T>
                cl_int set_arg(T& data) {
                    //~ std::cout << "Setting kernel argument " << N << std::endl;
                    err = clSetKernelArg(kernel, N, sizeof(T), &data);
                    return err;
                }

                void operator()() { run(); }
                void operator()(const int* global_work_size_, const int* local_work_size_) {
                    std::memcpy(global_work_size, global_work_size_, sizeof(global_work_size));
                    std::memcpy(local_work_size, local_work_size_, sizeof(local_work_size));
                    run();
                }
                void operator()(bool) { run(); finish(); }

                void run() {
                    cl_int err = clEnqueueNDRangeKernel(command_queue, kernel, WORK_DIM, NULL, global_work_size, local_work_size, 0, NULL, &event);
                    if(err) {
                        std::cerr << "Ran the kernel: " << error_string(err) << "\t global work size: " << global_work_size[0] << ", local: " << local_work_size[0] << std::endl;
                    }
                }

                void finish() {
                    clFinish(command_queue);
                }

                void acquire_gl(cl_mem* vbo_cl) {
                    //~ std::cout << "Aqcuire gl" << std::endl;
                    glFinish();
                    //~ std::cout << "gl finished" << std::endl;
                    cl_int err = clEnqueueAcquireGLObjects(command_queue, 1, vbo_cl, 0,0,0);
                    if(err) std::cout << error_string(err) << std::endl;
                    //~ std::cout << "Got gl lock" << std::endl;
                }

                void release_gl(cl_mem* vbo_cl) {
                    clEnqueueReleaseGLObjects(command_queue, 1, vbo_cl, 0,0,0);
                }
        };
    }
}
#endif
