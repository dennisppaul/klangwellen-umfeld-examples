#include "fft_adapter.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define which FFT library to use here
// #define USE_FFTW
// #define USE_KISSFFT
// #define USE_CMSIS_DSP

#ifdef USE_FFTW
#include <fftw3.h>
#elif defined(USE_KISSFFT)
#include "kiss_fft.h"
#include "kiss_fftr.h"
#elif defined(USE_CMSIS_DSP)
#include "arm_math.h"
#else
#error "no FFT library selected"
#endif

struct FFTAdapter {
#ifdef USE_FFTW
    fftw_plan plan;
    double *in;
    fftw_complex *out;
#elif defined(USE_KISSFFT)
    kiss_fftr_cfg cfg;
    kiss_fft_cpx *out;
    kiss_fft_scalar *in;
#elif defined(USE_CMSIS_DSP)
    arm_rfft_fast_instance_f32 S;
    float32_t *in;
    float32_t *out;
#endif
    float *magnitude;
    size_t size;
};

FFTAdapter* fft_adapter_create(size_t size) {
    FFTAdapter *adapter = (FFTAdapter*)malloc(sizeof(FFTAdapter));
    if (!adapter) return NULL;
    adapter->size = size;

#ifdef USE_FFTW
    adapter->in = (double*)fftw_malloc(sizeof(double) * size);
    adapter->out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (size/2 + 1));
    adapter->plan = fftw_plan_dft_r2c_1d(size, adapter->in, adapter->out, FFTW_ESTIMATE);
#elif defined(USE_KISSFFT)
    adapter->in = (kiss_fft_scalar*)malloc(sizeof(kiss_fft_scalar) * size);
    adapter->out = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * (size/2 + 1));
    adapter->cfg = kiss_fftr_alloc(size, 0, NULL, NULL);
#elif defined(USE_CMSIS_DSP)
    adapter->in = (float32_t*)malloc(sizeof(float32_t) * size);
    adapter->out = (float32_t*)malloc(sizeof(float32_t) * size);
    arm_rfft_fast_init_f32(&adapter->S, size);
#endif

    adapter->magnitude = (float*)malloc(sizeof(float) * (size / 2 + 1));

    return adapter;
}

void fft_adapter_input_samples(FFTAdapter *adapter, const float *samples, size_t size) {
    if (size > adapter->size) size = adapter->size;
#ifdef USE_FFTW
    for (size_t i = 0; i < size; ++i) {
        adapter->in[i] = (double)samples[i]; // Convert float to double
    }
#else
    memcpy(adapter->in, samples, size * sizeof(float));
#endif
}

void fft_adapter_execute(FFTAdapter *adapter) {
#ifdef USE_FFTW
    fftw_execute(adapter->plan);
    for (size_t i = 0; i < adapter->size / 2 + 1; ++i) {
        adapter->magnitude[i] = sqrt(adapter->out[i][0] * adapter->out[i][0] + adapter->out[i][1] * adapter->out[i][1]);
    }
#elif defined(USE_KISSFFT)
    kiss_fftr(adapter->cfg, adapter->in, adapter->out);
    for (size_t i = 0; i < adapter->size / 2 + 1; ++i) {
        adapter->magnitude[i] = sqrt(adapter->out[i].r * adapter->out[i].r + adapter->out[i].i * adapter->out[i].i);
    }
#elif defined(USE_CMSIS_DSP)
    arm_rfft_fast_f32(&adapter->S, adapter->in, adapter->out, 0);
    arm_cmplx_mag_f32(adapter->out, adapter->magnitude, adapter->size / 2 + 1);
#endif
}

const float* fft_adapter_get_frequency_analysis(const FFTAdapter *adapter) {
    return adapter->magnitude;
}

void fft_adapter_destroy(FFTAdapter *adapter) {
    if (!adapter) return;

#ifdef USE_FFTW
    fftw_destroy_plan(adapter->plan);
    fftw_free(adapter->in);
    fftw_free(adapter->out);
#elif defined(USE_KISSFFT)
    free(adapter->cfg);
    free(adapter->in);
    free(adapter->out);
#elif defined(USE_CMSIS_DSP)
    free(adapter->in);
    free(adapter->out);
#endif

    free(adapter->magnitude);
    free(adapter);
}