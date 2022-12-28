#define CL_HPP_TARGET_OPENCL_VERSION 220
#define CL_HPP_ENABLE_EXCEPTIONS
#define PROGRAM_FILE "blank.cl"
#define KERNEL_FUNC "blank"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>

#ifdef MAC
#include <OpenCL/cl.hpp>
#else
#include <CL/opencl.hpp>
#endif

int main(void) {
   cl::vector<cl::Platform> platforms;
   cl::vector<cl::Device> devices;
   cl::Event profileEvent;
   cl_ulong start, end;
   int data[10];

   try {
      // Place the GPU devices of the first platform into a context
      cl::Platform::get(&platforms);
      platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
      cl::Context context(devices);
      
      // Create kernel
      std::ifstream programFile(PROGRAM_FILE);
      std::string programString(std::istreambuf_iterator<char>(programFile),
            (std::istreambuf_iterator<char>()));
      cl::Program::Sources source(1, programString);
      cl::Program program(context, source);
      program.build(devices);
      cl::Kernel kernel(program, KERNEL_FUNC);

      // Create buffer and make it a kernel argument
      cl::Buffer buffer(context, 
            CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(data), data);
      kernel.setArg(0, buffer);
      
      size_t timer_resolution;
      devices[0].getInfo(CL_DEVICE_PROFILING_TIMER_RESOLUTION, &timer_resolution);
      std::cout << timer_resolution << std::endl;
      
      // Enqueue kernel-execution command with profiling event
      cl::CommandQueue queue(context, devices[0], CL_QUEUE_PROFILING_ENABLE);
      queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(1), cl::NDRange(1), nullptr, &profileEvent);
      queue.finish();

      // Configure event processing
      start = profileEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>();
      end = profileEvent.getProfilingInfo<CL_PROFILING_COMMAND_COMPLETE>();
      std::cout << "Elapsed time: " << (end - start) << " ns." << std::endl;
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;   
   }
   return 0;
}
