#pragma once
#include "../processor.h"
#include "../channel/frequency_xlator.h" 
#include "../loop/agc.h"

namespace dsp::demod {
    class SSB : public Processor<complex_t, float> {
        using base_type = Processor<complex_t, float>;
    public:
        enum Mode {
            USB,
            LSB
        };

        SSB() {}

        SSB(stream<complex_t>* in, Mode mode, double bandwidth, double samplerate, double agcRate) { init(in, mode, agcRate); }

        void init(stream<complex_t>* in, Mode mode, double agcRate) {
            _mode = mode;
            agc.init(NULL, 1.0, agcRate);
            base_type::init(in);
        }

        void setMode(Mode mode) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            _mode = mode;
        }

        void setAGCRate(double agcRate) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            agc.setRate(agcRate);
        }

        int process(int count, const complex_t* in, float* out) {

        }

    protected:
        Mode _mode;
        double _bandwidth;
        double _samplerate;
        channel::FrequencyXlator xlator;
        loop::AGC<float> agc;

    };
};