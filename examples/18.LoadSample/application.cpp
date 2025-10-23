#include "Umfeld.h"

#include "KlangWellen.h"
#include "BeatDSP.h"
#include "Sampler.h"

using namespace klangwellen;
using namespace umfeld;

klangwellen::Sampler sampler(48000);
BeatDSP              beat_timer(48000);
void                 beat_event(uint32_t beat_counter);
std::vector<float>   sample_data_buffer;

void settings() {
    size(1024, 768);
    audio(0, 2, 48000);
}

void setup() {
    const std::vector<uint8_t> sample_data_buffer_raw = loadBytes("teilchen.raw");
    sample_data_buffer.resize(sample_data_buffer_raw.size() / sizeof(float));
    std::memcpy(sample_data_buffer.data(), sample_data_buffer_raw.data(), sample_data_buffer_raw.size());
    sampler.set_buffer(sample_data_buffer.data(), sample_data_buffer.size());
    sampler.play();

    beat_timer.set_callback(beat_event);
    beat_timer.set_bpm(120 * 4);
}

void beat_event(const uint32_t beat_counter) {
    if (beat_counter % 16 == 0) {
        sampler.set_speed(beat_counter / 16 % 4 * 0.25 + 0.25f);
        sampler.rewind();
        sampler.play();
    }
}

void draw() {
    background(0.85);
    fill(1.0, 0.25, 0.35);
    noStroke();

    circle(mouseX, mouseY, 100);
}


void audioEvent(const PAudio& audio) {
    float sample_buffer[audio.buffer_size];
    for (int i = 0; i < audio.buffer_size; ++i) {
        beat_timer.process();
        const float sample = sampler.process();
        sample_buffer[i]   = sample;
    }
    if (audio.output_channels == 2) {
        merge_interleaved_stereo(sample_buffer,
                                 sample_buffer,
                                 audio.output_buffer,
                                 audio.buffer_size);
    }
}
