#pragma once

/// @file engine.hpp
/// @brief STRATUM Volumetric Display Engine

#include <cstdint>
#include <memory>
#include <vector>

namespace stratum {

/// Configuration for the volumetric engine.
struct VolumetricConfig {
    uint32_t layer_count = 16;       // 16 depth planes per website spec
    float depth_range_m = 15.0f;
    float transparency = 0.85f;      // 85% optical clarity
    uint32_t width = 7680;           // 8K per layer
    uint32_t height = 4320;
    bool spectra_sync = true;
};

/// Lifecycle state of the engine.
enum class EngineState {
    Uninitialized,
    Ready,
    Running,
    Error,
};

/// The main volumetric display engine.
/// Manages multi-plane transparent displays for glasses-free 3D depth.
class VolumetricEngine {
public:
    explicit VolumetricEngine(const VolumetricConfig& config);
    ~VolumetricEngine();

    /// Initialize hardware and rendering pipeline.
    bool initialize();

    /// Start rendering loop.
    bool start();

    /// Stop rendering.
    void stop();

    /// Shutdown and release resources.
    void shutdown();

    /// Get current engine state.
    [[nodiscard]] EngineState state() const { return state_; }

    /// Get current configuration.
    [[nodiscard]] const VolumetricConfig& config() const { return config_; }

private:
    VolumetricConfig config_;
    EngineState state_ = EngineState::Uninitialized;
};

} // namespace stratum
