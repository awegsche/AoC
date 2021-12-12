
#include "cuda_helpers.h"
#include <chrono>

#include <imgui.h>

using std::chrono::milliseconds;
using stclock = std::chrono::steady_clock;
using timepoint = std::chrono::steady_clock::time_point;
using std::chrono::duration_cast;

class FpsCounter {
	timepoint last_;
	milliseconds update_interval_;
	uint32_t framecount_;
	float fps_;

public:
	FpsCounter(int interval)
		: last_(stclock::now()), update_interval_(interval), framecount_(0) {}

	void display() {
		framecount_++;

		auto elapsed = duration_cast<milliseconds>(stclock::now() - last_);
		if (elapsed > update_interval_) {
			fps_ = 1000.0f * (float)framecount_ / elapsed.count();
			framecount_ = 0;
			last_ = stclock::now();
		}

		ImGui::Begin("FPS counter");
		ImGui::Text("%.2f", fps_);
		ImGui::Text("more text");
		ImGui::End();
	}
};
