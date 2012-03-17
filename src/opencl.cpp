#include <iostream>
#include <string>
#include <cstdlib>
#include <CL/opencl.h>
#include "opencl.hpp"
#include <vector>
#include <fstream>

namespace CPGL {
    namespace CL {
        void read_file(const std::string& file, std::string& out) {
            std::ifstream t(file);

            t.seekg(0, std::ios::end);
            out.reserve(t.tellg());
            t.seekg(0, std::ios::beg);

            out.assign((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        }
        // Helper function to get error string from a code
        const std::string oclErrorString(cl_int error) {
            static const std::vector<std::string> errorStrings = {
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
                {"CL_INVALID_KERNEL_NAME"},
                {"CL_INVALID_KERNEL_DEFINITION"},
                {"CL_INVALID_KERNEL"},
                {"CL_INVALID_ARG_INDEX"},
                {"CL_INVALID_ARG_VALUE"},
                {"CL_INVALID_ARG_SIZE"},
                {"CL_INVALID_KERNEL_ARGS"},
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
            return errorStrings[-error];
        }

        void CLHost::create_context() {
            std::cout << "Initializing OpenCL Context..." << std::endl;

            // Get the OpenCL Platforms available
            error_code = clGetPlatformIDs(1, &platform, NULL);
            std::cout << "clGetPlatformIDs: " << oclErrorString(error_code) << std::endl;

            // Get the number of available OpenCL Devices, CPUs as well as GPUs
            error_code = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
            std::cout << "clGetDeviceIDs (get number of devices): " << oclErrorString(error_code)
                << " (answer: " << numDevices << ")" << std::endl;

            // Create the device list
            devices = new cl_device_id[numDevices];
            error_code = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
            std::cout << "clGetDeviceIDs (create device list): " << oclErrorString(error_code) << std::endl;

            // Create the context with the devices
            context = clCreateContext(0, numDevices, devices, NULL, NULL, &error_code);
            std::cout << "clCreateContext: " <<  oclErrorString(error_code) << std::endl;

            // Choose the first device
            deviceUsed = 0;
        }

        CLHost::CLHost() {
            create_context();
        }
        CLHost::CLHost(std::string path) {
            create_context();
            load_program(path);
        }

        CLHost::~CLHost()
        {
            std::cout << "Releasing OpenCL Host memory\n" << std::endl;
            if(program) clReleaseProgram(program);
            if(context) clReleaseContext(context);
            if(devices) delete(devices);
            printf("OpenCL Host memory released\n");
        }

        void CLHost::load_program(const std::string path, const char* args) {
            std::string cSourceCL;

            std::cout << "Loading the program from file: " << path << std::endl;

            read_file(path, cSourceCL);
            std::cout << "== LOADED FOLLOWING PROGRAM ==" << std::endl
                << cSourceCL << std::endl
                << "== END OF PROGRAM ==" << std::endl;

            // Create the program
            size_t program_length = cSourceCL.length();
            program = clCreateProgramWithSource(context, 1,
                              (const char **) &cSourceCL, &program_length, &error_code);
            std::cout << "clCreateProgramWithSource: " << oclErrorString(error_code) << std::endl;

            buildExecutable(args);
        }

        void CLHost::buildExecutable(const char* ARGS)
        {
            // Build the program executable

            std::cout << "Building the program..." << std::endl;
            error_code = clBuildProgram(program, 0, NULL, ARGS, NULL, NULL);
            std::cout << "clBuildProgram: " << oclErrorString(error_code) << std::endl;

            cl_build_status build_status;
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &build_status, NULL);

            size_t ret_val_size;
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
            std::cout << "clGetProgramBuildInfo: " << oclErrorString(error_code) << std::endl;

            // Allocate memory for the Build Log and, print it and free it.
            char *build_log = (char*) malloc(sizeof(char)*ret_val_size +1);
            error_code = clGetProgramBuildInfo(program, devices[deviceUsed], CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);
            std::cout << "clGetProgramBuildInfo: " << oclErrorString(error_code) << std::endl;
            build_log[ret_val_size] = '\0';
            std::cout << "== BUILD LOG ==" << std::endl
                << build_log << std::endl
                << "== END OF LOG ==" << std::endl;
            free(build_log);
            std::cout << "...Done building." << std::endl;
        }






        Kernel::Kernel(CLHost* host, const std::string kernel_name) {
            this->host = host;

            // Create the command queue we will use to execute OpenCL commands
            command_queue = clCreateCommandQueue(host->context, host->devices[host->deviceUsed], CL_QUEUE_PROFILING_ENABLE, &err);

            // Initialize our kernel from the program
            kernel = clCreateKernel(host->program, kernel_name.c_str(), &err);
            std::cout << "clCreateKernel: " << oclErrorString(err) << std::endl;
        }

        Kernel::~Kernel() {
            std::cout << "Releasing OpenCL App Memory" << std::endl;
            if(kernel) clReleaseKernel(kernel);
            if(command_queue) clReleaseCommandQueue(command_queue);
            // Release any and all cl resources
            std::cout << "Done releasing OpenCL App Memory" << std::endl;
        }

        void Kernel::run() {
            std::cout << "Running the kernel..." << std::endl;

            // The size of the workgroups.
            size_t localSize[2] = {128, 0};
            clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, globalSize, localSize, 0, NULL, &event);
        }

        void Kernel::wait() {
            clFinish(command_queue);
        }
    }
}
