module Ssah.Comm where

import Data.List
import Data.Tuple.Select

import Ssah.Utils

data Comm = Comm Sym Bool String String String String Ticker String deriving (Show)


mkComm :: [[Char]] -> Comm
mkComm ["comm", sym, fetch, ctype, unit, exch, gepic, yepic, name] = 
    Comm sym bfetch ctype unit exch gepic yepic name
    where bfetch = (fetch == "W")


commTuple (Comm sym fetch ctype unit exch gepic yepic name) =
  (sym, fetch, ctype, unit, exch, gepic, yepic, name)

commSym :: Comm -> Sym
commSym c = sel1 $ commTuple c



findComm :: [Comm] -> Sym -> Comm
findComm comms sym =
  case hit of
    Just value -> value
    Nothing -> error ("ERR: findComm couldn't find Comm with Sym " ++ sym)
  where
    hit = find (\c -> sym == (commSym c)) comms

findTicker :: [Comm] -> Sym -> Ticker
findTicker comms sym =
  yepic $ findComm comms sym
               
fetchRequired :: Comm -> Bool
fetchRequired c = sel2 $ commTuple c

commType c = sel3 $ commTuple c

commCurrency :: Comm -> String
commCurrency c = sel4 $ commTuple c

yepic :: Comm -> String
yepic c = sel7 $ commTuple c

commTicker = yepic

getComms inputs = makeTypes mkComm "comm" inputs


yepics comms = map yepic $ filter fetchRequired comms    
