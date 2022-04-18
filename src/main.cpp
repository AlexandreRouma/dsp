#include <stdio.h>
#include <dsp/bench/speed_tester.h>

#include <dsp/demod/fm.h>

#define TEST_BUFFER_SIZE    1250
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

int main() {
    dsp::stream<dsp::complex_t>* input;
    dsp::stream<float>* output;

    // ============= DSP Under Test =============
    input = new dsp::stream<dsp::complex_t>;

    dsp::demod::FM dut(input, 0.5);

    output = &dut.out;
    // ==========================================

    // Run benchmark
    dsp::bench::SpeedTester<dsp::complex_t, float> st(input, output);
    dut.start();
    for (int i = 0; i < TEST_COUNT; i++) {
        dut.reset();
        printf("%lf MS/s\n", st.benchmark(TEST_DURATION, TEST_BUFFER_SIZE) / 1000000.0);
    }
    dut.stop();


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