#pragma once

namespace dsp::taps {
    int estimateTapCount(double transWidth, double samplerate) {
        return 3.8 * samplerate / transWidth;
    }
}