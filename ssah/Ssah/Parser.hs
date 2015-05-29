module Ssah.Parser where

import Control.Monad
--import Control.Monad.IO.Class
import Data.Char
import System.Directory
import System.Path.Glob

import Ssah.Config

filterInputs inputs =
  filter (\x -> isAlpha (x !! 0)) nonblanks
  where all = (lines . unlines) inputs
        nonblanks = filter (\x -> length x > 0) all






-- FIXME I don't think the parser handles "" correctly (see fin with S "" for example)

eatWhiteXXX str = snd (span isSpace str)

eatWhite "" = ""
eatWhite ('#':xs) = ""
eatWhite (x:xs) = if isSpace x then eatWhite xs else x:xs

       


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

{-
fileListXXX :: IO [[Char]]
fileListXXX = do
  files1 <- glob "/home/mcarter/redact/docs/accts2014/data/*.txt"
  files2 <- glob "/home/mcarter/.ssa/yahoo/*.txt"
  files3 <- glob "/home/mcarter/.ssa/gofi/*.txt"
  files4 <- glob "/home/mcarter/redact/docs/accts2014/companies/*"
  --let files = files1 ++ files2 ++ files3 ++ files4
  --let files = liftM concat [files1, files2, files3, files4]
  let files = concat [files1, files2, files3, files4]
  -- print files
  --files <- liftM concat [files1, files2, files3, files4]
  return files
-}

fileList :: IO [String]
fileList = do
  globs <- readConf
  let files = mapM glob globs
  g <- files
  let h = concat g
  return h


readInputs = do
  -- let globs = readConf
  --contents <- mapM readFile fileList
  files <- fileList
  contents <- mapM readFile files
  let allLines = filterInputs contents
  let commands = map foldLine allLines
  return commands


