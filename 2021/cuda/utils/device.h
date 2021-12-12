
#ifndef DEVICE_H
#define DEVICE_H

#include <cuda_runtime.h>
#include <fmt/format.h>

#include "cuda_helpers.h"

constexpr auto THOUSEND = 1000;
constexpr auto MILLION = THOUSEND*THOUSEND;

// Convenience class to get device information
class CudaDevice {

public:
	CudaDevice(int id) : id_(id), queried_(false) {}

	auto query() {
		LAUNCH(cudaGetDeviceProperties(&properties_, id_));
		queried_ = true;
	}

	friend class fmt::formatter<CudaDevice>;

private:
	int id_;
	bool queried_;

	cudaDeviceProp properties_;
};

template<>
struct fmt::formatter<CudaDevice>
{
	constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}
	template<typename FormatContext>
	auto format(CudaDevice const& dev, FormatContext& ctx) -> decltype(ctx.out()) {
		auto&& out = ctx.out();
		fmt::format_to( out, "Device {}", dev.id_);
		if (dev.queried_) {
			//fmt::format_to(out, "\n\tName:        \33[1m{}\33[0m", dev.properties_.name);
			//fmt::format_to(out, "\n\tCUDA:        \33[1m{}.{}\33[0m", dev.properties_.major, dev.properties_.minor);
			//fmt::format_to(out, "\n\tClock:       {}\33[90mMHz\33[0m", dev.properties_.clockRate / 1000);
			//fmt::format_to(out, "\n\tMaxGridSize: {}\33[90mM\33[0m x {}\33[90mK\33[0m x {}\33[90mK\33[0m",
			//	dev.properties_.maxGridSize[0] / MILLION,
			//	dev.properties_.maxGridSize[1] / THOUSEND,
			//	dev.properties_.maxGridSize[2] / THOUSEND
			//	);
			fmt::format_to(out, "\n\tName:        {}", dev.properties_.name);
			fmt::format_to(out, "\n\tCUDA:        {}.{}", dev.properties_.major, dev.properties_.minor);
			fmt::format_to(out, "\n\tClock:       {}MHz", dev.properties_.clockRate / 1000);
			fmt::format_to(out, "\n\tMaxGridSize: {}M x {}K x {}K",
				dev.properties_.maxGridSize[0] / MILLION,
				dev.properties_.maxGridSize[1] / THOUSEND,
				dev.properties_.maxGridSize[2] / THOUSEND
				);
			fmt::format_to(out, "\n\tName: {}", dev.properties_.name);
		}
		else
			fmt::format_to(out, " (not queried)");
		return out;

	}
};

#endif // DEVICE_H
