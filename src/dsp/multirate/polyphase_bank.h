#pragma once
#include <vector>
#include "../taps/tap.h"

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
        pb.phases = (T**)volk_malloc(phaseCount * sizeof(T*), volk_get_alignment());

        // Allocate phases
        pb.tapsPerPhase = (taps.size + phaseCount - 1) / phaseCount;
        for (int i = 0; i < phaseCount; i++) {
            pb.phases[i] = (T*)volk_malloc(pb.tapsPerPhase * sizeof(T), volk_get_alignment());
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
            volk_free(bank.phases[i]);
        }
        volk_free(bank.phases);
        bank.phases = NULL;
        bank.phaseCount = 0;
        bank.tapsPerPhase = 0;
    }
}