#!/usr/bin/gnuplot
set terminal png nocrop enhanced size 800,600 font "arial,8"
set output "OUTPUT_".id.".png"


set key inside left top vertical Right noreverse enhanced autotitle box lt "black"
#set datafile separator {"q" | whitespace}
set title "Fitness robby" font ",20"
set key left box
set samples 50
plot "data/run".id.".dat" using 2:6 with lines #, \
     #"data/run0.dat" u 1:2 t 'Beam' w linespoints
