#use Grammar::Debugger;
use experimental :macros;

use Text::Table::Simple; # zef install Text::Table::Simple

my $desc = q:to"FIN";
record person
	name string;
	age  int;
end-record
FIN

grammar rec {
	token TOP { <ws>* 'record' \s+ <rec-name> <field-descriptor>+ <ws> 'end-record' <ws> }	
	token rec-name { \S+ }
	token field-descriptor { <ws>* <field-name> <ws>+ <field-type> <ws>* ';' }
	token field-name { \S+ }
	token field-type { <[a..z]>+ }
	token ws { <[\r\n\t\ ]> }
}



my $r = rec.parse($desc);

my $inp = q:to"FIN";
adam	26
joe	23
mark	51
FIN

sub splitter($line) { 
	my @lst = split /\s+/, $line; 
}


sub matrixify(&splitter, $data)
{
	my @d = (split /\n/, (trim-trailing $data)).map( -> $x { splitter $x ; } );
	@d;
}

my @m = matrixify &splitter, $inp;

my @cols = $r<field-descriptor>.map(->$fd { $fd<field-name>});
lol2table(@cols, @m).join("\n").say;
my %rlook;
for [0..@cols.elems-1] -> $i { %rlook{@cols[$i]} = $i ;};
#say %rlook;

grammar predi {
	token TOP { <ws>* <arg> <ws>* <rel> <ws>* <arg> <ws>* }
	token arg { <field-name> | <value> }
	token field-name { <[a..z]> \S+ }
	token value { <[0..9]>+ }
	token ws { <[\r\n\t\ ]> }
	token rel { '<' }
}

#class table 
sub filter-sub($pred-str) {
	my $pr = predi.parse($pred-str);

	sub get-val($idx, $row) {
	       	my $v = $pr<arg>[$idx];
		my $ret;
		if $v<field-name>:exists {
			#say "look up", %rlook
			$ret = $row[%rlook{$v}];
		} else {
			$ret = $v<value> ;
		}
		$ret;
	}

	my @filtered;
	for @m -> $row { 
		my $v1 = get-val(0, $row);
		my $v2 = get-val(1, $row);
		if $v1 < $v2 { @filtered.append: $row; }
	}
			
	@filtered;
}

macro enq($s) {
	quasi { Q ({{{$s}}}) };       
};

macro tfilter($expr) {
	# my $str1 = Q ($expr);
	quasi { 
		my $str = Q ({{{$expr}}});
		filter-sub $str; 
		#say $str; 
	};
}

#my $t = enq(foo bar);

my @some = filter-sub("age < 50");
#my @some = tfilter(age < 50);
lol2table(@cols, @some).join("\n").say;



