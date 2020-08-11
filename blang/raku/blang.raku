
my @bcodes;

sub push(int32 $x) { @bcodes.push($x); }

my int32 $psh = 1 +< 24;
my int32 $pri = 2 +< 24;

my %opcodes;
%opcodes<$psh> = "PUSH";
%opcodes<$pri> = "PRI";

#  = %( $pri => "PRI", $psh => "PUSH" );

sub push-int(int32 $x ) {
	push ($psh + $x); # TODO what about large numbers?
}


grammar G {
	rule TOP { ^ <stmts> $ }
	rule stmts { <statement>* }
	rule statement { <prin> }
	rule prin { 'print' <int> { push-int $<int>.Int ; push $pri ; } }

	token int	{ <[0..9]>+ }
}

my $input = slurp;

my $m = G.parse($input);


say @bcodes;

for @bcodes -> $bcode {
	say "bcode=$bcode";
	my $opcode = $bcode +& (255 +<24);
	say "opcode=$opcode";
	my $opname = %opcodes<$opcode>;
	if $opcode == $pri { say "found print";}
	my $val = $bcode - $opcode;
	say "$opname $val";
}
