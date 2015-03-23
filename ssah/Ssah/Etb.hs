module Ssah.Etb where

import Data.Function (on)
import Data.List
import Data.Maybe
import Data.Ord
import GHC.Exts
import Text.Printf

import Ssah.Aggregate
--import Ssah.Flow -- FIXME Schedule module for destruction
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Post
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo


showEtbAcc :: Nacc -> [Ntran] -> Maybe (String, Nacc, Pennies)
showEtbAcc nacc ntrans =
  if (length ntrans > 0) then Just (unlines lines, nacc, last runningTotal) else Nothing
  where
    sortedNtrans = sortBy (comparing  ntranDstamp) ntrans
    pennies = map ntranP sortedNtrans
    runningTotal = cumPennies pennies
    display ntran tot =
      printf "%s %4s %-25.25s %s %s" dstamp cr desc (show pennies) (show tot)
      where (dstamp, dr, cr, pennies, clear, desc) = ntranTuple ntran

    ntranLines = map2 display sortedNtrans runningTotal
    lines = [show nacc ] ++ ntranLines ++ ["\n\n"]

{-
etbLine :: Nacc -> Pennies -> String
etbLine nacc total =
  printf "%4s %20.20s %s" acc desc totalStr
  where
    acc = (naccAcc nacc)::String
    desc = (naccDesc nacc)::String
    totalStr = (show total)::String
-}

etbLine :: Post -> Pennies -> String
etbLine post runningTotal = (showPost post) ++ (show runningTotal)
 
printEtbAcc naccs posts = 
  text
  where
    dr = postDr $ head posts
    nacc = fromJust $ find (\n -> dr == (naccAcc n)) naccs
    runningTotals = cumPennies $ map postPennies posts
    hdr = (showNacc nacc) ++ "\n"
    body = map2 etbLine posts runningTotals    
    text = hdr ++ (unlines body) ++ "\n"

  --print nacc


  
--createEtb :: Ledger
createEtb  = do
  ledger <- readLedger
  let naccs  = ledgerNaccs ledger
  let ntrans = ledgerNtrans ledger
  let (start, end) = ledgerPeriod ledger
  let posts = postingsFromNtrans ntrans
  let reordPosts = sortBy (comparing $ postDr) posts
  let grps = groupBy ((==) `on`  postDr) reordPosts
  let pea = printEtbAcc naccs
  putAll (map pea grps)

  
  --printAll posts
  --let flows = genFlows ledger
  -- FIXME ignore transactions after period end
  --let flows = generateFlows ledger
      {-
  let remap (Ntran dstamp dr cr pennies clear desc) =
        (n1, n2)
        where
          (drA, crA) = if dstamp < start then (alt dr naccs, alt cr naccs) else (dr, cr)
          n1 = Ntran dstamp drA crA pennies clear desc
          n2 = Ntran dstamp crA drA (negPennies pennies) clear desc
  let (ntrans1, ntrans2) = unzip $ map remap (ntrans ++ flows)
  let ntrans3 = ntrans1 ++ ntrans2
  let naccNtrans = strictlyCombineKeys naccAcc ntranDr naccs ntrans3
  let maybeAccts = map2 showEtbAcc naccs naccNtrans
  let accts = mapMaybe id maybeAccts
  let (blocks, properNaccs, balances) = unzip3 accts
  putStrLn (unlines blocks)
  -- putStrLn (unlines (map prettyFlow flows))


  
  let etb = map2 etbLine properNaccs balances
  printAll etb
  print "FIXME NOW"
-}
      
mainEtb = createEtb
