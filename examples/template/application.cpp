/*
 * this example demonstrates how to load a sample and apply a low pass filter
 * to it. it also shows how to resample a sample to a different sample rate.
 */

#include "Umfeld.h"
#include "KlangWellen.h"

using namespace umfeld;
using namespace klangwellen;

void settings() {
    size(1024, 768);
    audio();
}

void setup() {}

void draw() {
    background(0.85);
    fill(1.0, 0.25, 0.35);
    noStroke();
    
    circle(mouseX, mouseY, 100);
}

void audioEvent(const PAudio& audio) {
    float sample_buffer[audio.buffer_size];
    for (int i = 0; i < audio.buffer_size; i++) {
        float sample     = 0.0;
        sample_buffer[i] = sample;
    }
    if (audio.output_channels == 2) {
        merge_interleaved_stereo(sample_buffer, sample_buffer, audio.output_buffer, audio.buffer_size);
    }
}
