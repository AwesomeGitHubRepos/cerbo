{-# LANGUAGE DoAndIfThenElse, NoOverloadedStrings, TypeSynonymInstances, GADTs, CPP #-}

module Utils  where

import Control.Exception
import Data.Char
import Data.List
import Data.Maybe
import Data.Ord
import Data.Time
import Data.Time.LocalTime
import GHC.Float
import System.Locale (defaultTimeLocale)
import Text.Printf
import Text.Read (readMaybe)


spaces n = replicate n ' '

type Acc = String
type Desc = String -- description
type Dstamp = String
type Etb = [(String, Pennies)]
type Folio = String

type Percent = Float -- [0, 1]
--instance Show Percent where
spacePercent = spaces 7
showPercent p = printf "%7.2f" $ p * 100.0

type Period = (Dstamp, Dstamp)

type Qty = Float
spaceQty = spaces 12
showQty q = printf "%12.3f" (q::Float)

type Rox = Float

type Sym = String
spaceSym = spaces 4
showSym s = printf "%4.4s" s


type Ticker = String
type Tstamp = String

newtype Pennies = Pennies Integer

enPennies :: Float -> Pennies
enPennies pounds =
  Pennies i
  where
    d = 100.0 * float2Double pounds -- use Double for awkward rounding
    i = (round d :: Integer)

penTest = enPennies $ asFloat "82301.87"

unPennies :: Pennies -> Float
unPennies (Pennies p) = (fromIntegral p) / 100.0
spacePennies = spaces 12





instance Show Pennies where
  show (Pennies p) = printf "%12.2f" (unPennies (Pennies p)) -- FIXME probable small rounding problems

infixl 6 |+|
Pennies a |+| Pennies b = Pennies (a+b)

infixl 6 |-|
Pennies a |-| Pennies b = Pennies (a-b)


scalep :: Pennies -> Float -> Pennies
scalep p by = enPennies( by * (unPennies p))

negPennies :: Pennies -> Pennies -- unary negate pennies
negPennies p = (Pennies 0) |-| p


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

clean = stripChars "\"%\n+"


asFloat :: String -> Float
asFloat v =  read (clean v) :: Float 

--tryAsFloat str = handle (\_ -> Left "WTF") (asFloat str)
--tryAsFloat str = (Right $ asFloat str) `catch` \e -> "WTF"

-- http://is.gd/4Pzvew "Smarter validation"
asEitherFloat str =
  case (readMaybe $ clean str) :: Maybe Float of
    Just num -> Right num
    Nothing -> Left $  "Bad float: '" ++ str ++ "'"

asMaybeFloat :: String -> Maybe Float
asMaybeFloat str = readMaybe $ clean str 
  

asPennies :: String -> Pennies -- String of form #0.00
asPennies pounds = enPennies (asFloat pounds)

noPennies :: Pennies -> Bool
noPennies p = 0.0 == unPennies p


    
  
printn n  lst = mapM_ print  (take n lst)
printAll lst = mapM_ print lst

--putAll [] = putStr ""
--putAll xs = mapM_ putStrLn xs

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


sortOnMc :: Ord b => (a -> b) -> [a] -> [a]
sortOnMc f =
  map snd . sortBy (comparing fst) . map (\x -> let y = f x in y `seq` (y, x))

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
-- date/time functions

now = getZonedTime

{-
fmtNow fmt = do
  loc <- System.Locale.defaultTimeLocale
  n <- now
  --f1 <- formatTime
  return formatTime loc n  fmt
-}

time1 :: IO LocalTime
time1 = fmap zonedTimeToLocalTime getZonedTime

time2 :: IO (String, String)
time2 = do
  t1 <- time1
  let t2 = show t1
  let ds = take 10 t2
  let ts = drop 11 t2
  return (ds, ts)

dateString :: IO String
dateString = do
  (ds, _) <- time2
  return ds

timeString :: IO String
timeString = do
  (_, ts) <- time2
  return (take 8 ts)

--myTime = Data.Time.defaultTimeLocale
{- FIXME following needs web page. It rpint UTC time
myTime = System.Locale.defaultTimeLocale

dateString = do
  let now = getCurrentTime
  dstamp <- fmap (formatTime myTime "%Y-%m-%d") now
  return dstamp

timeString = do
  let now = getCurrentTime
  tstamp <- fmap (formatTime myTime "%H:%M:%S") now
  return tstamp
-}

-----------------------------------------------------------------------
-- Misc routines

ones = (1.0::Float)  : ones -- infinited list of 1.0's

upperCase = map toUpper
