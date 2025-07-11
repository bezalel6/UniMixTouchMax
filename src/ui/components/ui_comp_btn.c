// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.3
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#include "../ui.h"

// COMPONENT btn

lv_obj_t * ui_btn_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_btn;
    cui_btn = lv_button_create(comp_parent);
    lv_obj_set_width(cui_btn, lv_pct(100));
    lv_obj_set_height(cui_btn, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_btn, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(cui_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t * cui_btnLbl;
    cui_btnLbl = lv_label_create(cui_btn);
    lv_obj_set_width(cui_btnLbl, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_btnLbl, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_btnLbl, LV_ALIGN_CENTER);
    lv_label_set_text(cui_btnLbl, "text");

    lv_obj_t ** children = lv_malloc(sizeof(lv_obj_t *) * _UI_COMP_BTN_NUM);
    children[UI_COMP_BTN_BTN] = cui_btn;
    children[UI_COMP_BTN_BTNLBL] = cui_btnLbl;
    lv_obj_add_event_cb(cui_btn, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_btn, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_btn_create_hook(cui_btn);
    return cui_btn;
}

