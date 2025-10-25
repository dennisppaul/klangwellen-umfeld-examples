#include "Umgebung.h"

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLE_RATE DEFAULT_AUDIO_SAMPLE_RATE

#include "ADSR.h"
#include "Reverb.h"
#include "Wavetable.h"

using namespace umgebung;
using namespace klangwellen;
using namespace std;

#include <iostream>
#include <cmath>
#include <vector>

class BandPassFilter {
public:
    BandPassFilter(float low_cutoff, float high_cutoff, float sample_rate) : fSampleRate(sample_rate) {
        x1 = x2 = 0.0;
        y1 = y2 = 0.0;
        set_cutoff_frequencies(low_cutoff, high_cutoff);
    }
    
    void set_cutoff_frequencies(float low_cutoff, float high_cutoff) {
        coeffs = calculateBandPassCoefficients(low_cutoff, high_cutoff, fSampleRate);
    }

    float process(float signal) {
            float x = signal;
            float y = coeffs.b0 * x + coeffs.b1 * x1 + coeffs.b2 * x2 - coeffs.a1 * y1 - coeffs.a2 * y2;

            x2 = x1;
            x1 = x;
            y2 = y1;
            y1 = y;

           return y;
    }

    void process(float* signal, size_t num_samples) {
        for (size_t i = 0; i < num_samples; ++i) {
            float x = signal[i];
            float y = coeffs.b0 * x + coeffs.b1 * x1 + coeffs.b2 * x2 - coeffs.a1 * y1 - coeffs.a2 * y2;

            x2 = x1;
            x1 = x;
            y2 = y1;
            y1 = y;

            signal[i] = y;
        }
    }

private:
    struct BiquadCoefficients {
        float a0, a1, a2;
        float b0, b1, b2;
    };

    BiquadCoefficients coeffs;
    float x1, x2;  // Input states
    float y1, y2;  // Output states
    const float fSampleRate;

    BiquadCoefficients calculateBandPassCoefficients(float low_cutoff, float high_cutoff, float sample_rate) {
        float omega1 = 2.0 * M_PI * low_cutoff / sample_rate;
        float omega2 = 2.0 * M_PI * high_cutoff / sample_rate;
        float alpha = sin((omega2 - omega1) / 2.0) / (2.0 * cos((omega1 + omega2) / 2.0));
        float cos_omega = cos((omega1 + omega2) / 2.0);

        BiquadCoefficients coeffs;
        coeffs.b0 = alpha;
        coeffs.b1 = 0.0;
        coeffs.b2 = -alpha;
        coeffs.a0 = 1.0 + alpha;
        coeffs.a1 = -2.0 * cos_omega;
        coeffs.a2 = 1.0 - alpha;

        // Normalize coefficients
        coeffs.b0 /= coeffs.a0;
        coeffs.b1 /= coeffs.a0;
        coeffs.b2 /= coeffs.a0;
        coeffs.a1 /= coeffs.a0;
        coeffs.a2 /= coeffs.a0;
        coeffs.a0 = 1.0;

        return coeffs;
    }
};

class UmgebungApp : public PApplet {

    klangwellen::ADSR      fADSR;
    klangwellen::Wavetable fWavetable{1024, klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE};
    klangwellen::Reverb    fReverb;
    
    BandPassFilter bandpassfilter{300, 3000, KlangWellen::DEFAULT_SAMPLE_RATE};
    
    bool fIsPlaying   = false;
    float r           = 0;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        klangwellen::Wavetable::square(fWavetable.get_wavetable(), fWavetable.get_wavetable_size());
        fWavetable.set_frequency(22.5*2);
        fWavetable.set_amplitude(0.5);
    }

    void draw() {
        background(1);
        fill(0);
        noStroke();
        if (fIsPlaying) {
            rect(width / 2 - 50, height / 2 - 50, 100, 100);
        } else {
            rect(width / 2 - 25, height / 2 - 25, 50, 50);
        }
    }

    void audioblock(float** input, float** output, int length) {
        for (int i = 0; i < length; i++) {
            float mSample = fWavetable.process();
            mSample = fADSR.process(mSample);
            mSample = bandpassfilter.process(mSample);
            mSample = fReverb.process(mSample);
            mSample = KlangWellen::clamp(mSample);
            for (int j = 0; j < audio_output_channels; ++j) {
                output[j][i] = mSample;
            }
        }
        
        r+=0.3;
        bandpassfilter.set_cutoff_frequencies(abs(sin(r)) * 350 + 50, abs(sin(r * 0.57)) * 2000 + 400);
    }

    void mousePressed() {
        fADSR.start();
        fIsPlaying = true;
    }

    void mouseReleased() {
        fADSR.stop();
        fIsPlaying = false;
    }
};

PApplet *umgebung::instance() {
    return new UmgebungApp();
}
