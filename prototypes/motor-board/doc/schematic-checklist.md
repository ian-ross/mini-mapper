# Schematic review checklist

Is the design passively or actively safe? For example, if an MCU or
FPGA is being programmed, held in reset, or in their POR, does the
circuit enter a safe / idle / known state? Is the circuit only “hot”
under control (can it be)?

Can all relevant environmental changes be detected? If changes in
environment indicate a change in operating state or mode, can they be
detected? Examples: VBUS high on a self-powered USB device, SD card in
socket, a particular connector plugged in, water ingress, etc.

BOM has full orderable part numbers including all options and sufixes.
BOM is complete including PCB, enclosure, screws, bags, accessories,
memory cards, manuals, customer boxes, labels, master shipping carton.

## General

* [X] CAD ERC 100% clean. If some errors are invalid due to toolchain
      quirks, each exception must be inspected and signed off as invalid.
* [X] Verify pin numbers of all schematic symbols against datasheet or
      external interface specification document (if not yet board proven).
* [X] Schematic symbol matches chosen component package
* [X] Thermal pads are connected to correct power rail (may not always
      be ground)
* [?] Pull-up / -down resistors appropriate value for expected
      environment/noise?
* [X] Enable pins checked for polarity. Not all datasheets indicate
      this well, pin description text checked? Part number verified
      for ICs with orderable polarity configurations?
* [X] Enable and shutdown pins pulled to the correct level when not
      under control? (e.g. shut down unless controlled)
* [X] Pull-ups on open-drain, open-collector nets? Values for required
      slew rates / throughput / noise?
* [X] Passive / buffered visual indicators (e.g. LEDs) at appropriate
      level for the idle level of the protocol? (e.g. UART idle high,
      SPI clock idle low, etc)
* [X] Exposed pads on no-leads packages tied to correct net? Needs to
      be left floating?

## Passive components

* [X] Power/voltage/tolerance ratings specified as required
* [X] Ceramic capacitors appropriately de-rated for C/V curve
* [X] Polarized components specified in schematic if using electrolytic caps etc

## Power supply

### System power input

* [X] Check total input capacitance and add inrush limiter if needed

### Regulators

* [X] Verify estimated power usage per rail against regulator rating
* [X] Linear regulators and voltage reference ICs are stable with selected output cap ESR

### Decoupling

* [X] Decoupling present for all ICs
* [X] Decoupling meets/exceeds vendor recommendations if specified
* [X] Bulk decoupling present at PSU

### General

* [X] All power inputs fed by correct voltage
* [X] Check high-power discrete semiconductors and passives to confirm
      they can handle expected load
* [X] Analog rails filtered/isolated from digital circuitry as needed

## Signals

### Digital

* [X] Signals are correct logic level for input pin
* [X] Pullups on all open-drain outputs
* [X] Active high/low enable signal polarity correct

### Analog

* [X] Verify polarity of op-amp feedback

### Debugging / reworkability

* [X] Use 0-ohm resistors vs direct hard-wiring for strap pins when
      possible
* [X] Provide multiple ground clips/points for scope probes
* [X] Dedicated ground in close proximity to analog test points
* [X] Test points on all power rails
* [X] Test points on interesting signals which may need probing for
      bringup/debug

## Thermal

* [X] Power estimates for all large / high power ICs
* [X] Thermal calculations for all large / high power ICs
* [X] Specify heatsinks as needed

## Power

* [X] Power pins tied to correct voltage rail?
* [X] Dropout voltage of regulators checked against complete
      range/tolerance of source supply? Not just nominal/ideal rating.
* [X] Minimum load met for stability of all regulators?
* [X] Final circuit maximum load rechecked against regulator ratings?
* [X] Digital ICs have correct number and value of bypass capacitors
      for frequencies in use?
* [X] Bypass and output capacitor values checked for all regulators?
      Correctly derated? Appropriate  ESR for stability?
* [X] Analog rails properly decoupled with ferrites / inductors /
      capacitors?

## Sensors / Signal Conditioning

* [X] Dividers and filters buffered with op-amp before ADC?
* [X] Op-amp stability checked given load capacitance on output?
* [X] Unused op-amps in dual or quad packages terminated?

## Connectors

* [X] Power and its ground on same connector?
* [X] Related inputs/outputs on same connector?
