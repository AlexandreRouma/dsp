#pragma once
#include "../processor.h"

namespace dsp::math {
    template<class T>
    class Delay : public Processor<T, T> {
        using base_type = Processor<T, T>;
    public:
        Delay() {}

        Delay(stream<T>* in, int delay) { init(in, delay); }

        ~Delay() {
            if (!base_type::_block_init) { return; }
            freeBuffer(buffer);
        }

        void init(stream<T>* in, int delay) {
            
            base_type::init(in);
        }

    private:
        T* buffer;
        T* bufStart;
    };
}