my @heap;
my $i; # pointer to heap


my @stack;
sub spush($x) { @stack.push: $x ; }
sub spop() { return @stack.pop ; }

enum cmds <epush eadd ehalt>;

# symbols are like :this

sub hp($x) { @heap.push: $x; }
sub at() { return @heap[$i] ; }


sub ADD() { hp eadd; }
sub PUSH($n) { hp epush ; hp  $n; }
sub HALT() { hp ehalt; }



sub run()
{
	$i = 0;
	#while @heap[$i] != ehalt {
	loop {
		given @heap[$i] {
			when eadd { spush (spop() + spop()) ;  }
			when epush { say "found epush"; $i++; spush (at); }
			when ehalt { say "found ehalt"; last; }
			default { say "unrecognised instruction"; }
		}
		$i += 1;
	}
}





PUSH 100;
PUSH 200;
ADD;
HALT;

say @heap;

run;

say @stack;
