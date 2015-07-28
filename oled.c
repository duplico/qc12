/*
 * oled.c
 *
 *  Created on: Jul 6, 2015
 *      Author: George
 */

/*
 *   OLED (OLED_0.96)
 *        (write on rise, change on fall,
 *         CS active low, MSB first)
 *        eUSCI_A1
 *        ste, miso, clk
 *        DC        P2.6
 *        RES       P2.7
 */

#include <oled.h>
#include <grlib.h>
#include <qc12_oled.h>
#include <qc12.h>

uint8_t oled_anim_state = OLED_ANIM_DONE;
uint8_t anim_index = 0;
uint8_t anim_loops = 0;
uint8_t anim_frame_skip = 0;
const qc12_anim_t *anim_data;

int8_t char_pos_x = 0;
int8_t char_pos_y = 0;

void init_oled() {
    qc12_oledInit();
    GrContextInit(&g_sContext, &g_sqc12_oled);
    GrContextBackgroundSet(&g_sContext, ClrBlack);
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrContextFontSet(&g_sContext, &SYS_FONT);
    GrClearDisplay(&g_sContext);
    GrFlush(&g_sContext);
}

void oled_draw_pane(uint8_t softkey_sel) {
    GrContextFontSet(&g_sContext, &NAME_FONT);
    GrStringDraw(&g_sContext, my_conf.handle, -1, 0, 0, 0);
    uint8_t title_width = GrStringWidthGet(&g_sContext, titles[my_conf.title_index], -1);
    uint8_t the_width = GrStringWidthGet(&g_sContext, "the", -1);
    GrStringDraw(&g_sContext, "the", -1, 63 - title_width - the_width - 3, NAME_FONT_HEIGHT, 0);
    GrStringDraw(&g_sContext, titles[my_conf.title_index], -1, 63 - title_width, NAME_FONT_HEIGHT, 1);
    GrLineDrawH(&g_sContext, 0, 64, 2*NAME_FONT_HEIGHT+1);
    GrContextFontSet(&g_sContext, &SOFTKEY_LABEL_FONT);
    GrStringDrawCentered(&g_sContext, "                ", -1, 31, 127 - SOFTKEY_FONT_HEIGHT/2, 1);
    GrStringDrawCentered(&g_sContext, sk_labels[softkey_sel], -1, 32, 127 - SOFTKEY_FONT_HEIGHT/2, 1);
    GrLineDrawH(&g_sContext, 0, 64, 116);
    GrFlush(&g_sContext);
}

void do_move(uint8_t move_signal) {
    if (!move_signal)
        return;
    uint8_t move_flag = move_signal >> 6;
    uint8_t move_amt = move_signal & 0b00111111;
    switch (move_flag) {
    case 0: // up
        char_pos_y += move_amt;
        break;
    case 1: // down
        char_pos_y -= move_amt;
        break;
    case 2: // left
        char_pos_x -= move_amt;
        if (char_pos_x < -64) {
            char_pos_x+=128;
        }
        break;
    case 3: // right
        char_pos_x += move_amt;
        if (char_pos_x > 64) {
            char_pos_x-=128;
        }
        break;
    default:
        __never_executed();
    }
}

void oled_anim_disp_frame(const tImage* image) {
    GrClearDisplay(&g_sContext);
    GrImageDraw(&g_sContext, image, char_pos_x, SPRITE_Y - char_pos_y);
    oled_draw_pane(idle_mode_softkey_sel);
    GrFlush(&g_sContext);
}

void oled_consider_walking_back() {
    // Determine whether we need to walk back onto the screen after anim.
    if (char_pos_x < -16) {
        oled_play_animation(&walking, (uint8_t) (-char_pos_x/16) - 1);
    } else if (char_pos_x > 16) {
        oled_play_animation(&walking_left, (uint8_t) (char_pos_x/16) - 1);
    }
}

void oled_anim_next_frame() {
    if (oled_anim_state == OLED_ANIM_DONE)
        return;

    do_move(anim_data->movement[anim_index]);
    oled_anim_disp_frame(anim_data->images[anim_index]);

    anim_index++;

    // If we need to loop, loop:
    if (anim_loops && anim_data->looped) {
        if (anim_index == anim_data->loop_end) {
            anim_index = anim_data->loop_start;
            anim_loops--;
        }
    } else if (anim_loops && anim_index == anim_data->len) {
        anim_index = 0;
        anim_loops--;
    }

    if (anim_index == anim_data->len) {
        s_oled_anim_finished = 1;
        oled_anim_state = OLED_ANIM_DONE;
        oled_consider_walking_back();
    }
}

void oled_play_animation(const qc12_anim_t *anim, uint8_t loops) {
    anim_index = 0;
    anim_loops = loops;
    anim_data = anim;
    anim_frame_skip = anim->speed;
    oled_anim_state = OLED_ANIM_START;
    s_oled_anim_finished = 0;
}

void oled_timestep() {
    if (oled_anim_state) {
        if (anim_frame_skip) {
            anim_frame_skip--;
        } else {
            anim_frame_skip = anim_data->speed;
            oled_anim_next_frame();
        }
    }
}
