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


my @wav = sq-wave(8000, 10, 440);
my $min = min @wav;
my $max = max @wav;
say "Min is $min, max is $max";

@wav = integrate @wav, 1/8000;
@wav = integrate @wav, 1/8000;
save-wave @wav;


@wav = @wav.map({ $_ == -1.0 ?? 0 !! 255 } );
my $blob = blob8.new(@wav); 
spurt "/tmp/dsp.raw", $blob;
run "aplay", "/tmp/dsp.raw";
#say $blob;
