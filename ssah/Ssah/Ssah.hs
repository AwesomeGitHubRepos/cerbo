module Ssah where

--import Data.Char
import Data.Either
import Data.List
import Data.String.Utils
--import Data.Text
import Data.Tuple.Select
--import System.Directory
--import System.Path.Glob

import Comm
import Config
import Etran
import Financial
import Nacc
import Ntran
import Parser
import Returns
import Yahoo
import Utils

ssahTest :: String
ssahTest = "hello from Ssah"







--precacheCommsUsing :: [Comm] -> IO [StockQuote]
precacheCommsUsing concurrently comms = do
  quotes <- fetchCommQuotes concurrently comms -- will be filtered automatically
  file1 <- outFile "yahoo-cached.txt"
  saveStockQuotes file1 $ rights quotes
  ds <- dateString
  fname <- outFile ("yahoo" ++ fileSep ++ ds ++ ".txt")
  --let fname = "/home/mcarter/.ssa/yahoo/" ++ ds ++ ".txt"
  saveStockQuotes fname $rights quotes
  return quotes
 
-- | Download the Comms that apparently require fetching, and store to disk
precacheComms concurrently = do
  inputs <- readInputs
  let comms = makeTypes mkComm "comm" inputs
  cache <- precacheCommsUsing concurrently comms
  --print cache
  return cache

loadPrecachedComms = do
  yf <- yfile
  contents <- readFile yf
  let commands = map foldLine (lines contents)
  let quotes = getQuotes commands
  return quotes
  
-- mainSsah = print "TODO!"


data Price = Price String String Double deriving (Show)

mkPrice :: [[Char]] ->Price
mkPrice["P", dstamp, _, sym, price, _ ] =
    Price dstamp sym (asDouble price)


rox :: Double -> Comm -> Double
rox  usd c =
  scale
  where
    curr = commCurrency c
    tbl = [ ("USD", usd), ("P", 1.0), ("GBP", 100.0), ("NIL", 1.0) ]
    lup = lookup curr tbl
    scale = case lup of
      Nothing -> 666.0
      Just q -> q

-- | Fetch StockQuotes of Comm for which a fetch is required
--fetchCommQuotes :: [Comm] ->  IO [StockQuote]
fetchCommQuotes concurrently comms = do
  let hitComms = filter fetchRequired comms
  let tickers = map yepic hitComms
  usd <- fetchUsd
  -- let usd = 1.5
  let roxs = map (rox usd) hitComms
  fetchQuotesA concurrently tickers roxs

mkPeriod :: [[Char]] -> Period
mkPeriod ["period", start, end] =
  (start, end)
  
getPeriods inputs = makeTypes mkPeriod "period" inputs


printQuotes = do
  inputs <- readInputs
  let quotesYahoo =  getQuotes inputs
  printAll quotesYahoo
  let quotesGoogle = getGoogles inputs
  printAll quotesGoogle

  
allComms :: IO [Comm]
allComms = do
  inputs <- readInputs -- for testing purposes
  let comms = getComms inputs
  return comms
  


{-
ntrans = withLedger getNtrans
-}

-----------------------------------------------------------------------
-- Etb storage and retrieval

etbAsText etb =
  unlines $ map makeEtbLine etb
  where
    makeEtbLine etbEl =
      replace " " "" text1
      where
        (name, total) = etbEl
        pounds = unPennies total
        p = round $ pounds * 100
        text1 = name ++ "!" ++ (show p) ++ "!" ++ (show total)

  
storeEtb etb = do
  --let text = makeEtbFields totalTab naccs
  writeFile "/home/mcarter/.ssa/hssa-etb.txt" (etbAsText etb)

-----------------------------------------------------------------------

