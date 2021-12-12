#ifndef COMMON_H
#define COMMON_H
#include <cuda.h>
#include <cuda_runtime.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "ext/dear_spdlog/src/dear_sink.h"



// -------------------------------------------------------------------------------------------------
// ---- Logging ------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
//#using LOG = spdlog::info;
//#using DEB = spdlog::debug;
//#using WARN = spdlog::warn;
//#using ERR = spdlog::error;

#define LOG(...) spdlog::info(__VA_ARGS__)
#define DBG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define TRC(...) SPDLOG_TRACE(__VA_ARGS__)
#define WRN(...) spdlog::warn(__VA_ARGS__)
#define ERR(...) spdlog::error(__VA_ARGS__)

std::shared_ptr<dear_spdlog::dear_sink<std::mutex>> setup_logging();

#define LAUNCHM(_function, _message) \
    {cudaError_t err = _function; \
    if (err != cudaSuccess) ERR("LAUNCH failed {} at line {}", cudaGetErrorString(err), __LINE__);\
    else SPDLOG_DEBUG("{}", _message);}

#define LAUNCH(_function) \
    {cudaError_t err = _function; \
    if (err != cudaSuccess) ERR("LAUNCH failed {} at line {}", cudaGetErrorString(err), __LINE__);}

#define GL_CHECK(_function) \
    _function;\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) {\
        ERR("GL error: {} at line {}", err, __LINE__); \
    }


/**
 * @brief A managed pointer to device and host data
 * 
 * @tparam T the type of the managed data
 */
template<typename T>
class Managed {
    T* host_ptr;
    T* device_ptr;
    size_t size;

public:
    Managed(size_t len): host_ptr(nullptr), device_ptr(0), size(len*sizeof(T)) { }

    void malloc_both() {
        malloc_host();
        malloc_device();
    }

    void malloc_host() {
        free_host();
        host_ptr = (T*)malloc(size);
    }

    void malloc_device() {
        free_device();
        SPDLOG_TRACE("allocating {} bytes of device memory", size);
        LAUNCH(cudaMalloc(&device_ptr, size));
        LAUNCH(cudaDeviceSynchronize());

    }

    void swap(Managed<T>& other) {
        T* temp_device_ptr = this->device_ptr;
        T* temp_host_ptr = this->host_ptr;

        this->device_ptr = other.device_ptr;
        this->host_ptr = other.host_ptr;

        other.device_ptr = temp_device_ptr;
        other.host_ptr = temp_host_ptr;
    }

    /**
     * @brief Copies host data to device
     * 
     */
    void cp_host_to_device() const {
        LAUNCH(cudaMemcpy(device_ptr, host_ptr, size, cudaMemcpyHostToDevice));
        LAUNCH(cudaDeviceSynchronize());
    }

    /**
     * @brief Copies device data to host
     * 
     */
    void cp_device_to_host() const {
        LAUNCH(cudaMemcpy(host_ptr, device_ptr, size, cudaMemcpyDeviceToHost));
        LAUNCH(cudaDeviceSynchronize());
    }

    T& operator[](size_t index) const {
        return host_ptr[index];
    }

    /**
     * @brief Get the host ptr
     * 
     * @return T* 
     */
    T* get_host_ptr() {
        return host_ptr;
    }

    /**
     * @brief Get the device ptr
     * 
     * @return T* 
     */
    T* get_device_ptr() {
        return device_ptr;
    }

    void free_host() {
        if (host_ptr) {
            free(host_ptr);
            host_ptr = nullptr;
        }
    }

    void free_device() {
        if (device_ptr) {
            LAUNCH(cudaFree(device_ptr));
            LAUNCH(cudaDeviceSynchronize());
            device_ptr = nullptr;
        }
    }

    ~Managed() {
        free_host();
        free_device();
    }
};

#endif // COMMON_H
