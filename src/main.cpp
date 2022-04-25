#include <stdio.h>
#include <dsp/bench/speed_tester.h>

#include <dsp/loop/pll.h>

#include <dsp/taps/low_pass.h>
#include <dsp/taps/band_pass.h>
#include <dsp/taps/high_pass.h>

#define TEST_BUFFER_SIZE    64000
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

int main() {
    //dsp::tap<float> taps = dsp::taps::bandPass<float>(200.0, 1500.0, 50.0, 3000.0);

    //dsp::tap<float> taps = dsp::taps::lowPass(750.0/2.0, 50.0, 3000.0);

    dsp::tap<float> taps = dsp::taps::highPass(750.0, 50.0, 3000.0);

    for (int i = 0; i < taps.size; i++) {
        printf("%0.15f\n", (float)taps.taps[i]);
    }

    return 0;

    dsp::stream<dsp::complex_t>* input;
    dsp::stream<dsp::complex_t>* output;

    // ============= DSP Under Test =============
    input = new dsp::stream<dsp::complex_t>;

    dsp::loop::PLL dut(input, 0.0001);

    output = &dut.out;
    // ==========================================


    // Run benchmark
    dsp::bench::SpeedTester<dsp::complex_t, dsp::complex_t> st(input, output);
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