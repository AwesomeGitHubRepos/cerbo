module Ntran where

import Data.Tuple.Select

import Utils

data Ntran = Ntran Dstamp Acc Acc Pennies String String deriving (Show)

mkNtran :: [String] -> Ntran
mkNtran ["ntran", dstamp, dr, cr, pennies, clear, desc] =
  Ntran dstamp dr cr (asPennies pennies) clear desc

getNtrans = makeTypes mkNtran "ntran"

ntranTuple (Ntran dstamp dr cr pennies clear desc) =
  (dstamp, dr, cr, pennies, clear, desc)

ntranDstamp :: Ntran -> Dstamp
ntranDstamp ntran = sel1 $ ntranTuple ntran

ntranDr :: Ntran -> Acc
ntranDr ntran = sel2 $ ntranTuple ntran

ntranCr :: Ntran -> Acc
ntranCr ntran = sel3 $ ntranTuple ntran

ntranP :: Ntran -> Pennies
ntranP  ntran = sel4 $ ntranTuple ntran
