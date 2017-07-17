use v6;
unit module Shlex:ver<0.0.0>:auth<Mark Carter>;

#use Grammar::Debugger;

grammar Shlex {
	token TOP {<.ws>? (<word>|<qstring>|<comment>)* <.ws>?}
	token ws { \s+ }
	token comment { '#' .* }
	#rule word { <[a..zA..Z0..9\\-]>+ }
	token word1 { \S+ }
	rule word { <word1> }
	token ascii_char { <-["\\]> } # anything not a " or \
	token escaped_char { "\\\"" } # literal \" 
	token qstr { [<escaped_char>|<ascii_char>]* }
	#token qstring { '"' [<escaped_char>|<ascii_char>]* '"' <.ws>?  }
	token qstring { '"' <qstr> '"' <.ws>?  }
}




#my $m = Shlex.parse(Q[goodbye "\"cruel\" world"  ]);
#say "First  component:$m[0][0].Str()"; # OUTPUT: First  component:goodbye
#say "Second component:$m[0][1].Str()"; # OUTPUT: Second component:"\"cruel\" world"

#say Shlex.parse("hello \"good fello\" #what say you").gist;

class ShlexActions {
	has @.fields is rw;

	method word ($/) { @!fields.append: $<word1>.Str(); }
	method qstring ($/) { @!fields.append: $<qstr>.Str() ; }
}

#my $shacts = ShlexActions.new;
#Shlex.parse("hellow \"new world\"  to-be-or-not #to be", :actions($shacts));

sub shlex-fields(Str $str) is export { 
	my $shacts = ShlexActions.new;
	Shlex.parse($str, :actions($shacts));
	return $shacts.fields;
}

