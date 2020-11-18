# words

Implementation notes

## >CFA ( dw -- cfa)

Convert a dictionary header pointer to cfa (code field address, aka codeword pointer).

If dw is 0, then 0 is returned


## EMBIN

Functionally equivalent of BRANCH at execution time, but is used by SEE to identify embedded binary data,
such as from Z". It then knows to display the next items in the heap, rather than trying to dereference
it, which is disasterous!

## SEE

See also: EMBIN

