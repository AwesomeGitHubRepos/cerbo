#use trace;

sub xsay($x) {}
sub tsay($x) { say "\t$x"; }


grammar G {
	rule TOP 	{ ^ <stmts> $ }
	rule stmts 	{ <statement>* }
	rule statement 	{ <print-stmt> | <for-loop> | <assign> }
	rule print-stmt	{ 'print' <expr> }
	rule for-loop 	{ 'for' <var> '=' <expr> 'to'  <expr>  <stmts> 'next' }
	rule assign	{ 'let' <var> '=' <expr> }

	token var 	{ <[a..z]>+   }
	rule expr 	{ <expr-p>+ % '+' }
	#rule expr 	{ <expr-p> ('+' <expr-p>)* }
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

	@ FUNC: print integer
	@ IN: r0 integer to be printed
printd:
	stmdb 	sp!, {lr}
	mov	r1, r0
	ldr	r0, =_printd
	bl 	printf
	ldmia	sp!, {pc}
_printd:
	.asciz "Printing %d\n"	
];

sub write-varnames(%vnames) {

	say "@ variables";
	for  %vnames.keys.sort -> $k {
		say ".balign 4\n$k: .word %vnames{$k}";
	}
}


class A {
	has %.varnames;

	method !add-var($k, $v) { %.varnames{$k} = $v; }

	method TOP ($/) { ; say $bye; write-varnames $.varnames ; }

	method expr-p($/) {
		if $<num> {
			my $label = "const_$<num>";
			self!add-var( $label , $<num>);
			tsay "@ move a constant to a register";
			tsay "ldr r0, =$label";
			tsay "ldr r0, [r0]";
			tsay ""
		} else {
			say "var found";
		}
	}

	method print-stmt($/) { 
		#say "\t@ print statement"; 
		#say "\tmov	r0, #66" ;
		tsay "bl	printd";
	}

	method for-loop($/) { }
	method var ($/) { my $vname = $/.Str ; xsay "Adding var: $vname" ; self!add-var($vname,  0) ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

