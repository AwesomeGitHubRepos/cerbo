Useful for keyboard testing.

Compare the results with 
[Keys not working](https://wiki.archlinux.org/index.php/Home_and_End_keys_not_working)

Procedure is:
infocmp $TERM > terminfo.src

Then edit it to change the escape codes. For example change khome and 
kend:

khome=\E[1~, kend=\E[4~,

Warning: Ensure that no other key use the same character sequence.

Then compile the new terminfo (which saves it to your ~/.terminfo 
directory)

tic terminfo.src

And lastly specify the new terminfo in your shell's environment 
variables

export TERMINFO=~/.terminfo

BETTER to use:
* `read' bash command
* `showkeys -a'
