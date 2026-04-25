/**
 * @file screen_menu_experimental_settings_release.cpp
 */

#include "screen_menu_experimental_settings.hpp"
#include "WindowMenuSpin.hpp"
#include "ScreenHandler.hpp"
#include "window_msgbox.hpp"
#include <common/sys.hpp>
#include "string.h" // memcmp
#include "MItem_experimental_tools.hpp"

void ScreenMenuExperimentalSettings::clicked_return() {
    ExperimentalSettingsValues current(*this); // ctor will handle load of values
    // unchanged
    if (current == initial) {
        Screens::Access()->Close();
        return;
    }

    switch (MsgBoxQuestion(_(save_and_reboot), Responses_YesNoCancel)) {
    case Response::Yes:
        Item<MI_Z_AXIS_LEN>().Store();
        Item<MI_STEPS_PER_UNIT_E>().Store();
        Item<MI_DIRECTION_E>().Store();

        sys_reset();
    case Response::No:
        Screens::Access()->Close();
        return;
    default:
        return; // do nothing
    }
}

ScreenMenuExperimentalSettings::ScreenMenuExperimentalSettings()
    : ScreenMenuExperimentalSettings__(_(label))
    , initial(*this) {}

void ScreenMenuExperimentalSettings::windowEvent(window_t *sender, GUI_event_t ev, void *param) {
    if (ev != GUI_event_t::CHILD_CLICK) {
        ScreenMenu::windowEvent(sender, ev, param);
        return;
    }

    switch (ClickCommand(intptr_t(param))) {
    case ClickCommand::Return:
        clicked_return();
        break;
    case ClickCommand::Reset_Z:
        Item<MI_Z_AXIS_LEN>().SetVal(DEFAULT_Z_MAX_POS);
        Invalidate();
        break;
    case ClickCommand::Reset_steps:
        Item<MI_STEPS_PER_UNIT_E>().SetVal(std::abs(config_store().axis_steps_per_unit_e0.default_val));
        Invalidate();
        break;
    case ClickCommand::Reset_directions:
        Item<MI_DIRECTION_E>().set_index(0);
        Invalidate();
        break;
    case ClickCommand::Reset_load_unload:
        Item<MI_AUTOLOAD_INSERT_LENGTH>().SetVal(config_store().autoload_insert_length_mm.default_val);
        Item<MI_AUTO_FILAMENT_LOAD_LENGTH>().SetVal(config_store().auto_filament_load_length_mm.default_val);
        Item<MI_FILAMENT_UNLOAD_LENGTH>().SetVal(config_store().filament_unload_length_mm.default_val);
        Item<MI_UNLOAD_RAMMING_SCALE>().SetVal(config_store().unload_ramming_scale_percent.default_val);
        Item<MI_UNLOAD_COOLING_RETRACT>().SetVal(config_store().unload_cooling_retract_mm.default_val);
        set_autoload_insert_length_mm(config_store().autoload_insert_length_mm.default_val);
        set_auto_filament_load_length_mm(config_store().auto_filament_load_length_mm.default_val);
        set_filament_unload_length_mm(config_store().filament_unload_length_mm.default_val);
        set_unload_ramming_scale_percent(config_store().unload_ramming_scale_percent.default_val);
        set_unload_cooling_retract_mm(config_store().unload_cooling_retract_mm.default_val);
        Invalidate();
        break;
    default:
        break;
    }
}

bool ExperimentalSettingsValues::operator==(const ExperimentalSettingsValues &other) const {
    return memcmp(this, &other, sizeof(ExperimentalSettingsValues)) == 0;
}
bool ExperimentalSettingsValues::operator!=(const ExperimentalSettingsValues &other) const {
    return !(*this == other);
}

ExperimentalSettingsValues::ExperimentalSettingsValues(ScreenMenuExperimentalSettings__ &parent)
    : z_len(parent.Item<MI_Z_AXIS_LEN>().GetVal())
    , steps_per_unit_e(parent.Item<MI_STEPS_PER_UNIT_E>().GetVal() * ((parent.Item<MI_DIRECTION_E>().get_index() == 1) ? -1 : 1))
    , probe_x_offset_mm(parent.Item<MI_PROBE_X_OFFSET>().value())
    , probe_y_offset_mm(parent.Item<MI_PROBE_Y_OFFSET>().value())
    , auto_filament_load_length_mm(parent.Item<MI_AUTO_FILAMENT_LOAD_LENGTH>().GetVal())
    , autoload_insert_length_mm(parent.Item<MI_AUTOLOAD_INSERT_LENGTH>().GetVal())
    , filament_unload_length_mm(parent.Item<MI_FILAMENT_UNLOAD_LENGTH>().GetVal())
    , unload_ramming_scale_percent(parent.Item<MI_UNLOAD_RAMMING_SCALE>().GetVal())
    , unload_cooling_retract_mm(parent.Item<MI_UNLOAD_COOLING_RETRACT>().GetVal())
    , enable_eeprom_save(parent.Item<MI_ENABLE_EEPROM_SAVE>().value())
    , enable_print_finish_melody(parent.Item<MI_ENABLE_PRINT_FINISH_MELODY>().value())

{}
