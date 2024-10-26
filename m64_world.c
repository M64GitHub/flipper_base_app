#include <furi.h>

/* generated by fbt from .png files in images folder */
#include <m64_world_icons.h>

#include <gui/gui.h>
#include <gui/icon_i.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/menu.h>
#include <gui/modules/popup.h>

#define TAG "m64world_app"

/** ids for all scenes used by the app */
typedef enum {
    M64AppScene_MainMenu,
    M64AppScene_FirstPopup,
    M64AppScene_SecondPopup,
    M64AppScene_count
} M64AppScene;

/** ids for the 2 types of view used by the app */
typedef enum { M64AppView_Menu, M64AppView_Popup } M64AppView;

/** the app context struct */
typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Menu* menu;
    Popup* popup;
} M64App;

/** all custom events */
typedef enum { M64AppEvent_ShowPopupOne, M64AppEvent_ShowPopupTwo } M64AppEvent;

/* main menu scene */

/** indices for menu items */
typedef enum { M64AppMenuSelection_One, M64AppMenuSelection_Two } M64AppMenuSelection;

/** main menu callback - sends a custom event to the scene manager based on the menu selection */
void m64world_app_menu_callback_main_menu(void* context, uint32_t index) {

    FURI_LOG_T(TAG, "m64world_app_menu_callback_main_menu");
    M64App* app = context;
    switch(index) {
    case M64AppMenuSelection_One:
        scene_manager_handle_custom_event(app->scene_manager, M64AppEvent_ShowPopupOne);
        break;
    case M64AppMenuSelection_Two:
        scene_manager_handle_custom_event(app->scene_manager, M64AppEvent_ShowPopupTwo);
        break;
    }
}

/** resets the menu, gives it content, callbacks and selection enums */
void m64world_app_scene_on_enter_main_menu(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_enter_main_menu");
    M64App* app = context;
    menu_reset(app->menu);

    // NB. icons are specified as NULL below, because:
    // * icons are _always_ animated by the menu
    // * the icons provided (&I_one, &I_two) are generated by the build process
    // * these icons do not have a framerate (resulting in a division by zero)
    menu_add_item(
        app->menu,
        "Screen 1",
        NULL,
        M64AppMenuSelection_One,
        m64world_app_menu_callback_main_menu,
        app);
    menu_add_item(
        app->menu,
        "Screen 2",
        NULL,
        M64AppMenuSelection_Two,
        m64world_app_menu_callback_main_menu,
        app);
    view_dispatcher_switch_to_view(app->view_dispatcher, M64AppView_Menu);
}

/** main menu event handler - switches scene based on the event */
bool m64world_app_scene_on_event_main_menu(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_event_main_menu");
    M64App* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case M64AppEvent_ShowPopupOne:
            scene_manager_next_scene(app->scene_manager, M64AppScene_FirstPopup);
            consumed = true;
            break;
        case M64AppEvent_ShowPopupTwo:
            scene_manager_next_scene(app->scene_manager, M64AppScene_SecondPopup);
            consumed = true;
            break;
        }
        break;
    default: // eg. SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

void m64world_app_scene_on_exit_main_menu(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_exit_main_menu");
    M64App* app = context;
    menu_reset(app->menu);
}

/* popup 1 scene */

void m64world_app_scene_on_enter_popup_one(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_enter_popup_one");
    M64App* app = context;
    popup_reset(app->popup);
    popup_set_context(app->popup, app);
    popup_set_header(app->popup, "BAM64!", 64, 10, AlignCenter, AlignTop);
    popup_set_icon(app->popup, 10, 10, &I_cvc_36x36);
    popup_set_text(app->popup, "1 2 3 4, everybody on the floor!", 64, 20, AlignLeft, AlignTop);
    view_dispatcher_switch_to_view(app->view_dispatcher, M64AppView_Popup);
}

bool m64world_app_scene_on_event_popup_one(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_event_popup_one");
    UNUSED(context);
    UNUSED(event);
    return false; // don't handle any events
}

void m64world_app_scene_on_exit_popup_one(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_exit_popup_one");
    M64App* app = context;
    popup_reset(app->popup);
}

/* popup 2 scene */

void m64world_app_scene_on_enter_popup_two(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_enter_popup_two");
    M64App* app = context;
    popup_reset(app->popup);
    popup_set_context(app->popup, app);
    popup_set_header(app->popup, "BAM64 2!", 64, 10, AlignCenter, AlignTop);
    popup_set_icon(app->popup, 10, 10, &I_cvc_36x36);
    popup_set_text(app->popup, "Pupop 2!! :)", 64, 20, AlignLeft, AlignTop);
    view_dispatcher_switch_to_view(app->view_dispatcher, M64AppView_Popup);
}

bool m64world_app_scene_on_event_popup_two(void* context, SceneManagerEvent event) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_event_popup_two");
    UNUSED(context);
    UNUSED(event);
    return false; // don't handle any events
}

void m64world_app_scene_on_exit_popup_two(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_on_exit_popup_two");
    M64App* app = context;
    popup_reset(app->popup);
}

/** collection of all scene on_enter handlers - in the same order as their enum */
void (*const m64world_app_scene_on_enter_handlers[])(void*) = {
    m64world_app_scene_on_enter_main_menu,
    m64world_app_scene_on_enter_popup_one,
    m64world_app_scene_on_enter_popup_two};

/** collection of all scene on event handlers - in the same order as their enum */
bool (*const m64world_app_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    m64world_app_scene_on_event_main_menu,
    m64world_app_scene_on_event_popup_one,
    m64world_app_scene_on_event_popup_two};

/** collection of all scene on exit handlers - in the same order as their enum */
void (*const m64world_app_scene_on_exit_handlers[])(void*) = {
    m64world_app_scene_on_exit_main_menu,
    m64world_app_scene_on_exit_popup_one,
    m64world_app_scene_on_exit_popup_two};

/** collection of all on_enter, on_event, on_exit handlers */
const SceneManagerHandlers m64world_app_scene_event_handlers = {
    .on_enter_handlers = m64world_app_scene_on_enter_handlers,
    .on_event_handlers = m64world_app_scene_on_event_handlers,
    .on_exit_handlers = m64world_app_scene_on_exit_handlers,
    .scene_num = M64AppScene_count};

/** custom event handler - passes the event to the scene manager */
bool m64world_app_scene_manager_custom_event_callback(void* context, uint32_t custom_event) {
    FURI_LOG_T(TAG, "m64world_app_scene_manager_custom_event_callback");
    furi_assert(context);
    M64App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

/** navigation event handler - passes the event to the scene manager */
bool m64world_app_scene_manager_navigation_event_callback(void* context) {
    FURI_LOG_T(TAG, "m64world_app_scene_manager_navigation_event_callback");
    furi_assert(context);
    M64App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

/** initialise the scene manager with all handlers */
void m64world_app_scene_manager_init(M64App* app) {
    FURI_LOG_T(TAG, "m64world_app_scene_manager_init");
    app->scene_manager = scene_manager_alloc(&m64world_app_scene_event_handlers, app);
}

/** initialise the views, and initialise the view dispatcher with all views */
void m64world_app_view_dispatcher_init(M64App* app) {
    FURI_LOG_T(TAG, "m64world_app_view_dispatcher_init");
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    // allocate each view
    FURI_LOG_D(TAG, "m64world_app_view_dispatcher_init allocating views");
    app->menu = menu_alloc();
    app->popup = popup_alloc();

    // assign callback that pass events from views to the scene manager
    FURI_LOG_D(TAG, "m64world_app_view_dispatcher_init setting callbacks");
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, m64world_app_scene_manager_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, m64world_app_scene_manager_navigation_event_callback);

    // add views to the dispatcher, indexed by their enum value
    FURI_LOG_D(TAG, "m64world_app_view_dispatcher_init adding view menu");
    view_dispatcher_add_view(app->view_dispatcher, M64AppView_Menu, menu_get_view(app->menu));

    FURI_LOG_D(TAG, "m64world_app_view_dispatcher_init adding view popup");
    view_dispatcher_add_view(app->view_dispatcher, M64AppView_Popup, popup_get_view(app->popup));
}

/** initialise app data, scene manager, and view dispatcher */
M64App* m64world_app_init() {
    FURI_LOG_T(TAG, "m64world_app_init");
    M64App* app = malloc(sizeof(M64App));
    m64world_app_scene_manager_init(app);
    m64world_app_view_dispatcher_init(app);
    return app;
}

/** free all app data, scene manager, and view dispatcher */
void m64world_app_free(M64App* app) {
    FURI_LOG_T(TAG, "m64world_app_free");
    scene_manager_free(app->scene_manager);
    view_dispatcher_remove_view(app->view_dispatcher, M64AppView_Menu);
    view_dispatcher_remove_view(app->view_dispatcher, M64AppView_Popup);
    view_dispatcher_free(app->view_dispatcher);
    menu_free(app->menu);
    popup_free(app->popup);
    free(app);
}

/** go to trace log level in the dev environment */
void m64world_app_set_log_level() {
#ifdef FURI_DEBUG
    furi_log_set_level(FuriLogLevelTrace);
#else
    furi_log_set_level(FuriLogLevelInfo);
#endif
}

/** entrypoint */
int32_t m64_world_app(void* p) {
    UNUSED(p);
    m64world_app_set_log_level();

    // create the app context struct, scene manager, and view dispatcher
    FURI_LOG_I(TAG, "M64 world starting...");
    M64App* app = m64world_app_init();

    // set the scene and launch the main loop
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, M64AppScene_MainMenu);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    // free all memory
    FURI_LOG_I(TAG, "M64 world finishing...");
    furi_record_close(RECORD_GUI);
    m64world_app_free(app);
    return 0;
}
