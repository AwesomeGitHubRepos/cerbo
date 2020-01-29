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

sub ms($targ, @outs) # match string
{
	return False if $targ ne $yytext;
	set-match;
	yylex;
	M_OUT(@outs);
	return True;
}


sub M_OUT(@args) {
	#my @args1 = args[0];
	#say args[1];
	
	for @args { 
		#my $arg = $_;
		#say "M_OUT arg:$arg";
		print ($_ eq "*" ?? $mtext !! $_), "  " ; 
	}
	return True;
}

sub M_PROGRAM() {
	#return M_OUT(("foo", "bar"));
	#return ((ms '.SYNTAX', ("foo", "bar")) and (M_OUT ".SYNTAX", "found"));
	return (ms('.SYNTAX', [".SYNTAX"])
		and id(["*", "\n"]));
}

sub parse() {
	yylex;
	M_PROGRAM;
}

init-parser ".SYNTAX PROGRAM world 'this is a string'  .NUMBER \$\$  ., .OUT";
parse;
