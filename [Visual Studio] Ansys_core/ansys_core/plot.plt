set terminal pdf 
show terminal
set output "test.pdf"
#
set style data lines
set zeroaxis
set grid
set autoscale y
#
set title "Распределенная нагрузка"
unset key
set ylabel 'p, Н/мм'
set xlabel 'z, мм'
plot "output.txt" using 1:3 title "p(z)"
#
set title "Поперечная сила"
unset key
set ylabel 'Q_y, Н'
set xlabel 'z, м'
plot "output.txt" using 1:4 title "Q_y(z)"
#
set title "Изгибающий момент в сечении"
unset key
set ylabel 'M_x, Н*м'
set xlabel 'z, мм'
plot "output.txt" using 1:5 title "M_x(z)"
#
set title "Угол поворота нормали"
unset key
set ylabel '{/Symbol J}, {/Symbol \260}'
set xlabel 'z, мм'
plot "output.txt" using 1:6 title "{/Symbol J}(z)"
#
set title "Прогиб"
unset key
set ylabel 'v, мм'
set xlabel 'z, мм'
plot "output.txt" using 1:7 title "v(z)"
#
set title "Максимальные напряжения"
unset key
set ylabel '{/Symbol s}_{max}, МПа'
set xlabel 'z, мм'
plot "output.txt" using 1:8 title "{/Symbol s}_{max}(z)"