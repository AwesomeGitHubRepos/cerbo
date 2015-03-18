module Ssah.Snap where

import Data.Tuple.Select

import Ssah.Ssah
import Ssah.Utils


-- FIXME LOW Handle cases of etrans not in comms

totalQty ::  [Etran] -> Comm -> Qty
totalQty etrans comm =
  qtys commEtrans
  where
    hit e = (commSym comm) == (etranSym e)
    commEtrans = filter hit  etrans

snap :: [Comm] -> [Etran] -> IO ()
snap comms etrans = do
  let qtys = map (totalQty etrans) comms
  let pairs = zip comms qtys
  let hit (c,q) = (commType c) == "OEIC" || q > 0.0
  let hits = filter hit pairs
  --let tickers = map commTickers
  print hits
  print "FIXME NOW - finishe this off. It's quite advanced"
  
snapAll = do
  inputs <- readInputs
  let comms = getComms inputs
  let etrans = getEtrans inputs
  snap comms etrans
