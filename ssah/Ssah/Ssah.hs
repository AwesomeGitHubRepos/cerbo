module Ssah.Ssah where

--import Data.Char
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
import Ssah.Yahoo
import Ssah.Utils

ssahTest :: String
ssahTest = "hello from Ssah"







precacheCommsUsing :: [Comm] -> IO [StockQuote]
precacheCommsUsing comms = do
  quotes <- fetchCommQuotes comms -- will be filtered automatically
  saveStockQuotes "/home/mcarter/.ssa/yahoo-cached.txt" quotes
  ds <- dateString
  let fname = "/home/mcarter/.ssa/yahoo/" ++ ds ++ ".txt"
  saveStockQuotes fname quotes
  return quotes
 
-- | Download the Comms that apparently require fetching, and store to disk
precacheComms = do
  inputs <- readInputs
  let comms = makeTypes mkComm "comm" inputs
  cache <- precacheCommsUsing comms
  --print cache
  return cache

loadPrecachedComms = do
  contents <- readFile "/home/mcarter/.ssa/yahoo-cached.txt"
  let commands = map foldLine (lines contents)
  let quotes = getQuotes commands
  return quotes
  
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
fetchCommQuotes :: [Comm] ->  IO [StockQuote]
fetchCommQuotes comms = do
  let hitComms = filter fetchRequired comms
  let tickers = map yepic hitComms
  usd <- fetchUsd
  let roxs = map (rox usd) hitComms
  fetchQuotesA tickers roxs

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

data Ledger = Ledger [Comm] [Etran] [Financial] [Ntran] [Nacc] Period [StockQuote] deriving (Show)

readLedger :: IO Ledger
readLedger = do
  inputs <- readInputs
  let comms = getComms inputs
  let etrans = getEtrans inputs
  let financials = getFinancials inputs
  let ntrans = getNtrans inputs
  let naccs = getNaccs inputs
  let period = last $ getPeriods inputs
  let yahoos = getQuotes inputs
  let googles = getGoogles inputs
  let quotes = yahoos ++ googles
  --      let period = last periods
  let ledger = Ledger comms etrans financials ntrans naccs period quotes
  --printAll quotes
  return ledger

printQuotes = do
  inputs <- readInputs
  let quotesYahoo =  getQuotes inputs
  printAll quotesYahoo
  let quotesGoogle = getGoogles inputs
  printAll quotesGoogle

ledgerTuple (Ledger comms etrans financials ntrans naccs period quotes) =
  (comms, etrans, financials, ntrans, naccs, period, quotes)

ledgerComms :: Ledger -> [Comm]
ledgerComms l = sel1 $ ledgerTuple l

ledgerEtrans :: Ledger -> [Etran]
ledgerEtrans l = sel2 $ ledgerTuple l

ledgerFinancials :: Ledger -> [Financial]
ledgerFinancials l = sel3 $ ledgerTuple l

ledgerNtrans :: Ledger -> [Ntran]
ledgerNtrans l = sel4 $ ledgerTuple l

ledgerNaccs :: Ledger -> [Nacc]
ledgerNaccs l = sel5 $ ledgerTuple l

ledgerPeriod :: Ledger -> Period
ledgerPeriod l = sel6 $ ledgerTuple l

ledgerQuotes :: Ledger -> [StockQuote]
ledgerQuotes l = sel7 $ ledgerTuple l
  
allComms :: IO [Comm]
allComms = do
  inputs <- readInputs -- for testing purposes
  let comms = getComms inputs
  return comms
  
withLedger f = do
  inputs <- readInputs
  let result = f inputs
  printAll result

ntrans = withLedger getNtrans


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
