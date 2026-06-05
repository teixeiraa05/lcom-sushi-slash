#include "../include/game/waves.h"
#include "../include/macros/physics.h"

#include <stddef.h>
#include <stdlib.h>

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

#define WAVE_FIRST_DELAY_TICKS 120
#define WAVE_GAP_MIN_TICKS 60
#define WAVE_GAP_MAX_TICKS 180

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
} wave_difficulty_profile_t;

static const wave_difficulty_profile_t DIFFICULTY_PROFILES[] = {
  [EASY] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_EASY,
                                  WAVE_MAX_SPEED_EASY,
                                  WAVE_GRAVITY_EASY),
  },
  [NORMAL] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_NORMAL,
                                  WAVE_MAX_SPEED_NORMAL,
                                  WAVE_GRAVITY_NORMAL),
  },
  [HARD] = {
    .motion = WAVE_MOTION_PROFILE(WAVE_MIN_SPEED_HARD,
                                  WAVE_MAX_SPEED_HARD,
                                  WAVE_GRAVITY_HARD),
  },
};

static const wave_event_t INTRO_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t SINGLE_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t BURST_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 3, 1),
};

static const wave_event_t BURST_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_event_t STAGGERED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t STAGGERED_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 1, 1),
};

static const wave_event_t MIXED_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t MIXED_BOMB_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 2, 0),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
};

static const wave_event_t HARD_MIX_EVENTS[] = {
  WAVE_EVENT(0, WAVE_SPAWN_BOMB, 2, 0),
  WAVE_EVENT(0, WAVE_SPAWN_REGULAR, 1, 1),
  WAVE_EVENT(60, WAVE_SPAWN_REGULAR, 2, 1),
  WAVE_EVENT(60, WAVE_SPAWN_BOMB, 1, 0),
};

static const wave_t PREDEFINED_WAVES[WAVE_PATTERN_COUNT] = {
  [WAVE_PATTERN_INTRO] = {
    .pattern = WAVE_PATTERN_INTRO,
    .events = INTRO_EVENTS,
    .event_count = ARRAY_COUNT(INTRO_EVENTS),
  },
  [WAVE_PATTERN_SINGLE] = {
    .pattern = WAVE_PATTERN_SINGLE,
    .events = SINGLE_EVENTS,
    .event_count = ARRAY_COUNT(SINGLE_EVENTS),
  },
  [WAVE_PATTERN_BOMB] = {
    .pattern = WAVE_PATTERN_BOMB,
    .events = BOMB_EVENTS,
    .event_count = ARRAY_COUNT(BOMB_EVENTS),
  },
  [WAVE_PATTERN_BURST] = {
    .pattern = WAVE_PATTERN_BURST,
    .events = BURST_EVENTS,
    .event_count = ARRAY_COUNT(BURST_EVENTS),
  },
  [WAVE_PATTERN_BURST_BOMB] = {
    .pattern = WAVE_PATTERN_BURST_BOMB,
    .events = BURST_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(BURST_BOMB_EVENTS),
  },
  [WAVE_PATTERN_STAGGERED] = {
    .pattern = WAVE_PATTERN_STAGGERED,
    .events = STAGGERED_EVENTS,
    .event_count = ARRAY_COUNT(STAGGERED_EVENTS),
  },
  [WAVE_PATTERN_STAGGERED_BOMB] = {
    .pattern = WAVE_PATTERN_STAGGERED_BOMB,
    .events = STAGGERED_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(STAGGERED_BOMB_EVENTS),
  },
  [WAVE_PATTERN_MIXED] = {
    .pattern = WAVE_PATTERN_MIXED,
    .events = MIXED_EVENTS,
    .event_count = ARRAY_COUNT(MIXED_EVENTS),
  },
  [WAVE_PATTERN_MIXED_BOMB] = {
    .pattern = WAVE_PATTERN_MIXED_BOMB,
    .events = MIXED_BOMB_EVENTS,
    .event_count = ARRAY_COUNT(MIXED_BOMB_EVENTS),
  },
  [WAVE_PATTERN_HARD_MIX] = {
    .pattern = WAVE_PATTERN_HARD_MIX,
    .events = HARD_MIX_EVENTS,
    .event_count = ARRAY_COUNT(HARD_MIX_EVENTS),
  },
};

static const uint8_t EASY_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BOMB,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_STAGGERED_BOMB,
};

static const uint8_t NORMAL_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BOMB,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_BURST_BOMB,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_MIXED,
  WAVE_PATTERN_MIXED_BOMB,
};

static const uint8_t HARD_POOL[] = {
  WAVE_PATTERN_SINGLE,
  WAVE_PATTERN_BURST,
  WAVE_PATTERN_STAGGERED,
  WAVE_PATTERN_STAGGERED_BOMB,
  WAVE_PATTERN_MIXED,
  WAVE_PATTERN_MIXED_BOMB,
  WAVE_PATTERN_HARD_MIX,
};

static const wave_difficulty_profile_t *get_profile(difficulty_t difficulty) {
  if (difficulty < EASY || difficulty > HARD) {
    difficulty = EASY;
  }

  return &DIFFICULTY_PROFILES[difficulty];
}

static uint32_t rand_gap(void) {
  return WAVE_GAP_MIN_TICKS +
         (uint32_t)(rand() % (WAVE_GAP_MAX_TICKS - WAVE_GAP_MIN_TICKS + 1));
}

static uint8_t pick_wave(difficulty_t difficulty, uint8_t wave_count) {
  const uint8_t *pool = EASY_POOL;
  uint8_t count = ARRAY_COUNT(EASY_POOL);

  if (difficulty == NORMAL) {
    pool = NORMAL_POOL;
    count = ARRAY_COUNT(NORMAL_POOL);
  }
  else if (difficulty == HARD) {
    pool = HARD_POOL;
    count = ARRAY_COUNT(HARD_POOL);
  }

  for (uint8_t i = 0; i < count; i++) {
    uint8_t selected = pool[rand() % count];
    if (selected < wave_count) return selected;
  }

  return WAVE_PATTERN_SINGLE < wave_count ? WAVE_PATTERN_SINGLE : 0;
}

static void next_wave(wave_runtime_t *runtime, difficulty_t difficulty,
                      uint8_t wave_count, uint32_t current_tick) {
  runtime->current_wave = pick_wave(difficulty, wave_count);
  runtime->current_event = 0;
  runtime->next_wave_start_tick = current_tick + rand_gap();
  runtime->wave_start_tick = runtime->next_wave_start_tick;
}

void waves_init(wave_runtime_t *runtime, uint32_t current_tick) {
  if (runtime == NULL) return;

  runtime->current_wave = WAVE_PATTERN_INTRO;
  runtime->current_event = 0;
  runtime->wave_start_tick = current_tick + WAVE_FIRST_DELAY_TICKS;
  runtime->next_wave_start_tick = runtime->wave_start_tick;
  runtime->status = WAVE_STATUS_IDLE;
}



wave_status_t waves_update(wave_runtime_t *runtime, const wave_t *waves,
                           uint8_t wave_count, difficulty_t difficulty,
                           uint32_t current_tick,
                           wave_spawn_request_t *out_request, bool *has_request) {
  if (has_request != NULL) *has_request = false;

  if (runtime == NULL || waves == NULL || out_request == NULL ||
      has_request == NULL || wave_count == 0) {
    if (runtime != NULL) runtime->status = WAVE_STATUS_ERROR;
    return WAVE_STATUS_ERROR;
  }

  if (runtime->status == WAVE_STATUS_ERROR) {
    return runtime->status;
  }

  runtime->status = WAVE_STATUS_RUNNING;

  if (runtime->current_wave >= wave_count) {
    runtime->current_wave = pick_wave(difficulty, wave_count);
  }

  while (true) {
    const wave_t *wave = &waves[runtime->current_wave];

    if (wave->event_count == 0 || wave->events == NULL) {
      runtime->status = WAVE_STATUS_ERROR;
      return runtime->status;
    }

    if (current_tick < runtime->next_wave_start_tick) {
      return runtime->status;
    }

    if (runtime->current_event >= wave->event_count) {
      next_wave(runtime, difficulty, wave_count, current_tick);
      continue;
    }

    const wave_event_t *event = &wave->events[runtime->current_event];
    if (current_tick - runtime->wave_start_tick < event->delay_ticks) {
      return runtime->status;
    }

    *out_request = event->request;
    *has_request = true;
    runtime->current_event++;
    runtime->wave_start_tick = current_tick;
    return runtime->status;
  }

  return runtime->status;
}

const wave_t *waves_get_predefined(wave_pattern_t pattern) {
  if (pattern >= WAVE_PATTERN_COUNT) return NULL;

  return &PREDEFINED_WAVES[pattern];
}

const motion_profile_t *waves_get_difficulty_motion(difficulty_t difficulty) {
  return &get_profile(difficulty)->motion;
}
