#ifndef FFT_ADAPTER_H
#define FFT_ADAPTER_H

#include <stddef.h>

typedef struct FFTAdapter FFTAdapter;

#ifdef __cplusplus
extern "C" {
#endif

FFTAdapter* fft_adapter_create(size_t size);
void fft_adapter_input_samples(FFTAdapter *adapter, const float *samples, size_t size);
void fft_adapter_execute(FFTAdapter *adapter);
const float* fft_adapter_get_frequency_analysis(const FFTAdapter *adapter);
void fft_adapter_destroy(FFTAdapter *adapter);

#ifdef __cplusplus
}
#endif

#endif // FFT_ADAPTER_H