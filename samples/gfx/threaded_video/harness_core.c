/* Minimal libretro core for the threaded video harness: supports
 * running without content, so the frontend treats it as a real core
 * (the menu can be closed over it, unlike the dummy core), and hands
 * the frontend a frame per retro_run, sometimes duplicated and
 * sometimes taller than the geometry it declared. */
#include <string.h>
#include <stdint.h>
#include <libretro.h>

#define W 320
#define H 240
#define H_OVERSIZE 600

static retro_video_refresh_t video_cb;
static retro_environment_t   environ_cb;
static uint16_t frame[W * H_OVERSIZE];
static unsigned runs;

void retro_set_environment(retro_environment_t cb)
{
   bool no_content = true;
   environ_cb = cb;
   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);
}
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { (void)cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { (void)cb; }
void retro_set_input_poll(retro_input_poll_t cb) { (void)cb; }
void retro_set_input_state(retro_input_state_t cb) { (void)cb; }
void retro_init(void) { }
void retro_deinit(void) { }
unsigned retro_api_version(void) { return RETRO_API_VERSION; }
void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name    = "threaded_video_harness";
   info->library_version = "1";
   info->valid_extensions = "";
}
void retro_get_system_av_info(struct retro_system_av_info *info)
{
   memset(info, 0, sizeof(*info));
   info->timing.fps         = 60.0;
   info->timing.sample_rate = 48000.0;
   info->geometry.base_width   = W;
   info->geometry.base_height  = H;
   info->geometry.max_width    = W;
   info->geometry.max_height   = H;
   info->geometry.aspect_ratio = 4.0f / 3.0f;
}
void retro_set_controller_port_device(unsigned port, unsigned device) { (void)port; (void)device; }
void retro_reset(void) { }
void retro_run(void)
{
   unsigned h = (runs % 61 == 60) ? H_OVERSIZE : H;
   unsigned i;
   runs++;
   for (i = 0; i < W * h; i++)
      frame[i] = (uint16_t)(runs + i);
   if (runs % 3 == 0)
      video_cb(NULL, W, h, W * 2);
   else
      video_cb(frame, W, h, W * 2);
}
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize(void *data, size_t size) { (void)data; (void)size; return false; }
bool retro_unserialize(const void *data, size_t size) { (void)data; (void)size; return false; }
void retro_cheat_reset(void) { }
void retro_cheat_set(unsigned index, bool enabled, const char *code) { (void)index; (void)enabled; (void)code; }
bool retro_load_game(const struct retro_game_info *game) { (void)game; return true; }
bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num) { (void)type; (void)info; (void)num; return false; }
void retro_unload_game(void) { }
unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
void *retro_get_memory_data(unsigned id) { (void)id; return NULL; }
size_t retro_get_memory_size(unsigned id) { (void)id; return 0; }
