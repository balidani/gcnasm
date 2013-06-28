/*
 * OpenCL kernel binary generator
 *
 * Main module
 *
 * This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
 * and it is hereby released to the general public under the following terms:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * (OpenCL sample code was used to create this application)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

void listDeviceInfo()
{
	cl_uint platform_count;
	cl_uint device_count;

	cl_platform_id *platforms;
	cl_device_id *devices;
	
	size_t platform_info_size;
	size_t device_info_size;
	
	char *platform_info;
	char *device_info;

	uint32_t i, j;
	
	printf("Available platform IDs and devices:\n");

	// Platform count
	clGetPlatformIDs(0, NULL, &platform_count);
	
	// Get platforms
	platforms = (cl_platform_id *) malloc(platform_count * sizeof(cl_platform_id));
	clGetPlatformIDs(platform_count, platforms, NULL);

	for (i = 0; i < platform_count; ++i)
	{
		// Get platform name
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &platform_info_size);
		
		platform_info = (char *) malloc(platform_info_size);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, platform_info_size, platform_info, NULL);

		printf("Platform %d -- %s\n", i, platform_info);

		// Get devices
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);

		devices = (cl_device_id *) malloc(device_count * sizeof(cl_device_id));
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, device_count, devices, NULL);

		for (j = 0; j < device_count; ++j)
		{
			// Get device name
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &device_info_size);

			device_info = (char *) malloc(device_info_size);
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, device_info_size, device_info, NULL);

			printf("\tDevice %d -- %s\n", j, device_info);
		}
	}
}

// Note that error checking is omitted
int main(int argc, char* argv[])
{
	// Parameters
	uint32_t platform_id;
	uint32_t device_id;
	const char *binary_path;
	const char *kernel_path;

	// Platforms, devices
	cl_uint platform_count;
	cl_platform_id *platforms;
	cl_platform_id platform;
	
	cl_uint device_count;
	cl_device_id* devices;

	// OpenCL variables
	cl_context context;
	cl_command_queue command_queue;
	cl_program program;
	cl_mem input_buffer, salt_buffer, output_buffer;
	cl_kernel kernel;

	// Input, output
	const uint32_t data_size = 64;
	uint32_t *input, *salt, *output;
	size_t global_work_size[1];

	// etc.
	FILE* fp;
	int binary_loaded;
	uint32_t i;

	// Check arguments
	if (argc != 5) 
	{
		printf("Usage: %s <platform_id> <device_id> <binary_path> <kernel_path>\n\n", argv[0]);
		listDeviceInfo();

		return EXIT_FAILURE;
	}

	// Setup arguments
	platform_id = atoi(argv[1]);
	device_id = atoi(argv[2]);
	binary_path = argv[3];
	kernel_path = argv[4];

	// Platforms
	clGetPlatformIDs(0, NULL, &platform_count);

	if (platform_id >= platform_count) 
	{
		printf("[*] Error, no such platform\n");
		return EXIT_FAILURE;
	}

	platforms = (cl_platform_id *) malloc(platform_count * sizeof(cl_platform_id));
	clGetPlatformIDs(platform_count, platforms, NULL);
	platform = platforms[platform_id];
	free(platforms);

	// Devices
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &device_count);	
	
	if (device_id >= device_count)
	{
		printf("[*] Error, no such device\n");
		return EXIT_FAILURE;
	}

	devices = (cl_device_id *) malloc(device_count * sizeof(cl_device_id));
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, device_count, devices, NULL);

	// Context
	context = clCreateContext(NULL,1, devices, NULL, NULL, NULL);

	// Command queue
	command_queue = clCreateCommandQueue(context, devices[device_id], 0, NULL);
	
	// Try to load binary
	binary_loaded = 0;
	fp = fopen(binary_path, "rb");

	if (fp == NULL) 
	{
		uint8_t *kernel_source;
		size_t kernel_size[1];

		printf("[*] Loading kernel from source\n");

		fp = fopen(kernel_path, "r");
		fseek(fp, 0, SEEK_END);
		kernel_size[0] = ftell(fp);
		rewind(fp);

		kernel_source = (uint8_t *) calloc(kernel_size[0], sizeof(uint8_t *));
		fread(kernel_source, 1, kernel_size[0], fp);
		fclose(fp);

		program = clCreateProgramWithSource(context, 1, (const char **) &kernel_source, kernel_size,  NULL);
	} 
	else
	{
		uint8_t *binary;
		size_t binary_size;

		printf("[*] Loading kernel from binary\n");

		fseek(fp, 0, SEEK_END);
		binary_size = ftell(fp);
		rewind(fp);

		binary = (uint8_t *) calloc(binary_size, sizeof(uint8_t *));
		fread(binary, 1, binary_size, fp);
		fclose(fp);

		program = clCreateProgramWithBinary(context, 1, &(devices[device_id]), &binary_size, (const unsigned char**) &binary, NULL, NULL);
		binary_loaded = 1;
	}

	// Build program
	clBuildProgram(program, 1, &(devices[device_id]), "-save-temps", NULL, NULL);

	// Save binary
	if (!binary_loaded) 
	{
		uint8_t *binary;
		size_t binary_size;

		printf("Saving kernel binary\n");

		// Binary size
		clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * 1, &binary_size, NULL);

		// Get data
		binary = (uint8_t *) calloc(binary_size, sizeof(uint8_t *));
		clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(uint8_t *) * 1, &binary, NULL);

		// Save data
		fp = fopen(binary_path, "wb");
		fwrite(binary, 1, binary_size, fp);
		fclose(fp);
	}

	// Input, output
	input = (uint32_t *) calloc(data_size, sizeof(uint32_t));
	salt = (uint32_t *) calloc(data_size, sizeof(uint32_t));
	output = (uint32_t *) calloc(data_size, sizeof(uint32_t));

	printf("Input (salt):\n");
	for (i = 0; i < data_size; ++i) 
	{
		input[i] = i;
		salt[i] = 0;
		printf("%d (%d), ", input[i], salt[i]);
	}
	printf("\n");
	
	// Memory buffers
	input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (data_size) * sizeof(uint32_t), (void *)input, NULL);
	salt_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR , (data_size) * sizeof(uint32_t), (void *)salt, NULL);
	output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY , (data_size) * sizeof(uint32_t), NULL, NULL);

	// Kernel
	kernel = clCreateKernel(program, "sample", NULL);

	// Kernel arguments
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &input_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &salt_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &output_buffer);

	// Execute kernel
	global_work_size[0] = data_size;
	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	// Read output
	clEnqueueReadBuffer(command_queue, output_buffer, CL_TRUE, 0, data_size * sizeof(uint32_t), output, 0, NULL, NULL);

	printf("Output:\n");
	for (i = 0; i < data_size; ++i) 
	{
		printf("%d, ", output[i]);
	}
	printf("\n");

	// Clean up
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(input_buffer);
	clReleaseMemObject(salt_buffer);
	clReleaseMemObject(output_buffer);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	if (devices != NULL)
		free(devices);

	return EXIT_SUCCESS;
}
