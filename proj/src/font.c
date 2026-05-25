#include "../include/game/font.h"

#include <stdlib.h>
#include <stdio.h>

#include "../assets/numbers/zero.xpm"
#include "../assets/numbers/one.xpm"
#include "../assets/numbers/two.xpm"
#include "../assets/numbers/three.xpm"
#include "../assets/numbers/four.xpm"
#include "../assets/numbers/five.xpm"
#include "../assets/numbers/six.xpm"
#include "../assets/numbers/seven.xpm"
#include "../assets/numbers/eight.xpm"
#include "../assets/numbers/nine.xpm"

#include "../assets/letters/a.xpm"
#include "../assets/letters/b.xpm"
#include "../assets/letters/c.xpm"
#include "../assets/letters/d.xpm"
#include "../assets/letters/e.xpm"
#include "../assets/letters/f.xpm"
#include "../assets/letters/g.xpm"
#include "../assets/letters/h.xpm"
#include "../assets/letters/i.xpm"
#include "../assets/letters/j.xpm"
#include "../assets/letters/k.xpm"
#include "../assets/letters/l.xpm"
#include "../assets/letters/m.xpm"
#include "../assets/letters/n.xpm"
#include "../assets/letters/o.xpm"
#include "../assets/letters/p.xpm"
#include "../assets/letters/q.xpm"
#include "../assets/letters/r.xpm"
#include "../assets/letters/s.xpm"
#include "../assets/letters/t.xpm"
#include "../assets/letters/u.xpm"
#include "../assets/letters/v.xpm"
#include "../assets/letters/w.xpm"
#include "../assets/letters/x.xpm"
#include "../assets/letters/y.xpm"
#include "../assets/letters/z.xpm"

int font_create(font_t *font, uint16_t char_width) {
    if (font == NULL) return 1;

    font->char_width = char_width;
    font->num_chars = 36; // 10 digits + 26 letters
    font->sprites = malloc(sizeof(sprite_t*) * font->num_chars);
    if (font->sprites == NULL) {
        return 1;
    }

    xpm_map_t char_xpms[36] = {
        (xpm_map_t)zero_xpm, (xpm_map_t)one_xpm, (xpm_map_t)two_xpm, (xpm_map_t)three_xpm,
        (xpm_map_t)four_xpm, (xpm_map_t)five_xpm, (xpm_map_t)six_xpm, (xpm_map_t)seven_xpm,
        (xpm_map_t)eight_xpm, (xpm_map_t)nine_xpm,
        (xpm_map_t)a_xpm, (xpm_map_t)b_xpm, (xpm_map_t)c_xpm, (xpm_map_t)d_xpm,
        (xpm_map_t)e_xpm, (xpm_map_t)f_xpm, (xpm_map_t)g_xpm, (xpm_map_t)h_xpm,
        (xpm_map_t)i_xpm, (xpm_map_t)j_xpm, (xpm_map_t)k_xpm, (xpm_map_t)l_xpm,
        (xpm_map_t)m_xpm, (xpm_map_t)n_xpm, (xpm_map_t)o_xpm, (xpm_map_t)p_xpm,
        (xpm_map_t)q_xpm, (xpm_map_t)r_xpm, (xpm_map_t)s_xpm, (xpm_map_t)t_xpm,
        (xpm_map_t)u_xpm, (xpm_map_t)v_xpm, (xpm_map_t)w_xpm, (xpm_map_t)x_xpm,
        (xpm_map_t)y_xpm, (xpm_map_t)z_xpm,
    };

    for (size_t i = 0; i < font->num_chars; i++) {
        font->sprites[i] = malloc(sizeof(sprite_t));
        if (font->sprites[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(font->sprites[j]);
            }
            free(font->sprites);
            return 1; 
        } 
        if (sprite_load(font->sprites[i], char_xpms[i]) != 0) {
            for (size_t j = 0; j <= i; j++) {
                free(font->sprites[j]);
            }
            free(font->sprites);
            return 1;
        }
    }
    return 0;
}

void font_destroy(font_t *font) {
    if (font == NULL || font->sprites == NULL) return;

    for (size_t i = 0; i < font->num_chars; i++) {
        if (font->sprites[i] != NULL) {
            sprite_destroy(font->sprites[i]);
            free(font->sprites[i]);
        }
    }
    free(font->sprites);
    font->sprites = NULL;
}

int font_glyph_index(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0'; // 0-9 maps to 0-9
    } else if (c >= 'a' && c <= 'z') {
        return 10 + (c - 'a'); // a-z maps to 10-35
    } else {
        return -1; // Unsupported character
    }
}
