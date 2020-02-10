
my $asm =  "../aburry/schorre-metaii.vm";


my %labels;
my @label-refs;

my $ip = 0; # instruction pointer
my @heap; # set of instructions
my $oc = 2** 24;
sub encode($loc, $opcode, $val) { @heap[$loc] = $opcode* $oc + $val;}
sub decode($loc) { my $code = @heap[$loc]; return ($code div $oc, $code % $oc); }

enum OpType ( none => 0, str => 1, lbl => 2);

my @opcodes = (
	("adr", lbl),
	("be", none),
	("bf", lbl),
	("bt", lbl),
	("ci", none),	
	("cl", str),
	("cll", lbl),
	("end", none),
	("gn1", none),
	("id", none),
	("lb", none),
	("out", none),
	("r", none),
	("set", none),
	("sr", none),
	("tst", str)
);

# CREATE A LOOK-UP TABLE OF THE OPCODES
my %opnums;
my $opnum;
for @opcodes {
	%opnums{$_[0]} = $opnum++;
}
#say %opnums;

# REMEMBER STRINGS
my @str-table;


# COMPILE THE ASSEMBLEY CODE
sub add-instruction($line) {
	my ($cmd, $arg) =  ($line ~~ rx/ \s+ (\S+) \s* (.*) /).list;
	my $num = %opnums{$cmd};
	my $arg1 = 0;
	my $type = @opcodes[$num][1];
	if $type == lbl {
		@label-refs.push((@heap.elems, $arg));  # for subsequent back-filling
	} elsif $type == str {
		$arg1 = @str-table.elems;
		@str-table.push($arg);
	}

	@heap.push($num * $oc + $arg1);
}

for $asm.IO.lines -> $line {
	next if chars($line) == 0;
	#say $line.at(0); 
	if $line ~~ rx/ ^\s / {  # instruction
		add-instruction $line;
	} else { # label
		#say "adding label <$line>";
		%labels{$line} = @heap.elems;
	}

	say $line;
}

# backfill the label references
#say @label-refs;
for @label-refs {
	my ($loc, $label) = $_;
	my ($opcode, $val) = decode($loc);
	$val =  %labels{$label};
	#say "label-ref val: $val";
	encode $loc, $opcode, $val;
}

sub disassemble() {	
	$ip = 0;
	#$label-ref = 0;
	loop {
		my ($code, $val) = decode($ip);
		my ($opcode, $type) = @opcodes[$code];
		if $type == none {
			$val = "";
		} elsif $type == str {
			$val =  @str-table[$val];
		} else { # label
			say "TODO label decode";
		}

		for %labels.kv -> $label, $loc {
			#my $key = $_[0];
			#say "foo $_[1]";
			if $loc == $ip {
				say "$ip LABEL $label";
			}
		}

		say "     $ip $opcode $val";
		$ip++;
		last if $ip >= @heap.elems;
	}
}

disassemble;

say %labels;
#say @opcode-strings;
