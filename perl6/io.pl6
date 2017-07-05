# https://trizen.gitbooks.io/perl6-rosettacode/programming_tasks/S/Special_characters.html
# backspace (no delete)
my $BS = "\b";

# http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
#my $LEFT = "\0331D";

say ("hellow world" ~ ($BS x 5) ~ "there" ); # OUTPUT: hellow there
#sleep 2;
