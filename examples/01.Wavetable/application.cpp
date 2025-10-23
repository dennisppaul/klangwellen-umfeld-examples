#include "Umfeld.h"

#include "KlangWellen.h"
#include "Wavetable.h"

using namespace klangwellen;
using namespace umfeld;

Wavetable wavetable(256, 48000);

void settings() {
    size(1024, 768);
    audio(0, 2, 48000);
}

void setup() {
    wavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH, 5);
    wavetable.set_frequency(55);
    wavetable.set_amplitude(0.25);
}

void draw() {
    background(0.85);
    fill(1.0, 0.25, 0.35);
    noStroke();

    const float frequency = 55 + 55 * floor(map(mouseY, 0, height, 1, 5));
    wavetable.set_frequency(frequency);

    circle(mouseX, mouseY, frequency);
}

void audioEvent(const PAudio& audio) {
    float sample_buffer[audio.buffer_size];
    for (int i = 0; i < audio.buffer_size; ++i) {
        const float sample = wavetable.process();
        sample_buffer[i]   = sample;
    }
    if (audio.output_channels == 2) {
        merge_interleaved_stereo(sample_buffer,
                                 sample_buffer,
                                 audio.output_buffer,
                                 audio.buffer_size);
    }
}