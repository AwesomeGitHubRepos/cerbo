sub isalpha($x) { return $x (elem) (['a'..'z'] (|) ['A'..'Z']) ; }
sub isdigit($x) { return $x (elem) ['0'..'9']; }
sub isalnum($x) { return isalpha($x) or isdigit($x); }
sub iswhite($x) { return True if $x eq "\n"; return  " \t\r\n".contains($x); }
#sub iswhite($x) { say "iswhite:", ord($x); return  ord($x) (elem) [ 9 (|) 10 (|) 13 (|) 32] ; }

my $input_text;
my $pos;
sub tat($n)   { return ($n < chars($input_text) ?? substr($input_text, $n, 1) !! False); }
sub cpeek() { return tat($pos); }
sub cget()  { return tat($pos++); }
sub init-parser($str)
{
	$input_text = $str;
	$pos = 0;
	#yylex;
}

my $yytype;
my $yytext;

sub collect() { $yytext ~=  cget(); }

sub skipws() {
	while iswhite(cpeek()) {
		#say ord(cpeek());
		#if cpeek() eq "\n" { say "found newline"; }
		cget(); 
	}
}
sub yylex()
{
	$yytype = "eof";
	return False unless cpeek();
	$yytext = "";
	#say "yylex check on alpha:", cpeek(), ", ", isalpha(cpeek());
	skipws();
	return False unless cpeek();
	if isdigit(cpeek()) {
		$yytype = "num";
		while isdigit(cpeek()) { collect(); }
	} elsif (cpeek() eq "'") { 
		#say "found proper string";
		$yytype = "qstr";
		cget();
		while cpeek() ne "'" { collect(); }
		cget();
	} elsif  cpeek() eq "." {
		$yytype = "str";
		collect();
		while isalnum(cpeek()) or cpeek() eq "," { collect(); }
	} elsif isalpha(cpeek()) {
		$yytype = "id";
		while isalnum(cpeek()) { collect(); }
	} else {
		#say "hit default yylex";
		$yytype = "str";
		collect();
	}

	return True;
}

sub debug-lexer()
{
	while yylex() { say "$yytype <$yytext>"; }
}

#debug-lexer();

my $mtext;
my $mtype;

sub set-match() {
	$mtext = $yytext;
	$mtype = $yytype;
}

sub id( @outs) # match .ID
{
	return False if $yytype ne "id";
	#say "id matched";
	set-match;
	#say "mtext is now $mtext";
	yylex;
	M_OUT(@outs);
	return True;
}

sub qstr(@outs) # match quoted string
{
	return False if $yytype ne "qstr";
	#say "qstr:True";
	set-match;
	yylex;
	M_OUT(@outs);
	return True;
}

sub ms($targ, @outs) # match target string string
{
	return False if $targ ne $yytext;
	set-match;
	yylex;
	M_OUT(@outs);
	return True;
}

sub zom(&fn) { while fn() {}; return True; }

sub M_OUT(@args) {
	#my @args1 = args[0];
	#say args[1];
	
	for @args { 
		#my $arg = $_;
		#say "M_OUT arg:$arg";
		print ($_ eq "*" ?? $mtext !! $_) ; 
	}
	return True;
}


sub M_OOT() { # Output. Schorre calls it OUTPUT
	return ms(".OUT",["M_OUT"]);
}

sub M_PAT() { # Pattern. Schorre calls it EX3
	return (	
		id(["*", "()"])
		or ms(".ID", ["--.ID--"])
		or (ms("\$", ["zom(\{"]) and M_PAT() and M_OUT(["\})"]))
		or qstr(["<", "*", ">"])
	);
}

sub M_POO() { # Pattern Or Output. Schorre call it EX2
	#say "M_EX2 called";
	return zom({M_PAT() or M_OOT()});
}


sub M_ALT() { # Handle alternatives, the lowest precedence. It's what Schorre calls EX1
	return (
		M_POO() and zom({ (ms("/", [" or "]) and M_POO()); } )
	);
}

sub M_ST() {
	return (
		id(["sub ", "*", "() \{ return (\n" ]) # Output something like `sub PROGRAM() {'
		and ms("=", [])
		and M_ALT()
		and ms(".,", ["\n\);}\n\n"])
	);
}

sub M_PROGRAM() {
	return (
		ms('.SYNTAX', [])
		and id(["*", "();\n\n"]) # the main routine that has to be called, e.g. `PRORGAM();'
		and zom({M_ST()})
		and ms('.END', [".END at last"])
		);
}

sub parse() {
	yylex;
	M_PROGRAM;
}

my $p = q:to/EOS/;
.SYNTAX PROGRAM 
	PROGRAM = '.SYNTAX' .ID $ ST SUB '.END' .,
	world = foo  .OUT / bar / smurf turf 'geek' .,
	bar 	= baz .,
.END
EOS

#say $p;

init-parser $p;
parse;
