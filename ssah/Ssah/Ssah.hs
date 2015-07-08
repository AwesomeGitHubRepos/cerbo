-- DO NOT USE THIS MODULE - DELETE IT. 08-Jul-2015
module Ssah  where

import Data.Either
import Data.List
import Data.String.Utils
import Data.Tuple.Select


import Comm
import Config
import Etran
import Financial
import Nacc
import Ntran
import Parser
import Returns
import Yahoo
import Utils



{-




data Price = Price String String Double deriving (Show)

mkPrice :: [[Char]] ->Price
mkPrice["P", dstamp, _, sym, price, _ ] =
    Price dstamp sym (asDouble price)






printQuotes = do
  inputs <- readInputs
  let quotesYahoo =  getQuotes inputs
  printAll quotesYahoo
  let quotesGoogle = getGoogles inputs
  printAll quotesGoogle

  
allComms :: IO [Comm]
allComms = do
  inputs <- readInputs -- for testing purposes
  let comms = getComms inputs
  return comms
  

-}
{-
-----------------------------------------------------------------------
-- Etb storage and retrieval

etbAsText etb =
  unlines $ map makeEtbLine etb
  where
    makeEtbLine etbEl =
      replace " " "" text1
      where
        (name, total) = etbEl
        pounds = unPennies total
        p = round $ pounds * 100
        text1 = name ++ "!" ++ (show p) ++ "!" ++ (show total)

  
storeEtb etb = do
  --let text = makeEtbFields totalTab naccs
  writeFile "/home/mcarter/.ssa/hssa-etb.txt" (etbAsText etb)

-----------------------------------------------------------------------
-}
