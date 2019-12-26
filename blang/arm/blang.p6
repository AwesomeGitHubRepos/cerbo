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
	say ".data";
	for  %vnames.keys.sort -> $k {
		say ".balign 4\n$k: .word %vnames{$k}";
	}
}


class A {
	has %.varnames;

	method !add-var($k, $v) { %.varnames{$k} = $v; }

	method TOP ($/) { say $bye; write-varnames $.varnames ; }

	method statement($/) { 
		if $<print-stmt> {
			say $<print-stmt>.made; 
		} elsif $<assign> {
			say $<assign>.made;
		}

	}

	method assign($/) {
		my $res = "\t@ ASSIGN\n" ~ $<expr>.made;
		my $label = "$<var>";
		self!add-var($label, 0);
		$res ~= "	ldr 	r1, =$label\n	str	r0, [r1]\n";
		$/.make($res);
	}

	method expr($/) {
		#say "@ expr:";
		my $res = "\t@ begin expr statement\n";
		my $n = $<expr-p>.elems;
		my $asm = @<expr-p>[0].made;

		#say $asm;
		$res = $res ~ $<expr-p>.first.made;
		@<expr-p>.shift;		
		for @<expr-p> -> $x {
			#$res ~= "\tpush \{r0\}\n";
			$res ~= "	mov	r1, r0\n";
			$res ~= $x.made;
			#$res ~= "\tpop \{r1\}\n";
			$res ~= "\tadd r0, r0, r1\n";

		}
		$res ~= "\t@end expr statement\n";
		#say $res;
		$/.make($res);
		#say $<expr-p>.elems; 
	}

	method expr-p($/) {
		my $label;
		if $<num> {
			$label = "const_$<num>";
			self!add-var( $label , $<num>);

		} else {
			$label = "$<var>";
			self!add-var($label, 0);
		}
		my $asm = "ldr r0, =$label";
		my $res  =  "\n\t@ expr-p:num move a constant to a register\n\t$asm\n\tldr r0, [r0]\n\n";
		$/.make($res);
	}

	method print-stmt($/) { 
		my $res = $<expr>.made ~ "\n	bl	printd\n";
		$/.make($res);
		#say "\t@ print statement"; 
		#say "\tmov	r0, #66" ;
		#tsay "bl	printd";
	}

	method for-loop($/) { }
	method var ($/) { my $vname = $/.Str ; xsay "Adding var: $vname" ; self!add-var($vname,  0) ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

