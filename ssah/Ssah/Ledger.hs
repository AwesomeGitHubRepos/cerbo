module Ssah.Ledger where

import Control.Monad

import Ssah.Comm
import Ssah.Etran
import Ssah.Financial
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Parser
import Ssah.Returns
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo

--data Ledger = Ledger [Comm] [Etran] [Financial] [Ntran] [Nacc] Period [StockQuote] [Return] deriving (Show)

data Ledger = Ledger
    { comms :: [Comm]
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
  ledger { etrans = trEtrans, ntrans = trNtrans}
  where
    trEtrans = filter (\e -> (etranDstamp e) <= (end ledger)) $ etrans ledger
    trNtrans = filter (\n -> (ntranDstamp n) <= (end ledger)) $ ntrans ledger
  
readLedger' inputs =
  let comms = getComms inputs in
  let etrans = getEtrans inputs in
  --let financials = getFinancials inputs in
  --let ntrans = getNtrans inputs in
  --let naccs = getNaccs inputs in
  let (start, end) = last $ getPeriods inputs in
  let yahoos = getQuotes inputs in 
  let googles = getGoogles inputs in 
  let synths = synthSQuotes comms etrans in
  let quotes = yahoos ++ googles ++ synths in
  let comms1 = deriveComms start end quotes comms in
  let etrans1 = deriveEtrans start comms1 etrans in
  --let returns =  in
  --let ledger = Ledger comms etrans financials ntrans naccs period quotes returns
  Ledger
         { comms = comms1
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
    ds = etranDstamp e
    ticker = findTicker comms (etranSym e)
    amount = unPennies $ etranAmount e
    qty = etranQty e
    price = 100.0 * amount / qty

            
synthSQuotes :: [Comm] -> [Etran] -> [StockQuote] -- create synthetic stock quotes
synthSQuotes comms etrans =  map  (etranToSQuote comms)  etrans


