module Ssah.Snap  where

import Data.Either
import Data.Function
import Data.List
import Data.Ord
import Data.Tuple.Select
import GHC.Exts
import Text.Printf

--import Ssah.Ntran
import Ssah.Aggregate
import Ssah.Comm
import Ssah.Etran
import Ssah.Ledger
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


-- | False => use cached version, True => download values afresh
snapDownloading :: Bool -> IO ()
snapDownloading afresh = do
  ds <- dateString
  ts <- timeString
  let header = ds ++ " " ++ ts
  putStrLn header
  led <- readLedger
  let theComms = comms led
  let theEtrans = etrans led
  pres <- fmap partitionEithers $ precacheCommsUsing theComms
  loaded <- loadPrecachedComms
  let (errs, fetchedQuotes) = if afresh
                              then  pres
                              else ([], loaded)
  
  let fetchableComms = filter fetchRequired theComms

  let sortedEtrans = sortBy (comparing $ etranSym) theEtrans
  --let grpEtrans  = groupByKey etranSym etrans
  let grpEtrans = groupBy (\x y -> (etranSym x) == (etranSym y)) sortedEtrans
  --let grpEtrans = groupBy (compare `on` etranSym) etrans
  let agg etrans =
        (sym , qty, want, price, amount, profit, chgpc, oops)
        where
          qty = qtys etrans
          sym = etranSym $ head etrans
          comm = find (\c -> commSym c == sym) theComms
          ctype = fmap commType  comm
          ticker = fmap commTicker comm
          msq = find (\s -> Just (quoteTicker s) == ticker) fetchedQuotes
          (price, chg, chgpc, oops) = case msq of
            Just s -> (quotePrice s, quoteChg s, quoteChgPc s, "")
            Nothing -> (0.0, 0.0, 0.0, "* ERR")
          --sq (Nothing) = error ("Can't lookup StockQuote for sym" ++ sym)
          --sq (Just msq) = msq
          want = qty > 0 && (ctype == Just "YAFI")
          amount = qty * price / 100.0
          profit = qty * chg  /100.0

  let aggEtrans = map agg  grpEtrans
  let hitEtrans = filter sel3 aggEtrans
  let etrans1 = sortBy (comparing $ sel1) hitEtrans
  let tAmount = sum $ map sel5 etrans1
  let tProfit = sum $ map sel6 etrans1
  let tPc = tProfit/(tAmount - tProfit) * 100.0
  let etrans2 = etrans1 ++ [ ("TOTAL", 0.0, True, 0.0, tAmount, tProfit, tPc, "")]
  let texy (sym, qty, want, price, amount, profit, chgpc, oops) =
        s1 ++ s2 ++ s3
        where
          s1 = printf "%5s %12.2f " (sym::String) (qty::Float)
          s2 = printf "%12.2f %12.2f "  (price::Float) (amount::Float)
          s3 = printf "%12.2f %5.2f %s" (profit::Float) (chgpc::Float) (oops::String)


  let lines2 = map texy etrans2
  --printAll lines2
  mapM_ putStrLn lines2

  let index idx = case (find (\q -> idx == quoteTicker q) fetchedQuotes) of
        Just sq -> texy (idx, 0.0, True, 0.0, (quotePrice sq), (quoteChg sq), (quoteChgPc sq), "")
        Nothing -> idx ++ " not found"

  --purStrLn (map index ["^FTAS", "
  mapM_ (putStrLn . index) ["^FTSE", "^FTAS", "^FTMC"]
  --putStrLn index "
  putStrLn "\n---\n\n"
  print errs


snap1 = snapDownloading True

snap2 = snapDownloading False

hsnap = snap1

