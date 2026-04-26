/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"
#include "../../module/configuration_store.h"
#include "../../core/serial.h"
#include "../../inc/MarlinConfig.h"
#include "../../module/planner.h"
#include <cmath>
#include <config_store/store_c_api.h>
#include <config_store/store_instance.hpp>

/** \addtogroup G-Codes
 * @{
 */

/**
 *### M500: Store settings in EEPROM <a href="https://reprap.org/wiki/G-code#M500:_Store_parameters_in_non-volatile_storage">M500: Store parameters in non-volatile storage</a>
 *
 * Save all configurable settings to EEPROM.
 *
 *#### Usage
 *
 *    M500
 */
void GcodeSuite::M500() {
  if (get_enable_eeprom_save()) {
    const auto &s = planner.settings;
    const auto preserve_direction_sign = [](const float current_signed_steps, const float new_steps_magnitude) {
      return std::signbit(current_signed_steps) ? -std::abs(new_steps_magnitude) : std::abs(new_steps_magnitude);
    };
    config_store().axis_steps_per_unit_x.set(preserve_direction_sign(config_store().axis_steps_per_unit_x.get(), s.axis_steps_per_mm[X_AXIS]));
    config_store().axis_steps_per_unit_y.set(preserve_direction_sign(config_store().axis_steps_per_unit_y.get(), s.axis_steps_per_mm[Y_AXIS]));
    config_store().axis_steps_per_unit_z.set(preserve_direction_sign(config_store().axis_steps_per_unit_z.get(), s.axis_steps_per_mm[Z_AXIS]));
    config_store().axis_steps_per_unit_e0.set(preserve_direction_sign(config_store().axis_steps_per_unit_e0.get(), s.axis_steps_per_mm[E_AXIS]));

    config_store().marlin_max_feedrate_x.set(s.max_feedrate_mm_s[X_AXIS]);
    config_store().marlin_max_feedrate_y.set(s.max_feedrate_mm_s[Y_AXIS]);
    config_store().marlin_max_feedrate_z.set(s.max_feedrate_mm_s[Z_AXIS]);
    config_store().marlin_max_feedrate_e0.set(s.max_feedrate_mm_s[E_AXIS]);

    config_store().marlin_max_acceleration_x.set(s.max_acceleration_mm_per_s2[X_AXIS]);
    config_store().marlin_max_acceleration_y.set(s.max_acceleration_mm_per_s2[Y_AXIS]);
    config_store().marlin_max_acceleration_z.set(s.max_acceleration_mm_per_s2[Z_AXIS]);
    config_store().marlin_max_acceleration_e0.set(s.max_acceleration_mm_per_s2[E_AXIS]);

    config_store().marlin_min_segment_time_us.set(s.min_segment_time_us);
    config_store().marlin_acceleration.set(s.acceleration);
    config_store().marlin_retract_acceleration.set(s.retract_acceleration);
    config_store().marlin_travel_acceleration.set(s.travel_acceleration);
    config_store().marlin_min_feedrate.set(s.min_feedrate_mm_s);
    config_store().marlin_min_travel_feedrate.set(s.min_travel_feedrate_mm_s);
#if HAS_CLASSIC_JERK
    config_store().marlin_max_jerk_x.set(s.max_jerk.x);
    config_store().marlin_max_jerk_y.set(s.max_jerk.y);
    config_store().marlin_max_jerk_z.set(s.max_jerk.z);
#if !HAS_LINEAR_E_JERK
    config_store().marlin_max_jerk_e.set(s.max_jerk.e);
#endif
#else
    config_store().marlin_junction_deviation_mm.set(planner.junction_deviation_mm);
#endif

    (void)settings.save();
    config_store().save_all();
  }
}

/**
 *### M501: Read settings from EEPROM <a href="https://reprap.org/wiki/G-code#M501:_Read_parameters_from_EEPROM">M501: Read parameters from EEPROM</a>
 *
 * Load all saved settings from EEPROM.
 *
 *#### Usage
 *
 *    M501
 */
void GcodeSuite::M501() {
  const auto saved_probe_offset = probe_offset;
  (void)settings.reset();
  auto s = planner.user_settings;
  s.axis_steps_per_mm[X_AXIS] = std::abs(config_store().axis_steps_per_unit_x.get());
  s.axis_steps_per_mm[Y_AXIS] = std::abs(config_store().axis_steps_per_unit_y.get());
  s.axis_steps_per_mm[Z_AXIS] = std::abs(config_store().axis_steps_per_unit_z.get());
  s.axis_steps_per_mm[E_AXIS] = std::abs(config_store().axis_steps_per_unit_e0.get());
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
#if ENABLED(USE_PRUSA_EEPROM_AS_SOURCE_OF_DEFAULT_VALUES)
  char gcode_buffer[64];
  snprintf(gcode_buffer, sizeof(gcode_buffer), "M851 X%f Y%f Z%f", static_cast<double>(saved_probe_offset.x), static_cast<double>(saved_probe_offset.y), static_cast<double>(saved_probe_offset.z));
  process_subcommands_now(gcode_buffer);
#else
  probe_offset = saved_probe_offset;
#endif
  (void)settings.report();
}

/**
 *### M502: Revert to default settings <a href="https://reprap.org/wiki/G-code#M502:_Restore_Default_Settings">M502: Restore Default Settings</a>
 *
 * Reset all configurable settings to their factory defaults. This only changes the settings in memory, not on EEPROM.
 *
 *#### Usage
 *
 *    M502
 */
void GcodeSuite::M502() {
  (void)settings.reset();
  set_probe_x_offset_mm(probe_offset.x);
  set_probe_y_offset_mm(probe_offset.y);
}

#if DISABLED(DISABLE_M503)

 /**
 *### M503: print settings currently in memory <a href="https://reprap.org/wiki/G-code#M503:_Report_Current_Settings">M503: Report Current Settings</a>
 *
 * Print a concise report of all runtime-configurable settings (in SRAM) to the host console.
 * This command reports the active settings which may or may not be the same as those stored in the EEPROM.
 *
 *#### Usage
 *
 *    M503
 */
  void GcodeSuite::M503() {
    (void)settings.report(!parser.boolval('S', true));
  }

#endif // !DISABLE_M503

/** @}*/
