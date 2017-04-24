#use Grammar::Debugger;
#use Grammar::Tracer;
#use experimental :macros;

use Text::Table::Simple; # zef install Text::Table::Simple


grammar rec {
	token TOP { <statement>* }	
	rule statement { <record-spec> | <load-file> | <show-table> }	
	token identifier { <[a..zA..Z]>+ }
	token ws { <[\r\n\t\ ]> }

	#token record-spec { <ws>* 'record' \s+ <rec-name> <field-descriptor>+ <ws> 'end-record' <ws> }	
	rule record-spec { 'record' <rec-name> <field-descriptor>+ 'end-record' }	
	token rec-name { \S+ }
	token field-descriptor { <ws>* <field-name> <ws>+ <field-type> <ws>* ';' }
	token field-name { \S+ }
	token field-type { <[a..z]>+ }

	rule load-file { 'load' 'tabbed' 'file' <load-file-name> 'of' <rec-name> 'into' <table-name> }
	token load-file-name { \S+ }
	token table-name { <identifier> }

	rule show-table { 'show' <table-name> }
}


class Field {
	has Str $.namo is rw;
	has Str $.type is rw;
}

class Rec {
	has Str $.namo;
	has Field @.fields;
	has %.flup; # look up from field name to an indexed number
	has Str @.fnames is rw; # field names

	method add_field(Field $f) {
		@.fields.push: $f;
		@.fnames.push: $f.namo;
		%.flup{$f.namo} = %.flup.elems;
	}

	method get-colnames() {  @.fnames  ;}
}

class Table {
	has Str $.nomo;
	has Rec $.r is rw;
	has @.data is rw;

	method load-tabbed-file(Str $file-name) {
		my $contents = slurp $file-name;
		@.data = (split /\n/, (trim-trailing $contents)).map( ->$x { split /\s+/, $x ; } );
	}

	method show() {
		my Rec $r = $.r;
		#say $r.WHAT;
		#say $r;
		$r.get-colnames();
		my @cols = $r.get-colnames();
		#say "cols are ", @cols;
		#say "data is: ", @.data;
		lol2table(@cols, @.data).join("\n").say;
	}

}

class qryActs {
	has Rec %.recs is rw;
	has Table %.tabs;

	method record-spec ($/) { 
		my $r = Rec.new(namo => $<rec-name>.Str);
		for $<field-descriptor> -> $fd {
			$r.add_field($fd.made);
		}
		%.recs{$<rec-name>.Str} = $r;
	}

	method field-descriptor ($/) { make Field.new(namo =>$<field-name>.Str, type => $<field-type>.Str); }

	method load-file ($/) { 
		my $nomo = $<table-name>.Str;
		my $tab = Table.new(nomo => $nomo);
		$tab.r = %.recs{$<rec-name>.Str};
		$tab.load-tabbed-file($<load-file-name>.Str);
		%.tabs{$nomo} = $tab;
	}

	method show-table ($/) { 
		%.tabs{$<table-name>.Str}.show();
	}
}




my $desc = q:to"FIN";
record person
	name string;
	age  int;
end-record

load tabbed file data.txt of person into people 
show people
FIN


#say $desc;

my $r = rec.parse($desc);
#say $r;
my $qa = qryActs.new;
my $r1 = rec.parse($desc, :actions($qa));
#say $qa.recs;

my $inp = q:to"FIN";
adam	26
joe	23
mark	51
FIN

my @m = (split /\n/, (trim-trailing $inp)).map( -> $x { split /\s+/, $x ; } );
my @cols = $qa.recs{"person"}.fnames;
say @cols;

sub print_table(@data) {
	lol2table(@cols, @data).join("\n").say;
}

#print_table @m;

grammar predi {
	token TOP { <ws>* <arg> <ws>* <rel> <ws>* <arg> <ws>* }
	token arg { <field-name> | <value> }
	token field-name { <[a..z]> \S+ }
	token value { <[0..9]>+ }
	token ws { <[\r\n\t\ ]> }
	token rel { '<' }
}

sub filter-sub($pred-str) {
	my $pr = predi.parse($pred-str);

	sub get-val($idx, $row) {
	       	my $v = $pr<arg>[$idx];
		my $ret;
		if $v<field-name>:exists {
			my $fnum = $qa.recs{"person"}.flup{$v};
			$ret = $row[$fnum];
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

my @some = filter-sub("age < 50");
print_table @some;
