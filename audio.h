#include "piano.h"

void audio_init(void);
void audio_try_push_samples(void (*fill_sample_buf)(int16_t*, int));
