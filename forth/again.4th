0 prompt
: begin here ; immediate
: ?again compile ?branch , ; immediate
: test 5 begin dup . 1 - dup ?again drop ;

." about to run test" cr
test
cr
