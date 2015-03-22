module Ssah.Etb where

import Ssah.Aggregate
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo

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
 
testSynthSQuotes = do
  ledger <- readLedger
  let comms = ledgerComms ledger
  let etrans = ledgerEtrans ledger
  let sqs = synthSQuotes comms etrans
  printAll sqs
  print "FIXME NOW"
  
showEtbAcc nacc ntrans =
  unlines lines 
  where
    lines = [show nacc ] ++ (map show ntrans) ++ ["\n\n"]


--createEtb :: Ledger
createEtb  = do
  ledger <- readLedger
  let naccs  = ledgerNaccs ledger
  let ntrans1 = ledgerNtrans ledger
  let opp (Ntran dstamp dr cr pennies clear desc) =
        Ntran dstamp cr dr (negPennies pennies) clear desc
  let ntrans2 = map opp ntrans1
  let ntrans = ntrans1 ++ ntrans2
  let naccNtrans = strictlyCombineKeys naccAcc ntranDr naccs ntrans
  let blocks = map2 showEtbAcc naccs naccNtrans
  putStrLn (unlines blocks)
  print "FIXME NOW"

