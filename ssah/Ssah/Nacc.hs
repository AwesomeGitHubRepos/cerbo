module Ssah.Nacc where

import Data.Tuple.Select

import Ssah.Utils

data Nacc = Nacc Acc String deriving (Show)

mkNacc :: [String] -> Nacc
mkNacc ["nacc", acc, desc] =
  Nacc acc desc

getNaccs = makeTypes mkNacc "nacc"

naccTuple (Nacc acc desc) =
  (acc, desc)

naccAcc :: Nacc -> Acc
naccAcc nacc = sel1 $ naccTuple nacc

naccDesc :: Nacc -> String
naccDesc nacc = sel2 $ naccTuple nacc
