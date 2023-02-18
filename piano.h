#ifndef __cplusplus
float strtof(const char *restrict nptr, char **restrict endptr);
#endif
#include <stdint.h>

/* gaps in your audio? increase this */
/* #define SAMPLES_PER_BUFFER (256*8) */
#define SAMPLES_PER_BUFFER (256*8)
#define SAMPLES_PER_SECOND 24000
/* #define SAMPLES_PER_SECOND 22050 */
/* #define SAMPLES_PER_SECOND 12000 */
/* #define SAMPLES_PER_SECOND 6000 */
/* #define SAMPLES_PER_SECOND 44100 */

typedef struct {
  void (*song_free)(void *);
  int (*song_chars)(void *p, char *buf, int buf_len);
} PianoOpts;

void piano_init(PianoOpts);
int piano_fill_sample_buf(int16_t *samples, int size);

int  piano_queue_song(void *, double times);
int  piano_unqueue_song(void *p);
int  piano_is_song_queued(void *p);
