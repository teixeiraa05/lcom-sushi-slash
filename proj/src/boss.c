#include "../include/game/boss.h"
#include "../include/macros/physics.h"
#include "../assets/sushi_pieces/sushiRoll4.xpm"
#include "../assets/sushi_pieces/sushiRoll4_left.xpm"
#include "../assets/sushi_pieces/sushiRoll4_right.xpm"

boss_t boss_init(combo_t combo, uint32_t countdown_ticks) {
    boss_t boss;
    
    // Choose a random side (left or right)
    int side = rand() % 2;
    int x = (side == 0) ? -100 : 800; // Spawn outside screen
    int vx = (side == 0) ? 2 : -2;    // Move toward center
    
    // Use slow-motion gravity (20x slower than normal)
    int gravity = SLOWMO_PHYSICS_FROM_PIXELS(3);
    
    // Create boss sushi with slow-motion physics
    boss.sushi = create_sushi_boss(x, -SUSHI_SPAWN_Y_OFFSET, vx, 0, gravity, NULL, 0);
    boss.combo = combo;
    boss.countdown = countdown_ticks;
    
    return boss;
}

int boss_verify_combo(boss_t* boss, char input) {
    // Check if the input matches the current combo letter
    char expected = combo_get_current(&boss->combo);
    
    if (expected == 0 || input != expected) {
        return 1;
    }
    
    // Advance to next letter
    combo_advance(&boss->combo);
    return 0;
}

void boss_update(boss_t* boss) {
    // Update velocity with gravity
    boss->sushi.vy += boss->sushi.gravity;
    
    // Update position
    boss->sushi.x += boss->sushi.vx;
    boss->sushi.y += boss->sushi.vy;
    
    // Decrement countdown
    if (boss->countdown > 0) {
        boss->countdown--;
    }
}

void boss_destroy(boss_t* boss) {
    if (boss->sushi.xpm != NULL) {
        sprite_destroy(boss->sushi.xpm);
        boss->sushi.xpm = NULL;
    }
}
