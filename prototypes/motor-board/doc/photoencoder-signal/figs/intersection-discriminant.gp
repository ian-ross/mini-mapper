set term pdfcairo enhanced
set output 'intersection-discriminant.pdf'

R = 10
r = 0.5
w = 0.5

to_rad(x) = x / 180 * pi
delta(x, sign) = 4 * R**2 * cos(x)**2 - sign * 4 * w * R * sin(x) - 4 * (R**2 - r**2) - w**2

set xrange [0:4.5]
set yrange [0:1.05]
set xlabel '{/Symbol f} (degrees)'
set ylabel '{/Symbol D}'
set border 3
set tics nomirror front

plot delta(to_rad(x), -1) with filledcurve x1 title 'Two intersections', delta(to_rad(x), 1) with filledcurve x1 title 'Four intersections'
