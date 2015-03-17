module Ssah where

import Data.Char
--import Data.Text
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

qty :: Etran -> Float
qty (Etran  _ _ _ _ q _) = q


qtys :: [Etran] -> Float
qtys es = sum $ map qty es

matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs

data Comm = Comm String Bool String String String String String String deriving (Show)


mkComm :: [[Char]] -> Comm
mkComm ["comm", sym, fetch, ctype, unit, exch, gepic, yepic, name] = 
    Comm sym bfetch ctype unit exch gepic yepic name
    where bfetch = (fetch == "W")

printn n  lst = mapM_ print  (take n lst)


yepic :: Comm -> String
yepic (Comm _ _ _ _ _ _ y _) = y

fetchRequired :: Comm -> Bool
fetchRequired (Comm _ f _ _ _ _ _ _) = f

yepics comms = map yepic $ filter fetchRequired comms

-- attempt to get around Jupyter crashing when using networking
makeYahooCsv = do
  inputs <- readInputs
  let comms = makeTypes mkComm "comm" inputs
  let ys = yepics comms
  fetchAndSave ys

loadYahooCsv = loadSaves
