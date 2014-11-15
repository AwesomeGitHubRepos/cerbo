#lang scribble/doc


@(require scribble/manual
          scribble/struct
          scribble/eval)

@title{@bold{Carali}: CArter's RAcket LIbrary}

This is just some test documentation.

@section{Great Expectations}

I'm wondering if this works at all.

@section{Money}

Example use of stats:
@codeblock|{
(require carali/money)
(stats 'barc '( 41.00 	43.20 	40.60 	50.20 	55.66 	52.74 	64.87 	66.25 	21.63 	22.70 ))
}|
