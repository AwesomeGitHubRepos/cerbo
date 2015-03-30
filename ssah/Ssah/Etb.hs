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


data Option = PrinAccs | PrinFin deriving (Eq)

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
  text
  where
    --dr = postDr $ head posts
    n = case nacc of
      Just x -> x
      Nothing -> error ("Couldn't locate account:" ++ dr)
    runningTotals = cumPennies $ map postPennies posts
    hdr = (showNacc n) ++ "\n"
    body = map2 etbLine posts runningTotals    
    text = hdr ++ (unlines body) ++ "\n"


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
  lup ++ augs
  where
    summate (a, n, posts) = (a, countPennies (map postPennies posts))
    lup = map summate es
    augs = augEtb lup
 
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

  let accLines = concatMap printEtbAcc grp
  putStr $ if (elem PrinAccs options) then accLines else ""
  --printPostsByNaccs postsByNaccs
  let etb = assembleEtb grp
  printAll etb
  --storeEtb etb --FIXME LOW

  putStrLn $ createEtranReport derivedEtrans
  
  --print etb
  let finStatements = createFinancials etb financials
  putAll $ if (elem PrinFin options)  then finStatements else []

  let portfolios = createPortfolios etb derivedComms
  putAll portfolios

  --let ftas = findComm comms "FTAS"
  let asxNow = commEndPriceOrDie derivedComms "FTAS"
  let createdReturns = createReturns end etb asxNow returns
  putStrLn createdReturns

  let epicReport = reportEpics derivedComms derivedEtrans
  putStrLn epicReport
  
  putStrLn "+ OK Finished"


optionSet1 = [PrinAccs,  PrinFin]
optionSet2 = [PrinFin]

createEtb = createEtbDoing optionSet2
mainEtb = createEtb
