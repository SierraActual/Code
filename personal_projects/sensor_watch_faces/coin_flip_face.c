/*
 * MIT License
 *
 * Copyright (c) 2023 <#author_name#>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include "coin_flip_face.h"

void coin_flip_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(coin_flip_state_t));
        memset(*context_ptr, 0, sizeof(coin_flip_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void coin_flip_face_activate(movement_settings_t *settings, void *context) {
    coin_flip_state_t *state = (coin_flip_state_t *)context;
    state->active = false;
    // Handle any tasks related to your watch face coming on screen.
}

static void _coin_flip_face_update_lcd(coin_flip_state_t *state) {
    char buf[11];
    const char colors[][7] = {"FLIP  ", " the  ", " coin "};
    sprintf(buf, "   %c%s", state->fast ? ' ' : ' ', colors[state->color]);
    watch_display_string(buf, 0);
}

static void _coin_flip_face_heads_lcd(coin_flip_state_t *state) {
    char buf[11];
    const char heads[][7] = {" Heads", " you  ", " guy  "};
    sprintf(buf, "   %c%s", state->fast ? ' ' : ' ', heads[0]);
    watch_display_string(buf, 0);
}

static void _coin_flip_face_tails_lcd(coin_flip_state_t *state) {
    char buf[11];
    const char tails[][7] = {" Tails", " you  ", " guy  "};
    sprintf(buf, "   %c%s", state->fast ? ' ' : ' ', tails[0]);
    watch_display_string(buf, 0);
}

bool coin_flip_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    coin_flip_state_t *state = (coin_flip_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            _coin_flip_face_update_lcd(state);
            break;

        case EVENT_MODE_BUTTON_UP:
            movement_move_to_next_face();
            break;

        case EVENT_LIGHT_BUTTON_UP:
            if (!state->active) {
                state->color = (state->color + 1) % 3;
                _coin_flip_face_update_lcd(state);
            }
            break;

        case EVENT_ALARM_BUTTON_UP:
            if (!state->active) {
                state->active = true;
                watch_clear_display();
                int rand_2 = rand() % 2;
                if (rand_2 == 0) _coin_flip_face_heads_lcd(state);
                else _coin_flip_face_tails_lcd(state);
            } else {
                state->active = false;
                _coin_flip_face_update_lcd(state);
            }
            break;
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void coin_flip_face_resign(movement_settings_t *settings, void *context) {
    watch_set_led_off();

    // handle any cleanup before your watch face goes off-screen.
}

