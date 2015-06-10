module Ledger where

import Control.Monad

import Comm
import Dps
import Etran
import Financial
import Nacc
import Ntran
import Parser
import Returns
import Ssah
import Utils
import Yahoo

--data Ledger = Ledger [Comm] [Etran] [Financial] [Ntran] [Nacc] Period [StockQuote] [Return] deriving (Show)

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

withLedger f = do
  inputs <- readInputs
  --let result = f inputs
  --printAll result
  return $ f inputs


-- | Read and trim ledger
ratl = liftM trimLedger readLedger

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
  
readLedger' inputs =
  let comms = getComms inputs in
  let etrans = getEtrans inputs in
  let (start, end) = last $ getPeriods inputs in
  let yahoos = getQuotes inputs in 
  let googles = getGoogles inputs in 
  let synths = synthSQuotes comms etrans in
  let quotes = yahoos ++ googles ++ synths in
  -- FIXME next 2 lines should prolly be in trim
  let comms1 = deriveComms start end quotes comms in
  let etrans1 = deriveEtrans start comms1 etrans in
  Ledger
         { comms = comms1
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

readLedger :: IO Ledger
readLedger = withLedger readLedger'


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


