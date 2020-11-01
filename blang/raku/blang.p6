#!/usr/bin/env perl6

enum Bcode <Add Call Drop Dup Halt Inc Jlt Push Sub>;
my @bcodes;
my @bvals;


my @sstack; # regular stack
sub spush(int32 $val) { @sstack.push($val); }
sub spop() { return pop(@sstack); }

my Str @kstrs; #constant strings

sub bpush(Bcode $code, int32 $val) { @bcodes.push($code) ; @bvals.push($val); }
sub bpush0(Bcode $code) { bpush $code, 0; }
sub slast() { return elems(@sstack) - 1 ; }


#sub do-drop() { spop; }
#sub do-dup() { spush( @sstack[slast] ; }
sub do-emit() { print chr(spop()); }
#sub do-inc() { @sstack[slast] += 1; }
#sub do-jlt() { if spop() < 0 { $ip += @bvals[$ip-1]; } }
sub do-print() { say spop; }
sub do-printkstr() { say @kstrs[spop()]; }
sub do-hello() { say "Hello from raku blang"; }

my @prims = (
	["emit", &do-emit],
	["print", &do-print], 
	["printkstr", &do-printkstr],
	["hello", &do-hello]);

sub find-prim($name) {
	loop (my $i=0; $i <elems(@prims); $i++)  {
		my ($key, $f) = @prims[$i];
		#say "key is $key";
		if $name eq $key { return $i; }
	}
	return -1;
}

my %labels;
#my @label-pos;
my @jump-ids;
my @jump-pos;


sub add-jump($id, $pos) { @jump-ids.push($id); @jump-pos.push($pos); }

sub found($str) { say "Found: $str"; }
sub xfound($str) {  }

sub add-sub($op) {
	if $op eq '+' {
		bpush0 Add;
	} else {
		bpush0 Sub;
	}
}

sub calls($func-name) {
	my $id = find-prim $func-name;
	die("calls:fatal:unknown function:$func-name") if $id == -1;
	bpush Call, $id; 
}

sub mk-kstr($kstr) {
	my $n = elems(@kstrs);
	@kstrs.push($kstr);
	bpush Push, $n;
}

grammar G {
	rule TOP { ^ <stmts> $ }
	rule stmts { <statement>* }
	rule statement { <call> | <dup> | <drop> | <halt> | <inc> | <jlt> | <label> | 
		<prin> | <push> | <sub> | <comment> }
	rule drop { 'drop' { found "drop"; bpush0 Drop; }}
	rule dup { 'dup' { found "dup"; bpush0 Dup; }}
	rule inc { 'inc' { bpush0 Inc; }}
	rule jlt { 'jlt' <id> { found "jlt"; add-jump $<id>, elems(@bcodes); bpush0 Jlt; }}
	rule label { <id> ':' {found "label"; %labels{$<id>} = elems(@bcodes); } }
	rule add	{ 'add' {bpush0 Add;}}
	rule sub { 'sub' { bpush0 Sub;}}
	#rule push { 'push' <int>  { bpush Push, $<int>.Int; }}
	rule push { 'push' <expr> }
	rule call { 'call' <id> {calls $<id>; } }
	rule halt { 'halt'  {xfound "halt"; bpush0 Halt;} }
	#rule expr	{ <expr-p>+ % <plus> }
	rule expr	{ <expr-p> ( <add-sub> <expr-p> { add-sub $<add-sub>;} )* }
	token add-sub	{ '+' | '-' }
	#token plus 	{ '+' {bpush0 Add;} }
	rule expr-p	{ <int> { bpush Push, $<int>.Int; }}

	rule prin { 'print' ((<expr> { calls "print"; }) | (<kstr> {mk-kstr $<kstr>.Str; calls "printkstr";})) }
	token comment	{ '#' \N*  }
	token kstr	{ '"' <( <str=-["]>* )> '"'  {say "found kstr $<str>"; } }
	#token kstr-1	{ 
	token id { <[a..zA..Z]>+ }
	token int	{ <[0..9]>+ }
	}

my $input = slurp;

my $m = G.parse($input);

# add on a final terminating halt
bpush0 Halt;
#@bcodes.push Halt;
#@bvals.push 0;

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
		print "$bcode\t";
		given $bcode {
			when Call { say @prims[$val][0]; }
			default { say $val; }
		}
	}
	say "---\n";
}
		


my $ip = 0;
sub run() {
	loop {
		my $bcode = @bcodes[$ip];
		my $val = @bvals[$ip];
		$ip++;
		given $bcode {
			when Add { my $v = spop; @sstack[slast] += $v; }
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
			default { die "Unknown opcode: $bcode before $ip";  }
		}
	}
	say "Bye";
}

say %labels;
resolve-labels;
disasm;
run;

