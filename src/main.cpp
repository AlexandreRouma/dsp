#include <stdio.h>
#include <dsp/bench/speed_tester.h>
#include <dsp/multirate/polyphase_bank.h>
#include <dsp/window/nuttall.h>
#include <dsp/taps/windowed_sinc.h>
#include <dsp/window/blackman.h>
#include <dsp/window/blackman_harris.h>
#include <dsp/window/blackman_nuttall.h>
#include <dsp/window/hamming.h>
#include <dsp/window/hann.h>
#include <dsp/window/nuttall.h>

#define TEST_BUFFER_SIZE    (2400000 / 200)
#define TEST_DURATION       1000.0
#define TEST_COUNT          5

#define IN_TYPE             dsp::complex_t
#define OUT_TYPE            dsp::stereo_t

int main() {
    int phases = 128;
    double bw = 0.5 / (double)phases;
    dsp::tap<float> lp = dsp::taps::windowedSinc<float>(phases * 8, dsp::math::freqToOmega(bw, 1.0), dsp::window::nuttall, 128.0);
    dsp::multirate::PolyphaseBank bank = dsp::multirate::buildPolyphaseBank<float>(phases, lp);
    for (int i = 0; i < phases; i++) {
        printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", bank.phases[i][0], bank.phases[i][1], bank.phases[i][2], bank.phases[i][3], bank.phases[i][4], bank.phases[i][5], bank.phases[i][6], bank.phases[i][7]);
    }


    // dsp::stream<IN_TYPE>* input;
    // dsp::stream<OUT_TYPE>* output;

    // // ============= DSP Under Test =============
    // input = new dsp::stream<IN_TYPE>;

    // dsp::demod::AM<OUT_TYPE> dut(input, dsp::demod::AM<OUT_TYPE>::AGCMode::CARRIER, 10000.0, 0.001, 0.001, 0.001, 30000.0);

    // output = &dut.out;
    // // ==========================================

    // // dut.setInSamplerate(2400000.0);
    // // dut.setOutSamplerate(24000.0);

    // // Run benchmark
    // dsp::bench::SpeedTester<IN_TYPE, OUT_TYPE> st(input, output);
    // dut.start();
    // for (int i = 0; i < TEST_COUNT; i++) {
    //     dut.reset();
    //     printf("%lf MS/s\n", st.benchmark(TEST_DURATION, TEST_BUFFER_SIZE) / 1000000.0);
    // }
    // dut.stop();

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