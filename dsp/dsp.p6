sub sq-wave($samf, $dur, $freq) {
	my $n = $samf/(2*$freq);
	my $single-wave = flat -1.0 xx $n, 1.0 xx $n;
	my $reps = $freq * $dur;
	return ($single-wave xx $reps).flat;
}

sub save-wave(@wav) {
	my $fout = open "/tmp/dsp.dat", :w;
	my $x = 0;
	for @wav { $fout.print("$x $_\n") ; $x += 1/8000; }
	$fout.close;
}

sub integrate(@wav, $dt) {
	my @res;
	my $y = 0;
	my $t = 0;

	#@res.append($y);
	for @wav { $t += $dt; $y += $dt* $_ ; @res.append( $y); }
	return @res;
}

sub scale($x, $x-lo, $x-hi, $lo, $hi)
{
	return $lo + ($hi - $lo) * ($x - $x-lo) / ($x-hi - $x-lo);
}

sub scale-array(@arr, $lo, $hi)
{
	my $min = min @arr;
	my $max = max @arr;
	return @arr.map({scale $_, $min, $max, $lo, $hi});
}


sub scale-integrate(@arr, $dt)
{
	return scale-array (integrate @arr, $dt), -1.0, 1.0;
}

my $sample-freq = 8000;
my $dt = 1/$sample-freq;

my @wav1 = sq-wave($sample-freq, 10, 440);
my @wav2 = scale-integrate @wav1, $dt;
my @wav3 = scale-integrate @wav2, $dt;

# save waves as plottable data
my $fout = open '/tmp/dsp.dat', :w;
$fout.print("t sq tri sin\n");
for (0..100) -> $i {
	my $t = $i * $dt;
	$fout.print("$t @wav1[$i] @wav2[$i] @wav3[$i]\n");
}
$fout.close();



#@wav = integrate @wav, $dt;
my @wav = scale-array(@wav3, 0, 255).map({$_.Int});
#save-wave @wav.head(100);


my $blob = blob8.new(@wav); 
spurt "/tmp/dsp.raw", $blob;
run "aplay", "/tmp/dsp.raw";
