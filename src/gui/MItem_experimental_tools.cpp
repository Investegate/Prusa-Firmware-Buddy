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
#include <config_store/store_instance.hpp>
#include <module/planner.h>
#include <cmath>

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

// Custom LGX/mod entries are intentionally colorized to distinguish them
// from native firmware items in the same Experimental menu.
static constexpr IWindowMenuItem::ColorScheme custom_mod_orange_scheme {
    .text = {
        .focused = COLOR_ORANGE,
        .unfocused = COLOR_ORANGE,
    },
};

MI_PROBE_X_OFFSET::MI_PROBE_X_OFFSET()
    : WiSpin(get_probe_x_offset_mm(), probe_offset_spin_config, _("Probe X offset")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_PROBE_X_OFFSET::OnClick() {
    const float offset = value();
    set_probe_x_offset_mm(offset);
    marlin_client::gcode_printf("M851 X%f", static_cast<double>(offset));
}

MI_PROBE_Y_OFFSET::MI_PROBE_Y_OFFSET()
    : WiSpin(get_probe_y_offset_mm(), probe_offset_spin_config, _("Probe Y offset")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

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
    : WiSpin(get_auto_filament_load_length_mm(), filament_length_spin_config, _("Fast load length")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_AUTO_FILAMENT_LOAD_LENGTH::OnClick() {
    set_auto_filament_load_length_mm(value());
}

MI_AUTOLOAD_INSERT_LENGTH::MI_AUTOLOAD_INSERT_LENGTH()
    : WiSpin(get_autoload_insert_length_mm(), filament_length_spin_config, _("Initial insert length")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_AUTOLOAD_INSERT_LENGTH::OnClick() {
    set_autoload_insert_length_mm(value());
}

MI_FILAMENT_UNLOAD_LENGTH::MI_FILAMENT_UNLOAD_LENGTH()
    : WiSpin(get_filament_unload_length_mm(), filament_length_spin_config, _("Unload length")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_FILAMENT_UNLOAD_LENGTH::OnClick() {
    set_filament_unload_length_mm(value());
}

static constexpr NumericInputConfig unload_ramming_scale_spin_config = {
    .min_value = 10,
    .max_value = 150,
    .unit = Unit::percent,
};

MI_UNLOAD_RAMMING_SCALE::MI_UNLOAD_RAMMING_SCALE()
    : WiSpin(get_unload_ramming_scale_percent(), unload_ramming_scale_spin_config, _("Unload ramming scale")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_UNLOAD_RAMMING_SCALE::OnClick() {
    set_unload_ramming_scale_percent(value());
}

static constexpr NumericInputConfig unload_cooling_retract_spin_config = {
    .min_value = 0,
    .max_value = 20,
    .unit = Unit::millimeter,
};

MI_UNLOAD_COOLING_RETRACT::MI_UNLOAD_COOLING_RETRACT()
    : WiSpin(get_unload_cooling_retract_mm(), unload_cooling_retract_spin_config, _("Unload cooling retract")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_UNLOAD_COOLING_RETRACT::OnClick() {
    set_unload_cooling_retract_mm(value());
}

MI_ENABLE_EEPROM_SAVE::MI_ENABLE_EEPROM_SAVE()
    : WI_ICON_SWITCH_OFF_ON_t(get_enable_eeprom_save(), _("Enable EEPROM save")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_ENABLE_EEPROM_SAVE::OnChange([[maybe_unused]] size_t old_index) {
    set_enable_eeprom_save(value());
}

MI_RESET_M500_TUNING::MI_RESET_M500_TUNING()
    : IWindowMenuItem(_("Reset M500 tuning")) {
    set_color_scheme(&custom_mod_orange_scheme);
}

void MI_RESET_M500_TUNING::click([[maybe_unused]] IWindowMenu &window_menu) {
    config_store().axis_steps_per_unit_x.set(std::abs(config_store().axis_steps_per_unit_x.default_val));
    config_store().axis_steps_per_unit_y.set(std::abs(config_store().axis_steps_per_unit_y.default_val));
    config_store().axis_steps_per_unit_z.set(std::abs(config_store().axis_steps_per_unit_z.default_val));
    config_store().marlin_max_feedrate_x.set(config_store().marlin_max_feedrate_x.default_val);
    config_store().marlin_max_feedrate_y.set(config_store().marlin_max_feedrate_y.default_val);
    config_store().marlin_max_feedrate_z.set(config_store().marlin_max_feedrate_z.default_val);
    config_store().marlin_max_feedrate_e0.set(config_store().marlin_max_feedrate_e0.default_val);
    config_store().marlin_max_acceleration_x.set(config_store().marlin_max_acceleration_x.default_val);
    config_store().marlin_max_acceleration_y.set(config_store().marlin_max_acceleration_y.default_val);
    config_store().marlin_max_acceleration_z.set(config_store().marlin_max_acceleration_z.default_val);
    config_store().marlin_max_acceleration_e0.set(config_store().marlin_max_acceleration_e0.default_val);
    config_store().marlin_min_segment_time_us.set(config_store().marlin_min_segment_time_us.default_val);
    config_store().marlin_acceleration.set(config_store().marlin_acceleration.default_val);
    config_store().marlin_retract_acceleration.set(config_store().marlin_retract_acceleration.default_val);
    config_store().marlin_travel_acceleration.set(config_store().marlin_travel_acceleration.default_val);
    config_store().marlin_min_feedrate.set(config_store().marlin_min_feedrate.default_val);
    config_store().marlin_min_travel_feedrate.set(config_store().marlin_min_travel_feedrate.default_val);
#if HAS_CLASSIC_JERK
    config_store().marlin_max_jerk_x.set(config_store().marlin_max_jerk_x.default_val);
    config_store().marlin_max_jerk_y.set(config_store().marlin_max_jerk_y.default_val);
    config_store().marlin_max_jerk_z.set(config_store().marlin_max_jerk_z.default_val);
#if !HAS_LINEAR_E_JERK
    config_store().marlin_max_jerk_e.set(config_store().marlin_max_jerk_e.default_val);
#endif
#else
    config_store().marlin_junction_deviation_mm.set(config_store().marlin_junction_deviation_mm.default_val);
#endif
    config_store().save_all();

    auto s = planner.user_settings;
    s.axis_steps_per_mm[X_AXIS] = std::abs(config_store().axis_steps_per_unit_x.get());
    s.axis_steps_per_mm[Y_AXIS] = std::abs(config_store().axis_steps_per_unit_y.get());
    s.axis_steps_per_mm[Z_AXIS] = std::abs(config_store().axis_steps_per_unit_z.get());
    s.axis_steps_per_mm[E_AXIS] = std::abs(planner.settings.axis_steps_per_mm[E_AXIS]);
    LOOP_XYZE_N(i) {
        s.axis_msteps_per_mm[i] = s.axis_steps_per_mm[i] * PLANNER_STEPS_MULTIPLIER;
    }

    s.max_feedrate_mm_s[X_AXIS] = config_store().marlin_max_feedrate_x.get();
    s.max_feedrate_mm_s[Y_AXIS] = config_store().marlin_max_feedrate_y.get();
    s.max_feedrate_mm_s[Z_AXIS] = config_store().marlin_max_feedrate_z.get();
    s.max_feedrate_mm_s[E_AXIS] = config_store().marlin_max_feedrate_e0.get();

    s.max_acceleration_mm_per_s2[X_AXIS] = config_store().marlin_max_acceleration_x.get();
    s.max_acceleration_mm_per_s2[Y_AXIS] = config_store().marlin_max_acceleration_y.get();
    s.max_acceleration_mm_per_s2[Z_AXIS] = config_store().marlin_max_acceleration_z.get();
    s.max_acceleration_mm_per_s2[E_AXIS] = config_store().marlin_max_acceleration_e0.get();

    s.min_segment_time_us = config_store().marlin_min_segment_time_us.get();
    s.acceleration = config_store().marlin_acceleration.get();
    s.retract_acceleration = config_store().marlin_retract_acceleration.get();
    s.travel_acceleration = config_store().marlin_travel_acceleration.get();
    s.min_feedrate_mm_s = config_store().marlin_min_feedrate.get();
    s.min_travel_feedrate_mm_s = config_store().marlin_min_travel_feedrate.get();
#if HAS_CLASSIC_JERK
    s.max_jerk.x = config_store().marlin_max_jerk_x.get();
    s.max_jerk.y = config_store().marlin_max_jerk_y.get();
    s.max_jerk.z = config_store().marlin_max_jerk_z.get();
#if !HAS_LINEAR_E_JERK
    s.max_jerk.e = config_store().marlin_max_jerk_e.get();
#endif
#else
    planner.junction_deviation_mm = config_store().marlin_junction_deviation_mm.get();
#endif
    planner.apply_settings(s);
    planner.refresh_positioning();
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
