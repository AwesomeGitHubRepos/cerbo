module Main where


--import System.Locale (defaultTimeLocale)
--import Data.Time

--import Data.Time.Format (formatTime)
--import Data.Time.LocalTime (getCurrentTimeZone)

import System.IO

import Ssah.Ssah
import Ssah.Utils

soFar = do
    inputs <-readInputs
    printn 3 inputs
    print "."
    let comms = getComms inputs -- makeTypes mkComm "comm" inputs
    printn 3 comms
    putStrLn "."
    putStrLn "Loading Cached Yahoo data. Consider using fresh download"
    y <- loadYahooCsv
    printn 3 y
    putStrLn "."
    let etrans = getEtrans inputs -- makeTypes mkEtran "etran" inputs
    printn 3 etrans
    putStrLn "I'm confused"
    putStrLn "But then, aren't we all?"
    let prices = makeTypes mkPrice "P" inputs
    printn 3 prices

soFar1 = do

  dstamp <- dateString
  print dstamp
  let fname = "/home/mcarter/.ssa/destroy-" ++ dstamp ++ ".txt"
     
  tstamp <- timeString
  h <- openFile fname WriteMode
  print tstamp
  hPutStr h tstamp
  hFlush h
  hClose h
  
--main = blah
main = soFar1

--soFar
