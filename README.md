<p align="center">
  <img width="900" alt="INVESTEGATE logo" src="https://github.com/user-attachments/assets/23cf7c59-ede0-4711-90d5-675f6a435695" />
</p>

<p align="center">
  <img width="69" alt="INVESTEGATE logo" src="https://github.com/user-attachments/assets/cc0d7d16-de54-402b-a90c-4dff2d34c8ce" />
</p>

# INVESTEGATE Custom Firmware for Prusa MK3.5 / MK3.5S

Unofficial custom firmware for the **Original Prusa MK3.5 / MK3.5S**, based on official **Prusa-Firmware-Buddy v6.5.3**.

This firmware adds experimental tuning options for **Bondtech LGX / LGX Shortcut** and similar third-party extruder setups while keeping the original Prusa firmware behavior as close as possible.

More information, documentation and related 3D printing projects: <strong><a href="https://investegate.de">INVESTEGATE.de</a></strong>

---

## Latest release

Current release:

**v1.0 / 6.5.3 MOD v1.0**

Download the firmware from the GitHub Releases page:

[Download 6.5.3 MOD v1.0](https://github.com/Investegate/Prusa-Firmware-Buddy/releases/tag/v1.0)

Firmware file:

`mk3.5_firmware_6.5.3_MODv1.0.bbf`

Do **not** use the automatically generated source code ZIP/TAR files for flashing.

---

## Compatibility

Supported printers:

- Original Prusa MK3.5
- Original Prusa MK3.5S

Base firmware:

- Official Prusa-Firmware-Buddy v6.5.3

Build target:

- `mk3.5_release_boot`

---

## Important warning

This is **unofficial custom firmware**.

Use it at your own risk.

Before flashing, save your current printer settings and calibration values.

After flashing, verify your settings and run a small test print before normal use.

---

## Custom firmware flashing / appendix seal

Prusa MK3.5 / MK3.5S printers accept only officially signed firmware in their factory state.

To install unofficial custom firmware, the **appendix seal on the xBuddy board must be broken first**.

According to Prusa, breaking this seal does **not** void the printer warranty. However, custom firmware is unofficial, and Prusa does not take responsibility for damage caused by unofficial firmware.

Official Prusa instructions: [Flashing custom firmware - Prusa Help](https://help.prusa3d.com/article/flashing-custom-firmware-core-one-l-core-one-mk4-s-mk3-9-s-mk3-5-s_814967)

---

## Installation

The easiest way to transfer the firmware to the printer is via **Prusa Connect**.

Flashing via **USB** is also possible.

When the flashing process begins, the printer will show a warning that **unsigned firmware** was detected.

Select **Ignore** to continue with the installation.

In testing, the printer retained all previous calibration values after flashing. This also worked when downgrading again.

Even so, it is strongly recommended to save your current values before flashing.

---

## How to open the Experimental menu

On the printer, open **Settings**.

Then press and hold **Return** until the **Experimental Settings** menu opens.

### Testing status / color meaning

- 🟧 **Orange** = custom options from this mod  
  These options were successfully and thoroughly tested. No errors were found during testing.

- 🟩 **Green** = Prusa developer options  
  These are existing developer options from Prusa and are not part of this mod itself.

- 🟥 **Red** = planned future custom options  
  Future features that are not yet fully tested will later be shown in red until they have been thoroughly validated in real use.

Use these settings carefully and change only one value at a time. After changing filament handling or probe settings, run a small test print before normal use.

---

## Screenshots

### Start screen

<p align="center">
  <img width="500" alt="MK3.5S custom firmware splash screen" src="https://github.com/user-attachments/assets/7ae30dd3-a4a3-40b7-a4a4-4843b3109d40" />
</p>

Screenshot of the custom splash / boot screen.

### Experimental menu - custom mod options

<p align="center">
  <img width="500" alt="MK3.5S custom menu probe settings" src="https://github.com/user-attachments/assets/035324a1-8502-4695-8bfc-32ce0ec649df" />
</p>

<p align="center">
  <img width="500" alt="MK3.5S custom menu load unload settings" src="https://github.com/user-attachments/assets/ff8a2df2-1bc1-4a9b-ba1d-2f15ba220f78" />
</p>

<p align="center">
  <img width="500" alt="MK3.5S custom menu reset M500 settings" src="https://github.com/user-attachments/assets/1835a496-3eb1-496f-a61c-a0ba06f1bb59" />
</p>

Screenshots showing the new Experimental menu entries added by this mod.

All custom mod options are highlighted in **orange**.

---

## Main features

### Custom firmware branding / splash text

Adds a small custom firmware text to the boot / splash screen.

The original Prusa branding and boot behavior were intentionally kept as close to stock as possible.

---

### Custom Experimental Menu entries

Additional custom settings were added to the Experimental menu.

Custom mod entries are highlighted in **orange** to visually separate them from native Prusa menu items.

Added / modified entries include:

- Print finish melody
- Probe X position
- Probe Y position
- Reset probe position
- Initial insert length
- Fast load length
- Unload length
- Unload cooling retract
- Unload ramming scale
- Reset load/unload
- Enable EEPROM save
- Reset M500 tuning

---

### Probe X/Y position controls

This release adds direct M851 X/Y controls:

- Probe X position
- Probe Y position
- Reset probe position

These are **direct M851 X/Y values**, not relative offsets.

Stock Prusa default:

```gcode
M851 X23.00 Y5.00
```

Recommended Bondtech LGX / LGX Shortcut value:

```gcode
M851 X23.00 Y12.00
```

Important:

`M851 Z` / Live-Z is not changed by these menu controls.

Do not blindly copy Z values from another printer. Z / Live-Z is printer-specific and must be calibrated on your own machine.

Example only:

```gcode
M851 X23.00 Y12.00 Z-1.50
```

Your Z value will likely be different.

---

### Reset probe position

`Reset probe position` resets only Probe X/Y to Prusa defaults:

```gcode
M851 X23.00 Y5.00
```

It does **not** change:

- M851 Z / Live-Z
- Load / unload values
- M500 tuning values
- E-steps
- Stepper current
- Axis direction
- Print finish melody
- EEPROM save setting

---

### Load / unload tuning

The following filament handling values are configurable from the Experimental menu:

- Initial insert length
- Fast load length
- Unload length
- Unload cooling retract
- Unload ramming scale
- Reset load/unload

#### Initial insert length

Controls the first slow insert / load-to-gears movement.

Used by:

- Dashboard autoload
- Menu → Filament → Load
- M600 filament change during print

#### Fast load length

Controls the later hot fast-load movement.

This remains separate from Initial insert length.

#### Unload length

Controls the unload distance.

The maximum value is capped to:

```text
199 mm
```

Reason: testing showed that single unload moves around 200 mm and above can become unreliable.

Unload chunking is intentionally not included in v1.0.

#### Unload cooling retract

Executed after ramming and before the remaining unload.

The duration is approximately 5 seconds. The selected distance controls the retract speed.

Examples:

```text
5 mm  = approx. 1 mm/s for 5 seconds
10 mm = approx. 2 mm/s for 5 seconds
15 mm = approx. 3 mm/s for 5 seconds
```

`0 mm` disables this step.

#### Unload ramming scale

Range:

```text
0–150 %
```

Behavior:

```text
0 %     = ramming disabled
1–150 % = scaled ramming
100 %   = default behavior
```

---

### Reset load/unload

`Reset load/unload` resets only these five values:

- Initial insert length
- Fast load length
- Unload length
- Unload ramming scale
- Unload cooling retract

It does **not** change:

- Probe X/Y
- M851 Z / Live-Z
- M92 E
- M500 tuning values
- Axis direction
- Enable EEPROM save
- Print finish melody
- PID settings
- Stepper current
- Native Prusa settings

---

### M600 / filament change improvements

The print-time filament change path now respects the custom filament handling values.

Affected values:

- Initial insert length
- Fast load length
- Unload length
- Unload cooling retract
- Unload ramming scale

This improves tuning for LGX / LGX Shortcut and similar third-party extruder setups.

Notes:

- The MK3.5 UI filament-change path uses the stubbed M600 / Pause FSM path.
- Auto Retract was not changed.
- Marlin M603 / M503 compatibility was preserved.

---

### Optional print finish melody

Adds an optional print finish melody.

Default:

```text
Off
```

When enabled, the melody plays only after a successful print finish.

It does **not** play on:

- Cancel
- Abort
- Pause
- Filament change
- Crash
- Failed print

Melody sequence:

```gcode
M300 S1200 P150
G4 P200
M300 S1500 P150
G4 P200
M300 S1800 P300
G4 P500
```

---

### Safer M500 / M501 / M502 / M503 / M851 behavior

This release includes safeguards and fixes for:

- preserving axis direction signs
- keeping M603 compatibility
- synchronizing terminal `M851 X/Y` with the menu
- protecting `M851 Z` / Live-Z during `M502`
- keeping Probe X/Y reset separate from Live-Z
- keeping load/unload reset separate from other tuning values

#### M502 behavior

`M502` resets runtime X/Y probe position to factory defaults but preserves the current Z / Live-Z value.

Example before:

```gcode
M851 X23.00 Y12.00 Z-1.50
```

Expected after `M502`:

```gcode
M851 X23.00 Y5.00 Z-1.50
```

So:

- X/Y return to Prusa factory defaults
- Z / Live-Z remains unchanged

---

## Recommended LGX setup

For Bondtech LGX / LGX Shortcut:

```text
Probe X position: 23
Probe Y position: 12
```

Then verify with:

```gcode
M503
```

Expected example:

```gcode
M851 X23.00 Y12.00 Z<your Live-Z>
```

Do not copy another printer’s Z / Live-Z value.

### Extruder current

This release does not change extruder current.

A typical MK3.5 value seen in `M503` is:

```gcode
M906 T0 E490
```

This means 490 mA and is within the typical practical range for LGX-style setups.

---

## Defaults

| Setting | Default |
|---|---:|
| Probe X position | 23 |
| Probe Y position | 5 |
| Initial insert length | 45 mm |
| Fast load length | 45 mm |
| Unload length | 45 mm |
| Unload ramming scale | 100 % |
| Unload cooling retract | 0 mm |
| Print finish melody | Off |
| Enable EEPROM save | Off |

---

## Before flashing

Save your current settings:

```gcode
M503
```

Keep a copy of the full output.

Important values to note:

- M851 X/Y/Z
- M92 E
- M906 E
- M603 L/U
- Live-Z / First Layer Calibration value
- Existing custom load/unload values

---

## After flashing

Run:

```gcode
M503
```

Check:

- Probe/M851 X/Y/Z
- Live-Z / First Layer Calibration
- Extruder steps
- Extruder current
- Load/unload values

For LGX / LGX Shortcut:

```text
Probe X position: 23
Probe Y position: 12
```

Then run a small test print.

---

## Test status

The orange custom menu options included in this mod were successfully and thoroughly tested.

Tested before release:

- Firmware boots
- Menu structure works
- Load / unload works
- M600 filament change during print works
- Probe X/Y menu works
- M851 X/Y synchronization works
- M502 preserves M851 Z / Live-Z
- Print finish melody works as optional feature
- `mk3.5_release_boot` build completed successfully
- Final test print completed successfully

No errors were found during testing of the orange custom mod options.

---

## Not included in v1.0

The following features are intentionally not included in v1.0:

- Microstep switching
- E-step / microstep menu
- Extruder current menu
- Speed / acceleration hardware-limit overrides
- Fan overdrive
- Fan startup boost
- Heater / thermistor selection
- Unload chunking above 199 mm
- Auto Retract changes
- Special “Off” text for 0 % ramming scale

Reason: v1.0 is intended to stay focused and stable.

---

## Planned future ideas

Possible v1.1+ topics:

- Hotend heater selection
- Hotend thermistor selection
- Heatbed thermistor / sensor selection
- LGX preset profiles
- Third-party extruder profiles
- Extruder current tuning
- Microstep / E-step integration
- Advanced speed and acceleration limit controls
- Improved UI labels / help texts
- Optional unload chunking

Future custom options that are added before full real-world validation are planned to be shown in **red** until they have been thoroughly tested.

Heater and thermistor support is safety-critical and must be implemented carefully with correct temperature tables, limits, PID behavior and thermal runaway protection.

---

## Project notes and contributions

This project was only possible with the help of **ChatGPT Plus** and **Codex** as development assistance tools.

Additional developers, testers and contributors are welcome.

Feature requests, ideas and suggestions can be submitted through GitHub issues.

Please note that I currently only own and test with a **Bondtech LGX Shortcut** setup. Other extruders, hotends, heaters or thermistor configurations cannot be properly validated by me without access to the required hardware.

Support for additional hardware should therefore be developed and tested together with users or developers who actually own that hardware.

When reporting issues or requesting support, please include as much information as possible, for example:

- Printer model
- Extruder and hotend setup
- Firmware version used
- Relevant `M503` output
- Steps to reproduce the issue
- Photos or screenshots if helpful

I also publish related articles, guides and 3D printing projects on my blog:

[INVESTEGATE Blog](https://investegate.de/blog)

---

## Support the project

If this firmware helps you, you can support the project here:

[Donate via PayPal](https://www.paypal.com/donate?hosted_button_id=5F525DYM8GXK4)

---

## Upstream project

This custom firmware is based on the official open-source **Prusa-Firmware-Buddy** project by Prusa Research:

[Prusa-Firmware-Buddy](https://github.com/prusa3d/Prusa-Firmware-Buddy)

---

## Disclaimer

This is unofficial custom firmware.

It is not made, approved or supported by Prusa Research or Bondtech.

Use at your own risk.

Always verify your printer settings after flashing and run a small test print before normal use.
