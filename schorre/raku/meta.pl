sub isalpha($x) { return $x (elem) (['a'..'z'] (|) ['A'..'Z']) ; }
sub isdigit($x) { return $x (elem) ['0'..'9']; }
sub isalnum($x) { return isalpha($x) or isdigit($x); }
sub iswhite($x) { return  " \t\r\n".contains($x); }

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

sub yylex()
{
	$yytype = "eof";
	return False unless cpeek();
	$yytext = "";
	#say "yylex check on alpha:", cpeek(), ", ", isalpha(cpeek());
	while iswhite(cpeek()) { cget(); }
	return False unless cpeek();
	if isdigit(cpeek()) {
		$yytype = "num";
		while isdigit(cpeek()) { collect(); }
	} elsif (cpeek() eq "'") { 
		#say "found proper string";
		$yytype = "str";
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


sub ms($targ) # match string
{
	return False if $targ ne $yytext;
	say "ms:bingo";
	yylex;
	return True;
}

sub M_OUT(|args) 
{
	for args { print $_, " " ; }
		#my $str = args[1];
		#	say "M_OUT ", args.elems;

	return True;
}

sub M_PROGRAM() {
	return ((ms '.SYNTAX') and (M_OUT ".SYNTAX", "found"));
}

sub parse()
{
	yylex;
	M_PROGRAM;
}

init-parser ".SYNTAX hello world 'this is a string'  .NUMBER \$\$  ., .OUT";
parse;
