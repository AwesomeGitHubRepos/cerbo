module Nacc where

import Data.List
import Data.Tuple.Select
import Text.Printf

import Utils

data Nacc = Nacc Acc Acc String deriving (Show)

-- | alt is the alternative account to use if the transaction is before the start date
mkNacc :: [String] -> Nacc
mkNacc ["nacc", acc, alt, desc] =
  Nacc acc alt desc 

getNaccs = makeTypes mkNacc "nacc"

naccTuple (Nacc acc alt desc) =
  (acc, alt, desc)

naccAcc :: Nacc -> Acc
naccAcc nacc = sel1 $ naccTuple nacc

naccAlt :: Nacc -> Acc -- the alternative account when before
naccAlt nacc = sel2 $ naccTuple nacc

naccDesc :: Nacc -> String
naccDesc nacc = sel3 $ naccTuple nacc

alt :: Acc -> [Nacc] -> Acc
alt acc naccs =
  altAcc
  where
    nacc = find (\n -> acc == (naccAcc n)) naccs
    altAcc = case nacc of
      Just n -> naccAlt n
      Nothing -> "Error: couldn't find alt nacc"


showAcc :: Acc -> String
showAcc acc = printf "%-6.6s" acc


showNaccAcc :: Nacc -> String
showNaccAcc nacc = showAcc $ naccAcc nacc

showNacc :: Nacc -> String
showNacc nacc =
  let (acc, _ , desc) = naccTuple nacc in
  (showAcc acc) ++ " " ++ desc
  --printf "%-6.6s  %s" acc desc
