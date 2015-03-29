module Ssah.Post where

import Data.List
import Data.Ord
import Text.Printf
import Data.Tuple.Select

import Ssah.Aggregate
import Ssah.Etran
import Ssah.Ntran
import Ssah.Ssah
import Ssah.Utils

data Post = Post Dstamp Acc Acc Pennies Desc deriving (Show)

postTuple (Post dstamp dr cr pennies desc) =
  (dstamp, dr, cr, pennies, desc)


postDstamp p = sel1 $ postTuple p

postDr :: Post -> Acc
postDr p = sel2 $ postTuple p

postCr :: Post -> Acc
postCr p = sel3 $ postTuple p
postPennies p = sel4 $ postTuple p
postDesc p = sel5 $ postTuple p

postingsFromNtran ntran =
  [n1, n2]
  where
    (dstamp, dr, cr, pennies, _, desc) = ntranTuple ntran
    n1 = Post dstamp dr cr pennies desc
    n2 = Post dstamp cr dr (negp pennies) desc
    
postingsFromNtrans  ntrans = concatMap postingsFromNtran ntrans
  
postingsFromEtran etran =
  [n1, n2, n3, n4]
  where
    odstamp = etranOdstamp etran
    folio = etranFolio etran
    sym = etranSym etran
    opn = folio ++ "/b" --"opn"
    pga = folio ++ "/g" --"pga"
    prt = folio ++ "/c" --"prt"
    n1 = Post odstamp opn pga (negp $ etranStartValue etran) sym
    n2 = Post odstamp (etranFolio etran) pga (negp $ etranFlow etran) sym
    n3 = Post odstamp pga pga (negp $ etranProfit etran) sym -- FIXME - how can they be the same account ??
    n4 = Post odstamp prt pga (etranEndValue etran) sym

postingsFromEtrans etrans =
  concatMap postingsFromEtran etrans

testPostings = do -- won't work because it doesn't do any derivations
  ledger <- readLedger
  let etrans = ledgerEtrans ledger
  let posts = postingsFromEtrans etrans
  printAll posts

--createPostings :: [Ntran] -> [Etran] -> [Post]
createPostings ntrans derivedEtrans =
  postings
  where
    ntranPostings = postingsFromNtrans ntrans
    --derivedEtrans = deriveEtrans start comms etrans
    etranPostings = postingsFromEtrans derivedEtrans
    unsortedPostings = ntranPostings ++ etranPostings
    postings = sortBy (comparing $ postDstamp) unsortedPostings
    

--showPost :: Post -> String
showPost p =
  let (dstamp, dr, cr, pennies, desc) = postTuple p in
  printf "%s %4.4s %4.4s %20.20s %s" dstamp dr cr desc (show pennies) 

-- | Aggregate posts into account and date order
aggPosts :: [Post] -> [[Post]]
aggPosts posts =
  res
  where
    accOrder = sortBy (comparing $ postDr) posts
    grp = groupOn postDr accOrder
    sortSubGroup = sortOn postDstamp -- (comparing $ postDstamp)
    res = map sortSubGroup grp
