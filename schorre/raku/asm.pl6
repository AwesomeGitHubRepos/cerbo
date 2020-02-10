
my $asm =  "../aburry/schorre-metaii.vm";

#my $mii = slurp "../aburry/schorre-metaii.vm"; # metta in itself (asm)

#say $mii;

my %labels;
my $ip = 0; # instruction pointer

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


# COMPILE THE ASSEMBLEY CODE
for $asm.IO.lines -> $line {
	next if chars($line) == 0;
	#say $line.at(0); 
	if $line ~~ rx/ ^\s / {  # instruction
		my ($cmd, $arg) =  ($line ~~ rx/ \s+ (\S+) \s* (.*) /).list;
		#my $ins = trim $line;
		#say "balbel: $cmd,$arg" ;  
	} else { # label
		#say "adding label <$line>";
		%labels{$line} = $ip;
	}

	say $line;
}

say %labels;
