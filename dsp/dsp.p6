sub sq-wave($samf, $dur, $freq) {
	my $n = $samf/(2*$freq);
	my $single-wave = flat -1.0 xx $n, 1.0 xx $n;
	my $reps = $freq * $dur;
	return ($single-wave xx $reps).flat;
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

sub save(@wav, $name, $dt) {
	my $name1 = "/tmp/$name";

	# save raw audio for playing
	my @raw = scale-array @wav, 0 , 255;
	@raw = @raw.map({$_.Int});
	my $blob = blob8.new(@raw); 
	spurt ($name1 ~ ".raw"), $blob;

	# save the first 100 samples for plotting
	my $fout = open ($name1 ~ ".dat"), :w;	
	for (0..100) -> $i {
		my $t = $i * $dt;
		$fout.print("$t @wav[$i]\n");
	}
	$fout.close();
}

my $sample-freq = 8000;
my $dt = 1/$sample-freq;

my @wav1 = sq-wave($sample-freq, 10, 440);
save @wav1, "sqr", $dt;
my @wav2 = scale-integrate @wav1, $dt;
save @wav2, "tri", $dt;
my @wav3 = scale-integrate @wav2, $dt;
save @wav3, "sin", $dt;
