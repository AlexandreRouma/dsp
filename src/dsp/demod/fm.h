#pragma once
#include "../processor.h"
#include "../math/fast_atan2.h"
#include "../math/freq_to_omega.h"

namespace dsp::demod {
    class FM : public Processor<complex_t, float> {
        using base_type = Processor<complex_t, float>;
    public:
        FM() {}

        FM(stream<complex_t>* in, double deviation) { init(in, deviation); }

        FM(stream<complex_t>* in, double deviation, double samplerate) { init(in, deviation, samplerate); }

        virtual void init(stream<complex_t>* in, double deviation) {
            _invDeviation = 1.0 / deviation;
            base_type::init(in);
        }

        virtual void init(stream<complex_t>* in, double deviation, double samplerate) {
            init(in, math::freqToOmega(deviation, samplerate));
        }

        void setDeviation(double deviation) {
            _invDeviation = 1.0 / deviation;
        }

        void setDeviation(double deviation, double samplerate) {
            _invDeviation = 1.0 / math::freqToOmega(deviation, samplerate);
        }

        inline int process(int count, const complex_t* in, float* out) {
            // This is somehow faster than volk...
            float diff, currentPhase;
            float _phase = phase;
            for (int i = 0; i < count; i++) {
                // Get phase
                // TODO: Check if using real atan2 for accuracy is worth it
                currentPhase = math::fastAtan2(in[i].im, in[i].re);

                // Get delta and correct for wrap around
                diff = currentPhase - _phase;
                if (diff > 3.1415926535f) {
                    diff -= 2 * 3.1415926535f;
                }
                else if (diff <= -3.1415926535f) {
                    diff += 2 * 3.1415926535f;
                }

                // Normalize and write to output
                out[i] = diff * _invDeviation;

                // Save phase for delta
                _phase = currentPhase;
            }
            phase = _phase;

            return count;
        }

        int run() {
            int count = base_type::_in->read();
            if (count < 0) { return -1; }

            process(count, base_type::_in->readBuf, base_type::out.writeBuf);

            base_type::_in->flush();
            if (!base_type::out.swap(count)) { return -1; }
            return count;
        }

    protected:
        float _invDeviation;
        float phase = 0.0f;
    };
}