set title "perf"
set xlabel "iterations"
set ylabel "time(ns)"
set terminal png font " Times_New_Roman,12 "
set key left
set output "result.png"

plot \
"data.txt" using 1:2 with linespoints linewidth 2 title "cirbuf (orig)", \
"data.txt" using 1:3 with linespoints linewidth 2 title "cirbuf (mmap)" \