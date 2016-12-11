#!/usr/bin/env perl
system("{ (daycls | tail -n +261 ); hi52w ; } | sort > /tmp/momo");
#$pid = open(INP, "cat <(daycls) <(hi52w) | sort |")  or die "Couldn't fork: $!\n";

$holding = 0;

my $price_in, $price_out;
my $scale = 1;
my $buystr; $sellstr;
my $price;

sub doit { 
	my $hstate = "idle";

	#$holding = 0;
	$scale = 1;
	my $price0 = -1, $price1;
	#print "*** Buy criteria: ", $buystr, ", Sell criteria: ", $sellstr, "\n";
	open(INP, "/tmp/momo");
	foreach $line ( <INP> ) {
		#print $line;
		my @fields = split /\s+/, $line;
		$price = @fields[1];

		if($price0 == -1) { $price0 = $price};
		$price1 = $price;

		if( $hstate eq "idle" && $line =~ m/GDN_T/) {

			$hstate = "primed";
			print "GAP  ", $line;
		}

		if( $hstate eq "primed" && ($line =~ m/HI-T/) ) {
			$hstate = "holding";
			print "BUY $line";
			$price_in = $price;
		}

		#print "$state\n";

		if(($hstate eq "holding") && ($line =~ m/LO-T/)) {
			#print "DBUG $hstate $line";
		 	$hstate = "idle";
			$doing = "SELL";
			#if(eof(INP)) { $doing = "HOLD"; }
			print "$doing $line";
			$price_out = $price;
			$rel = $price_out / $price_in;
			print "GAIN $rel\n\n";
			#$scale *= $rel;

		}

	}
	close(INP);
	if($hstate eq "holding") {
		print "HOLD ", $price, "\n";
		$rel = $price / $price_in;
			print "GAIN $rel\n\n";
	}

	#print "Scale factor is: ", $scale, "\n";
	#$rel = $price1/$price0;
	#print "Non-momo price gain is: $price0, $price1, $rel\n";
	#if($scale > $rel) { print "WIN $buystr + $sellstr \n" } ;
	#print "RELSCALE $buystr + $sellstr ", $scale / $rel , "\n";
	
	print "\n\n";
}

doit();

if(0) {
$buystr =  "HI-T"; $sellstr = "LO-T";
doit();

$buystr =  "HI-T"; $sellstr = "LO-T|GDN_T";
doit();

$buystr =  "HI-T|GUP_T"; $sellstr = "LO-T";
doit();


$buystr =  "HI-T|GUP_T"; $sellstr = "LO-T|GDN_T";
doit();

$buystr =  "GUP_T"; $sellstr = "GDN_T";
doit();

$buystr =  "HI-T"; $sellstr = "GDN_T";
doit();

$buystr =  "LO-T"; $sellstr = "HI-T";
doit();

$buystr =  "GUP_T"; $sellstr = "LO-T";
doit();
}
