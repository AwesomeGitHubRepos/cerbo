
{
module Lexer where

-- seems to be standard requirements
import Data.Array 
}

%wrapper "basic"

$alpha = [a-zA-Z]

--startcode := 0

tokens :-
<0>  $white+   ;
<0>  $alpha+ { \s -> TokenSym s }
<0> \"([^\"]|\\.)*\" { \s -> TokenString $ take (length s - 2) $ drop 1 s }
--<0>  \" { begin string }
--<string> [^\"] { stringchar }
--<string> \"    {begin 0 }

{
data Token =
     TokenSym String
     | TokenString String
     deriving (Eq, Show)

}
