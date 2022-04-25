#pragma once
#include <volk/volk.h>

namespace dsp {
    template<class T>
    inline T* allocBuffer(int count) {
        return (T*)volk_malloc(count * sizeof(T), volk_get_alignment());
    }

    template<class T>
    inline void clearBuffer(T* buffer, int count, int offset = 0) {
        memset(&buffer[offset], 0, count * sizeof(T));
    }

    inline void freeBuffer(void* buffer) {
        volk_free(buffer);
    }
}