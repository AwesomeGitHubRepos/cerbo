%wrapper "basic"

$alpha = [a-zA-Z]

tokens :-

  $white+   ;
  $alpha+ { \s TokenSym s }

