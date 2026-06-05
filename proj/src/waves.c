#include "../include/game/waves.h"
#include "../include/macros/physics.h"

#include <stddef.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define WAVE_MIN_SPEED_EASY 22
#define WAVE_MAX_SPEED_EASY 24 
#define WAVE_MIN_SPEED_NORMAL 24
#define WAVE_MAX_SPEED_NORMAL 26
#define WAVE_MIN_SPEED_HARD 26
#define WAVE_MAX_SPEED_HARD 29

#define WAVE_GRAVITY_EASY PHYSICS_FROM_THOUSANDTHS(500)
#define WAVE_GRAVITY_NORMAL PHYSICS_FROM_THOUSANDTHS(620)
#define WAVE_GRAVITY_HARD PHYSICS_FROM_THOUSANDTHS(780)

#define WAVE_DELAY_EASY_PERCENT 200
#define WAVE_DELAY_NORMAL_PERCENT 150
#define WAVE_DELAY_HARD_PERCENT 100

#define WAVE_MOTION_PROFILE(min_speed, max_speed, gravity_value)               \
  {                                                                            \
    .min_vy = -PHYSICS_FROM_PIXELS(max_speed),                                 \
    .max_vy = -PHYSICS_FROM_PIXELS(min_speed),                                 \
    .gravity = (gravity_value),                                                \
    .min_vx = -PHYSICS_FROM_PIXELS((max_speed) / 10),                          \
    .max_vx = PHYSICS_FROM_PIXELS((max_speed) / 10)                            \
  }

#define WAVE_EVENT(delay, spawn_kind, spawn_amount, score)                     \
  {                                                                            \
    .delay_ticks = (delay),                                                    \
    .request = {                                                               \
      .kind = (spawn_kind),                                                    \
      .sushi_type = WAVE_RANDOM_SUSHI_TYPE,                                    \
      .amount = (spawn_amount),                                                \
      .score_value = (score),                                                  \
    },                                                                         \
  }

typedef struct {
  motion_profile_t motion;
  uint16_t spawn_delay_percent;
} wave_difficulty_profile_t;

static const wave_difficulty_profile_t DIFFICULTY_PROFILES[] = {
  [EASY] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_EASY,
                                  WAVE_MAX_SPEED_EASY,
                                  WAVE_GRAVITY_EASY),
    .spawn_delay_percent = WAVE_DELAY_EASY_PERCENT,
  },
  [NORMAL] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_NORMAL,
                                  WAVE_MAX_SPEED_NORMAL,
                                  WAVE_GRAVITY_NORMAL),
    .spawn_delay_percent = WAVE_DELAY_NORMAL_PERCENT,
  },
  [HARD] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_HARD,
                                  WAVE_MAX_SPEED_HARD,
                                  WAVE_GRAVITY_HARD),
    .spawn_delay_percent = WAVE_DELAY_HARD_PERCENT,
  },
};

static const wave_event_t SINGLE_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 3, 1),
};

static const wave_event_t STAGGERED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(30, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t MIXED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 3, 1),
  WAVE_EVENT(20, WAVE_SPAWN_BOMB, 2, 0),
};

static const wave_t PREDEFINED_WAVES[WAVE_PATTERN_COUNT] = {
  [WAVE_PATTERN_SINGLE] = {
    .pattern = WAVE_PATTERN_SINGLE,
    .events = SINGLE_EVENTS,
    .event_count = ARRAY_COUNT(SINGLE_EVENTS),
    .repeat_count = 1,
    .next_wave_delay_ticks = 60,
  },
  [WAVE_PATTERN_BURST] = {
    .pattern = WAVE_PATTERN_BURST,
    .events = BURST_EVENTS,
    .event_count = ARRAY_COUNT(BURST_EVENTS),
    .repeat_count = 1,
    .next_wave_delay_ticks = 80,
  },
  [WAVE_PATTERN_STAGGERED] = {
    .pattern = WAVE_PATTERN_STAGGERED,
    .events = STAGGERED_EVENTS,
    .event_count = ARRAY_COUNT(STAGGERED_EVENTS),
    .repeat_count = 1,
    .next_wave_delay_ticks = 80,
  },
  [WAVE_PATTERN_MIXED] = {
    .pattern = WAVE_PATTERN_MIXED,
    .events = MIXED_EVENTS,
    .event_count = ARRAY_COUNT(MIXED_EVENTS),
    .repeat_count = 1,
    .next_wave_delay_ticks = 100,
  },
};

static bool wave_repeat_finished(const wave_t *wave, const wave_runtime_t *runtime) {
  return wave->repeat_count != WAVE_FOREVER &&
         runtime->current_repeat + 1 >= wave->repeat_count;
}

static const wave_difficulty_profile_t *waves_get_difficulty_profile(difficulty_t difficulty) {
  if (difficulty < EASY || difficulty > HARD) {
    difficulty = EASY;
  }

  return &DIFFICULTY_PROFILES[difficulty];
}

static uint32_t waves_base_delay(const wave_t *wave) {
  if (wave != NULL && wave->pattern < WAVE_PATTERN_COUNT) {
    return PREDEFINED_WAVES[wave->pattern].next_wave_delay_ticks;
  }

  return wave == NULL ? 0 : wave->next_wave_delay_ticks;
}

static void waves_advance_to_next_wave(wave_runtime_t *runtime, const wave_t *wave,
                                       uint8_t wave_count,
                                       uint32_t current_tick) {
  if (runtime->current_wave + 1 >= wave_count) {
    runtime->current_wave = 0;
  }
  else {
    runtime->current_wave++;
  }

  runtime->current_event = 0;
  runtime->current_repeat = 0;
  runtime->next_wave_start_tick = current_tick + wave->next_wave_delay_ticks;
  runtime->wave_start_tick = runtime->next_wave_start_tick;
}

void waves_init(wave_runtime_t *runtime, uint32_t current_tick) {
  if (runtime == NULL) return;

  runtime->current_wave = 0;
  runtime->current_event = 0;
  runtime->current_repeat = 0;
  runtime->wave_start_tick = current_tick;
  runtime->next_wave_start_tick = current_tick;
  runtime->status = WAVE_STATUS_IDLE;
}

void waves_reset(wave_runtime_t *runtime, uint32_t current_tick) {
  waves_init(runtime, current_tick);
}

wave_status_t waves_update(wave_runtime_t *runtime, const wave_t *waves,
                           uint8_t wave_count, uint32_t current_tick,
                           wave_spawn_request_t *out_request, bool *has_request) {
  if (has_request != NULL) *has_request = false;

  if (runtime == NULL || waves == NULL || out_request == NULL ||
      has_request == NULL || wave_count == 0) {
    if (runtime != NULL) runtime->status = WAVE_STATUS_ERROR;
    return WAVE_STATUS_ERROR;
  }

  if (runtime->status == WAVE_STATUS_FINISHED ||
      runtime->status == WAVE_STATUS_ERROR) {
    return runtime->status;
  }

  runtime->status = WAVE_STATUS_RUNNING;

  while (runtime->current_wave < wave_count) {
    const wave_t *wave = &waves[runtime->current_wave];

    if (wave->event_count == 0 || wave->events == NULL) {
      runtime->status = WAVE_STATUS_ERROR;
      return runtime->status;
    }

    if (current_tick < runtime->next_wave_start_tick) {
      return runtime->status;
    }

    if (runtime->current_event >= wave->event_count) {
      if (wave_repeat_finished(wave, runtime)) {
        waves_advance_to_next_wave(runtime, wave, wave_count, current_tick);
      }
      else {
        if (runtime->current_repeat < UINT8_MAX) runtime->current_repeat++;
        runtime->current_event = 0;
        runtime->wave_start_tick = current_tick;
      }
      continue;
    }

    const wave_event_t *event = &wave->events[runtime->current_event];
    if (current_tick - runtime->wave_start_tick < event->delay_ticks) {
      return runtime->status;
    }

    *out_request = event->request;
    *has_request = true;
    runtime->current_event++;
    return runtime->status;
  }

  runtime->current_wave = 0;
  runtime->current_event = 0;
  runtime->current_repeat = 0;
  runtime->wave_start_tick = current_tick;
  runtime->next_wave_start_tick = current_tick;
  runtime->status = WAVE_STATUS_RUNNING;
  return runtime->status;
}

bool waves_is_finished(const wave_runtime_t *runtime) {
  return runtime == NULL || runtime->status == WAVE_STATUS_FINISHED;
}

const wave_t *waves_get_predefined(wave_pattern_t pattern) {
  if (pattern >= WAVE_PATTERN_COUNT) return NULL;

  return &PREDEFINED_WAVES[pattern];
}

const motion_profile_t *waves_get_difficulty_motion(difficulty_t difficulty) {
  return &waves_get_difficulty_profile(difficulty)->motion;
}

void waves_apply_difficulty(wave_t *waves, uint8_t wave_count, difficulty_t difficulty) {
  if (waves == NULL) return;

  const wave_difficulty_profile_t *profile = waves_get_difficulty_profile(difficulty);

  for (uint8_t i = 0; i < wave_count; i++) {
    uint32_t delay = waves_base_delay(&waves[i]);
    delay = (delay * profile->spawn_delay_percent) / 100;
    if (delay == 0) delay = 1;
    waves[i].next_wave_delay_ticks = delay;
  }
}
