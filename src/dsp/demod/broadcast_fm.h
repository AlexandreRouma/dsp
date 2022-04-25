#pragma once
#include "fm.h"
#include "../filter/fir.h"
#include "../loop/pll.h"
#include "../convert/real_to_complex.h"

namespace dsp::demod {
    class BroadcastFM : public Processor<complex_t, float> {
        using base_type = Processor<complex_t, float>;
    public:
        BroadcastFM() {}

        BroadcastFM(stream<complex_t>* in) { init(in); }

        virtual void init(stream<complex_t>* in) {

            base_type::init(in);
        }

        inline int process(int count, complex_t* in, float* out) {
            // Demodulate and convert to complex
            demod.process(count, in, demod.out.writeBuf);
            rtoc.process(count, demod.out.writeBuf, rtoc.out.writeBuf);

            // Filter out pilot and run through PLL
            pilotFir.process(count, rtoc.out.writeBuf, pilotFir.out.writeBuf);
            pilotPLL.process(count, pilotFir.out.writeBuf, pilotPLL.out.writeBuf);

            return count;
        }

        void reset() {
            assert(base_type::_block_init);
            demod.reset();
            pilotFir.reset();
            pilotPLL.reset();
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
        FM demod;
        filter::FIR<complex_t, complex_t> pilotFir;
        tap<complex_t> pilotFirTaps;
        convert::RealToComplex rtoc;
        loop::PLL pilotPLL;
        
    };
}