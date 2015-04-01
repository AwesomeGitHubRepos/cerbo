module Ssah.Etb where

import Control.Monad.IfElse
import Data.Function (on)
import Data.List
import Data.Maybe
import Data.Ord
--import Data.Set (fromList)
--import Data.String.Utils
import GHC.Exts
import System.IO
import Text.Printf

import Ssah.Aggregate
import Ssah.Comm
import Ssah.Epics
import Ssah.Etran
import Ssah.Financial
import Ssah.Flow 
import Ssah.Nacc
import Ssah.Ntran
import Ssah.Portfolio
import Ssah.Post
import Ssah.Returns
import Ssah.Ssah
import Ssah.Utils
import Ssah.Yahoo


data Option = PrinAccs | PrinEpics | PrinEtb | PrinEtrans
            | PrinFin | PrinPorts | PrinReturns deriving (Eq)

augEtb:: Etb -> Etb
augEtb etb =
  res
  where
    --getpe = getp etb
    --sumAccs' = sumAccs etb
    etb1 = sumAccs etb "inc" ["div", "int", "wag"]
    etb2 = sumAccs etb1 "exp" ["amz", "car", "chr", "cmp", "hol", "isp", "msc", "mum", "tax"]
    etb3 = sumAccs etb2 "ioe" ["inc", "exp"] -- income over expenditure
    etb3_1 = sumAccs etb3 "mine/g" ["hal/g", "hl/g", "tdi/g", "tdn/g"]
    etb4 = sumAccs etb3_1 "gain" ["mine/g", "ut/g"]
    etb5 = sumAccs etb4 "net" ["ioe", "gain"]
    etb5_1 = sumAccs etb5 "mine/b" ["hal/b", "hl/b", "tdi/b", "tdn/b"]
    etb6 = sumAccs etb5_1 "open" ["opn", "mine/b", "ut/b"]
    etb7 = sumAccs etb6 "cd1" ["net", "open"]
    etb8 = sumAccs etb7 "cash" ["hal", "hl", "ut", "rbs", "rbd", "sus", "tdi", "tdn", "tds", "vis"]
    etb8_1 = sumAccs etb8 "mine/c" ["hal/c", "hl/c", "tdi/c", "tdn/c"]
    etb9 = sumAccs etb8_1 "port" ["mine/c", "ut/c"]
    etb10 = sumAccs etb9 "nass" ["cash", "msa", "port"]
    res = etb10
    

etbLine :: Post -> Pennies -> String
etbLine post runningTotal = (showPost post) ++ (show runningTotal)
 
printEtbAcc (dr, nacc, posts) = 
  textLines
  where
    n = case nacc of -- FIXME LOW Can use an OrDie function
      Just x -> x
      Nothing -> error ("Couldn't locate account:" ++ dr)
    runningTotals = cumPennies $ map postPennies posts
    (acc, _, desc) = naccTuple n
    accHdr = "Acc: " ++ acc
    body = map2 etbLine posts runningTotals    
    textLines = [accHdr, desc] ++ body ++ [";"]


reportAccs grp =
  ["ACCS:"] ++ accs ++ ["."]
  where
    accs = concatMap printEtbAcc grp
  
assemblePosts :: [Nacc] -> [Post] -> [(Acc, Maybe Nacc, [Post])]
assemblePosts naccs posts =
  zip3 keys keyedNaccs keyPosts
  where
    sPosts = (sortOn postDstamp posts)
    keys = uniq $ map postDr sPosts
    keyedNaccs = map (\k -> find (\n -> k == (naccAcc n)) naccs) keys
    keyPosts = map (\k -> filter (\p -> k == (postDr  p)) sPosts) keys
    

assembleEtb :: [(Acc, Maybe Nacc, [Post])] -> [(Acc,  Pennies)]
assembleEtb es =
  augs
  where
    summate (a, n, posts) = (a, countPennies (map postPennies posts))
    lup = map summate es
    augs = augEtb lup

createEtbReport etb =
  ["ETB:"] ++  eLines ++  [totalLine, "."]
  where
    sorted = sortOn fst etb
    eLine (acc, pennies) = (psr 6 acc) ++ (show pennies)
    eLines = map eLine sorted
    total  = countPennies $ map snd sorted
    totalLine = eLine ("TOTAL", total)

    
-- FIXME ignore transactions after period end  
--createEtb :: Ledger
createEtbDoing  options = do
  ledger <- readLedger
  let (comms, etrans, financials, ntrans, naccs, period, quotes, returns) = ledgerTuple ledger
  let (start, end) = period
  let derivedQuotes = synthSQuotes comms etrans
  let allQuotes = quotes ++ derivedQuotes
  let derivedComms = deriveComms start end allQuotes comms
  let derivedEtrans = deriveEtrans start derivedComms etrans
  let posts = createPostings ntrans derivedEtrans
      

  let grp = assemblePosts naccs posts -- FIXME LOW put into order
  --printAll grp

  let putSection sec lines = putStrLn $ if (elem sec options) then lines else ""
  let printSection sec str = putSection sec $ unlines str
      
  --printSection PrinAccs $ concatMap printEtbAcc grp
  printSection PrinAccs $ reportAccs grp

  -- let epicReport = reportEpics derivedComms derivedEtrans
  putSection PrinEpics $ reportEpics derivedComms derivedEtrans
  
  let etb = assembleEtb grp
  printSection PrinEtb $ createEtbReport etb
  --storeEtb etb --FIXME LOW

  putSection PrinEtrans $ createEtranReport derivedEtrans
  
  --print etb
  -- let finStatements = createFinancials etb financials
  printSection PrinFin $ createFinancials etb financials

  --let portfolios = createPortfolios etb derivedComms
  printSection PrinPorts $ createPortfolios etb derivedComms

  --let ftas = findComm comms "FTAS"
  let asxNow = commEndPriceOrDie derivedComms "FTAS"
  let createdReturns = createReturns end etb asxNow returns
  putSection PrinReturns createdReturns



  putStrLn "+ OK Finished"


optionSet0 = [PrinAccs,  PrinEpics, PrinEtb, PrinEtrans, PrinFin, PrinPorts, PrinReturns]
optionSet2 = [PrinFin]
optionSetX = [PrinEtb]

createEtb = createEtbDoing optionSet0
mainEtb = createEtb
