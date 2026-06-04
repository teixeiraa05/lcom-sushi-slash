#ifndef __WAVES_H
#define __WAVES_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @file waves.h
 * @brief Declares the wave scheduling API used by gameplay.
 *
 * The wave module decides when and what should be spawned, but it does not
 * create sushi pieces directly. Gameplay consumes wave_spawn_request_t values
 * and turns them into concrete game objects.
 */

/** @brief Repeat count value meaning that a wave should repeat forever. */
#define WAVE_FOREVER 0

/** @brief Maximum number of input characters shown during a boss wave. */
#define WAVE_MAX_BOSS_SEQUENCE 8

/** @brief Sushi type value meaning that gameplay should choose the type. */
#define WAVE_RANDOM_SUSHI_TYPE 0xFF

/**
 * @brief Identifies a predefined wave pattern.
 *
 * Pattern names describe the structure of the wave. The concrete events for
 * each pattern are stored in PREDEFINED_WAVES and interpreted by
 * waves_update().
 */
typedef enum {
  /** @brief A small wave that asks gameplay to spawn one item. */
  WAVE_PATTERN_SINGLE,

  /** @brief A compact wave that asks gameplay to spawn multiple items together.
   */
  WAVE_PATTERN_BURST,

  /** @brief A wave whose items are separated by short delays. */
  WAVE_PATTERN_STAGGERED,

  /** @brief A wave mixing different spawn kinds, such as sushi and bombs. */
  WAVE_PATTERN_MIXED,

  /** @brief Number of predefined wave patterns. */
  WAVE_PATTERN_COUNT
} wave_pattern_t;

/**
 * @brief Type of spawn requested by a wave event.
 *
 * This only describes what gameplay should spawn. It does not create or own any
 * sushi, bomb, boss, sprite, or gameplay object.
 */
typedef enum {
  /** @brief Request a regular sushi piece. */
  WAVE_SPAWN_REGULAR,

  /** @brief Request a bomb hazard. */
  WAVE_SPAWN_BOMB,

  /** @brief Request a boss encounter. */
  WAVE_SPAWN_BOSS
} wave_spawn_kind_t;

/**
 * @brief Motion limits requested for spawned objects.
 *
 * Values are intentionally kept as plain integers so gameplay can decide how to
 * translate them into its physics representation.
 */
typedef struct {
  /** @brief Minimum vertical velocity for the spawned object. */
  int min_vy;

  /** @brief Maximum vertical velocity for the spawned object. */
  int max_vy;

  /** @brief Gravity applied to the spawned object. */
  int gravity;

  /** @brief Minimum horizontal velocity for the spawned object. */
  int min_vx;

  /** @brief Maximum horizontal velocity for the spawned object. */
  int max_vx;
} motion_profile_t;

/**
 * @brief Runtime state of the wave scheduler.
 */
typedef enum {
  /** @brief Scheduler is initialized but not currently advancing a wave. */
  WAVE_STATUS_IDLE,

  /** @brief Scheduler is actively processing wave events. */
  WAVE_STATUS_RUNNING,

  /** @brief Scheduler reached the end of all requested waves. */
  WAVE_STATUS_FINISHED,

  /** @brief Scheduler encountered invalid input. */
  WAVE_STATUS_ERROR
} wave_status_t;

/**
 * @brief Extra data used when a wave requests a boss encounter.
 */
typedef struct {
  /** @brief Number of ticks available to complete the boss sequence. */
  uint32_t boss_countdown_ticks;

  /** @brief Null-terminated key sequence required to defeat the boss. */
  char boss_sequence[WAVE_MAX_BOSS_SEQUENCE + 1];
} boss_data_t;

/**
 * @brief Request emitted by the wave scheduler and consumed by gameplay.
 *
 * Gameplay is responsible for turning this request into actual objects. For
 * example, a WAVE_SPAWN_REGULAR request may become one or more sushi_t values,
 * while a WAVE_SPAWN_BOSS request may move gameplay into the boss state.
 */
typedef struct {
  /** @brief Kind of object or encounter requested. */
  wave_spawn_kind_t kind;

  /** @brief Sushi asset/type to use, or WAVE_RANDOM_SUSHI_TYPE for random. */
  uint8_t sushi_type;

  /** @brief Number of objects requested by this spawn. */
  uint8_t amount;

  /** @brief Requested motion limits for the spawned objects. */
  motion_profile_t motion;

  /** @brief Score awarded if the requested object is successfully cut. */
  uint32_t score_value;

  /** @brief Boss-specific data, used only for WAVE_SPAWN_BOSS. */
  boss_data_t boss;
} wave_spawn_request_t;

/**
 * @brief Timed spawn request inside a wave.
 *
 * delay_ticks is measured from the start of the current wave repetition. When
 * the delay has elapsed, waves_update() copies request into its output
 * parameter.
 */
typedef struct {
  /** @brief Tick delay before this event should be emitted. */
  uint32_t delay_ticks;

  /** @brief Spawn request emitted when the delay is reached. */
  wave_spawn_request_t request;
} wave_event_t;

/**
 * @brief Definition of one wave pattern instance.
 */
typedef struct {
  /** @brief Identifier of the predefined pattern represented by this wave. */
  wave_pattern_t pattern;

  /** @brief Ordered list of timed spawn events. */
  const wave_event_t *events;

  /** @brief Number of entries in events. */
  uint8_t event_count;

  /** @brief Number of times to repeat the wave, or WAVE_FOREVER. */
  uint8_t repeat_count;

  /** @brief Delay before the next wave starts after this one finishes. */
  uint32_t next_wave_delay_ticks;
} wave_t;

/**
 * @brief Mutable state used while waves are being processed.
 */
typedef struct {
  /** @brief Index of the wave currently being processed. */
  uint8_t current_wave;

  /** @brief Index of the next event to emit within the current wave. */
  uint8_t current_event;

  /** @brief Current repetition count for the active wave. */
  uint8_t current_repeat;

  /** @brief Tick when the current wave repetition started. */
  uint32_t wave_start_tick;

  /** @brief Earliest tick at which the next wave may start. */
  uint32_t next_wave_start_tick;

  /** @brief Current scheduler status. */
  wave_status_t status;

} wave_runtime_t;

/**
 * @brief Table containing the predefined wave pattern definitions.
 */
extern const wave_t PREDEFINED_WAVES[WAVE_PATTERN_COUNT];

/**
 * @brief Initialize a wave runtime.
 *
 * @param runtime Runtime object to initialize.
 * @param current_tick Current timer tick.
 */
void waves_init(wave_runtime_t *runtime, uint32_t current_tick);

/**
 * @brief Reset a wave runtime to its initial state.
 *
 * @param runtime Runtime object to reset.
 * @param current_tick Current timer tick.
 */
void waves_reset(wave_runtime_t *runtime, uint32_t current_tick);

/**
 * @brief Advance the wave scheduler and return a due spawn request.
 *
 * @param runtime Mutable scheduler state.
 * @param waves Array of wave definitions to process.
 * @param wave_count Number of entries in waves.
 * @param current_tick Current timer tick.
 * @param out_request Output parameter filled with the due spawn request.
 * @param has_request Output parameter set to true if a request is due, false otherwise.
 * 
 * @return Scheduler status indicating the result of the update.
 */
wave_status_t waves_update(wave_runtime_t *runtime, const wave_t *waves,
                           uint8_t wave_count, uint32_t current_tick,
                           wave_spawn_request_t *out_request, bool *has_request);

/**
 * @brief Check whether the scheduler has finished processing its waves.
 *
 * @param runtime Runtime object to inspect.
 *
 * @return true if runtime is finished, false otherwise.
 */
bool waves_is_finished(const wave_runtime_t *runtime);

/**
 * @brief Get a predefined wave definition by pattern id.
 *
 * @param pattern Predefined pattern to retrieve.
 *
 * @return Pointer to the predefined wave, or NULL if pattern is invalid.
 */
const wave_t *waves_get_predefined(wave_pattern_t pattern);

#endif /* __WAVES_H */
