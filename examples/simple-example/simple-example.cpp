#include "Umgebung.h"

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLING_RATE DEFAULT_AUDIO_SAMPLE_RATE

#include "ADSR.h"
#include "Reverb.h"
#include "Wavetable.h"

using namespace umgebung;
using namespace klangwellen;
using namespace std;

class UmgebungApp : public PApplet {

    klangwellen::ADSR      fADSR;
    klangwellen::Wavetable fWavetable{1024, klangwellen::KlangWellen::DEFAULT_SAMPLING_RATE};
    klangwellen::Reverb    fReverb;
    
    bool fIsPlaying = false;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        klangwellen::Wavetable::sawtooth(fWavetable.get_wavetable(), fWavetable.get_wavetable_size(), 5);
        fWavetable.set_frequency(55);
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
            mSample = fReverb.process(mSample);
            for (int j = 0; j < audio_output_channels; ++j) {
                output[j][i] = mSample;
            }
        }
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
