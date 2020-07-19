#!/usr/bin/env python

# Generate photointerrupter signals for motor prototype board.

# ALL DISTANCES IN mm, ALL ANGLES IN RADIANS


import math
import random
import sys


# PHYSICAL DIMENSIONS

# Wheel diameter.
wheel_dia = 60.0

# Gearbox ratio.
gearbox_ratio = 48.0

# Diameter of circle of centres of holes in encoder disk (called D in
# notes).
encoder_centre_dia = 20.0
R = encoder_centre_dia / 2.0

# Diameter of holes in encoder disk (called d in notes).
encoder_hole_dia = 1.0
r = encoder_hole_dia / 2.0

# Optical aperture width (called w in notes).
aperture_width = 0.5
w = aperture_width

# Optical aperture height (called h in notes).
aperture_height = 2.0
h = aperture_height

# Number of holes in encoder disk.
n_encoder_holes = 32

# Maximum light current from phototransistor in photointerrupter.
light_current = 2.0E-3

# Realistic (?) dark current from phototransistor in photointerrupter.
# (Note this this is significantly larger than the dark current quoted
# in the photointerrupter datasheet [100 nA], because it includes a
# guessed value for the extra phototransistor current due to leakage
# of ambient light into the device.)
dark_current = 100.0E-6

# Current noise amplitude on output.
noise_amplitude = 5.0E-6
# noise_amplitude = 0


# DERIVED PARAMETERS

# Wheel circumference.
wheel_circ = math.pi * wheel_dia

# Angular separation between holes in encoder disk.
delta_phi = 2 * math.pi / n_encoder_holes

# Optical aperture area.
aperture_area = aperture_width * aperture_height


# ENCODER DISK HOLE/APERTURE OVERLAP CALCULATIONS

# Angular displacement limits for "four intersection" and "two
# intersection" cases.
limpos = math.asin((r - w/2) / R)
limneg = math.asin((r + w/2) / R)

# Degrees to radians.
def to_rad(phi):
    return phi / 180.0 * math.pi

# Intersection discriminant.
def delta(phi, sign):
    return (4.0 * R**2 * math.cos(phi)**2 -
            sign * 4.0 * w * R * math.sin(phi) -
            4.0 * (R**2 - r**2) - w**2)

# Overlap factors.
def thetamfac(phi, sign):
    return (w/2 - R * math.sin(phi))**2 + sign * delta(phi, -1) / 4
def thetam(phi):
    return math.acos(thetamfac(phi, -1) / thetamfac(phi, 1))
def thetapfac(phi, sign):
    return (w/2 + R * math.sin(phi))**2 + sign * delta(phi, 1) / 4
def thetap(phi):
    return math.acos(thetapfac(phi, -1) / thetapfac(phi, 1))

# Area of segment of an encoder disk hole.
def segment(theta):
    return r**2 / 2.0 * (theta - math.sin(theta))

# Overlap area expressions.
def aream(phi):
    return segment(thetam(phi))
def areap(phi):
    return math.pi * r**2 - segment(thetam(phi)) - segment(thetap(phi))

# Final area expression.
def area(phi):
    if phi <= -limneg:
        return 0.0
    if phi <= -limpos:
        return aream(-phi)
    if phi <= 0:
        return areap(-phi)
    if phi <= limpos:
        return areap(phi)
    if phi <= limneg:
        return aream(phi)
    return 0.0



# VELOCITY FUNCTIONS

# Constant velocity.
def constant_velocity(v):
    return (lambda t: v)

# Easing function for starting and stopping.
def ease(x):
    if x < 0.5:
        return 2 * x * x
    else:
        return 1 - 2 * (1 - x) * (1 - x)

# Simulated velocity history: start from rest, eased acceleration to
# constant speed, constant velocity segment, then eased deceleration
# to rest.
def v_history(t):
    ease_time = 3.0
    const_time = 2.0
    max_v = 50.0
    if t < 0.0:
        return 0.0
    if t < ease_time:
        return max_v * ease(t / ease_time)
    if t < ease_time + const_time:
        return max_v
    if t < 2 * ease_time + const_time:
        return max_v * ease((2 * ease_time + const_time - t) / ease_time)
    return 0.0

#vfn = constant_velocity(5.0)
vfn = v_history



# INTEGRATION TIME PARAMETERS

tstart = 0.0
tend = 8.0
dt = 0.0001


def calc_phi(angle):
    return math.fmod(angle + delta_phi / 2, delta_phi) - delta_phi / 2


wheel_angle = 0.0
distance = 0.0
encoder_disk_angle = 0.0

t = tstart
with open('eased.dat', 'w') as datfp:
    with open('simulation.pwl', 'w') as pwlfp:
        while t <= tend:
            # Current linear velocity.
            v = vfn(t)

            # Current angular velocity of wheel.
            omega_wheel = v / wheel_circ

            # Euler integration.
            distance += v * dt
            wheel_angle += omega_wheel * dt
            encoder_disk_angle = wheel_angle * gearbox_ratio

            # Angular displacement for aperture overlap calculations.
            phi = calc_phi(encoder_disk_angle)

            # Fraction of aperture overlapped by encoder disk hole.
            overlap_fraction = area(phi) / aperture_area

            # Phototransistor current assumed proportional to area
            # overlap fraction.
            current = dark_current
            current += (light_current - dark_current) * overlap_fraction
            current += random.gauss(0, noise_amplitude)

            print(t, v, distance, encoder_disk_angle / math.pi * 180.0,
                  phi / math.pi * 180.0, overlap_fraction, current, file=datfp)
            print('{},{}'.format(t, current), file=pwlfp)

            t += dt
