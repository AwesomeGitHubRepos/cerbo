module Ledger  where

import Control.Monad

import Comm
import Dps
import Etran
import Financial
import Nacc
import Ntran
import Parser
import Returns
--import Ssah
import Utils
import Yahoo

data Ledger = Ledger
    { comms :: [Comm]
    , dpss :: [Dps]
    , etrans :: [Etran]
    , financials :: [Financial]
    , ntrans :: [Ntran]
    , naccs :: [Nacc]
    , start :: Dstamp
    , end :: Dstamp
    , squotes :: [StockQuote]
    , returns :: [Return]
    }

{-
withLedger f = do
  inputs <- readInputs
  return $ f inputs
-}



-- FIXME trim on start, too
trimLedger ledger =
  ledger { etrans = etrans', ntrans = ntrans'}
  where
    etrans' = filter (\e -> (etDstamp e) <= (end ledger)) $ etrans ledger

    trNtrans :: [Ntran] -> [Ntran] -> [Ntran]
    trNtrans acc ([]) = reverse acc
    trNtrans acc (n:ns) =
      if ntranDstamp n > end ledger
      then trNtrans acc ns
      else trNtrans (n':acc) ns
      where
        (dstamp, dr, cr, pennies, clear, desc) = ntranTuple n
        theNaccs = naccs ledger
        (dr', cr') = if dstamp < (start ledger)
                     then (alt dr theNaccs, alt cr theNaccs)
                     else (dr, cr)
        n' = Ntran dstamp dr' cr' pennies clear desc
    ntrans' = trNtrans [] $ ntrans ledger

    --trNtrans = filter (\n -> (ntranDstamp n) <= (end ledger)) $ ntrans ledger


mkPeriod :: [[Char]] -> Period
mkPeriod ["period", start, end] =
  (start, end)
  
getPeriods inputs = makeTypes mkPeriod "period" inputs


readLedger' inputs =
  Ledger { comms = comms1
         , dpss = getDpss inputs
         , etrans = etrans1
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
    quotes = yahoos ++ googles ++ synths
    -- FIXME next 2 lines should prolly be in trim
    comms1 = deriveComms start end quotes comms
    etrans1 = deriveEtrans start comms1 etrans


readLedger :: IO Ledger
--readLedger = withLedger readLedger'
readLedger = do
  inputs <- readInputs
  return $ readLedger' inputs

-- | Read and trim ledger
ratl :: IO Ledger
ratl = liftM trimLedger readLedger



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


