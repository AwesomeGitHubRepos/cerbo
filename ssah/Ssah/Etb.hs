module Ssah.Etb where

import Data.Function (on)
import Data.List
import Data.Maybe
import Data.Ord
import Data.String.Utils
import GHC.Exts
import System.IO
import Text.Printf

import Ssah.Aggregate
import Ssah.Comm
import Ssah.Financial
import Ssah.Flow 
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Post
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo

makeEtbField totalTab nacc =
  unspaced
  where
    sym = naccAcc nacc
    entry = lookup sym  totalTab
    total = fromMaybe (Pennies 0) entry
    pounds = unPennies total
    p = round $ pounds * 100
    text1 = sym ++ "!" ++ (show p) ++ "!" ++ (show total)
    unspaced = replace " " "" text1
  
makeEtbFields totalTab naccs = unlines $ map (makeEtbField totalTab) naccs

storeEtb totalTab naccs = do
  let text = makeEtbFields totalTab naccs
  writeFile "/home/mcarter/.ssa/hssa-etb.txt" text

  

etbLine :: Post -> Pennies -> String
etbLine post runningTotal = (showPost post) ++ (show runningTotal)
 
printEtbAcc naccs posts = 
  text
  where
    dr = postDr $ head posts
    nacc = case find (\n -> dr == (naccAcc n)) naccs of
      Just n -> n
      Nothing -> error ("Couldn't locate account:" ++ dr)
    runningTotals = cumPennies $ map postPennies posts
    hdr = (showNacc nacc) ++ "\n"
    body = map2 etbLine posts runningTotals    
    text = hdr ++ (unlines body) ++ "\n"

  --print nacc


-- FIXME ignore transactions after period end  
--createEtb :: Ledger
createEtb  = do
  ledger <- readLedger
  let (comms, etrans, financials, ntrans, naccs, period, quotes) = ledgerTuple ledger
  let (start, end) = period
  let derivedQuotes = synthSQuotes comms etrans
  let allQuotes = quotes ++ derivedQuotes
  let derivedComms = deriveComms start end allQuotes comms
  let posts = createPostings start derivedComms ntrans etrans
  let reordPosts = sortBy (comparing $ postDr) posts

      
  let grps = groupBy ((==) `on`  postDr) reordPosts
  let tabulateGroup grp =
        (acc, bal)
        where
          acc = postDr $ head grp
          pennies = map postPennies grp
          bal = countPennies pennies
        
        
  let etbTab = map tabulateGroup grps
      
  let pea = printEtbAcc naccs
  let detailOutput = concatMap pea grps


  -- now create etb -- FIXME would be improved by using etbTab, which has already processed a lot
  let pennies = map (map postPennies) grps
  let pennyTots = map countPennies pennies
  --let theNacc grp = find (postDr $ head grp
  let summaryLine grp tot = (show $ head grp) ++ (show tot)
  let summaryLines = map2 summaryLine grps pennyTots
  let etbOut = unlines summaryLines

  let output = detailOutput ++ "\n\n" ++ etbOut
  writeFile "/home/mcarter/.ssa/hssa-etb.txt" output
  putStrLn output

  storeEtb etbTab naccs
  --print $ head grps
  --printAll etbTab
  --print posts
  print "Financials"
  let fins = createFinances financials
  printAll fins
  --printAll pennyTots
  printAll etbTab -- need to pass this into createFinances
  putStrLn "Finished"
      
mainEtb = createEtb
