#include <stdio.h>
#include <dsp/bench/speed_tester.h>
#include <dsp/multirate/rrc_interpolator.h>

#define TEST_BUFFER_SIZE    (2400000 / 200)
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

#define IN_TYPE             dsp::complex_t
#define OUT_TYPE            dsp::complex_t

int main() {
    dsp::stream<IN_TYPE>* input;
    dsp::stream<OUT_TYPE>* output;

    // ============= DSP Under Test =============
    input = new dsp::stream<IN_TYPE>;

    dsp::multirate::RRCInterpolator<dsp::complex_t> dut(input, 250000.0, 1000000.0, 0.6, 31);

    output = &dut.out;
    // ==========================================

    // Run benchmark
    dsp::bench::SpeedTester<IN_TYPE, OUT_TYPE> st(input, output);
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
    //     dut.reset();
    //     int outc = dut.process(TEST_BUFFER_SIZE, input->readBuf, dut.out.writeBuf);

    //     // Measure amplitude
    //     double maxAmp = 0.0;
    //     for (int i = 0; i < outc; i++) {
    //         double amp = dut.out.writeBuf[i].amplitude();
    //         if (amp > maxAmp) {
    //             maxAmp = amp;
    //         }
    //     }

    //     printf("%lf %lf\n", f, 20.0 * log10(maxAmp));
    // }

    return 0;
}