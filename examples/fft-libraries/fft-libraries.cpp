#include "Umgebung.h"

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLING_RATE DEFAULT_AUDIO_SAMPLE_RATE

#include "KlangWellen.h"
#include "DrawLib.h"

#include "ADSR.h"
#include "Reverb.h"
#include "Wavetable.h"
#include "fft_adapter.h"

using namespace umgebung;
using namespace klangwellen;
using namespace std;

class UmgebungApp : public PApplet {

    klangwellen::ADSR                     fADSR;
    klangwellen::Wavetable                fWavetable{1024, klangwellen::KlangWellen::DEFAULT_SAMPLING_RATE};
    klangwellen::Reverb                   fReverb;

    const static size_t FFT_BUFFER_SIZE = DEFAULT_FRAMES_PER_BUFFER;
    FFTAdapter *fft_analysis = fft_adapter_create(FFT_BUFFER_SIZE);

    float samples[FFT_BUFFER_SIZE];
    float samples_window[FFT_BUFFER_SIZE];

    float* fBuffer       = nullptr;
    int    fBufferLength = 0;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        klangwellen::Wavetable::sawtooth(fWavetable.get_wavetable(), fWavetable.get_wavetable_size(), 5);
        fWavetable.set_frequency(55);
        fWavetable.set_amplitude(0.5);
    }

    void perform_and_draw_fft(float *audio_samples) {
        fft_adapter_input_samples(fft_analysis, audio_samples, FFT_BUFFER_SIZE);
        fft_adapter_execute(fft_analysis);

        float frequencies[FFT_BUFFER_SIZE / 2 + 1];
        calculate_frequencies(frequencies, FFT_BUFFER_SIZE, DEFAULT_AUDIO_SAMPLE_RATE);

        const float *freq_analysis = fft_adapter_get_frequency_analysis(fft_analysis);
        for (size_t i = 0; i < FFT_BUFFER_SIZE / 2 + 1; i+=4) {
            stroke(255, 0, 0);
            {
                float x = map(log(frequencies[i]), log(20), log(20000), 20, width - 20);
                float y = map(freq_analysis[i], 0, 10, 0, 300);
                line(x, height * 0.5f, x, height * 0.5f + 2 - y);
            }
            stroke(0, 255, 0);
            {
                float x = map((float)i, 0, FFT_BUFFER_SIZE / 2 + 1, 20, width - 20);
                float y = map(freq_analysis[i], 0, 10, 0, 300);
                line(x, height * 0.5f, x, height * 0.5f + 2 - y);
            }
        }
    }

    void draw() {
        background(1);
        fill(0);
        noStroke();

        noFill();
        stroke(0);
        pushMatrix();
        translate(10, height / 2);
        if (fBuffer != nullptr) {
            umgebung::draw_buffer(this,
                                  fBuffer,
                                  fBufferLength, 8,
                                  width - 20,
                                  height - 20);
        }
        popMatrix();

        // draw frequency analysis
        noFill();
        perform_and_draw_fft(samples);
        translate(3, height * 0.33f);
        perform_and_draw_fft(samples_window);
    }

    void calculate_frequencies(float *frequencies, size_t size, float sample_rate) {
        for (size_t i = 0; i < size / 2 + 1; ++i) {
            frequencies[i] = i * sample_rate / size;
        }
    }

    void apply_hann_window(float *signal, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            signal[i] *= 0.5 * (1.0 - cos(2.0 * M_PI * i / (size - 1)));
        }
    }

    void audioblock(float** input, float** output, int length) {
        for (int i = 0; i < length; i++) {
            float mSample = fWavetable.process();
            mSample       = fADSR.process(mSample);
            mSample       = fReverb.process(mSample);
            for (int j = 0; j < audio_output_channels; ++j) {
                output[j][i] = mSample;
            }
        }
        fBuffer       = output[0];
        fBufferLength = length;

        KlangWellen::copy(output[0], samples, FFT_BUFFER_SIZE);
        KlangWellen::copy(output[0], samples_window, FFT_BUFFER_SIZE);
        apply_hann_window(samples_window, FFT_BUFFER_SIZE);

        // const float *freq_analysis = fft_adapter_get_frequency_analysis(fft_analysis);

        // float frequencies[FFT_BUFFER_SIZE / 2 + 1];
        // calculate_frequencies(frequencies, FFT_BUFFER_SIZE, DEFAULT_AUDIO_SAMPLE_RATE);

        // for (size_t i = 0; i < FFT_BUFFER_SIZE / 2 + 1; i+=4) {
        //     printf("%.0fHz:%.2f ", frequencies[i], freq_analysis[i]);
        // }
        // printf("\r\n");
    }

    void mousePressed() {
        fADSR.start();
    }

    void mouseReleased() {
        fADSR.stop();
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}
