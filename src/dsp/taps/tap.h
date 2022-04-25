#pragma once
#include <volk/volk.h>

namespace dsp {
    template<class T>
    class tap {
    public:
        T* taps = NULL;
        unsigned int size = 0;
    };

    namespace taps {
        template<class T>
        inline tap<T> alloc(int count) {
            tap<T> taps;
            taps.size = count;
            taps.taps = allocBuffer<T>(count);
            return taps;
        }

        template<class T>
        inline void free(tap<T>& taps) {
            if (!taps.taps) { return; }
            freeBuffer(taps.taps);
            taps.taps = NULL;
            taps.size = 0;
        }
    }
}