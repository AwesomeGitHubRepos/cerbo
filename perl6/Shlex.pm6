#use Grammar::Debugger;

grammar Shlex {
	token TOP {<.ws>? (<word>|<qstring>)* <.ws>?}
	token ws { \s+ }
	rule word { <[a..zA..Z0..9\\-]>+ }
	token ascii_char { <-["\\]> } # anything not a " or \
	token escaped_char { "\\\"" } # literal \" 
	token qstring { '"' [<escaped_char>|<ascii_char>]* '"' <.ws>?  }
}

my $m = Shlex.parse(Q[goodbye "\"cruel\" world"  ]);
say "First  component:$m[0][0].Str()"; # OUTPUT: First  component:goodbye
say "Second component:$m[0][1].Str()"; # OUTPUT: Second component:"\"cruel\" world"
	
