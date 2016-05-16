#! /bin/bash

#unset key;
set key inside right bottom;
set title "200 coppie 50 vecchi";
set xlabel "Generations";
set ylabel "Fitness values";
set term png;
set terminal pngcairo size 1024,768 enhanced font 'Verdana,10'
set output "out.png";
plot \
 "200_coppie_50_vecchi_005_mutazione" using 1:3 title "0.0005" with lines,\
 "200_coppie_50_vecchi_01_mutazione" using 1:3 title "0.001" with lines,\
 "200_coppie_50_vecchi_02_mutazione" using 1:3 title "0.002" with lines,\
 "200_coppie_50_vecchi_03_mutazione" using 1:3 title "0.003" with lines,\
 "200_coppie_50_vecchie_04_mutazione" using 1:3 title "0.004" with lines,\
 "200_coppie_50_vecchi" using 1:3 title "0.005" with lines,\
 "200_coppie_50_vecchie_06_mutazione" using 1:3 title "0.006" with lines,\
 "200_coppie_50_vecchie_07_mutazione" using 1:3 title "0.007" with lines,\
 "200_coppie_50_vecchie_08_mutazione" using 1:3 title "0.008" with lines
