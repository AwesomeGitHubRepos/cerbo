module Ssah.Utils where

import Data.Time
import System.Locale (defaultTimeLocale)

type Acc = String
type Dstamp = String
type Pennies = Int
type Qty = Float
type Rox = Float
type Sym = String
type Ticker = String
type Tstamp = String

matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs




stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v

asPennies :: String -> Pennies -- String of form #0.00
asPennies pounds =
  round $  100.0 * (asFloat pounds)

dateString = do
  let now = getCurrentTime
  dstamp <- fmap (formatTime defaultTimeLocale "%Y-%m-%d") now
  return dstamp

timeString = do
  let now = getCurrentTime
  tstamp <- fmap (formatTime defaultTimeLocale "%H:%M:%S") now
  return tstamp
    
  
printn n  lst = mapM_ print  (take n lst)
printAll lst = mapM_ print lst

map2 f list1 list2 =
  let f' (el1, el2) = f el1 el2 in
  map f' (zip list1 list2)


testMap2 = map2 (+) [10, 11] [12, 13]
