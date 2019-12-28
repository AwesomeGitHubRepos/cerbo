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
	adr	r0, _printd
	bl 	printf
	ldmia	sp!, {pc}
	_printd:
	.asciz "Printing %d\n"	
];

sub write-varnames(%vnames) {

	say "@ variables";
	say ".data";
	for  %vnames.keys.sort -> $k {
		#say ".balign 4\n$k: .word %vnames{$k}";
		say "$k: .word %vnames{$k}";
	}
}

my $lid =0;
sub mklabel() { $lid++; return "L$lid"; }

class A {
	has %.varnames;

	method !add-var($k, $v) { %.varnames{$k} = $v; }
	method !add-var1($k) { %.varnames{$k} = 0; }

	method TOP ($/) { say $<stmts>.made;  say $bye; write-varnames $.varnames ; }

	method stmts($/) {
		my $res = ""; 
		for @<statement> -> $stm { my $st1 = $stm.made ; $res ~= "$st1\n"; }
		$/.make($res);

	}

	method statement($/) { 
		if $<print-stmt> {
			$/.make($<print-stmt>.made); 
		} elsif $<assign> {
			$/.make($<assign>.made);
		} elsif $<for-loop> {
			$/.make($<for-loop>.made);
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
		my $stmts = $<stmts>.made;
		#my $stmts = "FOO";



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
	@ bl printd @ print var TODO remove
	$stmts

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
		my $res = "\t@ begin expr statement\n";
		my $n = $<expr-p>.elems;
		my $asm = @<expr-p>[0].made;

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
		$/.make($res);
		#say $<expr-p>.elems; 
	}

	method expr-p($/) {
		#my $label;
		if $<num> {
			#$label = "const_$<num>";
			#self!add-var( $label , $<num>);
			$/.make("	ldr 	r0, =" ~ $<num> ~ "	@ expr-p const\n");

		} else {
			my $label = "$<var>";
			self!add-var($label, 0);
			$/.make("	load 	r0, $label	@ expr-p var\n");
		}
		#my $res = "	load 	r0, $label\n";
		#$/.make($res);
	}

	method print-stmt($/) { 
		my $res = $<expr>.made ~ "\n	bl	printd\n";
		$/.make($res);
	}

	method var ($/) { my $vname = $/.Str ; xsay "Adding var: $vname" ; self!add-var($vname,  0) ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

