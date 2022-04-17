#pragma once
#include "block.h"

namespace dsp {
    template <class I, class O>
    class Processor : public block {
    public:
        Processor() {}

        Processor(stream<I>* in) { init(in); }

        virtual ~Processor() {}

        virtual void init(stream<I>* in) {
            _in = in;
            registerInput(_in);
            registerOutput(&out);
            _block_init = true;
        }

        virtual void setInput(stream<I>* in) {
            assert(_block_init);
            std::lock_guard<std::recursive_mutex> lck(ctrlMtx);
            tempStop();
            unregisterInput(_in);
            _in = in;
            registerInput(_in);
            tempStart();
        }

        virtual int run() = 0;

        stream<O> out;

    protected:
        stream<I>* _in;
    };
}
