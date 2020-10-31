#!/usr/bin/env perl6

enum Bcode <Call Halt Push>;
my @bcodes;
my @bvals;


my @sstack; # regular stack
sub spush(int32 $val) { @sstack.push($val); }
sub spop() { return pop(@sstack); }

sub bpush(Bcode $code, int32 $val) { @bcodes.push($code) ; @bvals.push($val); }

sub do-emit() { print chr(spop()); }
sub do-print() { print spop; }
sub do-hello() { say "Hello from raku blang"; }

my @prims = (
	["emit", &do-emit],
	["print", &do-print], 
	["hello", &do-hello]);

sub find-prim($name) {
	loop (my $i=0; $i <elems(@prims); $i++)  {
		my ($key, $f) = @prims[$i];
		#say "key is $key";
		if $name eq $key { return $i; }
	}
}



sub found($str) { say "Found: $str"; }
sub xfound($str) {  }

grammar G {
	rule TOP { ^ <stmts> $ }
	rule stmts { <statement>* }
	rule statement { <call> | <halt> | <push> | <comment> }
	rule push { 'push' <int>  { bpush Push, $<int>.Int; }}
	rule call { 'call' <id> {bpush Call, find-prim $<id>; } }
	rule halt { 'halt'  {xfound "halt"; bpush Halt, 0;} }

	#rule prin { 'print' <int> { push-int $<int>.Int ; push $pri ; } }
	token comment	{ '#' \N*  }
	token id { <[a..zA..Z]>+ }
	token int	{ <[0..9]>+ }
}

my $input = slurp;

my $m = G.parse($input);


say @bcodes;
say @bvals;

my $ip = 0;
loop {
	my $bcode = @bcodes[$ip];
	my $val = @bvals[$ip];
	$ip++;
	given $bcode {
		when Call { 
			#say @prims;
			my $func = @prims[$val][1];
			#say "func is $func";
			$func();
		}
		when Halt { last; }
		when Push { spush $val;}
		default { say "Unknown opcode"; }
	}
}
say "Bye";
