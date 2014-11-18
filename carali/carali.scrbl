#lang scribble/manual

            


@title{Carali}



@defmodule[carali]
                  
by Mark Carter (@tt{mcturra2000 at yahoo co uk})

This library provides various utility functions. It is CArters RAcket LIbrary.


@; ---------------------------------------------------------------------------

@section{Notes to self}

This section is mainly for my own benefit, so that I know what is going on.

@section{datetime}

Date and time manipulation routines.

@defproc[(epoch [year integer?] [month integer?] [day integer?]) integer?]{
Days since beginning of some epoch. 
    month in [1,12]
    day in [1,31]
     Example:
     There is one day difference between 01-Jul-2010 and 30-Jun-2010
     
     @#reader scribble/comment-reader
     (racket (- (epoch 2010 7 1) (epoch 2010 6 30)) ; => 1
     )
}

@; ---------------------------------------------------------------------------
@section{hashes}

Functions that work on hashes.

@defproc[ (hash+ [h hash?] [key any/c?] [v number?]) void?]{
Increments the KEY of hash H by V. Example:
@racketblock[
(define h (make-hash))
(hash+ h "foo" 10)
(hash+ h "bar" 11)
(hash+ h "foo" 12)
(print h)
]
will print the result
@racketblock[
'#hash(("foo" . 22) ("bar" . 11))
]
}

@; ---------------------------------------------------------------------------
@section[#:tag "http"]{http}

This section details the http packages available

@defproc[ (http-get [url string?]) string?]{
Retrieves the contents of a URL.

Right:
@itemlist[ 
@item{ @racket[(http-get "http://www.google.co.uk")] } 
@item{ @racket[(http-get "http://www.markcarter.me.uk/")] } 
@item{ @racket[(http-get "http://www.markcarter.me.uk/index.html")]} 
]

Wrong:
@itemlist[
@item{ @racket[  (http-get "www.markcarter.me.uk") ] }
 ]

}

          
 
 
@; ---------------------------------------------------------------------------
 
@section{epics}

The epics module defines functions which interact with Yahoo Finance.

Here is an example of how to obtain the Net Asset Value of Fidelity Special Situations fund,
which has a code of GB0003875100:

@racketblock[
(require (planet "html-parsing.rkt" ("neil" "html-parsing.plt" 1 2)))
(define html (http-get "http://uk.finance.yahoo.com/q?s=GB0003875100.L&ql=0"))
(define xml (html->xexp html))
(extract-tree '(3 6 3 4 5 3 2 3 2 2 2 2 1 1 2) xml)
]


This prints out "16.58" on 03-Sep-2011. The best way to determine how to extract the information you
need from html is to use the enumerate-tree and extract-tree functions from the lists module.


@defproc[ (oeic [epic string?]) number?]{
Retrieves a price from Yahoo Finance in pounds.

Example:
@scheme[(oeic "GB0003875100.L")] will retrieve the price for Fidelity Special Situations Fund.
}

@defproc[ (yafi [epic string?]) number?]{
Retrieves a price from Yahoo Finance in pounds

Example:
@scheme[(yafi "ULVR.L")] will retrieve the price for Unilever.

}    
 
@; --------------------------------------------------------------------------- 
@section{lists}

List algorithms.

@subsection{List inspection routines}

Sometimes you have a complex data tree, and you know that the data you want is 
"in there somewhere", but you don't know offhand where. For example, you may want
to download an html, convert it to a tree using the "html-parsing" package from Planet,
and extract a specific item of data. The tree can be complex, and you don't know
whereabout in the hierarchy the information is. So you would use "enumerate-tree"
to dump the tree, which will give you a list of nodes, and their "paths". You can
inspect the dump, and use the path to extract the specific information you want.

Here's an example:
@racketblock[
(define tree  '("how" ("now" ("cow" "farm") "brown" )))
(enumerate-tree tree)
]
This will give the printout:
@racketblock[
'(0)
"how"

'(1 0)
"now"

'(1 1 0)
"cow"

'(1 1 1)
"farm"

'(1 2)
"brown"
]

This tells you that "how" is the 0th element of tree (everything is zero-indexed). The 0th element
of the 1th subtree of TREE is "now"; and so on.

Once you know the PATH, you can extract the data element. So, in the example above,
@racket[(extract-tree '(1 1 0)  tree)]
gives the value @racket["cow"].

Alternatively @racket[(extract-tree '(1 1)  tree)] yields @racket['("cow" "farm")].

@defproc[(enumerate-tree [node list?]) list?]{
Prints "paths" to nodes and its contents. Counterpart of @racket[extract-tree].
}

@defproc[(extract-tree [path list?] [tree list?]) any]{
Extracts the node from TREE indicated from PATH. Counterpart of @racket[enumerate-tree].
}

@defproc[(sum-categories [alist list?]) hash?]{
Given a list on cons whose car is categories, and cdr is a numeric value, return a hash whose keys are the
categories, and whose values are the sums of values in those categories. Example:
@racketblock[(sum-categories '(( "foo" . 10) ("bar" . 11) ("foo" . 12)))]
returns @racketblock['#hash(("foo" . 22) ("bar" . 11))]
}