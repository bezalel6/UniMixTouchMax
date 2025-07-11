// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.3
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#include "../ui.h"

lv_obj_t * ui_balanceVolumeSlider;
lv_obj_t * ui_lblBalanceVolumeSlider;
lv_obj_t * ui_singleVolumeSlider;
lv_obj_t * ui_lblSingleVolumeSlider;
lv_obj_t * ui_primaryVolumeSlider;
lv_obj_t * ui_lblPrimaryVolumeSlider;
lv_obj_t * ui_screenMain = NULL;
lv_obj_t * ui_tabsModeSwitch = NULL;
lv_obj_t * ui_Master = NULL;
lv_obj_t * ui_pnlPrimaryAudioDevice = NULL;
lv_obj_t * ui_lblPrimaryAudioDeviceValue = NULL;
lv_obj_t * ui_containerPrimaryVolumeSlider = NULL;
lv_obj_t * ui_Single = NULL;
lv_obj_t * ui_pnlSingleSelectAudioDevice = NULL;
lv_obj_t * ui_selectAudioDevice = NULL;
lv_obj_t * ui_containerSingleVolumeSlider = NULL;
lv_obj_t * ui_Balance = NULL;
lv_obj_t * ui_pnlBalanceAudioDevices = NULL;
lv_obj_t * ui_selectAudioDevice2 = NULL;
lv_obj_t * ui_selectAudioDevice1 = NULL;
lv_obj_t * ui_containerBalanceVolumeSlider = NULL;
lv_obj_t * ui_statusView = NULL;
lv_obj_t * ui_containerBuildTime = NULL;
lv_obj_t * ui_lblBuildTime = NULL;
lv_obj_t * ui_lblBuildTimeValue = NULL;
lv_obj_t * ui_Label1 = NULL;
lv_obj_t * ui_Label3 = NULL;
lv_obj_t * ui_pnlSettings = NULL;
lv_obj_t * ui_lblSettingsHeader = NULL;
lv_obj_t * ui_actionBtns = NULL;
lv_obj_t * ui_btnGOTOLog = NULL;
lv_obj_t * ui_btnGOTOSettings = NULL;
lv_obj_t * ui_btnRequestStatus = NULL;
lv_obj_t * ui_pnlNetwork = NULL;
lv_obj_t * ui_wifiContainer = NULL;
lv_obj_t * ui_lblWifi = NULL;
lv_obj_t * ui_lblWifiStatus = NULL;
lv_obj_t * ui_objWifiIndicator = NULL;
lv_obj_t * ui_ssidContainer = NULL;
lv_obj_t * ui_lblSSID = NULL;
lv_obj_t * ui_lblSSIDValue = NULL;
lv_obj_t * ui_ipContainer = NULL;
lv_obj_t * ui_lblIP = NULL;
lv_obj_t * ui_lblIPValue = NULL;
lv_obj_t * ui_sdContainer = NULL;
lv_obj_t * ui_lblSD = NULL;
lv_obj_t * ui_objSDIndicator = NULL;
lv_obj_t * ui_lblSDStatus = NULL;
lv_obj_t * ui_mqttContainer = NULL;
lv_obj_t * ui_lblMQTT = NULL;
lv_obj_t * ui_objMQTTIndicator = NULL;
lv_obj_t * ui_lblMQTTValue = NULL;
lv_obj_t * ui_lblFPS = NULL;
lv_obj_t * ui_img = NULL;
// event funtions
void ui_event_containerSingleVolumeSlider_containerSingleVolumeSlider_singleVolumeSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_arc_set_text_value(ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                               target, "", "%");
    }
}

void ui_event_containerBalanceVolumeSlider_containerBalanceVolumeSlider_balanceVolumeSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_arc_set_text_value(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                               target, "", "%");
    }
}

void ui_event_statusView(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        slideIntoFrame_Animation(ui_pnlSettings, 0);
    }
}

void ui_event_pnlSettings(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        slideOutOfFrame_Animation(ui_pnlSettings, 0);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        slideOutOfFrame_Animation(ui_pnlSettings, 0);
    }
}

void ui_event_btnGOTOLog_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_screenDebug, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_screenDebug_screen_init);
    }
}

void ui_event_btnGOTOSettings_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        (e);
    }
}

// build funtions

void ui_screenMain_screen_init(void)
{
    ui_screenMain = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_screenMain, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_tabsModeSwitch = lv_tabview_create(ui_screenMain);
    lv_tabview_set_tab_bar_position(ui_tabsModeSwitch, LV_DIR_BOTTOM);
    lv_tabview_set_tab_bar_size(ui_tabsModeSwitch, 50);
    lv_obj_set_width(ui_tabsModeSwitch, lv_pct(100));
    lv_obj_set_height(ui_tabsModeSwitch, lv_pct(100));
    lv_obj_set_align(ui_tabsModeSwitch, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_tabsModeSwitch, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_set_style_border_width(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 2,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_color(lv_tabview_get_tab_bar(ui_tabsModeSwitch), lv_color_hex(0xF90000),
                                  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 255,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 2,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(lv_tabview_get_tab_bar(ui_tabsModeSwitch), LV_BORDER_SIDE_FULL,
                                 LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(lv_tabview_get_tab_bar(ui_tabsModeSwitch), lv_color_hex(0xE90000),
                                   LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 255,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 3,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(lv_tabview_get_tab_bar(ui_tabsModeSwitch), 2,  LV_PART_ITEMS | LV_STATE_DEFAULT);

    ui_Master = lv_tabview_add_tab(ui_tabsModeSwitch, "Master");
    lv_obj_remove_flag(ui_Master, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_pnlPrimaryAudioDevice = lv_obj_create(ui_Master);
    lv_obj_set_height(ui_pnlPrimaryAudioDevice, 50);
    lv_obj_set_width(ui_pnlPrimaryAudioDevice, lv_pct(100));
    lv_obj_set_align(ui_pnlPrimaryAudioDevice, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(ui_pnlPrimaryAudioDevice, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblPrimaryAudioDeviceValue = lv_label_create(ui_pnlPrimaryAudioDevice);
    lv_obj_set_width(ui_lblPrimaryAudioDeviceValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblPrimaryAudioDeviceValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblPrimaryAudioDeviceValue, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblPrimaryAudioDeviceValue, "-");

    ui_containerPrimaryVolumeSlider = ui_VolumeSlider_create(ui_Master);
    lv_obj_set_x(ui_containerPrimaryVolumeSlider, 0);
    lv_obj_set_y(ui_containerPrimaryVolumeSlider, 0);
    lv_obj_add_flag(ui_containerPrimaryVolumeSlider, LV_OBJ_FLAG_OVERFLOW_VISIBLE);     /// Flags

    lv_obj_set_width(ui_comp_get_child(ui_containerPrimaryVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER), 200);
    lv_obj_set_height(ui_comp_get_child(ui_containerPrimaryVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                      LV_SIZE_CONTENT);   /// 1
    lv_label_set_long_mode(ui_comp_get_child(ui_containerPrimaryVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                           LV_LABEL_LONG_CLIP);

    ui_Single = lv_tabview_add_tab(ui_tabsModeSwitch, "Single");
    lv_obj_remove_flag(ui_Single, LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags

    ui_pnlSingleSelectAudioDevice = lv_obj_create(ui_Single);
    lv_obj_set_height(ui_pnlSingleSelectAudioDevice, 50);
    lv_obj_set_width(ui_pnlSingleSelectAudioDevice, lv_pct(100));
    lv_obj_set_align(ui_pnlSingleSelectAudioDevice, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(ui_pnlSingleSelectAudioDevice, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_selectAudioDevice = lv_dropdown_create(ui_pnlSingleSelectAudioDevice);
    lv_dropdown_set_options(ui_selectAudioDevice, "None 1\nNone 2\nNone 3");
    lv_obj_set_width(ui_selectAudioDevice, lv_pct(50));
    lv_obj_set_height(ui_selectAudioDevice, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_selectAudioDevice, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_selectAudioDevice, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    ui_containerSingleVolumeSlider = ui_VolumeSlider_create(ui_Single);
    lv_obj_set_x(ui_containerSingleVolumeSlider, 0);
    lv_obj_set_y(ui_containerSingleVolumeSlider, 0);

    lv_obj_set_width(ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER), 200);
    lv_obj_set_height(ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                      LV_SIZE_CONTENT);   /// 1
    lv_label_set_long_mode(ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                           LV_LABEL_LONG_CLIP);

    ui_Balance = lv_tabview_add_tab(ui_tabsModeSwitch, "Balance");
    lv_obj_remove_flag(ui_Balance, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_pnlBalanceAudioDevices = lv_obj_create(ui_Balance);
    lv_obj_set_height(ui_pnlBalanceAudioDevices, 50);
    lv_obj_set_width(ui_pnlBalanceAudioDevices, lv_pct(100));
    lv_obj_set_align(ui_pnlBalanceAudioDevices, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_flex_flow(ui_pnlBalanceAudioDevices, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_pnlBalanceAudioDevices, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_pnlBalanceAudioDevices, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_selectAudioDevice2 = lv_dropdown_create(ui_pnlBalanceAudioDevices);
    lv_dropdown_set_options(ui_selectAudioDevice2, "None");
    lv_obj_set_width(ui_selectAudioDevice2, lv_pct(49));
    lv_obj_set_height(ui_selectAudioDevice2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_selectAudioDevice2, 0);
    lv_obj_set_y(ui_selectAudioDevice2, -1);
    lv_obj_set_align(ui_selectAudioDevice2, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_selectAudioDevice2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    ui_selectAudioDevice1 = lv_dropdown_create(ui_pnlBalanceAudioDevices);
    lv_dropdown_set_options(ui_selectAudioDevice1, "None");
    lv_obj_set_width(ui_selectAudioDevice1, lv_pct(49));
    lv_obj_set_height(ui_selectAudioDevice1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_selectAudioDevice1, 0);
    lv_obj_set_y(ui_selectAudioDevice1, -1);
    lv_obj_set_align(ui_selectAudioDevice1, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_flag(ui_selectAudioDevice1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags

    ui_containerBalanceVolumeSlider = ui_VolumeSlider_create(ui_Balance);
    lv_obj_set_x(ui_containerBalanceVolumeSlider, 0);
    lv_obj_set_y(ui_containerBalanceVolumeSlider, 0);

    lv_obj_set_width(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER), 200);
    lv_obj_set_height(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                      LV_SIZE_CONTENT);   /// 1
    lv_label_set_long_mode(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER),
                           LV_LABEL_LONG_CLIP);

    lv_arc_set_value(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER), 50);
    lv_arc_set_mode(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER),
                    LV_ARC_MODE_SYMMETRICAL);

    ui_statusView = lv_obj_create(ui_tabsModeSwitch);
    lv_obj_remove_style_all(ui_statusView);
    lv_obj_set_height(ui_statusView, 30);
    lv_obj_set_width(ui_statusView, lv_pct(100));
    lv_obj_set_align(ui_statusView, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(ui_statusView, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_statusView, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_obj_add_flag(ui_statusView, LV_OBJ_FLAG_OVERFLOW_VISIBLE);     /// Flags
    lv_obj_remove_flag(ui_statusView, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_side(ui_statusView, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_statusView, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_statusView, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_statusView, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_statusView, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_statusView, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_statusView, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_statusView, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_statusView, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_containerBuildTime = lv_obj_create(ui_statusView);
    lv_obj_remove_style_all(ui_containerBuildTime);
    lv_obj_set_height(ui_containerBuildTime, 50);
    lv_obj_set_width(ui_containerBuildTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align(ui_containerBuildTime, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_containerBuildTime, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_containerBuildTime, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_containerBuildTime, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblBuildTime = lv_label_create(ui_containerBuildTime);
    lv_obj_set_width(ui_lblBuildTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblBuildTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblBuildTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblBuildTime, "Build: ");

    ui_lblBuildTimeValue = lv_label_create(ui_containerBuildTime);
    lv_obj_set_width(ui_lblBuildTimeValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblBuildTimeValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblBuildTimeValue, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblBuildTimeValue, "9:41 PM    12/12");

    ui_Label1 = lv_label_create(ui_statusView);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "Wifi: Disconnected");

    ui_Label3 = lv_label_create(ui_statusView);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "SD: Mounted");

    ui_pnlSettings = lv_obj_create(ui_screenMain);
    lv_obj_set_width(ui_pnlSettings, 400);
    lv_obj_set_height(ui_pnlSettings, 200);
    lv_obj_set_x(ui_pnlSettings, 0);
    lv_obj_set_y(ui_pnlSettings, -200);
    lv_obj_set_align(ui_pnlSettings, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_pnlSettings, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_OVERFLOW_VISIBLE |
                    LV_OBJ_FLAG_EVENT_BUBBLE);    /// Flags
    lv_obj_remove_flag(ui_pnlSettings, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_pnlSettings, lv_color_hex(0x2765C1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_pnlSettings, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_pnlSettings, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_pnlSettings, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_pnlSettings, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblSettingsHeader = lv_label_create(ui_pnlSettings);
    lv_obj_set_width(ui_lblSettingsHeader, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblSettingsHeader, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblSettingsHeader, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_lblSettingsHeader, "SETTINGS");
    lv_obj_set_style_text_font(ui_lblSettingsHeader, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_actionBtns = lv_obj_create(ui_pnlSettings);
    lv_obj_remove_style_all(ui_actionBtns);
    lv_obj_set_width(ui_actionBtns, lv_pct(50));
    lv_obj_set_height(ui_actionBtns, lv_pct(80));
    lv_obj_set_align(ui_actionBtns, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_flex_flow(ui_actionBtns, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_actionBtns, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_obj_remove_flag(ui_actionBtns, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SNAPPABLE |
                       LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_btnGOTOLog = ui_btn_create(ui_actionBtns);
    lv_obj_set_x(ui_btnGOTOLog, 0);
    lv_obj_set_y(ui_btnGOTOLog, 0);

    lv_label_set_text(ui_comp_get_child(ui_btnGOTOLog, UI_COMP_BTN_BTNLBL), "Log");

    ui_btnGOTOSettings = ui_btn_create(ui_actionBtns);
    lv_obj_set_x(ui_btnGOTOSettings, 0);
    lv_obj_set_y(ui_btnGOTOSettings, 0);

    lv_label_set_text(ui_comp_get_child(ui_btnGOTOSettings, UI_COMP_BTN_BTNLBL), "System Overview");

    ui_btnRequestStatus = ui_btn_create(ui_actionBtns);
    lv_obj_set_x(ui_btnRequestStatus, 0);
    lv_obj_set_y(ui_btnRequestStatus, 0);

    lv_label_set_text(ui_comp_get_child(ui_btnRequestStatus, UI_COMP_BTN_BTNLBL), "Request Status");

    ui_pnlNetwork = lv_obj_create(ui_screenMain);
    lv_obj_set_width(ui_pnlNetwork, 200);
    lv_obj_set_height(ui_pnlNetwork, 268);
    lv_obj_set_x(ui_pnlNetwork, 10);
    lv_obj_set_y(ui_pnlNetwork, -65);
    lv_obj_set_align(ui_pnlNetwork, LV_ALIGN_LEFT_MID);
    lv_obj_set_flex_flow(ui_pnlNetwork, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_pnlNetwork, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_add_flag(ui_pnlNetwork, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_remove_flag(ui_pnlNetwork, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_wifiContainer = lv_obj_create(ui_pnlNetwork);
    lv_obj_remove_style_all(ui_wifiContainer);
    lv_obj_set_width(ui_wifiContainer, lv_pct(100));
    lv_obj_set_height(ui_wifiContainer, LV_SIZE_CONTENT);    /// 100
    lv_obj_remove_flag(ui_wifiContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblWifi = lv_label_create(ui_wifiContainer);
    lv_obj_set_width(ui_lblWifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblWifi, LV_SIZE_CONTENT);    /// 1
    lv_label_set_text(ui_lblWifi, "Wifi:");

    ui_lblWifiStatus = lv_label_create(ui_wifiContainer);
    lv_obj_set_width(ui_lblWifiStatus, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblWifiStatus, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblWifiStatus, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_lblWifiStatus, "-");

    ui_objWifiIndicator = lv_label_create(ui_wifiContainer);
    lv_obj_set_width(ui_objWifiIndicator, 10);
    lv_obj_set_height(ui_objWifiIndicator, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_objWifiIndicator, -1);
    lv_obj_set_y(ui_objWifiIndicator, 0);
    lv_obj_set_align(ui_objWifiIndicator, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_objWifiIndicator, "");

    ui_ssidContainer = lv_obj_create(ui_pnlNetwork);
    lv_obj_remove_style_all(ui_ssidContainer);
    lv_obj_set_width(ui_ssidContainer, lv_pct(100));
    lv_obj_set_height(ui_ssidContainer, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_align(ui_ssidContainer, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ssidContainer, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_remove_flag(ui_ssidContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblSSID = lv_label_create(ui_ssidContainer);
    lv_obj_set_width(ui_lblSSID, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblSSID, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblSSID, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_lblSSID, "SSID:");

    ui_lblSSIDValue = lv_label_create(ui_ssidContainer);
    lv_obj_set_width(ui_lblSSIDValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblSSIDValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblSSIDValue, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_lblSSIDValue, "-");

    ui_ipContainer = lv_obj_create(ui_pnlNetwork);
    lv_obj_remove_style_all(ui_ipContainer);
    lv_obj_set_width(ui_ipContainer, lv_pct(100));
    lv_obj_set_height(ui_ipContainer, LV_SIZE_CONTENT);    /// 1
    lv_obj_add_flag(ui_ipContainer, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_remove_flag(ui_ipContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblIP = lv_label_create(ui_ipContainer);
    lv_obj_set_width(ui_lblIP, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblIP, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblIP, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(ui_lblIP, "IP:");

    ui_lblIPValue = lv_label_create(ui_ipContainer);
    lv_obj_set_width(ui_lblIPValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblIPValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblIPValue, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_lblIPValue, "-");

    ui_sdContainer = lv_obj_create(ui_pnlNetwork);
    lv_obj_remove_style_all(ui_sdContainer);
    lv_obj_set_width(ui_sdContainer, lv_pct(100));
    lv_obj_set_height(ui_sdContainer, LV_SIZE_CONTENT);    /// 1
    lv_obj_remove_flag(ui_sdContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblSD = lv_label_create(ui_sdContainer);
    lv_obj_set_width(ui_lblSD, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblSD, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblSD, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(ui_lblSD, "SD:");

    ui_objSDIndicator = lv_label_create(ui_sdContainer);
    lv_obj_set_width(ui_objSDIndicator, 10);
    lv_obj_set_height(ui_objSDIndicator, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_objSDIndicator, -1);
    lv_obj_set_y(ui_objSDIndicator, 0);
    lv_obj_set_align(ui_objSDIndicator, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_objSDIndicator, "");

    ui_lblSDStatus = lv_label_create(ui_sdContainer);
    lv_obj_set_width(ui_lblSDStatus, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblSDStatus, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblSDStatus, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_lblSDStatus, "-");

    ui_mqttContainer = lv_obj_create(ui_pnlNetwork);
    lv_obj_remove_style_all(ui_mqttContainer);
    lv_obj_set_width(ui_mqttContainer, lv_pct(100));
    lv_obj_set_height(ui_mqttContainer, LV_SIZE_CONTENT);    /// 1
    lv_obj_add_flag(ui_mqttContainer, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_remove_flag(ui_mqttContainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_lblMQTT = lv_label_create(ui_mqttContainer);
    lv_obj_set_width(ui_lblMQTT, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblMQTT, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblMQTT, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(ui_lblMQTT, "MQTT:");

    ui_objMQTTIndicator = lv_label_create(ui_mqttContainer);
    lv_obj_set_width(ui_objMQTTIndicator, 10);
    lv_obj_set_height(ui_objMQTTIndicator, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_objMQTTIndicator, -1);
    lv_obj_set_y(ui_objMQTTIndicator, 0);
    lv_obj_set_align(ui_objMQTTIndicator, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_objMQTTIndicator, "");

    ui_lblMQTTValue = lv_label_create(ui_mqttContainer);
    lv_obj_set_width(ui_lblMQTTValue, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblMQTTValue, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_lblMQTTValue, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_lblMQTTValue, "-");

    ui_lblFPS = lv_label_create(ui_screenMain);
    lv_obj_set_width(ui_lblFPS, 100);
    lv_obj_set_height(ui_lblFPS, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblFPS, -78);
    lv_obj_set_y(ui_lblFPS, 30);
    lv_obj_set_align(ui_lblFPS, LV_ALIGN_TOP_RIGHT);
    lv_label_set_long_mode(ui_lblFPS, LV_LABEL_LONG_DOT);
    lv_label_set_text(ui_lblFPS, "FPS: 1000");

    ui_img = lv_image_create(ui_screenMain);
    lv_image_set_src(ui_img, &ui_img_2039083_png);
    lv_obj_set_width(ui_img, 100);
    lv_obj_set_height(ui_img, 100);
    lv_obj_set_x(ui_img, 255);
    lv_obj_set_y(ui_img, -67);
    lv_obj_set_align(ui_img, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_img, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_OVERFLOW_VISIBLE);     /// Flags
    lv_obj_remove_flag(ui_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_img, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER),
                        ui_event_containerSingleVolumeSlider_containerSingleVolumeSlider_singleVolumeSlider, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER),
                        ui_event_containerBalanceVolumeSlider_containerBalanceVolumeSlider_balanceVolumeSlider, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_statusView, ui_event_statusView, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_btnGOTOLog, ui_event_btnGOTOLog_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_btnGOTOSettings, ui_event_btnGOTOSettings_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_pnlSettings, ui_event_pnlSettings, LV_EVENT_ALL, NULL);
    ui_lblPrimaryVolumeSlider = ui_comp_get_child(ui_containerPrimaryVolumeSlider,
                                                  UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER);
    ui_primaryVolumeSlider = ui_comp_get_child(ui_containerPrimaryVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER);
    ui_lblSingleVolumeSlider = ui_comp_get_child(ui_containerSingleVolumeSlider,
                                                 UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER);
    ui_singleVolumeSlider = ui_comp_get_child(ui_containerSingleVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER);
    ui_lblBalanceVolumeSlider = ui_comp_get_child(ui_containerBalanceVolumeSlider,
                                                  UI_COMP_VOLUMESLIDER_LBLPRIMARYVOLUMESLIDER);
    ui_balanceVolumeSlider = ui_comp_get_child(ui_containerBalanceVolumeSlider, UI_COMP_VOLUMESLIDER_PRIMARYVOLUMESLIDER);

}

void ui_screenMain_screen_destroy(void)
{
    if(ui_screenMain) lv_obj_del(ui_screenMain);

    // NULL screen variables
    ui_screenMain = NULL;
    ui_tabsModeSwitch = NULL;
    ui_Master = NULL;
    ui_pnlPrimaryAudioDevice = NULL;
    ui_lblPrimaryAudioDeviceValue = NULL;
    ui_containerPrimaryVolumeSlider = NULL;
    ui_lblPrimaryVolumeSlider = NULL;
    ui_primaryVolumeSlider = NULL;
    ui_Single = NULL;
    ui_pnlSingleSelectAudioDevice = NULL;
    ui_selectAudioDevice = NULL;
    ui_containerSingleVolumeSlider = NULL;
    ui_lblSingleVolumeSlider = NULL;
    ui_singleVolumeSlider = NULL;
    ui_Balance = NULL;
    ui_pnlBalanceAudioDevices = NULL;
    ui_selectAudioDevice2 = NULL;
    ui_selectAudioDevice1 = NULL;
    ui_containerBalanceVolumeSlider = NULL;
    ui_lblBalanceVolumeSlider = NULL;
    ui_balanceVolumeSlider = NULL;
    ui_statusView = NULL;
    ui_containerBuildTime = NULL;
    ui_lblBuildTime = NULL;
    ui_lblBuildTimeValue = NULL;
    ui_Label1 = NULL;
    ui_Label3 = NULL;
    ui_pnlSettings = NULL;
    ui_lblSettingsHeader = NULL;
    ui_actionBtns = NULL;
    ui_btnGOTOLog = NULL;
    ui_btnGOTOSettings = NULL;
    ui_btnRequestStatus = NULL;
    ui_pnlNetwork = NULL;
    ui_wifiContainer = NULL;
    ui_lblWifi = NULL;
    ui_lblWifiStatus = NULL;
    ui_objWifiIndicator = NULL;
    ui_ssidContainer = NULL;
    ui_lblSSID = NULL;
    ui_lblSSIDValue = NULL;
    ui_ipContainer = NULL;
    ui_lblIP = NULL;
    ui_lblIPValue = NULL;
    ui_sdContainer = NULL;
    ui_lblSD = NULL;
    ui_objSDIndicator = NULL;
    ui_lblSDStatus = NULL;
    ui_mqttContainer = NULL;
    ui_lblMQTT = NULL;
    ui_objMQTTIndicator = NULL;
    ui_lblMQTTValue = NULL;
    ui_lblFPS = NULL;
    ui_img = NULL;

}
