module Ssah.Ssah where

import Data.Char
--import Data.Text
import Data.Tuple.Select
import System.Directory
--import System.FilePath.Glob
import System.Path.Glob

import Ssah.Yahoo
import Ssah.Utils

ssahTest :: String
ssahTest = "hello from Ssah"



filterInputs inputs =
  filter (\x -> isAlpha (x !! 0)) nonblanks
  where all = (lines . unlines) inputs
        nonblanks = filter (\x -> length x > 0) all








eatWhite str = snd (span isSpace str)

-- TODO fix bug where this is not a termination by a "
getQuoted str =
  (h, rest)
  where (h, t) = break (\x -> x == '"') (tail str)
        rest = drop 1 t
        --body =  init all
        --len = 2 + length body

getUnquoted str = (break isSpace str)
--  (len, body)
--  where body = fst (break isSpace str)
--        len = length body

lexeme str
  | length nonWhite == 0 = ("", "")
  | nonWhite !! 0 == '"' = (getQuoted nonWhite)
  | otherwise = (getUnquoted nonWhite)
  where nonWhite = eatWhite str

foldLine' acc str
  | length lex == 0 = acc
  | otherwise = foldLine' (acc ++ [lex]) rest
  where (lex, rest) = lexeme str
    
foldLine str = foldLine' [] str




readInputs = do
  files <- glob "/home/mcarter/redact/docs/accts2014/data/*.txt"
  contents <- mapM readFile files
  let allLines = filterInputs contents
  let commands = map foldLine allLines
  return commands


data Etran = Etran String String String String Float Float deriving (Show)

mkEtran :: [[Char]] -> Etran
mkEtran ["etran", dstamp, way, acc, sym, qty, amount] =
    Etran dstamp way acc sym (signed qty) (signed amount)
    where
        sgn1 = if way == "B" then 1.0 else -1.0
        signed f = (asFloat f ) * sgn1

etranTuple (Etran dstamp way acc sym qty amount) =
  (dstamp, way, acc, sym, qty, amount)

etranSym :: Etran -> Sym
etranSym e = sel4 $ etranTuple e

qty :: Etran -> Qty
qty e = sel5 $ etranTuple e


qtys :: [Etran] -> Float
qtys es = sum $ map qty es

getEtrans = makeTypes mkEtran "etran"

matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs

data Comm = Comm String Bool String String String String String String deriving (Show)


mkComm :: [[Char]] -> Comm
mkComm ["comm", sym, fetch, ctype, unit, exch, gepic, yepic, name] = 
    Comm sym bfetch ctype unit exch gepic yepic name
    where bfetch = (fetch == "W")


commTuple (Comm sym fetch ctype unit exch gepic yepic name) =
  (sym, fetch, ctype, unit, exch, gepic, yepic, name)

commSym :: Comm -> Sym
commSym c = sel1 $ commTuple c

allComms = do
  inputs <- readInputs -- for testing purposes
  let comms = getComms inputs
  return comms

fetchRequired :: Comm -> Bool
fetchRequired c = sel2 $ commTuple c

commType c = sel3 $ commTuple c

commCurrency :: Comm -> String
commCurrency c = sel4 $ commTuple c

yepic :: Comm -> String
yepic c = sel7 $ commTuple c


getComms inputs = makeTypes mkComm "comm" inputs


yepics comms = map yepic $ filter fetchRequired comms

{-
-- attempt to get around Jupyter crashing when using networking
makeYahooCsv = do
  inputs <- readInputs
  let comms = makeTypes mkComm "comm" inputs
  let ys = yepics comms
  fetchAndSave ys

loadYahooCsv = loadSaves
-}

mainSsah = print "TODO!"


data Price = Price String String Float deriving (Show)

mkPrice :: [[Char]] ->Price
mkPrice["P", dstamp, _, sym, price, _ ] =
    Price dstamp sym (asFloat price)

    

