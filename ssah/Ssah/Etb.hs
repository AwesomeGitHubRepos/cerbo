module Etb where

--import Control.Monad.IfElse
import Data.Either
import Data.Function (on)
import Data.List
import Data.Maybe
import Data.Ord
--import Data.Set (fromList)
--import Data.String.Utils
import GHC.Exts
import System.IO
import Text.Printf

import Aggregate
import Comm
import Config
import Dps
import Epics
import Etran
import Financial
import Cgt
import Ledger
import Nacc
import Ntran
import Portfolio
import Post
import Returns
import Snap
--import Ssah
import Utils
import Yahoo


data Option = PrinAccs | PrinCgt | PrinDpss | PrinEpics | PrinEtb | PrinEtrans
            | PrinFin | PrinPorts | PrinReturns | PrinSnap deriving (Eq)

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

--reportAccs :: Foldable t => t ([Char], Maybe Nacc, [Post]) -> [[Char]]
reportAccs grp =
  ["ACCS:"] ++ accs ++ ["."]
  where
    accs = concatMap printEtbAcc grp
  
assemblePosts :: [Nacc] -> [Post] -> [(Acc, Maybe Nacc, [Post])]
assemblePosts naccs posts =
  zip3 keys keyedNaccs keyPosts
  where
    sPosts = (sortOnMc postDstamp posts)
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
  eLines ++  [totalLine]
  where
    sorted = sortOnMc fst etb
    eLine (acc, pennies) = (psr 6 acc) ++ (show pennies)
    eLines = map eLine sorted
    total  = countPennies $ map snd sorted
    totalLine = eLine ("TOTAL", total)



mkReports  ledger options = do
  let theComms = comms ledger
  let theEtrans = etrans ledger
  let posts = createPostings (ntrans ledger) theEtrans      

  let grp = assemblePosts (naccs ledger) posts -- FIXME LOW put into order
  let etb = assembleEtb grp
  let asxNow = commEndPriceOrDie theComms "FTAS"
  let createdReturns = createReturns (end ledger) theEtrans asxNow (returns ledger)

  let mkRep (title, option, lines) = (title, if elem option options then unlines lines else "")
  let reps = map mkRep [
        ("accs",       PrinAccs,    reportAccs grp) ,
        ("cgt",        PrinCgt,     createCgtReport theEtrans),
        ("dpss",       PrinDpss,    createDpssReport theComms theEtrans (dpss ledger) ), 
        ("epics",      PrinEpics,   reportEpics theComms  theEtrans) ,
        ("etb",        PrinEtb,     createEtbReport etb) ,
        ("etrans",     PrinEtrans,  createEtranReport theEtrans),
        ("financials", PrinFin,     createFinancials etb (financials ledger)),
        ("portfolios", PrinPorts,   createPortfolios theEtrans theComms),
        ("returns",    PrinReturns, createdReturns)]
  return reps

-- mkAllReports = mkReports optionSet0

createSingleReport dtStamp reps = do
  let single (title, body) = (upperCase title) ++ ":\n"  ++ body ++ "."
  let outStr = unlines $ map single  reps
  putStrLn dtStamp
  putStrLn outStr
  putStrLn "+ OK Finished"
  f <- outFile "hssa.txt"
  writeFile f outStr

fileReport :: String -> (String, String) -> IO ()
fileReport dtStamp (title, body) = do
  f <- outFile (fileSep ++ "text" ++ fileSep ++ title ++ ".txt")
  writeFile f (dtStamp ++ "\n\n" ++ body)
  -- putStr ""
  --return ()

fileReports :: String -> [(String, String)] -> IO ()
fileReports _ [] = putStr ""
fileReports dtStamp (x:xs) = do
  fileReport dtStamp x
  fileReports dtStamp xs


freshQuotes :: Ledger -> Bool -> IO [Either String StockQuote]
freshQuotes ledger downloading = 
  if downloading then precacheCommsUsing True (comms ledger) else return ([])


createEtbDoing  options downloading = do
  ledger <- ratl
  --quotes1 <- snapDownloading (comms ledger) True downloading
  (errs, quotes1) <- fmap partitionEithers $ freshQuotes ledger downloading -- FIXME handle errs
  let quotes2 = (squotes ledger) ++ quotes1
  let ledger1 = ledger { squotes = quotes2 }
  reps <- mkReports ledger1 options
  dtStamp <- nowStr
  createSingleReport dtStamp reps
  fileReports dtStamp reps



--createEtbDoing options = return ()

optionSet0 = [PrinAccs,  PrinCgt, PrinDpss, PrinEpics, PrinEtb, PrinEtrans, PrinFin, PrinPorts, PrinReturns]
optionSet1 = [PrinDpss]
optionSet2 = [PrinReturns]
optionSetX = [PrinEtb]

createSection opt = createEtbDoing [opt] -- e.g. createSection PrinReturns

createCgt = createSection PrinCgt

webYes = True
webNo = False

createEtb = createEtbDoing optionSet0 webNo
mainEtb =  createEtbDoing optionSet0
