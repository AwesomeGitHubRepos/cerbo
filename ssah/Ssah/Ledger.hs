module Ledger  where

import Control.Monad
import Data.Either

import Comm
import Dps
import Etran
import Financial
import Nacc
import Ntran
import Parser
import Returns
import Types
import Utils
import Yahoo

data StockTrip = StockTrip
                 { stFile :: [StockQuote] -- from file cache
                 , stSynth :: [StockQuote] -- synthesised stock quotes
                 , stWeb :: [StockQuote] -- stock quotes downloaded from web
                 }

allSt:: StockTrip -> [StockQuote]
allSt (StockTrip f s w) = f ++ s ++ w
                 
data Ledger = Ledger
    { comms :: [Comm]
    , dpss :: [Dps]
    , etrans :: [Etran]
    , financials :: [Financial]
    , ntrans :: [Ntran]
    , naccs :: [Nacc]
    , start :: Dstamp
    , end :: Dstamp
    , squotes :: StockTrip
    , returns :: [Return]
    }

ledgerQuotes ledger = allSt $ squotes ledger


trimLedger :: Ledger -> Ledger
trimLedger ledger =
  ledger { comms = comms', etrans = etrans'', ntrans = ntrans'}
  where
    etrans' = filter (\e -> (etDstamp e) <= (end ledger)) $ etrans ledger

    trNtrans :: [Ntran] -> [Ntran] -> [Ntran]
    trNtrans acc ([]) = reverse acc
    trNtrans acc (n:ns) =
      if ntDstamp n > end ledger
      then trNtrans acc ns
      else trNtrans (n':acc) ns
      where
        Ntran dstamp dr cr pennies clear desc = n
        theNaccs = naccs ledger
        (dr', cr') = if dstamp < (start ledger)
                     then (alt dr theNaccs, alt cr theNaccs)
                     else (dr, cr)
        n' = Ntran dstamp dr' cr' pennies clear desc
    ntrans' = trNtrans [] $ ntrans ledger

    comms' = deriveComms (start ledger) (end ledger) (ledgerQuotes ledger) (comms ledger)
    etrans'' = deriveEtrans (start ledger) comms' etrans'
    --trNtrans = filter (\n -> (ntranDstamp n) <= (end ledger)) $ ntrans ledger


mkPeriod :: [[Char]] -> Period
mkPeriod ["period", start, end] =
  (start, end)
  
getPeriods inputs = makeTypes mkPeriod "period" inputs





readLedger' inputs =
  Ledger { comms = comms
         , dpss = getDpss inputs
         , etrans = etrans
         , financials = getFinancials inputs
         , ntrans = getNtrans inputs
         , naccs = getNaccs inputs
         , start = start
         , end = end
         , squotes = quotes
         , returns = getReturns inputs
         }
  where
    comms = getComms inputs
    etrans = getEtrans inputs
    (start, end) = last $ getPeriods inputs
    yahoos = getQuotes inputs 
    googles = getGoogles inputs
    synths = synthSQuotes comms etrans
    quotes = StockTrip (yahoos ++ googles)  synths []



freshQuotes :: Ledger -> Bool -> IO [Either String StockQuote]
freshQuotes ledger downloading = 
  if downloading then precacheCommsUsing True (comms ledger) else return ([])

{-
readLedger :: IO Ledger
--readLedger = withLedger readLedger'
readLedger = do
  inputs <- readInputs
  return $ readLedger' inputs
-}
-- | Read and trim ledger
ratl :: Bool -> IO Ledger
--ratlXXX = liftM trimLedger readLedger -- FIXME NOW do downloading if necessary
ratl fetch = do
  inputs <- readInputs
  let ledger1 = readLedger' inputs

  let squotes1 = squotes ledger1
  (errs, quotes) <- fmap partitionEithers $ freshQuotes ledger1 fetch -- FIXME handle errs
  let squotes2 = squotes1 { stWeb = quotes }
  let ledger2 = ledger1 { squotes = squotes2 }
  
  let ledger3 = trimLedger ledger2
  return ledger3


etranToSQuote :: [Comm] -> Etran -> StockQuote
etranToSQuote comms e =
  StockQuote ds "08:00:00" ticker 1.0 price 0.0 0.0
  where
    ds = etDstamp e
    ticker = findTicker comms (etSym e)
    amount = unPennies $ etAmount e
    qty = etQty e
    price = 100.0 * amount / qty

            
synthSQuotes :: [Comm] -> [Etran] -> [StockQuote] -- create synthetic stock quotes
synthSQuotes comms etrans =  map  (etranToSQuote comms)  etrans


