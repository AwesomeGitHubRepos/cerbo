module Ssah.Utils where

import Data.List
import Data.Maybe
import Data.Time
import System.Locale (defaultTimeLocale)
import Text.Printf

type Acc = String
type Desc = String -- description
type Dstamp = String
type Etb = [(String, Pennies)]
type Folio = String
type Period = (Dstamp, Dstamp)
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

myTime = Data.Time.defaultTimeLocale
--myTime = System.Locale.defaultTimeLocale

instance Show Pennies where
  show (Pennies p) = printf "%12.2f" (unPennies (Pennies p)) -- FIXME probable small rounding problems

infixl 6 |+|
Pennies a |+| Pennies b = Pennies (a+b)

infixl 6 |-|
Pennies a |-| Pennies b = Pennies (a-b)


--infixl 7 |*|
--Pennies a |*| scale = enPennies ( scale * (unPennies a))
scalep :: Pennies -> Float -> Pennies
scalep p by = enPennies( by * (unPennies p))

{-
infixl 7 0-|
(0-|) Pennies a = Pennies (-a)
  -}
--(-) :: Pennies -> Pennies
negPennies :: Pennies -> Pennies -- unary negate pennies
negPennies p = (Pennies 0) |-| p
{-  Pennies (negp p)
  where
    negp (Pennies posp ) = -posp
-}

negp = negPennies
    
cumPennies :: [Pennies] -> [Pennies]
--cumPennies (p:[]) = p
--cumPennies (p:ps) = p: |+| (cumPennies ps)
cumPennies ps =
  fst resultTuple
  where
    f (pennies, tot)  p =
      (pennies ++ [newTot],  newTot)
      where
        newTot = tot |+| p
    resultTuple = foldl f ([], Pennies 0) ps
      
testCumPennies = cumPennies [Pennies 3, Pennies 4, Pennies 5]
  
countPennies :: [Pennies] -> Pennies
countPennies ([]) = (Pennies 0)
countPennies (p:ps) = p |+| (countPennies ps)

testCountPennies = countPennies [(Pennies 3), (Pennies 4)]
  
matchHeads str = filter (\x -> head x == str)

makeTypes maker match  inputs = map maker $ matchHeads match inputs




stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v


asPennies :: String -> Pennies -- String of form #0.00
asPennies pounds = enPennies (asFloat pounds)

noPennies :: Pennies -> Bool
noPennies p = 0.0 == unPennies p

dateString = do
  let now = getCurrentTime
  dstamp <- fmap (formatTime myTime "%Y-%m-%d") now
  return dstamp

timeString = do
  let now = getCurrentTime
  tstamp <- fmap (formatTime myTime "%H:%M:%S") now
  return tstamp
    
  
printn n  lst = mapM_ print  (take n lst)
printAll lst = mapM_ print lst

map2 f list1 list2 =
  let f' (el1, el2) = f el1 el2 in
  map f' (zip list1 list2)



testMap2 = map2 (+) [10, 11] [12, 13]

map3 f list1 list2 list3 =
  let f' (el1, el2, el3) = f el1 el2 el3 in
  map f' (zip3 list1 list2 list3)

map4 f list1 list2 list3 list4 =
  let f' (el1, el2, el3, el4) = f el1 el2 el3 el4 in
  map f' (zip4 list1 list2 list3 list4)  

putAll alist =  mapM_ putStrLn alist

getp etb key = fromMaybe (Pennies 0) (lookup key etb)


doOrDie maybeX oops =
  case maybeX of
    Just x -> x
    Nothing -> error oops

 
gainpc :: Float -> Float -> Float
gainpc num denom = 100.0 * num / denom - 100.0   

lookupOrDie what table oopsText =
  case (lookup what table) of
    Just v -> v
    Nothing -> error oopsText

testlod1 = lookupOrDie 30 [(30, 31), (32, 33)] "not printed"
testlod2 = lookupOrDie 34 [(30, 31), (32, 33)] "you shall not pass"
{-
findOrDie what table oopsText =
  case (find (
-}

true x = True -- function which always returns true

-----------------------------------------------------------------------
-- printing routines

f3 :: Float -> String
f3 f = -- show a 3dp float as a string
  printf "%12.3f" f

f4 :: Float -> String
f4 f = -- show a 4dp float as a string
  printf "%12.4f" f
  
psr :: Int -> String -> String
psr n str = -- pad string right to length n
  let fmt = "%-" ++ (show n) ++ "." ++ (show n) ++ "s" in
  printf fmt str

-----------------------------------------------------------------------
