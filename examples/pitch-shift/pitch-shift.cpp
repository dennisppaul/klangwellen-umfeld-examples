#include "smbPitchShift.h"
#include "Umgebung.h"

#define KLANG_SAMPLES_PER_AUDIO_BLOCK DEFAULT_FRAMES_PER_BUFFER
#define KLANG_SAMPLE_RATE DEFAULT_AUDIO_SAMPLE_RATE

#include "KlangWellen.h"
#include "AudioFileReader.h"

using namespace umgebung;
using namespace klangwellen;
using namespace std;

class UmgebungApp : public PApplet {

    AudioFileReader fAudioFileReader;

    bool fIsPitchShifting = true;

    void settings() {
        size(1024, 768);
    }

    void setup() {
        fAudioFileReader.open("../umgebung-aus-wellen.wav");
        std::cout << "sample_rate: " << fAudioFileReader.sample_rate() << std::endl;
        std::cout << "channels   : " << fAudioFileReader.channels() << std::endl;
        std::cout << "length     : " << fAudioFileReader.length() << std::endl;
    }

    void draw() {
        background(1);
        fill(0);
        noStroke();
        if (fIsPitchShifting) {
            rect(width / 2 - 50, height / 2 - 50, 100, 100);
        } else {
            rect(width / 2 - 25, height / 2 - 25, 50, 50);
        }
    }

    void audioblock(float** input, float** output, const int length) {
        fAudioFileReader.read(length, output[0], AudioFileReader::ReadStyle::LOOP);

        if (fIsPitchShifting) {
            // constexpr long semitones  = 3;                        // shift up by 3 semitones
            // const float    pitchShift = pow(2., semitones / 12.); // convert semitones to factor
            const float pitchShift = map(mouseX, 0, width, 0.0, 5.0);
            smbPitchShift(pitchShift,
                          length,
                          length,
                          8,
                          DEFAULT_AUDIO_SAMPLE_RATE,
                          output[0],
                          output[0]);
        }

        for (int j = 1; j < audio_output_channels; ++j) {
            KlangWellen::copy(output[0], output[j], length);
        }
    }

    void mousePressed() {
        fIsPitchShifting = false;
    }

    void mouseReleased() {
        fIsPitchShifting = true;
    }
};

PApplet* umgebung::instance() {
    return new UmgebungApp();
}
