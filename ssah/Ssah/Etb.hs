module Ssah.Etb where

import Ssah.Aggregate
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Ssah
import Ssah.Utils

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
        Ntran dstamp cr dr (-pennies) clear desc
  let ntrans2 = map opp ntrans1
  let ntrans = ntrans1 ++ ntrans2
  let naccNtrans = combineKeysStrict naccAcc ntranDr naccs ntrans
  let blocks = map2 showEtbAcc naccs naccNtrans
  putStrLn (unlines blocks)
  print "FIXME NOW"

