module Ssah.Utils where

import Data.Time
import System.Locale (defaultTimeLocale)

type Dstamp = String
type Qty = Float
type Rox = Float
type Sym = String
type Ticker = String
type Tstamp = String



stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v

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
