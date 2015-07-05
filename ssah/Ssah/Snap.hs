module Snap  where

import Data.Either
import Data.Function
import Data.List
import Data.Ord
import Data.Tuple.Select
import GHC.Exts
import Text.Printf

--import Ssah.Ntran
import Aggregate
import Comm
import Etran
import Ledger
import Ssah
import Utils
import Yahoo


-- FIXME LOW Handle cases of etrans not in comms

totalQty ::  [Etran] -> Comm -> Qty
totalQty etrans comm =
  qtys commEtrans
  where
    hit e = (commSym comm) == (etSym e)
    commEtrans = filter hit  etrans


snapFmt = "%8s %9.2f %8.2f %6.2f"

mkSnapLine :: (StockQuote, Qty) -> (String, Double, Double)
mkSnapLine (sq, qty) =
  (str, amount, chg1)
  where
    StockQuote _ _ ticker _ price chg chgpc = sq
    amount = price * qty / 100.0
    chg1 = chg * qty / 100.0
    str = printf snapFmt ticker amount chg1 chgpc    


-- | False => use cached version, True => download values afresh
snapDownloading :: Bool -> Bool -> IO ()
snapDownloading concurrently afresh = do
  ds <- dateString
  ts <- timeString
  let header = ds ++ " " ++ ts
  putStrLn header
  led <- readLedger
  let theComms = comms led
  let theEtrans = etrans led
  pres <- fmap partitionEithers $ precacheCommsUsing concurrently theComms
  loaded <- loadPrecachedComms
  let (errs, fetchedQuotes) = if afresh
                              then  pres
                              else ([], loaded)
  
  let fetchableComms = filter fetchRequired theComms

  let sortedEtrans = sortBy (comparing $ etSym) theEtrans
  --let grpEtrans  = groupByKey etranSym etrans
  let grpEtrans = groupBy (\x y -> (etSym x) == (etSym y)) sortedEtrans
  --let grpEtrans = groupBy (compare `on` etranSym) etrans
  let agg etrans =
        (sym , qty, want, price, amount, profit, chgpc, oops)
        where
          qty = qtys etrans
          sym = etSym $ head etrans
          comm = find (\c -> commSym c == sym) theComms
          ctype = fmap commType  comm
          ticker = fmap commTicker comm
          msq = find (\s -> Just (sqTicker s) == ticker) fetchedQuotes
          (price, chg, chgpc, oops) = case msq of
            Just s -> (sqPrice s, sqChg s, sqChgpc s, "")
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
          s1 = printf "%5s %12.2f " (sym::String) (qty::Double)
          s2 = printf "%12.2f %12.2f "  (price::Double) (amount::Double)
          s3 = printf "%12.2f %5.2f %s" (profit::Double) (chgpc::Double) (oops::String)


  let lines2 = map texy etrans2
  --printAll lines2
  mapM_ putStrLn lines2

  let index idx = case (find (\q -> idx == sqTicker q) fetchedQuotes) of
        Just sq -> texy (idx, 0.0, True, 0.0, (sqPrice sq), (sqChg sq), (sqChgpc sq), "")
        Nothing -> idx ++ " not found"

  --purStrLn (map index ["^FTAS", "
  mapM_ (putStrLn . index) ["^FTSE", "^FTAS", "^FTMC"]
  --putStrLn index "
  putStrLn "\n---\n\n"
  print errs


snap1 = snapDownloading True True

snap2 = snapDownloading True False

snapSlow = snapDownloading False True -- download syms one at a time (slow for debugging)

hsnap = snap1

