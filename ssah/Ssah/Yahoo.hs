module Ssah.Yahoo where

import Control.Concurrent.Async
import Control.Monad
import Data.List
import Data.List.Split
import Data.String.Utils
import Data.Tuple.Select
import Network.HTTP
import System.IO
import Text.Printf

import Ssah.Utils

--import Network.Wreq
--import Control.Applicative
--import Control.Lens
--import Data.ByteString.Lazy.Char8


-- TODO put some of these in Utils
type Dstamp = String
type Rox = Float
type Ticker = String
type Tstamp = String


splitStr = Data.String.Utils.split -- alias a function



symUrl sym =
  pre ++ sym ++ post
  where
    pre =  "http://download.finance.yahoo.com/d/quotes.csv?s="
    post = "&f=sl1c1p2&e=.csv"

--type FetchedStr = String -- a string result fetched from Yahoo

getUrl :: String -> IO String
getUrl url = do
    response <- simpleHTTP $ getRequest url
    getResponseBody response

stripJunk = stripChars "\"+%\n"

--type Price = Float
--type Rox   = Float

-- data StockUnit = Raw | Pennies deriving (Show)
data StockQuote = StockQuote String String String Float Float Float Float deriving (Show)
quoteTuple (StockQuote       dstamp tstamp ticker rox   price chg   chgpc ) =
  (dstamp, tstamp, ticker, rox, price, chg, chgpc) 

--quotePrice :: StockQuote -> Price
quotePrice sq = sel3 $ quoteTuple sq
quoteSym   sq = sel1 $ quoteTuple sq

str4 :: Float -> String -- TODO promote to Utils
str4 f = printf "%9.4f" f

fmtTicker ticker = printf "%6s" ticker

quoteAsText :: StockQuote -> String
quoteAsText sq = 
  intercalate "  " fields
  where
    --fmt = "yahoo\t%s\t%s\t%s\t%12.4f\tP\n"
    fields = ["yahoo", dstamp, tstamp, (fmtTicker ticker), (str4 rox),   (str4 price), (str4 chg),   (str4 chgpc), "P\n"]
    (dstamp, tstamp, ticker, rox,   price, chg,   chgpc) = quoteTuple sq
    

testQuoteAsText = do
  let sq = StockQuote "2015-03-18" "12:59:23" "HYH" 1.0 47.5745 0.5645 1.2008
  print (quoteAsText sq)

decodeFetch :: String -> String -> Float -> String -> StockQuote
decodeFetch dstamp tstamp rox serverText =
  StockQuote dstamp tstamp ticker rox   price chg   chgpc
  where
    ticker:priceStr:chgStr:chgpcStr:[] = splitStr "," serverText
    price = asFloat(priceStr)
    chg  = asFloat(chgStr)
    chgpc = asFloat(chgpcStr)

fetchQuote ::  String -> String -> (String, Float)  -> IO StockQuote
fetchQuote dstamp tstamp (ticker, rox) =  do
  let url = symUrl $ replace "^" "%5E" ticker
  resp <- getUrl url
  let clean = stripJunk resp
  let sq =  decodeFetch dstamp tstamp rox clean
  return sq


testFtas = fetchQuote  "2015-03-18" "12:59:23" ("^FTAS", 1.0)
testHyh = fetchQuote "2015-03-18" "12:59:23" ("HYH", 1.0)



fetchQuotes :: Dstamp -> Tstamp -> [(Ticker, Rox)] -> IO [StockQuote]
fetchQuotes dstamp tstamp pairs = do
  let fq = fetchQuote dstamp tstamp
  res <- mapConcurrently fq pairs
  return res

testTickers = [ ("AML.L", 1.0), ("ULVR.L", 1.0), ("HYH", 1.0)]
testFetches = fetchQuotes "2015-03-18" "12:59:23" testTickers


-- TODO perform Rox scaling

   




--fetchAndDecode urls = fmap (liftM decodeFetch) (fetchSyms urls)

yfile = "/home/mcarter/.ssa/yahoo.csv"


 -- TODO reinstate!
fetchAndSave tickerPairs = do
  -- TODO rox translation
  dstamp <- dateString
  tstamp <- timeString
  let fname = "/home/mcarter/.ssa/yahoo/" ++ dstamp ++ ".txt"
  h <- openFile fname WriteMode
  let writeQuote quote = hPrintf h (quoteAsText quote)
  --let  roxs = replicate (length urls) 1.0 -- TODO FIXME 1.0
  --let pairs  = zip urls roxs
  quotes <- fetchQuotes  dstamp tstamp tickerPairs
  mapM_ writeQuote quotes
  hFlush h
  hClose h

testFas = fetchAndSave testTickers


loadSaves = do
  let txt = readFile yfile
  let ls = (liftM lines) txt
  fmap (liftM decodeFetch) ls

