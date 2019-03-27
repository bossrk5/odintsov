set terminal pdf enchanced
set output "test.pdf"

set style data lines
set zeroaxis

plot "output.txt" using 1:3 title "p(z)"
plot "output.txt" using 1:4 title "Q_y(z)"
plot "output.txt" using 1:5 title "M_x(z)"
plot "output.txt" using 1:6 title "{/Symbol J}(z)"
plot "output.txt" using 1:7 title "v(z)"
plot "output.txt" using 1:8 title "{/Symbol s}_{max}(z)"
