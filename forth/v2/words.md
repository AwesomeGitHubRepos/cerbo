# words

Implementation notes

## EMBIN

Functionally equivalent of BRANCH at execution time, but is used by SEE to identify embedded binary data,
such as from Z". It then knows to display the next items in the heap, rather than trying to dereference
it, which is disasterous!

## SEE

See also: EMBIN

