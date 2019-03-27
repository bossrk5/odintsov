set terminal pdf 
show terminal
set output "test.pdf"

set style data lines
set zeroaxis

set ylabel 'Давление p'
set xlabel 'z'
plot "output.txt" using 1:3 title "p(z)"

set ylabel 'Поперечная сила Q_y'
set xlabel 'z'
plot "output.txt" using 1:4 title "Q_y(z)"

set ylabel 'Момент M_x'
set xlabel 'z'
plot "output.txt" using 1:5 title "M_x(z)"

set ylabel 'Угол поворота {/Symbol J}'
set xlabel 'z'
plot "output.txt" using 1:6 title "{/Symbol J}(z)"

set ylabel 'Перемещение v'
set xlabel 'z'
plot "output.txt" using 1:7 title "v(z)"

set ylabel 'Напряжение {/Symbol s}_{max}'
set xlabel 'z'
plot "output.txt" using 1:8 title "{/Symbol s}_{max}(z)"

