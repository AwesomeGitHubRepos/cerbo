my @heap;
my $i; # pointer to heap

#say @heap.elems;

my @stack;
sub spush($x) { @stack.push: $x ; }
sub spop() { return @stack.pop ; }

my %plabels; # position of labels
my %clabels; # calls to labels

enum cmds <ebranch epush eadd ehalt>;

# symbols are like :this

sub hp($x) { @heap.push: $x; }
sub at() { return @heap[$i] ; }


sub ADD() { hp eadd; }
sub B($label) { hp ebranch ; hp ebranch ; %clabels{@heap.elems()-1} = $label; }
sub HALT() { hp ehalt; }
sub LABEL($label) { %plabels{$label} = @heap.elems; }
sub PUSH($n) { hp epush ; hp  $n; }




sub run()
{

	# resolve labels
	for %clabels.kv -> $k, $v {
		@heap[$k] = %plabels{$v} ;
	}
	say @heap;

	
	# run the VM
	$i = 0;
	#while @heap[$i] != ehalt {
	loop {
		given @heap[$i] {
			when eadd { spush (spop() + spop()) ;  }
			when ebranch { say "found ebranch" ; $i++; $i = at; next; } 
			when epush { say "found epush"; $i++; spush (at); }
			when ehalt { say "found ehalt"; last; }
			default { say "unrecognised instruction"; }
		}
		$i += 1;
	}
}




B "A01";
PUSH 66;
LABEL "A01";
PUSH 100;
PUSH 200;
ADD;
HALT;

say @heap;

run;

say @stack;
