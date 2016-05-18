#! /bin/bash

#unset key;
set key inside right bottom;
set title "200 coppie";
set xlabel "Generations";
set ylabel "Fitness values";
set term png;
set output "out.png";
plot "200_coppie_120_vecchi" using 1:3 title "120" with lines,\
 "200_coppie_130_vecchi" using 1:3 title "130" with lines,\
 "200_coppie_140_vecchi" using 1:3 title "140" with lines,\
 "200_coppie_150_vecchi" using 1:3 title "150" with lines,\
 "200_coppie_160_vecchi" using 1:3 title "160" with lines
