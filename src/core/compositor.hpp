#pragma once

/// @file compositor.hpp
/// @brief Multi-plane depth compositor using depth peeling algorithm.

#include <cstdint>
#include <vector>

namespace stratum {

/// A single depth layer in the volumetric display.
struct DepthLayer {
    uint32_t layer_index;
    float near_depth_m;
    float far_depth_m;
    float opacity;
    std::vector<uint8_t> pixel_data;
};

/// Multi-plane compositor that decomposes scenes into depth layers
/// for transparent display rendering.
class MultiPlaneCompositor {
public:
    explicit MultiPlaneCompositor(uint32_t layer_count, float total_depth_m);

    /// Decompose a depth map into discrete display layers.
    std::vector<DepthLayer> decompose(
        const float* depth_map,
        const uint8_t* color_data,
        uint32_t width,
        uint32_t height
    );

    /// Blend layers with transparency for final output.
    std::vector<uint8_t> blend_layers(const std::vector<DepthLayer>& layers);

    [[nodiscard]] uint32_t layer_count() const { return layer_count_; }

private:
    uint32_t layer_count_;
    float total_depth_m_;
    float layer_spacing_m_;
};

} // namespace stratum
