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
        Item<MI_LGX_PROBE_X_OFFSET>().Store();
        Item<MI_LGX_PROBE_Y_OFFSET>().Store();
        Item<MI_LGX_AUTO_FILAMENT_LOAD_LENGTH>().Store();
        Item<MI_LGX_FILAMENT_UNLOAD_LENGTH>().Store();
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
    , probe_x_offset(parent.Item<MI_LGX_PROBE_X_OFFSET>().GetVal())
    , probe_y_offset(parent.Item<MI_LGX_PROBE_Y_OFFSET>().GetVal())
    , auto_load_length(parent.Item<MI_LGX_AUTO_FILAMENT_LOAD_LENGTH>().GetVal())
    , unload_length(parent.Item<MI_LGX_FILAMENT_UNLOAD_LENGTH>().GetVal())
    , eeprom_save_enabled(parent.Item<MI_LGX_ENABLE_EEPROM_SAVE>().value())
    , steps_per_unit_e(parent.Item<MI_STEPS_PER_UNIT_E>().GetVal() * ((parent.Item<MI_DIRECTION_E>().get_index() == 1) ? -1 : 1))
    , touch_ena(parent.Item<MI_SERIAL_PRINTING_SCREEN_ENABLE>().value())

{}
