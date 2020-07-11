set term pdfcairo enhanced
set output 'overlap-areas.pdf'

R = 10
r = 0.5
w = 0.5
h = 2.0

limpos = asin((r - w/2) / R)
limneg = asin((r + w/2) / R)

to_rad(x) = x / 180 * pi
delta(x, sign) = 4 * R**2 * cos(x)**2 - sign * 4 * w * R * sin(x) - 4 * (R**2 - r**2) - w**2

thetamfac(x, sign) = (w/2 - R * sin(x))**2 + sign * delta(x, -1) / 4
thetam(x) = acos(thetamfac(x, -1) / thetamfac(x, 1))

thetapfac(x, sign) = (w/2 + R * sin(x))**2 + sign * delta(x, 1) / 4
thetap(x) = acos(thetapfac(x, -1) / thetapfac(x, 1))

segment(theta) = r**2 / 2 * (theta - sin(theta))

aream(x) = segment(thetam(x))
areap(x) = pi * r**2 - segment(thetam(x)) - segment(thetap(x))

area(x) = x <= -limneg ? 0 : (x <= -limpos ? aream(-x) : (x <= 0 ? areap(-x) : (x <= limpos ? areap(x) : (x <= limneg ? aream(x) : 0))))

set xrange [-10:10]
set xlabel '{/Symbol f} (degrees)'
set ylabel 'Optical beam fraction'
set border 3
set tics nomirror front
set samples 10000

plot [-5:5] area(to_rad(x)) / (w * h) notitle
