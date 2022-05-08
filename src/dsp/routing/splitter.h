#pragma once
#include "../sink.h"

namespace dsp::routing {
    template <class T>
    class Splitter : public Sink<T> {
        using base_type = Sink<T>;
    public:
        Splitter() {}

        Splitter(stream<T>* in) { base_type::init(in); }

        int run() {
            int count = _in->read();
            if (count < 0) { return -1; }

            for (const auto& stream : outStreams) {
                memcpy(stream->writeBuf, _in->readBuf, count * sizeof(T));
                if (!stream->swap(count)) {
                    _in->flush();
                    return -1;
                }
            }

            _in->flush();

            return count;
        }

    protected:
        std::vector<stream<T>*> outStreams;

    };
}