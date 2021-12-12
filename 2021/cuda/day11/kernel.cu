#include "kernel.h"
#include <iostream>
#include <cuda.h>
#include "utils/cuda_helpers.h"


__global__ void kernel(float3* output, unsigned int width, unsigned int height) {
	auto x = blockIdx.x * blockDim.x + threadIdx.x;
	auto y = blockIdx.y * blockDim.y + threadIdx.y;

	auto index = (x + width * y);

	output[index].x = 1.0f;
}

void render(float3* buffer, unsigned int w, unsigned int h) {
	dim3 block(16, 16, 1);

	dim3 grid(w / block.x, h / block.y, 1);

	std::cout << "about to call kernel" << std::endl;

	kernel << <grid, block >> > (buffer, w, h);

}

__global__ void part1_kernel(uint8_t* field, uint8_t* swap_field, uint32_t* flashes, size_t width, size_t height) {


}

void part1(Managed<uint8_t>& field1, Managed<uint8_t>& field2, Managed<uint32_t>& flashed, size_t width, size_t height) {

	dim3 block(16, 16, 1);
	dim3 grid(width / block.x, height / block.y, 1);

	part1_kernel << <grid, block >> > (
		field1.get_device_ptr(),
		field2.get_device_ptr(),
		flashed.get_device_ptr(),
		width, height);
}
