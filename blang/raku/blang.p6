#!/usr/bin/env perl6

enum Bcode <Call Drop Dup Halt Inc Jlt Push Sub>;
my @bcodes;
my @bvals;


my @sstack; # regular stack
sub spush(int32 $val) { @sstack.push($val); }
sub spop() { return pop(@sstack); }

sub bpush(Bcode $code, int32 $val) { @bcodes.push($code) ; @bvals.push($val); }
sub bpush0(Bcode $code) { bpush $code, 0; }
sub slast() { return elems(@sstack) - 1 ; }


#sub do-drop() { spop; }
#sub do-dup() { spush( @sstack[slast] ; }
sub do-emit() { print chr(spop()); }
#sub do-inc() { @sstack[slast] += 1; }
#sub do-jlt() { if spop() < 0 { $ip += @bvals[$ip-1]; } }
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

my %labels;
#my @label-pos;
my @jump-ids;
my @jump-pos;


sub add-jump($id, $pos) { @jump-ids.push($id); @jump-pos.push($pos); }

sub found($str) { say "Found: $str"; }
sub xfound($str) {  }

grammar G {
	rule TOP { ^ <stmts> $ }
	rule stmts { <statement>* }
	rule statement { <call> | <dup> | <drop> | <halt> | <inc> | <jlt> | <label> | <push> | <sub> | <comment> }
	rule drop { 'drop' { found "drop"; bpush0 Drop; }}
	rule dup { 'dup' { found "dup"; bpush0 Dup; }}
	rule inc { 'inc' { bpush0 Inc; }}
	rule jlt { 'jlt' <id> { found "jlt"; add-jump $<id>, elems(@bcodes); bpush0 Jlt; }}
	rule label { <id> ':' {found "label"; %labels{$<id>} = elems(@bcodes); } }
	rule sub { 'sub' { bpush0 Sub;}}
	rule push { 'push' <int>  { bpush Push, $<int>.Int; }}
	rule call { 'call' <id> {bpush Call, find-prim $<id>; } }
	rule halt { 'halt'  {xfound "halt"; bpush0 Halt;} }

	#rule prin { 'print' <int> { push-int $<int>.Int ; push $pri ; } }
	token comment	{ '#' \N*  }
	token id { <[a..zA..Z]>+ }
	token int	{ <[0..9]>+ }
	}

my $input = slurp;

my $m = G.parse($input);


say @bcodes;
say @bvals;

sub resolve-labels() {
	loop (my $i = 0; $i < elems(@jump-ids); $i++) {
		my $id = @jump-ids[$i];
		say "seolve-labels: id $id";
		my $pos = %labels{$id};
		my $here = @jump-pos[$i];
		say "resolve-labels: pos $pos here $here";
		@bvals[@jump-pos[$i]] = $pos -$here;
	}
}


sub disasm() {
	loop (my $i=0; $i < elems(@bcodes); $i++) {
		print "$i\t";
		my $bcode = @bcodes[$i];
		my $val = @bvals[$i];
		say "$bcode\t$val";
		#given $bcode 
			#	when Call 
		}
}
		


my $ip = 0;
sub run() {
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
			when Drop { spop;}
			when Dup { spush( @sstack[slast]); }
			when Halt { last; }
			when Inc { @sstack[slast] += 1; }
			when Jlt { if spop() < 0 { $ip += @bvals[$ip-1]-1;} }
			when Push { spush $val;}
			when Sub { my $v = spop; @sstack[slast] -= $v; }
			default { say "Unknown opcode: $bcode before $ip"; }
		}
	}
	say "Bye";
}

say %labels;
resolve-labels;
disasm;
run;

