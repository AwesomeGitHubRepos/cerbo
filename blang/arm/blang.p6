grammar G {
	rule TOP 	{ <stmts> }
	rule stmts 	{ <statement>* }
	rule statement 	{ <print-stmt> | <for-loop> | <assign> }
	rule print-stmt	{ 'print' <var> }
	rule for-loop 	{ 'for' <var> '=' <expr> 'to'  <expr>  <stmts> 'next' }
	rule assign	{ <var> '=' <expr> }

	token var 	{ <[a..z]>+ }
	rule expr 	{ <expr-p> ('+' <expr-p>)* }
	rule expr-p	{ <num> | <var> }
 	token num	{ <[0..9]>+ }
}

#my $input = <STDIN>;
#my $input = slurp \*STDIN;
my $input = slurp;

say $input;

my $m = G.parse($input);
say $m;
