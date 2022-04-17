#pragma once
#include "../processor.h"
#include "../math/norm_phase_diff.h"
#include "phase_control_loop.h"

// TODO: FINISH THIS

namespace dsp::loop {
    class PLL : public Processor<complex_t, complex_t> {
        using base_type = Processor<complex_t, complex_t>;
    public:
        PLL() {}

        PLL(stream<complex_t>* in, double bandwidth, double initFreq = 0.0, double initPhase = 0.0) { init(in, bandwidth, initFreq, initPhase); }

        void init(stream<complex_t>* in, double bandwidth, double initFreq = 0.0, double initPhase = 0.0) {
            pcl.init(0.1, 0.1, 0.0, -2.0f * FL_M_PI, 2.0f * FL_M_PI, 0.0, -1.0, 1.0);
            base_type::init(in);
        }

        void reset() {
            assert(base_type::_block_init);
            base_type::tempStop();

            base_type::tempStart();
        }

        inline int process(int count, const complex_t* in, complex_t* out) {
            float err;
            for (int i = 0; i < count; i++) {
                out[i] = { cosf(pcl.phase), sinf(pcl.phase) };
                pcl.advance(math::normPhaseDiff(atan2f(in[i].re, in[i].im) - pcl.phase));
            }
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
        PhaseControlLoop<float> pcl;

        float vcoPhase = 0.0f;
        float vcoFrequency = 0.0f;
        float alpha = 0.0f;
        float _alpha = 0.1f;
        float _beta = 0.1f;
        complex_t lastVCO = { 1.0f, 0.0f };

    };
}