#use trace;

sub xsay($x) {}


grammar G {
	rule TOP 	{ ^ <stmts> $ }
	rule stmts 	{ <statement>* }
	rule statement 	{ <print-stmt> | <for-loop> | <assign> }
	rule print-stmt	{ 'print' <expr> }
	rule for-loop 	{ 'for' <var> '=' <expr> 'to'  <expr>  <stmts> 'next' }
	rule assign	{ 'let' <var> '=' <expr> }

	token var 	{ <[a..z]>+   }
	rule expr 	{ <expr-p> ('+' <expr-p>)* }
	rule expr-p	{ <num> | <var> }
 	token num	{ <[0..9]>+ }
}

my $input = slurp;

my $m = G.parse($input);
xsay $m;

# print the prologue 
say  Q [
@ Automatically-generated assembler code
.global main
main:
	@ entry point
	push    {ip, lr}
];


# exit and cleanup before we start putting out data
my $bye = Q [
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}

print_num_str:
	.asciz "Printing %d\n"	
];

sub write-varnames($vnames) {

	say "@ variables";
	for $vnames.keys.sort {
		say ".balign 4\n$_: .word 0";
	}
}


class A {
	has $.varnames = SetHash.new;

	method TOP ($/) { ; say $bye; write-varnames $.varnames ; }

	method print-stmt($/) { 
		say "\t@ print statement"; 
		say "\tldr	r0, =print_num_str" ; 
		say "\tmov	r1, #66" ;
		say "\tbl printf";
	}

	method for-loop($/) { }
	method var ($/) { my $vname = $/.Str ; xsay "Adding var: $vname" ; $.varnames{"$vname"}++ ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

