module Ssah.Snap  where

import Data.Tuple.Select
import Text.Printf

--import Ssah.Ntran
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo


-- FIXME LOW Handle cases of etrans not in comms

totalQty ::  [Etran] -> Comm -> Qty
totalQty etrans comm =
  qtys commEtrans
  where
    hit e = (commSym comm) == (etranSym e)
    commEtrans = filter hit  etrans


snapFmt = "%8s %9.2f %8.2f %6.2f"

mkSnapLine :: (StockQuote, Qty) -> (String, Float, Float)
mkSnapLine (sq, qty) =
  (str, amount, chg1)
  where
    (_, _, ticker, _, price, chg, chgpc) = quoteTuple sq
    amount = price * qty / 100.0
    chg1 = chg * qty / 100.0
    str = printf snapFmt ticker amount chg1 chgpc    

snap1 :: [Comm] -> [Etran] -> IO ()
snap1 comms etrans = do
  let qtys = map (totalQty etrans) comms
  let pairs = zip comms qtys
  let hit (c,q) = ctc == "INDX" || (ctc == "YAFI" && q > 0.0)
        where ctc = commType c
  let hits = filter hit pairs
  --let tickers = map commTickers
  --printAll hits
  let (hitComms, hitQty) = unzip hits
  let tickers = map yepic hitComms
  usd <- fetchUsd
  let roxs = map (rox usd) hitComms
  --print $ length tickers
  --print $ length roxs
  hitQuotes <- fetchQuotesA tickers roxs
  let results = map mkSnapLine  (zip hitQuotes hitQty)
  let (lines, amounts, changes) = unzip3 results
  putStrLn (unlines lines)
  let tAmounts = sum amounts
  let tChanges = sum changes
  let tPc = tChanges / (tAmounts - tChanges) * 100.0
  let tLine = printf snapFmt "TOTAL" tAmounts tChanges tPc
  putStrLn tLine
      

snapAll :: IO ()
snapAll = do
  led <- readLedger
  let comms = ledgerComms led
  let etrans = ledgerEtrans led
  snap1 comms etrans

snap = snapAll

hsnap = snap

