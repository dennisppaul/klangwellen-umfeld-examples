#include "Umfeld.h"

#include "KlangWellen.h"
#include "BeatDSP.h"
#include "SAM.h"
#include "Vocoder.h"
#include "Wavetable.h"

using namespace klangwellen;
using namespace umfeld;

SAM       sam(48000);
Wavetable wavetable(256, 48000);
Vocoder   vocoder(48000, 13, 3);
BeatDSP   beat_timer(48000);

float* modulator_buffer;
float* carrier_buffer;
void   beat_event(uint32_t beat_counter);

void settings() {
    size(1024, 768);
    audio(0, 2, 48000);
}

void setup() {
    modulator_buffer = new float[audio_device->buffer_size];
    carrier_buffer   = new float[audio_device->buffer_size];

    sam.speak("hello world");

    wavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH);
    wavetable.set_frequency(55);
    wavetable.set_amplitude(1.0);

    vocoder.set_reaction_time(0.03);
    vocoder.set_formant_shift(1.0);
    vocoder.set_volume(4);

    beat_timer.set_callback(beat_event);
    beat_timer.set_bpm(80);
}

void beat_event(const uint32_t beat_counter) {
    wavetable.set_frequency(27.5 * KlangWellen::pow(2, beat_counter % 4));
    sam.speak_from_buffer();
}

void draw() {
    background(0.85);
    fill(1.0, 0.25, 0.35);
    noStroke();

    circle(mouseX, mouseY, 100);
}


void audioEvent(const PAudio& audio) {
    float sample_buffer[audio.buffer_size];
    sam.process(modulator_buffer, audio.buffer_size);
    wavetable.process(carrier_buffer, audio.buffer_size);
    vocoder.process(carrier_buffer, modulator_buffer, sample_buffer, audio.buffer_size);
    beat_timer.process(sample_buffer, audio.buffer_size);
    if (audio.output_channels == 2) {
        merge_interleaved_stereo(sample_buffer,
                                 sample_buffer,
                                 audio.output_buffer,
                                 audio.buffer_size);
    }
}
