#define CL_HPP_TARGET_OPENCL_VERSION 220
#define CL_HPP_ENABLE_EXCEPTIONS
#define PROGRAM_FILE "blank.cl"
#define KERNEL_FUNC "blank"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>

#include <CL/opencl.hpp>

int main(void) {
   
   cl::vector<cl::Platform> platforms;
   cl::vector<cl::Device> devices;
   int i, j;

   // Data and rectangle geometry
   float fullMatrix[80], zeroMatrix[80];
   cl::array<cl::size_type, 3> bufferOrigin, hostOrigin, region;

   try {
      // Initialize data
      for(i=0; i<80; i++) {
         fullMatrix[i] = i*1.0f;
         zeroMatrix[i] = 0.0f;
      }

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

      // Create matrix buffer and make it the kernel's first argument
      cl::Buffer matrixBuffer(context, 
         CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
         sizeof(zeroMatrix), zeroMatrix);
      kernel.setArg(0, matrixBuffer);

      // Create queue and enqueue kernel-execution command
      cl::CommandQueue queue(context, devices[0]);
      queue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(1), cl::NDRange(1));

      // Update the content of the buffer
      queue.enqueueWriteBuffer(matrixBuffer, CL_TRUE, 0, 
            sizeof(fullMatrix), fullMatrix);

      // Read a rectangle of data from the buffer
      std::cout << sizeof(float) << std::endl;
      bufferOrigin[0] = (5*sizeof(float)); 
      bufferOrigin[1] = 3;
      bufferOrigin[2] = 0;
      hostOrigin[0] = (1*sizeof(float)); 
      hostOrigin[1] = 1;
      hostOrigin[2] = 0;
      region[0] = (4*sizeof(float)); 
      region[1] = 4;
      region[2] = 1;
      queue.enqueueReadBufferRect(matrixBuffer, CL_TRUE, 
         bufferOrigin, hostOrigin, region, 
         10*sizeof(float), 0, 
         10*sizeof(float), 0, zeroMatrix);

      /* Display updated buffer */
      for(i=0; i<8; i++) {
         for(j=0; j<10; j++) {
            printf("%6.1f", zeroMatrix[j+i*10]);
         }
         printf("\n");
      }
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;   
   }

   return 0;
}
