# https://rosettacode.org/wiki/Get_system_command_output#Perl_6
# https://docs.perl6.org/type/IO::Pipe

my $p = run 'ls', '-al', :out;
my $o = $p.out;

# make it more difficult for ourselves rather than just slurping
loop {
	my $txt = $o.get;
	last if $o.eof;
	say $txt;
}
$o.close;
