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
#include "f_u_face.h"

void f_u_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(f_u_state_t));
        memset(*context_ptr, 0, sizeof(f_u_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void f_u_face_activate(movement_settings_t *settings, void *context) {
    f_u_state_t *state = (f_u_state_t *)context;
    state->active = false;
    // Handle any tasks related to your watch face coming on screen.
}

static void _f_u_face_update_lcd(f_u_state_t *state) {
    char buf[11];
    const char colors[][7] = {" Fuck ", " you  ", " guy  "};
    sprintf(buf, "   %c%s", state->fast ? 'F' : 'S', colors[state->color]);
    watch_display_string(buf, 0);
}

static void _f_u_face_fuck_lcd(f_u_state_t *state) {
    char buf[11];
    const char fuck[][7] = {" Fuck ", " you  ", " guy  "};
    sprintf(buf, "   %c%s", state->fast ? ' ' : ' ', fuck[0]);
    watch_display_string(buf, 0);
}

static void _f_u_face_you_lcd(f_u_state_t *state) {
    char buf[11];
    const char you[][7] = {" Fuck ", " you  ", " guy  "};
    sprintf(buf, "   %c%s", state->fast ? ' ' : ' ', you[1]);
    watch_display_string(buf, 0);
}

bool f_u_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    f_u_state_t *state = (f_u_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            _f_u_face_update_lcd(state);
            break;
        case EVENT_TICK:
                if (state->active) {
                    if (event.subsecond % 2 == 0) _f_u_face_fuck_lcd(state);
                    else _f_u_face_you_lcd(state);
                    }
            break;

        case EVENT_MODE_BUTTON_UP:
            movement_move_to_next_face();
            break;
        case EVENT_LIGHT_BUTTON_UP:
            if (!state->active) {
                state->color = (state->color + 1) % 3;
                _f_u_face_update_lcd(state);
            }
            break;
        case EVENT_ALARM_BUTTON_UP:
            if (!state->active) {
                state->active = true;
                watch_clear_display();
                movement_request_tick_frequency(state->fast ? 8 : 2);            
            } else {
                state->active = false;
                watch_set_led_off();
                _f_u_face_update_lcd(state);
            }
            break;

        case EVENT_ALARM_LONG_PRESS:
            if (!state->active) {
                state->fast = !state->fast;
                _f_u_face_update_lcd(state);
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

void f_u_face_resign(movement_settings_t *settings, void *context) {
    watch_set_led_off();

    // handle any cleanup before your watch face goes off-screen.
}

