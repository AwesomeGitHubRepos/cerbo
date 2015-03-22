module Ssah.Utils where

import Data.Time
import System.Locale (defaultTimeLocale)
import Text.Printf

type Acc = String
type Dstamp = String
--type Pennies = Int
type Qty = Float
type Rox = Float
type Sym = String
type Ticker = String
type Tstamp = String

newtype Pennies = Pennies Int
enPennies :: Float -> Pennies
enPennies pounds = Pennies (round (pounds * 100.0) :: Int)
unPennies :: Pennies -> Float
unPennies (Pennies p) = (fromIntegral p) / 100.0

instance Show Pennies where
  show (Pennies p) = printf "%12.2f" (unPennies (Pennies p)) -- FIXME probable small rounding problems

--(-) :: Pennies -> Pennies
negPennies :: Pennies -> Pennies -- unary negate pennies
negPennies p =
  Pennies (negp p)
  where
    negp (Pennies posp ) = -posp


matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs




stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v


asPennies :: String -> Pennies -- String of form #0.00
asPennies pounds = enPennies (asFloat pounds)


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
