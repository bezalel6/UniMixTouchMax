SET(SOURCES components/ui_comp_volumeslider.c
    components/ui_comp.c
    screens/ui_screenMain.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
