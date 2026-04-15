/**
 * @file screen_menu_experimental_settings_release.hpp
 * @brief experimental settings for MINI printer
 * !! DO not include directly, include screen_menu_experimental_settings.hpp instead
 */
#pragma once

#include "screen_menu.hpp"
#include "MItem_menus.hpp"
#include "MItem_experimental_tools.hpp"

// parent alias
using ScreenMenuExperimentalSettings__ = ScreenMenu<GuiDefaults::MenuFooter, MI_SAVE_AND_RETURN,
#if PRINTER_IS_PRUSA_MK3_5()
    MI_ALT_FAN,
#endif
    MI_Z_AXIS_LEN, MI_RESET_Z_AXIS_LEN, MI_STEPS_PER_UNIT_E, MI_RESET_STEPS_PER_UNIT, MI_DIRECTION_E, MI_RESET_DIRECTION,
    MI_LGX_PROBE_X_OFFSET, MI_LGX_PROBE_Y_OFFSET, MI_LGX_AUTO_FILAMENT_LOAD_LENGTH, MI_LGX_FILAMENT_UNLOAD_LENGTH, MI_LGX_ENABLE_EEPROM_SAVE,
    MI_SERIAL_PRINTING_SCREEN_ENABLE>;

struct ExperimentalSettingsValues {
    ExperimentalSettingsValues(ScreenMenuExperimentalSettings__ &parent);

    int32_t z_len;
    int32_t steps_per_unit_e; // has stored both index and polarity
    float probe_x_offset_mm;
    float probe_y_offset_mm;
    int32_t auto_filament_load_length_mm;
    int32_t filament_unload_length_mm;
    int32_t enable_eeprom_save;
    size_t touch_ena;

    // this is only safe as long as there are no gaps between variables
    // all variables are 32bit now, so it is safe
    bool operator==(const ExperimentalSettingsValues &other) const;
    bool operator!=(const ExperimentalSettingsValues &other) const;
};
