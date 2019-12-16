my @heap;
my $i; # pointer to heap

#say @heap.elems;

my @stack;
sub spush($x) { @stack.push: $x ; }
sub spop() { return @stack.pop ; }

my %plabels; # position of labels
my %clabels; # calls to labels
sub add_clabel($n, $label) { %clabels{@heap.elems()} = $label; hp $n; }

enum cmds <ebranch ebtp edot epush eadd ehalt eld eldl est>;

sub xsay { my @foo = @_ ; return; }

#sub xsay($a) {}
#sub xsay($a, $b) {}

sub hp($x) { @heap.push: $x; }
sub at() { return @heap[$i] ; }
sub pat($idx, $val) { @heap[$idx] = $val; }


sub ADD() { hp eadd; }
sub B($label) { hp ebranch ; add_clabel ebranch,  $label; }
sub BLK($n) { for [1..$n] { hp 701; } ; }
sub BTP($label) { hp ebtp; add_clabel 704, $label; }
sub DOT() { hp edot; }
sub HALT() { hp ehalt; }
sub LABEL($label) { %plabels{$label} = @heap.elems; }
sub LD($label) { hp eld; add_clabel 703, $label ; }
sub LDL($x) { hp eldl; hp $x; }
sub PUSH($n) { hp epush ; hp  $n; }
sub ST($label) { hp est; add_clabel 702, $label;  }



sub do_st() 
{
	#say @stack;
	$i++ ; 
	my $loc = at; 
	my $v = spop() ; 
	#say $v;
	pat $loc, $v; 
	#say "est $loc"; 
}

sub run()
{

	# resolve labels
	for %clabels.kv -> $k, $v {
		@heap[$k] = %plabels{$v} ;
	}
	#say @heap;


	# run the VM
	$i = 0;
	loop {
		given @heap[$i] {
			when eadd { xsay "ADD ", @stack; spush (spop() + spop()) ;  }
			when ebranch { xsay "found ebranch" ; $i++; $i = at; next; } 
			when ebtp { xsay "found ebtp" ; $i++; if spop() != 0 {$i = at; next; }; } 
			when edot { say spop(); }
			when epush { xsay "found epush"; $i++; spush (at); }
			when ehalt { xsay "found ehalt"; last; }
			when eld { $i++ ; my $v = @heap[at()]; spush $v; }
			when eldl { $i++ ; spush (at()); }
			when est { do_st; }
			default { say "unrecognised instruction"; }
		}
		$i += 1;
	}
}



sub test1 () { 
	B "START";
	LABEL "X"; BLK 1;
	LABEL "START";
	LDL 555;
	ST  "X";
	LD "X";
	HALT;
}

sub test2 () {
	# count up the sums from 0 to 5 inc. Result should be 15
	B "START";
	LABEL "X"; BLK 1;
	LABEL "START";
	LDL 0; 
	LDL 5;
	ST  "X";
	LABEL "LOOP";
	LD "X";
	ADD;
	LD "X";
	LDL -1;
	ADD;
	ST "X";
	LD "X";
	BTP "LOOP";
	DOT;
	HALT;
}

test2;


xsay @heap;

run;

xsay @stack;
xsay @heap;
