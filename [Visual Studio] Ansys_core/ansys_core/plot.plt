set terminal pdf 
show terminal
set output "plots.pdf"
#
set style data lines
set zeroaxis
set grid
set autoscale y
#
#

# Line style for axes
# Define a line style (we're calling it 80) and set 
# lt = linetype to 0 (dashed line)
# lc = linecolor to a gray defined by that number
set style line 80 lt 0 lc rgb "#808080"

# Set the border using the linestyle 80 that we defined
# 3 = 1 + 2 (1 = plot the bottom line and 2 = plot the left line)
# back means the border should be behind anything else drawn
set border 3 back ls 80 

# Line style for grid
# Define a new linestyle (81)
# linetype = 0 (dashed line)
# linecolor = gray
# lw = lineweight, make it half as wide as the axes lines
set style line 81 lt 0 lc rgb "#808080" lw 0.5

# Draw the grid lines for both the major and minor tics
set grid xtics
set grid ytics
set grid mxtics
set grid mytics

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