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
            taps.taps = (T*)volk_malloc(count * sizeof(T), volk_get_alignment());
            return taps;
        }

        template<class T>
        inline void free(tap<T>& taps) {
            if (!taps.taps) { return; }
            volk_free(taps.taps);
            taps.taps = NULL;
            taps.size = 0;
        }
    }
}