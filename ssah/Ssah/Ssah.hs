module Ssah.Ssah where

--import Data.Char
import Data.Either
import Data.List
import Data.String.Utils
--import Data.Text
import Data.Tuple.Select
--import System.Directory
--import System.Path.Glob

import Ssah.Comm
import Ssah.Etran
import Ssah.Financial
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Parser
import Ssah.Returns
import Ssah.Yahoo
import Ssah.Utils

ssahTest :: String
ssahTest = "hello from Ssah"







--precacheCommsUsing :: [Comm] -> IO [StockQuote]
precacheCommsUsing concurrently comms = do
  quotes <- fetchCommQuotes concurrently comms -- will be filtered automatically
  saveStockQuotes "/home/mcarter/.ssa/yahoo-cached.txt" $ rights quotes
  ds <- dateString
  let fname = "/home/mcarter/.ssa/yahoo/" ++ ds ++ ".txt"
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
  contents <- readFile "/home/mcarter/.ssa/yahoo-cached.txt"
  let commands = map foldLine (lines contents)
  let quotes = getQuotes commands
  return quotes
  
-- mainSsah = print "TODO!"


data Price = Price String String Float deriving (Show)

mkPrice :: [[Char]] ->Price
mkPrice["P", dstamp, _, sym, price, _ ] =
    Price dstamp sym (asFloat price)


rox :: Float -> Comm -> Float
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

--getFinancials inputs = makeTypes mkFinancial "FIN" inputs

{-
mkFinancial :: [[Char]] -> Financial
mkPeriod ["FIN", action', param1', param2'] =
  Financial { action = action', param1 = param1', param2 = param2' }
-}


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

