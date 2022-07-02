#include <stdio.h>
#include <dsp/bench/speed_tester.h>
#include <dsp/clock_recovery/mm.h>

#define TEST_BUFFER_SIZE    (2400000 / 200)
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

#define IN_TYPE             float
#define OUT_TYPE            float

int main() {
    dsp::stream<IN_TYPE>* input;
    dsp::stream<OUT_TYPE>* output;

    // ============= DSP Under Test =============
    input = new dsp::stream<IN_TYPE>;

    dsp::clock_recovery::MM<float> dut(input, 2.0, 0.000001, 0.02, 0.01);

    output = &dut.out;
    // ==========================================

    // // dut.setInSamplerate(2400000.0);
    // // dut.setOutSamplerate(24000.0);

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