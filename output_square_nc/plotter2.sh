#! /bin/bash

#unset key;
set key inside right bottom;
set title "200 coppie";
set xlabel "Generations";
set ylabel "Fitness values";
set term png;
set output "out.png";
plot "200_coppie_70_vecchi" using 1:3 title "70" with lines,\
 "200_coppie_80_vecchi" using 1:3 title "80" with lines,\
 "200_coppie_90_vecchi" using 1:3 title "90" with lines,\
 "200_coppie_100_vecchi" using 1:3 title "100" with lines,\
 "200_coppie_110_vecchi" using 1:3 title "110" with lines,\
 "200_coppie_120_vecchi" using 1:3 title "120" with lines
