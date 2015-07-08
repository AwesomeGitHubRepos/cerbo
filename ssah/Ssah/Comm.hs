module Comm where

import Data.Either
import Data.List
import Data.Maybe
import Data.String.Utils
-- import Data.Tuple.Select

import Config
import Parser
import Utils
import Yahoo


{-
data CommDerived = CommDerived (Maybe Double) (Maybe Double) deriving (Show)

commDerivedTuple (CommDerived startPrice endPrice) =
  (startPrice, endPrice)
  
data Comm = Comm Sym Bool String String String String Ticker String (Maybe CommDerived) deriving (Show)
-}

data Comm = Comm
            { cmSym :: Sym
            , cmFetch :: Bool
            , cmType :: String
            , cmUnit :: String -- currency as string, e.g. USD P GBP NIL
            , cmExch :: String
            , cmGepic :: String
            , cmYepic :: Ticker
            , cmName :: String
            , cmStartPrice :: Maybe Double
            , cmEndPrice :: Maybe Double
            }deriving (Show)



mkComm :: [[Char]] -> Comm
mkComm ["comm", sym, fetch, ctype, unit, exch, gepic, yepic, name] = 
    Comm sym bfetch ctype unit exch gepic yepic name Nothing Nothing
    where bfetch = (fetch == "W")


--commTuple (Comm sym fetch ctype unit exch gepic yepic name derived) =
--  (sym, fetch, ctype, unit, exch, gepic, yepic, name, derived)

--commSym :: Comm -> Sym
--commSym c = sel1 $ commTuple c



findComm :: [Comm] -> Sym -> Comm
findComm comms sym =
  case hit of
    Just value -> value
    Nothing -> error ("ERR: findComm couldn't find Comm with Sym " ++ sym)
  where
    hit = find (\c -> sym == (cmSym c)) comms

findTicker :: [Comm] -> Sym -> Ticker
findTicker comms sym = cmYepic $ findComm comms sym
               
--fetchRequired :: Comm -> Bool
--fetchRequired c = sel2 $ commTuple c

--commType c = sel3 $ commTuple c

--commCurrency :: Comm -> String
--commCurrency c = sel4 $ commTuple c

--yepic :: Comm -> String
--yepic c = sel7 $ commTuple c

--commTicker = yepic

getComms inputs = makeTypes mkComm "comm" inputs

  
yepics comms = map cmYepic $ filter cmFetch comms    

deriveComm :: Dstamp -> Dstamp -> [StockQuote] -> Comm -> Comm
deriveComm start end quotes comm =
  --Comm sym fetch ctype unit exch gepic yepic name startPrice endPrice
  comm'
  where
    --(sym, fetch, ctype, unit, exch, gepic, yepic, name, _, _) = comm
    yepic = cmYepic comm
    startPrice = getStockQuote (\d -> d < start) yepic quotes
    endPrice = getStockQuote (\d -> d <= end) yepic quotes
    comm' = comm { cmStartPrice = startPrice, cmEndPrice = endPrice }
    --derived = Just (CommDerived startPrice endPrice)
  
deriveComms start end quotes comms  =
  map (deriveComm start end quotes) comms

{-
commDerived c =
  commDerivedTuple der
  where
    oops = error ("commDerived can't look up: " ++ (show c))
    der = doOrDie (sel9 $ commTuple c) oops
-}

--commStartPrice comm =  sel1 $ commDerived comm

commStartPriceOrDie comms sym =
  doOrDie (cmStartPrice comm) ("Can't find start price for:'" ++ sym ++ "'")
  where
    comm = findComm comms sym
  
-- commEndPrice comm = sel2 $ commDerived comm

commEndPriceOrDie comms sym =
  doOrDie (cmEndPrice comm) ("Can't find end price for:" ++ sym)
  where
    comm = findComm comms sym


precacheCommsUsing :: Bool -> [Comm] -> IO [Either String StockQuote]
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

rox :: Double -> Comm -> Double
rox  usd c =
  scale
  where
    curr = cmUnit c
    tbl = [ ("USD", usd), ("P", 1.0), ("GBP", 100.0), ("NIL", 1.0) ]
    lup = lookup curr tbl
    scale = case lup of
      Nothing -> 666.0
      Just q -> q

-- | Fetch StockQuotes of Comm for which a fetch is required
--fetchCommQuotes :: [Comm] ->  IO [StockQuote]
fetchCommQuotes concurrently comms = do
  let hitComms = filter cmFetch comms
  let tickers = map cmYepic hitComms
  usd <- fetchUsd
  -- let usd = 1.5
  let roxs = map (rox usd) hitComms
  fetchQuotesA concurrently tickers roxs
