set multiplot layout 3,1
plot '/tmp/sqr.dat' using 1:2 pt 6
plot '/tmp/tri.dat' using 1:2 with lines
plot '/tmp/sin.dat' using 1:2 with lines
unset multiplot
