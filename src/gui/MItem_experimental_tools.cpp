/**
 * @file MItem_experimental_tools.cpp
 * @author Radek Vana
 * @date 2021-08-03
 */
#include "MItem_experimental_tools.hpp"
#include "WindowMenuSpin.hpp"
#include "ScreenHandler.hpp"
#include "marlin_client.hpp"
#include "string.h" // memcmp
#include "img_resources.hpp"
#include <gui/menu_vars.h>
#include <config_store/store_c_api.h>

#if PRINTER_IS_PRUSA_MK3_5()
/*****************************************************************************/
// MI_ALT_FAN_CORRECTION
bool MI_ALT_FAN::init_index() {
    return config_store().has_alt_fans.get();
}

void MI_ALT_FAN::OnChange([[maybe_unused]] size_t old_index) {
    config_store().has_alt_fans.set(!config_store().has_alt_fans.get());
}
#endif

/*****************************************************************************/
// MI_Z_AXIS_LEN
static constexpr NumericInputConfig z_axis_len_spin_config {
    .min_value = Z_MIN_LEN_LIMIT,
    .max_value = Z_MAX_LEN_LIMIT,
    .unit = Unit::millimeter,
};

MI_Z_AXIS_LEN::MI_Z_AXIS_LEN()
    : WiSpin(get_z_max_pos_mm_rounded(), z_axis_len_spin_config, _("Z-axis length")) {}

void MI_Z_AXIS_LEN::Store() {
    set_z_max_pos_mm(GetVal());
}

/*****************************************************************************/
// MI_RESET_Z_AXIS_LEN
MI_RESET_Z_AXIS_LEN::MI_RESET_Z_AXIS_LEN()
    : IWindowMenuItem(_("Reset Z-length")) {}

void MI_RESET_Z_AXIS_LEN::click([[maybe_unused]] IWindowMenu &window_menu) {
    Screens::Access()->Get()->WindowEvent(nullptr, GUI_event_t::CHILD_CLICK, (void *)ClickCommand::Reset_Z);
}

static constexpr NumericInputConfig steps_per_unit_spin_config = {
    .min_value = 1,
    .max_value = 1000,
};

/*****************************************************************************/
// MI_STEPS_PER_UNIT_X
MI_STEPS_PER_UNIT_X::MI_STEPS_PER_UNIT_X()
    : WiSpin(get_steps_per_unit_x_rounded(), steps_per_unit_spin_config, _("X-axis steps per unit")) {}

void MI_STEPS_PER_UNIT_X::Store() {
    set_steps_per_unit_x(GetVal());
}

/*****************************************************************************/
// MI_STEPS_PER_UNIT_Y
MI_STEPS_PER_UNIT_Y::MI_STEPS_PER_UNIT_Y()
    : WiSpin(get_steps_per_unit_y_rounded(), steps_per_unit_spin_config, _("Y-axis steps per unit")) {}

void MI_STEPS_PER_UNIT_Y::Store() {
    set_steps_per_unit_y(GetVal());
}

/*****************************************************************************/
// MI_STEPS_PER_UNIT_Z
MI_STEPS_PER_UNIT_Z::MI_STEPS_PER_UNIT_Z()
    : WiSpin(get_steps_per_unit_z_rounded(), steps_per_unit_spin_config, _("Z-axis steps per unit")) {}

void MI_STEPS_PER_UNIT_Z::Store() {
    set_steps_per_unit_z(GetVal());
}

/*****************************************************************************/
// MI_STEPS_PER_UNIT_E
MI_STEPS_PER_UNIT_E::MI_STEPS_PER_UNIT_E()
    : WiSpin(get_steps_per_unit_e_rounded(), steps_per_unit_spin_config, _("Extruder steps per unit")) {}

void MI_STEPS_PER_UNIT_E::Store() {
    set_steps_per_unit_e(GetVal());
}

/*****************************************************************************/
// MI_RESET_STEPS_PER_UNIT
MI_RESET_STEPS_PER_UNIT::MI_RESET_STEPS_PER_UNIT()
    : IWindowMenuItem(_("Reset steps per unit")) {}

void MI_RESET_STEPS_PER_UNIT::click([[maybe_unused]] IWindowMenu &window_menu) {
    Screens::Access()->Get()->WindowEvent(nullptr, GUI_event_t::CHILD_CLICK, (void *)ClickCommand::Reset_steps);
}

/*****************************************************************************/
// WiSwitchDirection
static constexpr const char *switch_direction_items[] = {
    N_("Prusa"),
    N_("Wrong"),
};

WiSwitchDirection::WiSwitchDirection(bool current_direction_wrong, const string_view_utf8 &label_view)
    : MenuItemSwitch(label_view, switch_direction_items, current_direction_wrong) {}

/*****************************************************************************/
// MI_DIRECTION_X
MI_DIRECTION_X::MI_DIRECTION_X()
    : WiSwitchDirection(has_wrong_x(), _("X-axis direction")) {}

void MI_DIRECTION_X::Store() {
    get_index() == 1 ? set_wrong_direction_x() : set_PRUSA_direction_x();
}

/*****************************************************************************/
// MI_DIRECTION_Y
MI_DIRECTION_Y::MI_DIRECTION_Y()
    : WiSwitchDirection(has_wrong_y(), _("Y-axis direction")) {}

void MI_DIRECTION_Y::Store() {
    get_index() == 1 ? set_wrong_direction_y() : set_PRUSA_direction_y();
}

/*****************************************************************************/
// MI_DIRECTION_Z
MI_DIRECTION_Z::MI_DIRECTION_Z()
    : WiSwitchDirection(has_wrong_z(), _("Z-axis direction")) {}

void MI_DIRECTION_Z::Store() {
    get_index() == 1 ? set_wrong_direction_z() : set_PRUSA_direction_z();
}

/*****************************************************************************/
// MI_DIRECTION_E
MI_DIRECTION_E::MI_DIRECTION_E()
    : WiSwitchDirection(has_wrong_e(), _("Extruder direction")) {}

void MI_DIRECTION_E::Store() {
    get_index() == 1 ? set_wrong_direction_e() : set_PRUSA_direction_e();
}

/*****************************************************************************/
// MI_RESET_DIRECTION
MI_RESET_DIRECTION::MI_RESET_DIRECTION()
    : IWindowMenuItem(_("Reset directions")) {}

void MI_RESET_DIRECTION::click([[maybe_unused]] IWindowMenu &window_menu) {
    Screens::Access()->Get()->WindowEvent(nullptr, GUI_event_t::CHILD_CLICK, (void *)ClickCommand::Reset_directions);
}

static constexpr NumericInputConfig rms_current_spin_config = {
    .max_value = 800,
    .unit = Unit::milliamper,
};

/*****************************************************************************/
// MI_CURRENT_X
MI_CURRENT_X::MI_CURRENT_X()
    : WiSpin(config_store().axis_rms_current_ma_X_.get(), rms_current_spin_config, _("X current (0 default)")) {}

void MI_CURRENT_X::Store() {
    set_rms_current_ma_x(GetVal());
}

/*****************************************************************************/
// MI_CURRENT_Y
MI_CURRENT_Y::MI_CURRENT_Y()
    : WiSpin(config_store().axis_rms_current_ma_Y_.get(), rms_current_spin_config, _("Y current (0 default)")) {}

void MI_CURRENT_Y::Store() {
    set_rms_current_ma_y(GetVal());
}

/*****************************************************************************/
// MI_CURRENT_Z
MI_CURRENT_Z::MI_CURRENT_Z()
    : WiSpin(get_rms_current_ma_z(), rms_current_spin_config, _("Z current")) {}

void MI_CURRENT_Z::Store() {
    set_rms_current_ma_z(GetVal());
}

/*****************************************************************************/
// MI_CURRENT_E
MI_CURRENT_E::MI_CURRENT_E()
    : WiSpin(get_rms_current_ma_e(), rms_current_spin_config, _("Extruder current")) {}

void MI_CURRENT_E::Store() {
    set_rms_current_ma_e(GetVal());
}

static constexpr NumericInputConfig probe_offset_spin_config = {
    .min_value = -100,
    .max_value = 100,
    .unit = Unit::millimeter,
};

MI_PROBE_X_OFFSET::MI_PROBE_X_OFFSET()
    : WiSpin(get_probe_x_offset_mm(), probe_offset_spin_config, _("Probe X offset")) {}

void MI_PROBE_X_OFFSET::OnClick() {
    const float offset = value();
    set_probe_x_offset_mm(offset);
    marlin_client::gcode_printf("M851 X%f", static_cast<double>(offset));
}

MI_PROBE_Y_OFFSET::MI_PROBE_Y_OFFSET()
    : WiSpin(get_probe_y_offset_mm(), probe_offset_spin_config, _("Probe Y offset")) {}

void MI_PROBE_Y_OFFSET::OnClick() {
    const float offset = value();
    set_probe_y_offset_mm(offset);
    marlin_client::gcode_printf("M851 Y%f", static_cast<double>(offset));
}

static constexpr NumericInputConfig filament_length_spin_config = {
    .min_value = 1,
    .max_value = 1000,
    .unit = Unit::millimeter,
};

MI_AUTO_FILAMENT_LOAD_LENGTH::MI_AUTO_FILAMENT_LOAD_LENGTH()
    : WiSpin(get_auto_filament_load_length_mm(), filament_length_spin_config, _("Auto load length")) {}

void MI_AUTO_FILAMENT_LOAD_LENGTH::OnClick() {
    set_auto_filament_load_length_mm(value());
}

MI_FILAMENT_UNLOAD_LENGTH::MI_FILAMENT_UNLOAD_LENGTH()
    : WiSpin(get_filament_unload_length_mm(), filament_length_spin_config, _("Unload length")) {}

void MI_FILAMENT_UNLOAD_LENGTH::OnClick() {
    set_filament_unload_length_mm(value());
}

MI_ENABLE_EEPROM_SAVE::MI_ENABLE_EEPROM_SAVE()
    : WI_ICON_SWITCH_OFF_ON_t(get_enable_eeprom_save(), _("Enable EEPROM save")) {}

void MI_ENABLE_EEPROM_SAVE::OnChange([[maybe_unused]] size_t old_index) {
    set_enable_eeprom_save(value());
}

/*****************************************************************************/
// MI_RESET_CURRENTS
MI_RESET_CURRENTS::MI_RESET_CURRENTS()
    : IWindowMenuItem(_("Reset currents")) {}

void MI_RESET_CURRENTS::click([[maybe_unused]] IWindowMenu &window_menu) {
    Screens::Access()->Get()->WindowEvent(nullptr, GUI_event_t::CHILD_CLICK, (void *)ClickCommand::Reset_currents);
}

/*****************************************************************************/
// MI_SAVE_AND_RETURN
MI_SAVE_AND_RETURN::MI_SAVE_AND_RETURN()
    : IWindowMenuItem(_("Save and return"), &img::folder_up_16x16, is_enabled_t::yes, is_hidden_t::no) {
    has_return_behavior_ = true;
}

void MI_SAVE_AND_RETURN::click([[maybe_unused]] IWindowMenu &window_menu) {
    Screens::Access()->Get()->WindowEvent(nullptr, GUI_event_t::CHILD_CLICK, (void *)ClickCommand::Return);
}

/*****************************************************************************/
// MI_FAST_DRAW_ENABLE
// If this is put outside of ScreenMenuExperimental (that resets the printer
// after exiting), the config_store().fast_draw_enabled usage in ili9488
// must be reworked to not store the result in a static variable.
MI_FAST_DRAW_ENABLE::MI_FAST_DRAW_ENABLE()
    : WI_ICON_SWITCH_OFF_ON_t {
        config_store().fast_draw_enabled.get(),
        // translation: experimental menu item enabling faster display routines
        _("Fast Draw"),
    } {
}
void MI_FAST_DRAW_ENABLE::OnChange(size_t) {
    config_store().fast_draw_enabled.set(value());
}
