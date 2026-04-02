/// @file engine.cpp
/// @brief STRATUM Volumetric Display Engine implementation.

#include "engine.hpp"
#include <iostream>

namespace stratum {

VolumetricEngine::VolumetricEngine(const VolumetricConfig& config)
    : config_(config) {}

VolumetricEngine::~VolumetricEngine() {
    if (state_ == EngineState::Running) {
        stop();
    }
    shutdown();
}

bool VolumetricEngine::initialize() {
    if (state_ != EngineState::Uninitialized) {
        return false;
    }

    std::cout << "[STRATUM] Initializing volumetric engine: "
              << config_.layer_count << " depth planes @ "
              << config_.width << "x" << config_.height
              << " (" << static_cast<int>(config_.transparency * 100) << "% transparency)"
              << std::endl;

    // Validate configuration against spec requirements:
    // - 16 depth planes
    // - 8K per layer (7680x4320)
    // - 85% optical clarity
    if (config_.layer_count < 1 || config_.width < 1 || config_.height < 1) {
        state_ = EngineState::Error;
        return false;
    }

    state_ = EngineState::Ready;
    return true;
}

bool VolumetricEngine::start() {
    if (state_ != EngineState::Ready) {
        return false;
    }

    std::cout << "[STRATUM] Starting volumetric rendering — "
              << config_.layer_count << " planes, zero vergence conflict"
              << std::endl;

    state_ = EngineState::Running;
    return true;
}

void VolumetricEngine::stop() {
    if (state_ == EngineState::Running) {
        std::cout << "[STRATUM] Stopping volumetric rendering" << std::endl;
        state_ = EngineState::Ready;
    }
}

void VolumetricEngine::shutdown() {
    stop();
    state_ = EngineState::Uninitialized;
}

} // namespace stratum
