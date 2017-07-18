use v6;
unit module Shlex:ver<0.0.0>:auth<Mark Carter>;

#use Grammar::Debugger;

grammar Shlex {
	token TOP {<.ws>? (<word>|<qstring>|<comment>)* <.ws>?}
	token ws { \s+ }
	token comment { '#' .* }
	#rule word { <[a..zA..Z0..9\\-]>+ }
	token word1 { \S+ }
	token word { <word1> <.ws>? }
	token ascii_char { <-["\\]> } # anything not a " or \
	token escaped_char { "\\\"" } # literal \" 
	token qstr { [<escaped_char>|<ascii_char>]* }
	#token qstring { '"' [<escaped_char>|<ascii_char>]* '"' <.ws>?  }
	token qstring { '"' <qstr> '"' <.ws>?  }
}


class ShlexActions {
	has @.fields is rw;

	method word ($/) { @!fields.append: $<word1>.Str(); }
	method qstring ($/) { @!fields.append: $<qstr>.Str() ; }
}


sub shlex-fields(Str $str) is export { 
	my $shacts = ShlexActions.new;
	Shlex.parse($str, :actions($shacts));
	return $shacts.fields;
}


sub test-shlex is export {
	say shlex-fields "hello world";
}

