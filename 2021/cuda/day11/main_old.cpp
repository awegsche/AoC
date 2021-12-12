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

using std::cout, std::endl;
using std::vector;
using std::ifstream;

constexpr auto WWIDTH = 800;
constexpr auto WHEIGHT = 600;

using sclock = std::chrono::steady_clock;
using duration = std::chrono::duration<double>;

void print_field(uint8_t const* field, size_t width, size_t height) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            cout << field[y * width + x];
        }
        cout << "\n";
    }
}


int main(int argc, char* argv[]) {

    ERR("test error");

    ifstream input{ "../../input/day08_test_input.txt" };

    vector<uint8_t> field;

    // read first line, this gives width
    uint8_t cell = 0;
    input >> cell;
    size_t width = 0;
    size_t height = 1;
    while (cell != '\n') {
        field.push_back(cell - '0');
        width++;
        input >> cell;
    };

    input >> cell;
    while (cell != EOF) {
        for (int i = 0; i < width; i++)
			field.push_back(cell - '0');
        input.get();
        height++;
    }

    cout << "width: " << width << "< height: " << height << endl;

    print_field(field.data(), width, height);

    return;


    Managed<uint8_t> board{width*height};
    board.malloc_both();
    for (size_t i = 0; i < width * height; i++) {
        board.get_host_ptr()[i] = field[i];
    }
    board.cp_host_to_device();

	auto logsink = setup_logging();

	CudaDevice dev{ 0 };
	dev.query();
	LOG("dev = {}", dev);

	//GLFWwindow* window = initGui("hello", WWIDTH, WHEIGHT);

	cout << "hello" << endl;

    //CudaTextureBuffer buff{ WWIDTH, WHEIGHT };

    //buff.createVBO();

    FpsCounter fpscounter(500);

	//render(outbuffer.get_device_ptr(), WWIDTH, WHEIGHT);
	LAUNCH(cudaDeviceSynchronize());

    outbuffer.cp_device_to_host();

    auto out = outbuffer.get_host_ptr();
    for (int i = 0; i < 10; i++) {
        auto el = out[i * 3];
        cout << el.x << " " << el.y << " " << el.z << endl;
    }

    return;

    do {
        auto t0 = sclock::now();
        glfwPollEvents();
        auto t1 = sclock::now();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        //buff.map((void**)&outbuffer.get_device_ptr());


        //buff.unmap();
        glFlush();
        glFinish();

        //buff.draw();

        //beginFrameImGui();

        //DrawStats(t1 - t0);
        //fpscounter.display();

        //logsink->draw_imgui(5.0f, 5.0f, 500.0f, 1000.0f);

        ////fileDialog.Display();
        //endFrameImGui();

        //sutil::displayStats(state_update_time, render_time, display_time);
        glfwSwapBuffers(window);
    } while (!glfwWindowShouldClose(window));

    cleanUpGui(window);


	return 0;
}