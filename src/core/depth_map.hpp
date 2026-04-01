#pragma once

#include <cstdint>
#include <vector>

namespace stratum {

/// Depth estimation from stereo or monocular input.
class DepthEstimator {
public:
    /// Estimate depth from a single frame using neural network.
    std::vector<float> estimate_monocular(
        const uint8_t* frame, uint32_t width, uint32_t height
    );

    /// Estimate depth from stereo pair.
    std::vector<float> estimate_stereo(
        const uint8_t* left, const uint8_t* right,
        uint32_t width, uint32_t height
    );
};

} // namespace stratum
