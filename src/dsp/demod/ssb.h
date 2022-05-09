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
            xlator.init(NULL, (_mode == Mode::USB) ? (_bandwidth / 2.0) : (-_bandwidth / 2.0), _samplerate);
            agc.init(NULL, 1.0, agcRate);
            base_type::init(in);
        }

        void setMode(Mode mode) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            base_type::tempStop();
            _mode = mode;
            xlator.setOffset((_mode == Mode::USB) ? (_bandwidth / 2.0) : (-_bandwidth / 2.0), _samplerate);
            base_type::tempStart();
        }

        void setBandwidth(double bandwidth) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            base_type::tempStop();
            _bandwidth = bandwidth;
            xlator.setOffset((_mode == Mode::USB) ? (_bandwidth / 2.0) : (-_bandwidth / 2.0), _samplerate);
            base_type::tempStart();
        }

        void setSamplerate(double samplerate) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            base_type::tempStop();
            _samplerate = samplerate;
            xlator.setOffset((_mode == Mode::USB) ? (_bandwidth / 2.0) : (-_bandwidth / 2.0), _samplerate);
            base_type::tempStart();
        }

        void setAGCRate(double agcRate) {
            assert(base_type::_block_init);
            std::lock_guard<std::recursive_mutex> lck(base_type::ctrlMtx);
            agc.setRate(agcRate);
        }

        int process(int count, const complex_t* in, float* out) {
            agc.process(count, in, agc.out.writeBuf)
            xlator.process(count, agc.out.writeBuf, out);
            return count;
        }

    protected:
        Mode _mode;
        double _bandwidth;
        double _samplerate;
        channel::FrequencyXlator xlator;
        loop::AGC<float> agc;

    };
};