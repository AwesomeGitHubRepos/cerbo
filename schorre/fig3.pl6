my @heap;
my @stack;

enum cmds <epush eadd ehalt>;

# symbols are like :this

sub hp($x) { @heap.push: $x; }

sub ADD() { hp eadd; }
sub PUSH($n) { hp epush ; hp  $n; }
sub HALT() { hp ehalt; }



sub run()
{
	my $i = 0;
	#while @heap[$i] != ehalt {
	loop {
		given @heap[$i] {
			when eadd { say "found eadd";  }
			when epush { say "found epush"; $i++; }
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
