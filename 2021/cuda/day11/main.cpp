#include <iostream>
#include <cuda_runtime.h>
#include "utils/cuda_helpers.h"
#include "utils/device.h"
#include "utils/gui.h"
#include "utils/fpscounter.h"

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"

#include "kernel.h"
#include <vector>
#include <fstream>
#include <filesystem>

using std::cout, std::endl;
using std::vector;
using std::ifstream;
namespace fs = std::filesystem;

constexpr auto WWIDTH = 800;
constexpr auto WHEIGHT = 600;

using sclock = std::chrono::steady_clock;
using duration = std::chrono::duration<double>;

void print_field(uint8_t const* field, size_t width, size_t height) {
    cout << "field" << endl;
    for (size_t y = 0; y < height; y++) {
        auto stride = (y + 1) * (width + 2);
        for (size_t x = 0; x < width; x++) {
            cout << (int)field[stride + x + 1];
        }
        cout << "\n";
    }
}


int main(int argc, char* argv[]) {

    ERR("test error");

    cout << INPUT_DIR << endl;

    auto path = fs::path(INPUT_DIR) / "day11_test_input.txt";
    cout << path;
    if (!fs::exists(path)) return 0;
    LOG("file exists");
    ifstream input{path};

    
    vector<uint8_t> field;

    // read first line, this gives width
    char cell = 0;
    input >> cell;
    size_t width = 0;
    size_t height = 1;
    while (cell != '\n' && width < 20) {
        cout << (int)cell << endl;
        field.push_back(cell - '0');
        width++;
        input.get(cell);
    };

    input.get(cell);
    //input >> cell;
    while (!input.eof()) {
        for (int i = 0; i < width; i++) {
            field.push_back(cell - '0');
        cout << cell << endl;
        input.get(cell);
        }
        input.get(cell);
        height++;
    }

    cout << "width: " << width << "< height: " << height << endl;

    print_field(field.data(), width, height);


    Managed<uint8_t> board1{width*height};
    Managed<uint8_t> board2{width*height};
    Managed<uint32_t> flashed{width*height};

    board1.malloc_both();
    board2.malloc_both();
    flashed.malloc_both();

    auto b1 = board1.get_host_ptr();
    for (size_t i = 0; i < width + 2; i++) {
        b1[i] = 0;
    }
    for (size_t j = 0; j < height; j++) {
        auto stride = (j + 1) * (width + 2);
        auto stride_orig = j * width;
        b1[stride] = 0;
        b1[stride+width+1] = 0;
        for (size_t i = 0; i < width + 2; i++) {
            b1[stride + i+1] = field[stride+ i];
        }
    }
    auto last_stride = (height + 1) * (width + 2);
    for (size_t i = 0; i < width + 2; i++) {
        b1[last_stride+i] = 0;
    }
    board1.cp_host_to_device();

	auto logsink = setup_logging();

	CudaDevice dev{ 0 };
	dev.query();
	LOG("dev = {}", dev);

	cout << "hello" << endl;

    part1(board1, board2, flashed, width+2, height+2);
	LAUNCH(cudaDeviceSynchronize());
    board2.cp_device_to_host();

    print_field(board2.get_host_ptr(), width, height);

	return 0;
}