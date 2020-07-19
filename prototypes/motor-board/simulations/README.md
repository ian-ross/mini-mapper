# Motor encoder circuit simulations

These simulations are intended to explore the threshold and hysteresis
setup for the comparator used with the photointerrupter for the motor
encoder disk.

The comparator used in the simulations is the LT1017, though that's
not what's used in the real circuit.

## Simulations

There are four different simulations here:

 * `phototransistor-saturation.asc` just demonstrates how
   phototransistors go into saturation when drive sufficiently hard,
   using a photoisolator out of the LTSpice libraries for
   experimentation.

 * `no-noise/no-noise.asc` uses a simulated current signal without any
   superimposed noise to look at how the comparator works in a simple
   case.

 * `with-noise/jitter.asc` uses a simulated current signal with some
   superimposed noise, showing the bad effects of noise leading to
   spurious comparator output transitions when the phototransistor
   current crosses the comparator threshold slowly. This is no good
   for counting pulses, so we need to add hysteresis.

 * `with-noise/hysteresis.asc` uses the same simulated current signal
   withh superimposed noise, but includes a small amount of hysteresis
   in the comparator circuit, which eliminates the spurious
   transitions in the comparator output and gives a clean pulse train
   suitable for counting the motor encoder disk holes passing the
   optical aperture of the photointerrupter.

## Phototransistor current simulations

The `scripts/gen-signal.py` script generates simulated current signals
from the phototransistor in the photointerrupter as the motor encoder
disk turns. See the `../doc/photoencoder-signal` directory for the
details of how these simulations work.
