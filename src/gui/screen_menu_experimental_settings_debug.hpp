/**
 * @file screen_menu_experimental_settings_debug.hpp
 * @brief experimental settings for MINI printer
 * !! DO not include directly, include screen_menu_experimental_settings.hpp instead
 */
#pragma once

#include "screen_menu.hpp"
#include "MItem_menus.hpp"
#include "MItem_experimental_tools.hpp"

/*****************************************************************************/
// Screen
using ScreenMenuExperimentalSettings__ = ScreenMenu<GuiDefaults::MenuFooter,
    MI_SAVE_AND_RETURN,
#if PRINTER_IS_PRUSA_MK3_5()
    MI_ALT_FAN,
#endif
    MI_Z_AXIS_LEN,
    MI_RESET_Z_AXIS_LEN,
    MI_STEPS_PER_UNIT_X,
    MI_STEPS_PER_UNIT_Y,
    MI_STEPS_PER_UNIT_Z,
    MI_STEPS_PER_UNIT_E,
    MI_RESET_STEPS_PER_UNIT,
    MI_DIRECTION_X,
    MI_DIRECTION_Y,
    MI_DIRECTION_Z,
    MI_DIRECTION_E,
    MI_RESET_DIRECTION,
    MI_CURRENT_X,
    MI_CURRENT_Y,
    MI_CURRENT_Z,
    MI_CURRENT_E,
    MI_RESET_CURRENTS,
    MI_PROBE_X_OFFSET,
    MI_PROBE_Y_OFFSET,
    MI_AUTO_FILAMENT_LOAD_LENGTH,
    MI_FILAMENT_UNLOAD_LENGTH,
    MI_UNLOAD_RAMMING_SCALE,
    MI_ENABLE_EEPROM_SAVE,
    MI_SERIAL_PRINTING_SCREEN_ENABLE,
    MI_FAST_DRAW_ENABLE>;

struct ExperimentalSettingsValues {
    ExperimentalSettingsValues(ScreenMenuExperimentalSettings__ &parent);

    int32_t z_len;
    int32_t steps_per_unit_x;
    int32_t steps_per_unit_y;
    int32_t steps_per_unit_z;
    int32_t steps_per_unit_e;
    int32_t microsteps_x;
    int32_t microsteps_y;
    int32_t microsteps_z;
    int32_t microsteps_e;
    int32_t rms_current_ma_x;
    int32_t rms_current_ma_y;
    int32_t rms_current_ma_z;
    int32_t rms_current_ma_e;
    float probe_x_offset_mm;
    float probe_y_offset_mm;
    int32_t auto_filament_load_length_mm;
    int32_t filament_unload_length_mm;
    int32_t unload_ramming_scale_percent;
    uint8_t enable_eeprom_save;

    // this is only safe as long as there are no gaps between variabes
    // all variables are 32bit now, so it is safe
    bool operator==(const ExperimentalSettingsValues &other) const;
    bool operator!=(const ExperimentalSettingsValues &other) const;
};
