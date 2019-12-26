#use trace;

sub xsay($x) {}
sub tsay($x) { say "\t$x"; }


grammar G {
	rule TOP 	{ ^ <stmts> $ }
	rule stmts 	{ <statement>* }
	rule statement 	{ <print-stmt> | <for-loop> | <assign> }
	rule print-stmt	{ 'print' <expr> }
	rule for-loop 	{ 'for' <var> '=' <from=.expr> 'to'  <to=.expr>  <stmts> 'next' }
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

	@ macros

	.macro load reg, addr
	ldr \reg, =\addr
	ldr \reg, [\reg]
	.endm

	.macro store reg, addr
	push {r4}
	ldr r4, =\addr
	str \reg, [r4]
	pop {r4}
	.endm


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

my $lid =0;
sub mklabel() { $lid++; return "L$lid"; }

class A {
	has %.varnames;

	method !add-var($k, $v) { %.varnames{$k} = $v; }
	method !add-var1($k) { %.varnames{$k} = 0; }

	method TOP ($/) { say $bye; write-varnames $.varnames ; }

	method statement($/) { 
		if $<print-stmt> {
			say $<print-stmt>.made; 
		} elsif $<assign> {
			say $<assign>.made;
		} elsif $<for-loop> {
			say $<for-loop>.made;
		}

	}

	method for-loop($/) {
		my $to-label = mklabel;
		self!add-var1($to-label);
		my $for-test = mklabel;
		my $end-for  = mklabel;
		my $var = $<var>;
		my $from = $<from>.made;
		my $to = $<to>.made;



		my $res = Q:s [ 
	@ FOR
	@ for:from precalc
	$from
	store r0, $var

	@ for:to precalc
	$to
	store r0, $to-label

	@ for:test
$for-test:	
	load r0, $var
	load r1, $to-label
	cmp  r0, r1
	bgt $end-for

	@ ...
	bl printd @ print var TODO remove

	@ for:next
	load r0, $var
	add r0, r0, #1
	store r0, $var
	b $for-test
$end-for:	@for:end

	];

	$/.make($res);

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

	method var ($/) { my $vname = $/.Str ; xsay "Adding var: $vname" ; self!add-var($vname,  0) ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

