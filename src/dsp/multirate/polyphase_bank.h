#pragma once
#include <vector>
#include "../taps/tap.h"
#include "../buffer/buffer.h"

namespace dsp::multirate {
    template<class T>
    struct PolyphaseBank {
        int phaseCount;
        int tapsPerPhase;
        T** phases;
    };

    template<class T>
    inline PolyphaseBank<T> buildPolyphaseBank(int phaseCount, tap<T>& taps) {
        // Allocate bank
        PolyphaseBank<T> pb;
        pb.phaseCount = phaseCount;
        pb.phases = buffer::alloc<T*>(phaseCount);
        

        // Allocate phases
        pb.tapsPerPhase = (taps.size + phaseCount - 1) / phaseCount;
        for (int i = 0; i < phaseCount; i++) {
            pb.phases[i] = buffer::alloc<T>(pb.tapsPerPhase);
        }

        // Fill phases
        int totTapCount = phaseCount * pb.tapsPerPhase;
        for (int i = 0; i < totTapCount; i++) {
            pb.phases[i % phaseCount][i / phaseCount] = (i < taps.size) ? taps.taps[i] : 0;
        }

        return pb;
    }

    template<class T>
    inline void freePolyphaseBank(PolyphaseBank<T>& bank) {
        if (!bank.phases) { return; }
        for (int i = 0; i < bank.phaseCount; i++) {
            if (!bank.phases[i]) { continue; }
            buffer::free(bank.phases[i]);
        }
        buffer::free(bank.phases);
        bank.phases = NULL;
        bank.phaseCount = 0;
        bank.tapsPerPhase = 0;
    }
}