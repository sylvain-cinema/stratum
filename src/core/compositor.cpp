/// @file compositor.cpp
/// @brief Multi-plane depth compositor using depth peeling.

#include "compositor.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace stratum {

MultiPlaneCompositor::MultiPlaneCompositor(uint32_t layer_count, float total_depth_m)
    : layer_count_(layer_count)
    , total_depth_m_(total_depth_m)
    , layer_spacing_m_(total_depth_m / static_cast<float>(layer_count)) {

    std::cout << "[STRATUM] Compositor: " << layer_count << " layers, "
              << layer_spacing_m_ << "m spacing" << std::endl;
}

std::vector<DepthLayer> MultiPlaneCompositor::decompose(
    const float* depth_map,
    const uint8_t* color_data,
    uint32_t width,
    uint32_t height
) {
    std::vector<DepthLayer> layers;
    layers.reserve(layer_count_);

    for (uint32_t i = 0; i < layer_count_; ++i) {
        float near = static_cast<float>(i) * layer_spacing_m_;
        float far = near + layer_spacing_m_;

        DepthLayer layer;
        layer.layer_index = i;
        layer.near_depth_m = near;
        layer.far_depth_m = far;
        layer.opacity = 1.0f;
        layer.pixel_data.resize(width * height * 4, 0); // RGBA

        // Assign pixels to this layer based on depth
        for (uint32_t y = 0; y < height; ++y) {
            for (uint32_t x = 0; x < width; ++x) {
                uint32_t idx = y * width + x;
                float depth = depth_map[idx];

                if (depth >= near && depth < far) {
                    uint32_t rgba_idx = idx * 4;
                    uint32_t rgb_idx = idx * 3;

                    layer.pixel_data[rgba_idx + 0] = color_data[rgb_idx + 0]; // R
                    layer.pixel_data[rgba_idx + 1] = color_data[rgb_idx + 1]; // G
                    layer.pixel_data[rgba_idx + 2] = color_data[rgb_idx + 2]; // B

                    // Alpha based on depth confidence
                    float depth_center = (near + far) / 2.0f;
                    float distance_from_center = std::abs(depth - depth_center);
                    float alpha = 1.0f - (distance_from_center / layer_spacing_m_);
                    layer.pixel_data[rgba_idx + 3] = static_cast<uint8_t>(alpha * 255.0f);
                }
            }
        }

        layers.push_back(std::move(layer));
    }

    return layers;
}

std::vector<uint8_t> MultiPlaneCompositor::blend_layers(
    const std::vector<DepthLayer>& layers
) {
    if (layers.empty()) {
        return {};
    }

    // Back-to-front alpha compositing
    size_t pixel_count = layers[0].pixel_data.size();
    std::vector<uint8_t> result(pixel_count, 0);

    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        for (size_t i = 0; i < pixel_count; i += 4) {
            float src_a = static_cast<float>(it->pixel_data[i + 3]) / 255.0f;
            float dst_a = static_cast<float>(result[i + 3]) / 255.0f;

            float out_a = src_a + dst_a * (1.0f - src_a);
            if (out_a > 0.0f) {
                for (int c = 0; c < 3; ++c) {
                    float src = static_cast<float>(it->pixel_data[i + c]);
                    float dst = static_cast<float>(result[i + c]);
                    result[i + c] = static_cast<uint8_t>(
                        (src * src_a + dst * dst_a * (1.0f - src_a)) / out_a
                    );
                }
            }
            result[i + 3] = static_cast<uint8_t>(out_a * 255.0f);
        }
    }

    return result;
}

} // namespace stratum
