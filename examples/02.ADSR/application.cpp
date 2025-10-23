#include "Umfeld.h"

#include "KlangWellen.h"
#include "ADSR.h"
#include "Wavetable.h"
#include "BeatDSP.h"

using namespace klangwellen;
using namespace umfeld;

Wavetable wavetable(256, 48000);
ADSR      adsr(48000);
BeatDSP   beat_timer(48000);
void      beat_event(uint32_t beat_counter);

void settings() {
    size(1024, 768);
    audio(0, 2, 48000);
}

void setup() {
    wavetable.set_waveform(KlangWellen::WAVEFORM_SINE);

    adsr.set_attack(0.25f);
    adsr.set_decay(0.125f);
    adsr.set_sustain(0.5f);
    adsr.set_release(0.5f);

    beat_timer.set_callback(beat_event);
    beat_timer.set_bpm(60);
}

void draw() {
    background(0.85);
    fill(1.0, 0.25, 0.35);
    noStroke();

    const float frequency = 55 + 55 * floor(map(mouseY, 0, height, 1, 5));
    wavetable.set_frequency(frequency);

    circle(mouseX, mouseY, frequency);
}

void beat_event(const uint32_t beat_counter) {
    if (beat_counter % 2 == 0) {
        adsr.start();
    } else {
        adsr.stop();
    }
}

void audioEvent(const PAudio& audio) {
    float sample_buffer[audio.buffer_size];
    for (int i = 0; i < audio.buffer_size; ++i) {
        beat_timer.process();
        const float sample = wavetable.process() * adsr.process();
        sample_buffer[i]   = sample;
    }
    if (audio.output_channels == 2) {
        merge_interleaved_stereo(sample_buffer,
                                 sample_buffer,
                                 audio.output_buffer,
                                 audio.buffer_size);
    }
}