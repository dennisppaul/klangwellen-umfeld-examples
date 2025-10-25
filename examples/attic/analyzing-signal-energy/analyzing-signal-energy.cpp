#include "Umgebung.h"

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLE_RATE DEFAULT_AUDIO_SAMPLE_RATE

#include "DrawLib.h"

#include "ADSR.h"
#include "Reverb.h"
#include "Wavetable.h"
#include "ExponentialMovingAverage.h"
#include "EnvelopeFollower.h"
#include "RootMeanSquare.h"

using namespace umgebung;
using namespace klangwellen;
using namespace std;

class UmgebungApp : public PApplet {

    klangwellen::ADSR                     fADSR;
    klangwellen::Wavetable                fWavetable{1024, klangwellen::KlangWellen::DEFAULT_SAMPLE_RATE};
    klangwellen::Reverb                   fReverb;
    klangwellen::ExponentialMovingAverage fEMA{0.0005};
    RootMeanSquare                        fRMS{1024};
    EnvelopeFollower                      fEF;

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

    void draw() {
        background(1);
        fill(0);
        noStroke();
        circle(width * 0.25, height * 0.5f, 50 + 500 * fEF.get_current());
        circle(width * 0.5, height * 0.5f, 50 + 500 * fEMA.get_current());
        circle(width * 0.75, height * 0.5f, 50 + 500 * fRMS.get_current());

        noFill();
        stroke(0);
        translate(10, height / 2);
        if (fBuffer != nullptr) {
            umgebung::draw_buffer(this,
                                  fBuffer,
                                  fBufferLength, 8,
                                  width - 20,
                                  height - 20);
        }
    }

    void audioblock(float** input, float** output, int length) {
        for (int i = 0; i < length; i++) {
            float mSample = fWavetable.process();
            mSample       = fADSR.process(mSample);
            mSample       = fReverb.process(mSample);
            fEMA.process(mSample);
            fRMS.process(mSample);
            fEF.process(mSample);
            for (int j = 0; j < audio_output_channels; ++j) {
                output[j][i] = mSample;
            }
        }
        fBuffer       = output[0];
        fBufferLength = length;
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
