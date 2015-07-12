module Parser where

import Control.Monad
--import Control.Monad.IO.Class
import Data.Char
import System.Directory
import System.Path.Glob

import Config
import Types
import Utils

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


matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs


mkComm :: [[Char]] -> Comm
mkComm ["comm", sym, fetch, ctype, unit, exch, gepic, yepic, name] = 
    Comm sym bfetch ctype unit exch gepic yepic name Nothing Nothing
    where bfetch = (fetch == "W")

getComms inputs = makeTypes mkComm "comm" inputs


mkDps :: [[Char]] -> Dps
mkDps fields =
  Dps (map toUpper esym) dps
  where
    ["dps", esym, dpsStr ] = fields
    dps = --FIXME this should be abstracted (e.g. also in Yahoo.hs)
      case asEitherDouble dpsStr of
        Left msg -> error $ unlines ["mkDps double error conversion", show fields]
        Right v -> v

getDpss = makeTypes mkDps "dps"


mkEtran :: [[Char]] -> Etran
mkEtran fields =
    Etran dstamp etIsBuy folio sym qtyD amountP Nothing Nothing
    where
      ["etran", dstamp, way, folio, sym, qty, amount] = fields
      getDouble field name = --FIXME this should be abstracted (e.g. also in Yahoo.hs)
        case asEitherDouble field of
          Left msg -> error $ unlines ["mkEtran parseError", name, show fields, msg]
          Right v -> v
      etIsBuy = way == "B"
      sgn1 = if etIsBuy then 1.0 else (-1.0) :: Double
      qtyD = (getDouble qty "qty") * sgn1
      amountP = enPennies (sgn1 * (getDouble amount "amount"))

getEtrans = makeTypes mkEtran "etran"


-- | alt is the alternative account to use if the transaction is before the start date
mkNacc :: [String] -> Nacc
mkNacc ["nacc", acc, alt, desc] = Nacc acc alt desc 

getNaccs = makeTypes mkNacc "nacc"


mkNtran :: [String] -> Ntran
mkNtran ["ntran", dstamp, dr, cr, pennies, clear, desc] =
  Ntran dstamp dr cr (asPennies pennies) clear desc

getNtrans = makeTypes mkNtran "ntran"


mkReturn :: [String] -> Return
mkReturn ["return", arg2, arg3, arg4, arg5] =
  Return { idx = idxInt , dstamp = arg3
         , mine = (asDouble arg4), asx = (asDouble arg5) }
  where idxInt = (read arg2)::Int

getReturns inputs = makeTypes mkReturn "return" inputs


mkXacc :: [String] -> Xacc
mkXacc ("xacc":target:sources) = Xacc target sources

getXaccs  = makeTypes mkXacc "xacc"
