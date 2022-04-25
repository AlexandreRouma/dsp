#include <stdio.h>
#include <dsp/bench/speed_tester.h>

#include <dsp/channel/rx_vfo.h>

#include <dsp/chain.h>

#define TEST_BUFFER_SIZE    64000
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

int main() {
    dsp::stream<dsp::complex_t>* input;
    dsp::stream<dsp::complex_t>* output;

    // ============= DSP Under Test =============
    input = new dsp::stream<dsp::complex_t>;

    //dsp::channel::RxVFO dut(input, 1000000000.0, 250000.0, 200000.0, 1000000.0);

    dsp::multirate::PowerDecimator<dsp::complex_t> dut(NULL, 256);
    dsp::multirate::PowerDecimator<dsp::complex_t> dut2(NULL, 256);

    dsp::chain<dsp::complex_t> ch(input);
    ch.addBlock(&dut, false);
    ch.addBlock(&dut2, false);

    output = ch.out;
    // ==========================================


    ch.enableBlock(&dut2, [&](dsp::stream<dsp::complex_t>* out) {
        output = out;
    });


    // Run benchmark
    dsp::bench::SpeedTester<dsp::complex_t, dsp::complex_t> st(input, output);
    ch.start();
    for (int i = 0; i < TEST_COUNT; i++) {
        dut.reset();
        printf("%lf MS/s\n", st.benchmark(TEST_DURATION, TEST_BUFFER_SIZE) / 1000000.0);
    }
    ch.stop();


    // for (double f = -0.5; f <= 0.5; f += 0.001) {
    //     // Generate signal
    //     double phase = 0;
    //     for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
    //         dsp::complex_t cplx = { cosf(phase), sinf(phase) };
    //         input->readBuf[i] = cplx * dsp::window::nuttall(i,TEST_BUFFER_SIZE);
    //         phase += 2.0 * DB_M_PI * f;

    //         if (phase > 2.0 * DB_M_PI) {
    //             phase -= 2.0 * DB_M_PI;
    //         }
    //         else if (phase < 2.0 * DB_M_PI) {
    //             phase += 2.0 * DB_M_PI;
    //         }
    //     }

    //     // Process through resampler
    //     fir.reset();
    //     int outc = fir.process(TEST_BUFFER_SIZE, input->readBuf, fir.out.writeBuf);

    //     // Measure amplitude
    //     double maxAmp = 0.0;
    //     for (int i = 0; i < outc; i++) {
    //         double amp = fir.out.writeBuf[i].amplitude();
    //         if (amp > maxAmp) {
    //             maxAmp = amp;
    //         }
    //     }

    //     printf("%lf\n", 20.0 * log10(maxAmp));
    // }

    // return 0;

    return 0;
}