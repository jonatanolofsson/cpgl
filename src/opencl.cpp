#include <iostream>
#include <string>
#include <cstdlib>
#include <CL/opencl.h>
#include "opencl.hpp"
#include <vector>
#include <fstream>
#include "yaml-cpp/yaml.h"


#include "GL/glx.h"

namespace CPGL {
    extern YAML::Node config;
    namespace CL {
        void read_file(const std::string& file, std::string& out) {
            std::ifstream t(file);

            t.seekg(0, std::ios::end);
            out.reserve(t.tellg());
            t.seekg(0, std::ios::beg);

            out.assign((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        }

        void Host::create_context() { //cl_context_properties props* = 0) {
            std::cout << "Initializing OpenCL Context..." << std::endl;

            // Get the OpenCL Platforms available
            error_code = clGetPlatformIDs(1, &platform, NULL);
            std::cout << "clGetPlatformIDs: " << error_string(error_code) << std::endl;

            // Get the number of available OpenCL Devices, CPUs as well as GPUs
            error_code = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
            std::cout << "clGetDeviceIDs (get number of devices): " << error_string(error_code)
                << " (answer: " << numDevices << ")" << std::endl;

            // Create the device list
            devices = new cl_device_id[numDevices];
            error_code = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
            std::cout << "clGetDeviceIDs (create device list): " << error_string(error_code) << std::endl;

            // Create the context with the devices

    cl_context_properties props[] =
    {
        CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
        0
    };


            context = clCreateContext(props, numDevices, devices, NULL, NULL, &error_code);
            std::cout << "clCreateContext: " <<  error_string(error_code) << std::endl;

            // Choose the first device
            deviceUsed = 0;
        }

        Host::Host() {
            create_context();
        }
        Host::Host(const std::string path) { //, cl_context_properties props* = 0) {
            create_context();
            load_program(path);
        }

        Host::~Host()
        {
            std::cout << "Releasing OpenCL Host memory\n" << std::endl;
            if(program) clReleaseProgram(program);
            if(context) clReleaseContext(context);
            if(devices) delete(devices);
            printf("OpenCL Host memory released\n");
        }

        void Host::load_program(const std::string t, const char* args) {
            std::string cSourceCL;
            std::string path =
                config["directories"]["root"].as<std::string>("") +
                config["directories"]["element_files"].as<std::string>("") + t + ".cl";

            std::cout << "Loading the program from file: " << path << std::endl;

            read_file(path, cSourceCL);
            std::cout << "== LOADED FOLLOWING PROGRAM ==" << std::endl
                << cSourceCL << std::endl
                << "== END OF PROGRAM ==" << std::endl;

            // Create the program
            size_t program_length = cSourceCL.length();
            program = clCreateProgramWithSource(context, 1,
                              (const char **) &cSourceCL, &program_length, &error_code);
            std::cout << "clCreateProgramWithSource: " << error_string(error_code) << std::endl;

            buildExecutable(args);
        }

        void Host::buildExecutable(const char* ARGS)
        {
            // Build the program executable

            std::cout << "Building the program..." << std::endl;
            error_code = clBuildProgram(program, 0, NULL, ARGS, NULL, NULL);
            std::cout << "clBuildProgram: " << error_string(error_code) << std::endl;

            cl_build_status build_status;
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);

            size_t ret_val_size;
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
            std::cout << "clGetProgramBuildInfo: " << error_string(error_code) << std::endl;

            // Allocate memory for the Build Log and, print it and free it.
            char *build_log = (char*) malloc(sizeof(char)*ret_val_size +1);
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
            std::cout << "clGetProgramBuildInfo: " << error_string(error_code) << std::endl;
            build_log[ret_val_size] = '\0';
            std::cout << "== BUILD LOG ==" << std::endl
                << build_log << std::endl
                << "== END OF LOG ==" << std::endl;
            free(build_log);
            std::cout << "...Done building." << std::endl;
        }
    }
}
