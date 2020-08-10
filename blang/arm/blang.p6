#!/usr/bin/env perl6
use Template::Mustache;
#use trace;

sub xnote($x) {} # NB note prints to stderr
sub xsay($x) {}
sub tsay($x) { say "\t$x"; }




grammar G {
	rule TOP 	{ ^ <stmts> $ }
	rule stmts 	{ <statement>* }
	rule statement 	{ <printstr> | <print-stmt> | <for-loop> | <assign> | <goto> | <labelling>  }
	#rule dim	{ 'dim' <var> '(' <expr> ')' }
	rule goto	{ 'goto' <label-id> }
	rule labelling	{ <label-id> ':' }
	rule printstr	{ 'printstr' <expr> }
	rule print-stmt	{ 'print' <expr-list> }
	rule expr-list	{ <expr>+ % ',' }
	rule for-loop 	{ 'for' <var> '=' <from=.expr> 'to'  <to=.expr>  <stmts> 'next' }
	rule assign	{ 'let' <var> '=' <expr> }

	token kstr	{ '"' .*? '"'  }
	#regex kstr	{ ("\""([^\n\"\\]*(\\[.\n])*)*"\"") }
	token label-id	{ <:alpha>+ }
	token var 	{ <:alpha>+   }
	rule expr 	{ <expr-p>+ % '+' }
	rule expr-p	{ <num> | <var> | <kstr> }
	token num	{ <[0..9]>+ }
}

my $input = slurp;

my $m = G.parse($input);
xsay $m;

my $template = slurp  "template.asm";
my @plates = split("%%\n", $template);
my %plates;
for @plates -> $p {
	my @kv = split "\n", $p, 2;
	my $k =  @kv[0];
	my $v =  @kv[1];
	%plates{$k} = $v;
}

say %plates{"prologue"};




sub write-varnames(%vnames) {

	say "@ variables";
	say ".data";
	for  %vnames.keys.sort -> $k {
		say "$k: %vnames{$k}";
	}
}

my $lid =0;
sub mklabel() { $lid++; return "L$lid"; }

class A {
	has %.varnames;
	#has %.kstrs;

	method !add-asciz($k, $str) { %.varnames{$k} = ".asciz $str"};
	method !add-var($k, $v) { %.varnames{$k} = ".word $v"; }
	method !add-var1($k) { self!add-var( $k, 0); }

	method TOP ($/) { say $<stmts>.made; say %plates{"epilogue"}  ; write-varnames $.varnames ; }

	method printstr($/) {
		#my $label = $<expr>.made;
		my $expr = $<expr>.made;
		my $res = Q:s [
	$expr
	bl printstr
		];
		$/.make($res);
	}

	method goto($/) { my $res = "\tB $<label-id>"; $/.make("$res \t@ GOTO"); }

	method labelling($/) { $/.make("$<label-id>:\t@ LABEL"); }

	method kstr($/) { 
		my $label = mklabel;
		self!add-asciz( $label, $/.Str);
		$/.make($label);
	}

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
		} elsif $<goto> {
			$/.make($<goto>.made);
		} elsif $<for-loop> {
			$/.make($<for-loop>.made);
		} elsif $<labelling> {
			$/.make($<labelling>.made);
		} elsif $<printstr> {
			$/.make($<printstr>.made);
		} else { die "unhandled statement"; }

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
		my %vals = %( to-label => $to-label, for-test => $for-test, end-for => $end-for, 
			var => $var, from => $from, to => $to, stmts => $stmts );

		my $plate = %plates{"for-loop"};
		my $res = Template::Mustache.render($plate, %vals);
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
		if $<num> {
			$/.make("	ldr 	r0, =" ~ $<num> ~ "	@ expr-p const\n");

		} elsif $<var> {
			my $label = "$<var>";
			self!add-var($label, 0);
			$/.make("	load 	r0, $label	@ expr-p var\n");
		} elsif $<kstr> {
			my $label = $<kstr>.made;
			$/.make("	ldr	r0, =$label	@ expr-p kstr\n");
			#say "expr-p:kstr called";
		} else { die "Unhandles expr-p"; }

	}

	method print-stmt($/) { 
		#my $res = map *.made, $/.<expr
		my @exprs = $<expr-list>.values;
		#say "expression list is ", @exprs;
		my $res = "";

		#map { my $foo = *.made.perl ; say " TODO 2 $foo "; }, $/.<expr-list>;
		for @exprs -> $expr {
			#say "TODO 1 ";
			#say $express.made;
			#say "XXXX";
			$res ~= ($expr.made ~ "\n  bl      printd\n");
		}

		

		#say "TODO $res XXX";

		$/.make($res);
	}

	method var ($/) { my $vname = $/.Str ; xnote "Adding var: $vname" ; self!add-var($vname,  0) ; }
} 

my $acts = A.new;
my $ma = G.parse($input, :actions($acts));

