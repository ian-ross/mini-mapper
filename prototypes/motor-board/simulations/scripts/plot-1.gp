set term pdfcairo enhanced size 25cm,5cm
set output 'simulation-1.pdf'

set style data lines
set xrange [0:2]
set yrange [0:1.1]
set xlabel 'Time (s)'
set ylabel 'Current (mA)'
set border 3
set tics nomirror front

plot 'eased.dat' using 1:($7*1000) notitle
