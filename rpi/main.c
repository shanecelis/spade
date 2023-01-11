#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define yell puts
#if 1
  #define dbg puts
  #define dbgf printf
#else
  #define dbg(...) ;
  #define dbgf(...) ;
#endif

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"
/* #include "jerry_mem.h" */

#ifdef SPADE_AUDIO
#include "audio.h"
#endif
#include "ST7735_TFT.h"
#include "upload.h"

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
char errorbuf[512] = "";

/* #include "base_engine.c" */

/* #include "jerryscript.h" */
/* #include "jerryxx.h" */

/* jumbo builds out of laziness */
/* static void module_native_init(jerry_value_t exports); */
/* #include "js.h" */
/* #include "module_native.c" */

#define PER_CHAR (255)

#define SCREEN_SIZE_X (160)
#define SCREEN_SIZE_Y (128)
  /* typedef struct { uint8_t rgba[4]; } Color; */

  /* #define color16(r, g, b) ((Color) { r, g, b, 255 }) */

    static uint16_t color16(uint8_t r, uint8_t b, uint8_t g) {
      r = (uint8_t)((float)((float)r / 255.0f) * 31.0f);
      g = (uint8_t)((float)((float)g / 255.0f) * 31.0f);
      b = (uint8_t)((float)((float)b / 255.0f) * 63.0f);

      // return ((r & 0xf8) << 8) + ((g & 0xfc) << 3) + (b >> 3);
      return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
    }

    typedef uint16_t Color;

static void render_errorbuf(void) {
  int y = 0;
  int x = 0;
  for (int i = 0; i < sizeof(errorbuf); i++) {
    if (errorbuf[i] == '\0') break;
    if (errorbuf[i] == '\n' || x >= (SCREEN_SIZE_X / 8)) {
      y++;
      x = 0;
      if (errorbuf[i] == '\n') continue;
    }
    if (y >= (SCREEN_SIZE_Y / 8)) break;
    /* state->text_color[y][x] = color16(255, 0, 0); */
    /* state->text_char [y][x] = errorbuf[i]; */
    x++;
  }
}
typedef struct { int x, y, width, height, scale; } be_Rect;

static void render_calc_bounds(be_Rect *rect) {
  rect->width = SCREEN_SIZE_X;
  rect->height = SCREEN_SIZE_Y;

  rect->x = 0;
  rect->y = 0;
}


static Color render_pixel(be_Rect *game, int x, int y) {
  return color16(255,0,0);
}
static void render(void (*write_pixel)(int x, int y, Color c)) {
  be_Rect rect = {0};
  render_calc_bounds(&rect);

  for (int x = 0; x < 160; x++)
    for (int y = 0; y < 128; y++)
      write_pixel(x, y, render_pixel(&rect, x, y));
}

/* permanent loop rendering errbuf */
static void write_pixel(int x, int y, Color c);
static void fatal_error() {
  while (1) {
    /* text_clear(); */
    render_errorbuf();
    st7735_fill_start();
      render(write_pixel);
    st7735_fill_finish();
  }
}


#define HISTORY_LEN (64)
typedef struct {
  uint8_t history[HISTORY_LEN/8];
  uint8_t last_state, ring_i;
} ButtonState;
uint button_pins[] = {  5,  7,  6,  8, 12, 14, 13, 15 };
static ButtonState button_states[ARR_LEN(button_pins)] = {0};

static uint8_t button_history_read(ButtonState *bs, int i) {
  int q = 1 << (i % 8);
  return !!(bs->history[i/8] & q);
}
static void button_history_write(ButtonState *bs, int i, uint8_t v) {
  if (v)
    bs->history[i/8] |=   1 << (i % 8) ;
  else
    bs->history[i/8] &= ~(1 << (i % 8));
}

static void button_init(void) {
  for (int i = 0; i < ARR_LEN(button_pins); i++) {
    ButtonState *bs = button_states + i;

    gpio_set_dir(button_pins[i], GPIO_IN);
    gpio_pull_up(button_pins[i]);
    // gpio_set_input_hysteresis_enabled(button_pins[i], 1);
    // gpio_set_slew_rate(button_pins[i], GPIO_SLEW_RATE_SLOW);
    // gpio_disable_pulls(button_pins[i]);
  }
}

static void button_poll(void) {
  for (int i = 0; i < ARR_LEN(button_pins); i++) {
    ButtonState *bs = button_states + i;

    bs->ring_i = (bs->ring_i + 1) % HISTORY_LEN;
    button_history_write(bs, bs->ring_i, gpio_get(button_pins[i]));

    /* down is true if more than half are true */
    int down = 0;
    for (int i = 0; i < HISTORY_LEN; i++)
      down += button_history_read(bs, i);
    down = down > ((HISTORY_LEN*5)/6);

    if (down != bs->last_state) {
      bs->last_state = down;

      // spade_call_press(button_pins[i]);

      // queue_add_blocking(&button_queue, &(ButtonPress) { .pin = button_pins[i] });
      if (!down) multicore_fifo_push_blocking(button_pins[i]);

      //      if (button_pins[i] == 8) map_move(map_get_first('p'),  1,  0);
      // else if (button_pins[i] == 6) map_move(map_get_first('p'), -1,  0);
      // else if (button_pins[i] == 7) map_move(map_get_first('p'),  0,  1);
      // else if (button_pins[i] == 5) map_move(map_get_first('p'),  0, -1);
    }
  }
}

static void power_lights() {
  // left white light
  const pin_num_0 = 28;
  gpio_set_function(pin_num_0, GPIO_FUNC_PWM);
  uint slice_num_0 = pwm_gpio_to_slice_num(pin_num_0);
  pwm_set_enabled(slice_num_0, true);
  pwm_set_gpio_level(pin_num_0, 65535/8);

  // right blue light
  // const pin_num_1 = 4;
  // gpio_set_function(pin_num_1, GPIO_FUNC_PWM);
  // uint slice_num_1 = pwm_gpio_to_slice_num(pin_num_1);
  // pwm_set_enabled(slice_num_1, true);
  // pwm_set_gpio_level(pin_num_1, 65535/4);
}

static void core1_entry(void) {
  button_init();

  while (1) {
    button_poll();
  }
}

static void game_init(void) {
}

static int load_new_scripts(void) {
  return upl_stdin_read();
}

#if 0 //def SPADE_AUDIO
void piano_jerry_song_free(void *p) {
  /* it's straight up a jerry_value_t, not even a pointer to one */
  jerry_value_t jvt = (jerry_value_t)p;

  jerry_release_value(jvt);
}
int piano_jerry_song_chars(void *p, char *buf, int buf_len) {
  /* it's straight up a jerry_value_t, not even a pointer to one */
  jerry_value_t jvt = (jerry_value_t)p;

  int read = jerry_string_to_char_buffer(jvt, (jerry_char_t *)buf, (jerry_size_t) buf_len);
  return read;
}
#endif

static void write_pixel(int x, int y, Color c) {
  (void *)x;
  (void *)y;
  st7735_fill_send(c);
}

int main() {
  power_lights();

  stdio_init_all();

  st7735_init();

  /* jerry_init (JERRY_INIT_MEM_STATS); */
  /* init(sprite_free_jerry_object); /\* gosh i should namespace base engine *\/ */

  while(!save_read()) {
    strcpy(errorbuf, "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "    PLEASE UPLOAD   \n"
                     "       A GAME       \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     " sprig.hackclub.dev \n");
    render_errorbuf();
    st7735_fill_start();
      render(write_pixel);
    st7735_fill_finish();

    /* load_new_scripts(); */
  }

  multicore_launch_core1(core1_entry);

  /* drain keypresses */
  /* what really needs to be done here is to have button_init
   * record when it starts so that we can use that timestamp to
   * ignore these fake startup keypresses */
  sleep_ms(50);
  while (multicore_fifo_rvalid()) multicore_fifo_pop_blocking();

  while(!multicore_fifo_rvalid()) {
    strcpy(errorbuf, "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "    PRESS ANYTHING  \n"
                     "       TO RUN       \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     "                    \n"
                     " sprig.hackclub.dev \n");
    render_errorbuf();
    st7735_fill_start();
      render(write_pixel);
    st7735_fill_finish();

    load_new_scripts();
  }
  memset(errorbuf, 0, sizeof(errorbuf));
  /* text_clear(); */

  /* drain keypresses */
  while (multicore_fifo_rvalid()) multicore_fifo_pop_blocking();

  /* init js */
  /* js_run(save_read(), strlen(save_read())); */

#if 0 //def SPADE_AUDIO
  piano_init((PianoOpts) {
    .song_free = piano_jerry_song_free,
    .song_chars = piano_jerry_song_chars,
  });
  audio_init();
#endif

  absolute_time_t last = get_absolute_time();
  dbg("okay launching game loop");
  while(1) {
    /* input handling */
    /* puts("please tell me it's not the fifo"); */
    /* while (multicore_fifo_rvalid()) */
    /*   spade_call_press(multicore_fifo_pop_blocking()); */

    /* setTimeout/setInterval impl */
    absolute_time_t now = get_absolute_time();
    int elapsed = us_to_ms(absolute_time_diff_us(last, now));
    last = now;
    puts("frame?");
    /* spade_call_frame(elapsed); */

    /* puts("promises?"); */
    /* js_promises(); */

#if SPADE_AUDIO
    audio_try_push_samples();
#endif

    /* upload new scripts */
    /* puts("not load new scripts surely?"); */
    /* if (load_new_scripts()) break; */

    /* render */
    puts("uhh rendering? lol");
    render_errorbuf();
    st7735_fill_start();
      render(write_pixel);
    st7735_fill_finish();
  }

  strcpy(errorbuf, "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "    PLEASE REBOOT   \n"
                   "     YOUR SPRIG     \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   "                    \n"
                   " sprig.hackclub.dev \n");

  render_errorbuf();
  st7735_fill_start();
    render(write_pixel);
  st7735_fill_finish();

  watchdog_enable(0, false);
  while (1) {}
}
