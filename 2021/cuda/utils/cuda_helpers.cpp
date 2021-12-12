#include "cuda_helpers.h"

std::shared_ptr<dear_spdlog::dear_sink<std::mutex>> setup_logging() {

    // setup logging
    spdlog::set_pattern("%^%5l%$ | %v");
    spdlog::set_level(spdlog::level::trace);

    LOG("logging setup");
    SPDLOG_INFO("info");
    SPDLOG_DEBUG("debug");
    return dear_sink_mt();
}
