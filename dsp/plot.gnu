set multiplot layout 3,1
plot '/tmp/dsp.dat' using 1:2 pt 7
plot '/tmp/dsp.dat' using 3 with lines
plot '/tmp/dsp.dat' using 4 with lines
unset multiplot
