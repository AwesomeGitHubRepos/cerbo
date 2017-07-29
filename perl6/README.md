# Perl6

## Piping

There is a detailed disuccion of bi-directional communication with
another process on [perl6-users](http://www.nntp.perl.org/group/perl.perl6.users/2017/07/msg4091.html).

## Shlex

Example::

```perl6
use Shlex;

my $str = Q[hellow "new \"old\" world"  to-be-or-not #to be];
my @fields = shlex-fields $str; 
say @fields.perl; # OUTPUT ["hellow", "new \\\"old\\\" world", "to-be-or-not"]
say @fields[1]; # OUTPUT new \"old\" world
```


When I asked on a userlist, I was told to 
zef install Text::CSV for an example of how this could be done.

I was also advised:

If you wanted something like Perl 5's Text::ParseWords module
(shellwords/quotewords), I'm not sure it exists yet in the Perl 6
ecosystem.

Alternatively, the «...» builtin operator already does some kind of
word splitting respecting quotes, if that fits your needs:

  > say .perl for << 'a b' "a b" "\t\n" >>
  "a b"
  "a b"
  "\t\n"


However, I have implemented my own Shlex module which does the trick,
including escape quoting.
