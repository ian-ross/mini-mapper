set term pdfcairo enhanced
set output 'intersection-solutions.pdf'

R = 10
r = 0.5
w = 0.5

to_rad(x) = x / 180 * pi
delta(x, sign) = 4 * R**2 * cos(x)**2 - sign * 4 * w * R * sin(x) - 4 * (R**2 - r**2) - w**2
soln(x, sign1, sign2) = R * cos(x) + sign2 * sqrt(delta(x, sign1))

limpos = asin((r - w/2) / R) / pi * 180
limneg = asin((r + w/2) / R) / pi * 180

set xrange [0:4.5]
set yrange [8.75:11.5]
set xlabel '{/Symbol f} (degrees)'
set ylabel 'y (mm)'
set border 3
set tics nomirror front
set samples 100000

plot [0:limneg] soln(to_rad(x), -1, 1) title 'y_{-} (pos)', [0:limneg] soln(to_rad(x), -1, -1) title 'y_{-} (neg)', [0:limpos] soln(to_rad(x), 1, 1) title 'y_{+} (pos)', [0:limpos] soln(to_rad(x), 1, -1) title 'y_{+} (neg)'
