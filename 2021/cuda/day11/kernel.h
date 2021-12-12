#include <cuda.h>
#include "utils/cuda_helpers.h"

//void render(float3* buffer, unsigned int w, unsigned int h);

void part1(
	Managed<uint8_t>& field1,
	Managed<uint8_t>& field2,
	Managed<uint32_t>& flashed,
	size_t width,
	size_t height);