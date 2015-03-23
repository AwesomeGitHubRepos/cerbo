module Ssah.Flow where

import Data.Maybe
import Text.Printf

import Ssah.Ntran
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo


data Flow = Flow String Acc Ticker Pennies Pennies Pennies Pennies Pennies Pennies
flowTuple (Flow folio sym ticker costBefore profitBefore valueStart flowDuring profitDuring valueTo)
  = (folio, sym, ticker, costBefore, profitBefore, valueStart, flowDuring, profitDuring, valueTo)

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
 
testSynth = do
  ledger <- readLedger
  let comms = ledgerComms ledger
  let etrans = ledgerEtrans ledger
  let sqs = synthSQuotes comms etrans
  printAll sqs


genTrip :: Dstamp -> Dstamp -> [Comm] -> [StockQuote] -> Etran -> (Maybe Ntran, Maybe Ntran, Maybe Ntran)
genTrip start end  comms quotes etran =
  (Just n1,  n2, Just n3)
  where
    --(comms, etrans, ntrans, naccs, period, realQuotes) = ledgerTuple ledger
    --(start, end) = period
    (dstamp, way, acc, sym, qty, flow) = etranTuple etran
    eshow = printf "%s: %f" sym qty -- show etran
    --flow = enPennies amount
    n1 = Ntran dstamp "pfl" acc flow "C" eshow -- FIXME generalise "pga"

    ticker = findTicker comms (etranSym etran)
    startPrice = if dstamp < start then getStockQuote start ticker quotes else 0.0
    startAmount = enPennies (startPrice * 0.01 * qty)
    startGain = if dstamp < start then (startAmount |-| flow) else Pennies 0
    n2 = if dstamp < start then Just (Ntran dstamp "pop" "pgb" startGain "C" eshow) else Nothing

    endPrice = getStockQuote end ticker quotes
    endAmount = enPennies (endPrice * 0.01 * qty)
    currGain = endAmount |-| flow |-| startGain
    n3 = Ntran dstamp "prt" "pga" currGain "C" eshow 

    
--adjustEtrans :: Ledger -> [Ntran]
genTrips ledger =
  -- see data.c line c503, postings 70
  trips
  where
    (comms, etrans, ntrans, naccs, period, realQuotes) = ledgerTuple ledger
    (start, end) = period
    synthQuotes = synthSQuotes comms etrans
    quotes = realQuotes ++ synthQuotes
    etransOk = filter (\x -> etranDstamp x <= end) etrans
    trips = map (genTrip start end comms quotes) etransOk
{-
    flow e =
      (e, vbefore, flowDuring, profit, vend)
      where
        prior = (etranDstamp e) < start
        qty = etranQty e
        ticker = findTicker comms (etranSym e)
        vbefore = if prior then enPennies (0.01 * qty * (getStockQuote start ticker quotes)) else (Pennies 0)
        flowDuring = if prior then (Pennies 0) else (etranAmount e)
        vend = enPennies (0.01 * qty * (getStockQuote end ticker quotes))
        profit = vend |-| (vbefore |+| flowDuring)
        
    flows = map flow etransOk
 

prettyFlow (etran, vbefore, flowDuring, profit, vend) =
  unlines block
  where
    e = show etran
    b = "Vbefore: " ++ (show vbefore)
    d = "Flow:    " ++ (show flowDuring)
    p = "Profit:  " ++ (show profit)
    x = "Vend:    " ++ (show vend)
    block = [e, b, d, p, x, ""]

-}

genFlows ledger =
  valids
  where
    trips = genTrips ledger
    (trip1, trip2, trip3) = unzip3 trips
    oner = trip1 ++ trip2 ++ trip3
    valids = mapMaybe id oner
    
testFlows = do -- FIXME NOW
  ledger <- readLedger
  let valids = genFlows ledger
  --let pretties = map prettyFlow adj
  --  let oner = concat pretties
  --let oner = "FIXME NOW"
  --putStrLn oner
  printAll valids
