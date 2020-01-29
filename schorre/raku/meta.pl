sub isalpha($x) { return $x (elem) (['a'..'z'] (|) ['A'..'Z']) ; }
sub isdigit($x) { return $x (elem) ['0'..'9']; }
sub isalnum($x) { return isalpha($x) or isdigit($x); }
sub iswhite($x) { return " \t\rn\n".contains($x); }

my $text;
my $pos;
sub cpeek() { return ~$text[$pos]; }
sub cget()  { return ~$text[$pos++]; }
sub init-parser($str)
{
	$text = $str;
	$pos = 0;
}

my $yytype;
my $yytext;

sub collect() { $yytext ~=  cget(); }

sub yylex()
{
	$yytype = "unk";
	$yytext = "";
	while iswhite(cpeek()) { cget(); }
	given cpeek() {
		when isdigit(cpeek()) {
			$yytype = "num";
			while isdigit(cpeek()) { collect(); }
		}
		when (cpeek() eq "'") { # 39 is '
			$yytype = "str";
			cget();
			while cpeek() != "'" { collect(); }
			cget();
		}
		when cpeek() eq "." {
			$yytype = "str";
			collect();
			while isalnum(cpeek()) or cpeek() == "," { collect(); }
		}
		when isalpha(cpeek()) {
			$yytype = "id";
			while isalnum(cpeek()) { collect(); }
		}
		default {
			$yytype = "str";
			collect();
		}
	}
}

sub debug-lexer()
{
	while yylex() { say "$yytype <$yytext>"; }
}

init-parser "hello world";
debug-lexer();
