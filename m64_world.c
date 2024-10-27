#include "gui/canvas.h"
#include <furi.h>
#include <m64_world_icons.h>
#include <gui/gui.h>
#include <gui/icon_i.h>
#include <input/input.h>
#include <stdlib.h>
#include <dolphin/dolphin.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

typedef struct {
    uint8_t x;
    uint8_t y;
} Point;

typedef enum {
    GameStateRunning,
    GameStatePause,
    GameStateGameOver,
} GameState;

typedef enum {
    DirectionUp,
    DirectionRight,
    DirectionDown,
    DirectionLeft,
    DirectionNone
} Direction;

#define x_back_symbol 50
#define y_back_symbol 9

typedef struct {
    FuriMutex* mutex;
    Direction nextMovement;
    Point PointCursor;
    int Score;
    GameState state;
    int Tick;
} MyGameState;

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} MyGameEvent;

const NotificationSequence sequence_snd_err = {
    &message_vibro_on,

    &message_note_ds4,
    &message_delay_10,
    &message_sound_off,
    &message_delay_10,

    &message_note_ds4,
    &message_delay_10,
    &message_sound_off,
    &message_delay_10,

    &message_note_ds4,
    &message_delay_10,
    &message_sound_off,
    &message_delay_10,

    &message_vibro_off,
    NULL,
};

const NotificationSequence sequence_snd_bim = {

    &message_vibro_on,
    &message_note_c7,
    &message_delay_50,
    &message_sound_off,
    &message_vibro_off,
    NULL,
};

const NotificationSequence sequence_snd_tick = {

    &message_note_c7,
    &message_delay_10,
    &message_sound_off,
    NULL,
};

static void my_game_render_callback(Canvas* const canvas, void* ctx) {
    furi_assert(ctx);
    const MyGameState* mygame_state = ctx;
    furi_mutex_acquire(mygame_state->mutex, FuriWaitForever);

    canvas_reset(canvas);
    canvas_draw_icon(canvas, 40, 1, &I_m64_logo);

    Point f = mygame_state->PointCursor;
    f.x = f.x * 6 + 1;
    f.y = f.y * 6 + 1;
    canvas_draw_box(canvas, f.x, f.y, 6, 6);
    // canvas_set_color(canvas, ColorWhite);
    // canvas_set_color(canvas, ColorBlack);
    //
    canvas_set_font(canvas, FontSecondary);
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Tick: %u", mygame_state->Tick);
    canvas_draw_str_aligned(canvas, 65, 41, AlignCenter, AlignBottom, buffer);

    snprintf(buffer, sizeof(buffer), "Score: %u", mygame_state->Score);
    canvas_draw_str_aligned(canvas, 65, 62, AlignCenter, AlignBottom, buffer);

    // Pause and GameOver banner
    if(mygame_state->state == GameStatePause || mygame_state->state == GameStateGameOver) {
        // Screen is 128x64 px
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, 33, 19, 64, 26);

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_frame(canvas, 34, 20, 62, 24);

        canvas_set_font(canvas, FontPrimary);
        if(mygame_state->state == GameStateGameOver) {
            canvas_draw_str_aligned(canvas, 65, 31, AlignCenter, AlignBottom, "Game Over");
        }
        if(mygame_state->state == GameStatePause) {
            canvas_draw_str_aligned(canvas, 65, 31, AlignCenter, AlignBottom, "Pause");
        }

        canvas_set_font(canvas, FontSecondary);
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Score: %u", mygame_state->Score);
        canvas_draw_str_aligned(canvas, 65, 41, AlignCenter, AlignBottom, buffer);

        // Painting "back"-symbol, Help message for Exit App, ProgressBar
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, 25, 2, 81, 11);
        canvas_draw_box(canvas, 28, 54, 73, 9);
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_str_aligned(
            canvas, 65, 10, AlignCenter, AlignBottom, "Hold        to Exit App");
        snprintf(buffer, sizeof(buffer), "Score: %d", mygame_state->Score);
        canvas_draw_str_aligned(canvas, 65, 62, AlignCenter, AlignBottom, buffer);
        {
            canvas_draw_dot(canvas, x_back_symbol + 0, y_back_symbol);
            canvas_draw_dot(canvas, x_back_symbol + 1, y_back_symbol);
            canvas_draw_dot(canvas, x_back_symbol + 2, y_back_symbol);
            canvas_draw_dot(canvas, x_back_symbol + 3, y_back_symbol);
            canvas_draw_dot(canvas, x_back_symbol + 4, y_back_symbol);
            canvas_draw_dot(canvas, x_back_symbol + 5, y_back_symbol - 1);
            canvas_draw_dot(canvas, x_back_symbol + 6, y_back_symbol - 2);
            canvas_draw_dot(canvas, x_back_symbol + 6, y_back_symbol - 3);
            canvas_draw_dot(canvas, x_back_symbol + 5, y_back_symbol - 4);
            canvas_draw_dot(canvas, x_back_symbol + 4, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol + 3, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol + 2, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol + 1, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol + 0, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol - 1, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol - 2, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol - 3, y_back_symbol - 5);
            canvas_draw_dot(canvas, x_back_symbol - 2, y_back_symbol - 6);
            canvas_draw_dot(canvas, x_back_symbol - 2, y_back_symbol - 4);
            canvas_draw_dot(canvas, x_back_symbol - 1, y_back_symbol - 6);
            canvas_draw_dot(canvas, x_back_symbol - 1, y_back_symbol - 4);
            canvas_draw_dot(canvas, x_back_symbol - 1, y_back_symbol - 7);
            canvas_draw_dot(canvas, x_back_symbol - 1, y_back_symbol - 3);
        }
    }

    furi_mutex_release(mygame_state->mutex);
}

static void my_game_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    MyGameEvent event = {.type = EventTypeKey, .input = *input_event};
    furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

static void my_game_update_timer_callback(void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    MyGameEvent event = {.type = EventTypeTick};
    furi_message_queue_put(event_queue, &event, 0);
}

static void my_game_init_game(MyGameState* const mygame_state) {
    mygame_state->Score = 0;
    mygame_state->Tick = 0;

    mygame_state->nextMovement = DirectionNone;

    Point f = {18, 6};
    mygame_state->PointCursor = f;

    mygame_state->state = GameStateRunning;
}

static Point my_game_get_next_step(MyGameState const* const mygame_state) {
    Point next_pos = mygame_state->PointCursor;
    switch(mygame_state->nextMovement) {
    case DirectionUp:
        next_pos.y--;
        break;
    case DirectionRight:
        next_pos.x++;
        break;
    case DirectionDown:
        next_pos.y++;
        break;
    case DirectionLeft:
        next_pos.x--;
        break;
    case DirectionNone:
        break;
    }
    return next_pos;
}

static void
    my_game_process_game_step(MyGameState* const mygame_state, NotificationApp* notification) {
    if(mygame_state->state == GameStateGameOver) {
        return;
    }

    Point next_pos = my_game_get_next_step(mygame_state);
    mygame_state->PointCursor = next_pos;
    mygame_state->nextMovement = DirectionNone;

    // mygame_state->state = GameStateGameOver;
    if(mygame_state->state == GameStateGameOver) {
        notification_message_block(notification, &sequence_snd_err);
        return;
    }

    // notification_message(notification, &sequence_snd_bim);
    // notification_message(notification, &sequence_blink_red_100);
    // return;
}

int32_t m64_world_app(void* p) {
    UNUSED(p);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(MyGameEvent));

    MyGameState* mygame_state = malloc(sizeof(MyGameState));
    my_game_init_game(mygame_state);

    mygame_state->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    if(!mygame_state->mutex) {
        FURI_LOG_E("MyGame", "cannot create mutex\r\n");
        furi_message_queue_free(event_queue);
        free(mygame_state);
        return 255;
    }

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, my_game_render_callback, mygame_state);
    view_port_input_callback_set(view_port, my_game_input_callback, event_queue);

    FuriTimer* timer =
        furi_timer_alloc(my_game_update_timer_callback, FuriTimerTypePeriodic, event_queue);
    // furi_timer_start(timer, furi_kernel_get_tick_frequency() / 8);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / 2);

    // Open GUI and register view_port
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);

    notification_message_block(notification, &sequence_display_backlight_enforce_on);

    dolphin_deed(DolphinDeedPluginGameStart);

    MyGameEvent event;
    for(bool processing = true; processing;) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);

        furi_mutex_acquire(mygame_state->mutex, FuriWaitForever);

        if(event_status == FuriStatusOk) {
            if(event.type == EventTypeKey) {
                // press events
                if(event.input.type == InputTypePress || event.input.type == InputTypeRepeat) {
                    switch(event.input.key) {
                    case InputKeyUp:
                        if(mygame_state->state != GameStatePause) {
                            mygame_state->nextMovement = DirectionUp;
                        }
                        break;
                    case InputKeyDown:
                        if(mygame_state->state != GameStatePause) {
                            mygame_state->nextMovement = DirectionDown;
                        }
                        break;
                    case InputKeyRight:
                        if(mygame_state->state != GameStatePause) {
                            mygame_state->nextMovement = DirectionRight;
                        }
                        break;
                    case InputKeyLeft:
                        if(mygame_state->state != GameStatePause) {
                            mygame_state->nextMovement = DirectionLeft;
                        }
                        break;
                    case InputKeyOk:
                        if(mygame_state->state == GameStateGameOver) {
                            my_game_init_game(mygame_state);
                        }
                        if(mygame_state->state == GameStatePause) {
                            // furi_timer_start(timer, furi_kernel_get_tick_frequency() / 4);
                            mygame_state->state = GameStateRunning;
                        }
                        break;
                    case InputKeyBack:
                        if(mygame_state->state == GameStateRunning) {
                            furi_timer_stop(timer);
                            mygame_state->state = GameStatePause;
                            break;
                        }
                        if(mygame_state->state == GameStatePause) {
                            // furi_timer_start(timer, furi_kernel_get_tick_frequency() / 4);
                            mygame_state->state = GameStateRunning;
                            break;
                        }
                        if(mygame_state->state == GameStateGameOver) {
                            my_game_init_game(mygame_state);
                        }
                    default:
                        break;
                    }
                    my_game_process_game_step(mygame_state, notification);
                }
                //LongPress Events
                if(event.input.type == InputTypeLong) {
                    switch(event.input.key) {
                    case InputKeyBack:
                        // Game Exit
                        processing = false;
                        break;
                    case InputKeyOk:
                        mygame_state->state = GameStateGameOver;
                    default:
                        break;
                    }
                }
                //ReleaseKey Event
                if(event.input.type == InputTypeRelease) {
                    if(mygame_state->state != GameStatePause) {
                        mygame_state->nextMovement = DirectionNone;
                    }
                }
            } else if(event.type == EventTypeTick) {
                //////////////////////////////////////////////////////
                mygame_state->Tick++;
                // my_game_process_game_step(mygame_state, notification);
                // furi_timer_start(timer, furi_kernel_get_tick_frequency() / 4);
                notification_message_block(notification, &sequence_snd_tick);
            }
        } else {
            // event timeout
        }

        furi_mutex_release(mygame_state->mutex);
        view_port_update(view_port);
    }

    // Wait for all notifications to be played and return backlight to normal state
    notification_message_block(notification, &sequence_display_backlight_enforce_auto);

    furi_timer_free(timer);
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    furi_mutex_free(mygame_state->mutex);
    free(mygame_state);

    return 0;
}
