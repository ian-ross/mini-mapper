# Layout review checklist

## General

* [X] [Schematic review](schematic-checklist.md) complete and signed
      off, including pin swaps done during layout
* [X] DRC set up to fab constraints before layout starts?
* [X] Layout DRC 100% clean
* [X] Double check the drill vs annular ring size for vias?
* [X] Passives have Traces coming into pads symmetrically? Traces
      comming into opposing “sides” of pads can cause solder
      migration, tombstoning, or stresses when the board cools.
* [X] Margin to edge of board checked? Appropriate given board
      break-away (V-score or mouse bites)? Mouse bites can easily
      pull/crack solder mask at 20 mil in.
* [X] Components can be reworked without removing others?
* [X] Thermal reliefs for all pads of serviceable components?

## Decoupling

* [X] Decoupling caps as close to power pins as possible
* [X] Low inductance mounting used for decoupling (prefer ViP if
      available, otherwise "[]8" shaped side vias

## DFM / yield enhancement

* [X] All design rules within manufacturer's capability
* [X] Minimize use of vias/traces that push fab limits

## Footprints

* [X] Confirm components are available in the selected package
* [X] Confirm components (especially connectors and power regulators)
      are capable of desired current in the selected package
* [X] Verify schematic symbol matches the selected package
* [X] Confirm pinout diagram is from top vs bottom of package
* [X] (recommended) PCB printed 1:1 on paper and checked against
      physical parts
* [X] 3D models obtained (if available) and checked against footprints
* [X] Soldermask apertures on all SMT lands and PTH pads

## Power

* [X] Traces have sufficient width for current and allowable
      temperature rise?
* [X] Sufficient trace spacing for voltage?
* [X] Highest ground currents closest to supply return?
* [X] Via size and count appropriate for current?
* [X] Thermal vias for thermal pads?
* [X] Tenting set appropriately for thermal vias?
* [X] Thermal reliefs disabled for power component pads?
* [X] Separate supply rail traces to multiple regulators?

## Connectors

* [X] Drivers near connectors they are driving signals on?

## Mechanical

* [X] Confirm all connectors to other systems comply with the
      appropriate mechanical standard (connector orientation, key
      position, etc)
* [X] LEDs, buttons, and other UI elements on outward-facing side of
      board
* [X] Clearance around pluggable connectors for mating cable/connector
* [X] Clearance around mounting holes for screws
* [X] Confirm PCB dimensions and mounting hole size/placement against
      enclosure or card rack design
* [X] Verify mounting hole connection/isolation
* [X] Components not physically overlapping/colliding
* [X] Clearance provided around solder-in test points for probe tips
* [X] Drill holes checked? Correct hole spacing to mounting hardware?
* [X] Drill hole plating checked?
* [X] Adequate clearance around mounting holes for both the mounting
      hardware and the tool that fastens it?

## Thermal

* [X] Thermal reliefs used for plane connections (unless via is used
      for heatsinking)
* [X] Solid connections used to planes if heatsinking
* [X] Ensure thermal balance on SMT chip components to minimize risk
      of tombstoning

## Solder mask

* [X] Confirm SMD vs NSMD pad geometry
* [X] Adequate clearance around pads (typ. 50 um)
* [X] Soldermask relief set appropriately given expected soldermask
      expansion?
* [X] Soldermask minimum spoke size met?
* [X] Soldermask relief around fiducials?
* [X] Soldermask removed for all pads?

## Silkscreen

* [X] Text size within fab limits
* [X] Text not overlapping drills or component pads
* [X] Text removed entirely in, or moved outside of, high component/via density areas
* [X] Traceability markings (rev, date, name, etc) provided
* [X] Text mirrored properly on bottom layer
* [X] Test points labeled if space permits
* [X] Check fab silkscreen DPI? Can legend be printed clearly?
* [X] Pin-1 indicator visible after component is placed on board?
* [X] Power pins labeled with polarity and voltage ranges?
* [X] Connector pinouts printed on both sides of board and mirrored?
      (if two-sided silkscreen)
* [X] Part number, revision number, date code present?
* [X] Legend references all facing one or two directions?

## CAM production

* [X] KiCAD specific: rerun DRC and zone fills before exporting CAM
      files to ensure proper results
* [X] Export gerber/drill files at the same time to ensure consistency
* [X] Visually verify final CAM files to ensure no obvious
      misalignments

## Programming and Test

* [X] Test points on all important clock and data pins?
* [X] More test points?
* [X] Sure, but maybe more test points?
* [X] Ground reference test points near all high speed signals that
      might need to be probed?

## Sensing

* [X] Analog sensors close to their ADC or buffered soon?
* [X] ADCs on or toward digital part of layout? (ADCs use fast
      oscillators)
* [X] Feedback dividers accidentally bypassed with capacitor
      placement?
