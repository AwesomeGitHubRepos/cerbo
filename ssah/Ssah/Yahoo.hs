module Ssah.Yahoo where

import Control.Concurrent.Async
import Control.Monad
import Data.List
import Data.List.Split
import Data.String.Utils
import Data.Tuple.Select
import GHC.Exts
import Network.HTTP
import System.IO
import Text.Printf

--import Ssah.Ssah
import Ssah.Utils


--import Network.Wreq
--import Control.Applicative
--import Control.Lens
--import Data.ByteString.Lazy.Char8





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


mkQuote :: [String] -> StockQuote
mkQuote ["yahoo", dstamp, tstamp, ticker, rox, price, chg, chgpc, "P"] =
  StockQuote dstamp tstamp ticker (asFloat rox) (asFloat price) (asFloat chg) (asFloat chgpc)

getQuotes = makeTypes mkQuote "yahoo"

quoteDstamp sq = sel1 $ quoteTuple sq

quoteTicker sq = sel3 $ quoteTuple sq

--quotePrice :: StockQuote -> Price
quotePrice sq = sel5 $ quoteTuple sq
--quoteSym   sq = sel1 $ quoteTuple sq

quoteChg sq = sel6 $ quoteTuple sq
quoteChgPc sq = sel7 $ quoteTuple sq

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
    

fetchUsd = do
  let url1 = "http://download.finance.yahoo.com/d/quotes.csv?s="
  let url2 = "USDGBP"
  let url3 = "=X&f=nl1d1t1"
  let url  = url1 ++ url2 ++ url3
  resp <- getUrl url
  let clean = stripJunk resp
  let fields = splitStr "," clean
  let roxStr = (fields !! 1)
  let rox = 100.0 * (asFloat roxStr)
  return rox

testQuoteAsText = do
  let sq = StockQuote "2015-03-18" "12:59:23" "HYH" 1.0 47.5745 0.5645 1.2008
  print (quoteAsText sq)

decodeFetch :: String -> String -> Float -> String -> StockQuote
decodeFetch dstamp tstamp rox serverText =
  StockQuote dstamp tstamp ticker rox   price chg   chgpc
  where
    ticker:priceStr:chgStr:chgpcStr:[] = splitStr "," serverText
    price = asFloat(priceStr) * rox
    chg  = asFloat(chgStr) * rox
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

fetchQuotesA :: [Ticker] -> [Rox] -> IO [StockQuote]
fetchQuotesA tickers roxs = do
  ds <- dateString
  ts <- timeString
  let pairs = zip tickers roxs
  quotes <- fetchQuotes ds ts pairs
  return quotes

testFqa = fetchQuotesA ["HYH", "^FTAS"] [1.0, 1.0]

testTickers = [ ("AML.L", 1.0), ("ULVR.L", 1.0), ("HYH", 1000.0)]
testFetches = fetchQuotes "2015-03-18" "12:59:23" testTickers




--fetchAndDecode urls = fmap (liftM decodeFetch) (fetchSyms urls)

yfile = "/home/mcarter/.ssa/yahoo-cached.txt"


saveStockQuotes :: FilePath -> [StockQuote] -> IO ()
saveStockQuotes fname quotes = do
  h <- openFile fname WriteMode
  let writeQuote quote = hPrintf h (quoteAsText quote)
  mapM_ writeQuote quotes
  hFlush h
  hClose h

fetchAndSave :: [(Ticker, Rox)] -> IO ()   
fetchAndSave tickerPairs = do
  dstamp <- dateString
  tstamp <- timeString
  let fname = "/home/mcarter/.ssa/yahoo/" ++ dstamp ++ ".txt"
  quotes <- fetchQuotes  dstamp tstamp tickerPairs
  saveStockQuotes fname quotes 


testFas = fetchAndSave testTickers


loadSaves = do
  let txt = readFile yfile
  let ls = (liftM lines) txt
  fmap (liftM decodeFetch) ls

-- | Return the a price for Ticker on or before Dstamp
getStockQuote :: Dstamp -> Ticker -> [StockQuote] -> Float
getStockQuote dstamp ticker sqs =
  quotePrice sqLast
  where
    f sq = ticker == (quoteTicker sq) && (quoteDstamp sq) <= dstamp
    matches = filter f sqs
    sortedMatches = sortWith quoteDstamp matches 
    sqLast = last sortedMatches


mkGoogle :: [String] -> StockQuote
mkGoogle ["P", dstamp, tstamp, sym, priceStr, unit] =
  StockQuote dstamp tstamp ticker 1.0 priceF 0.0 0.0
  where
    priceRaw = (asFloat priceStr)
    rox1 = 1.0
    (ticker, scale) = case sym of
      "FTAS"  -> ("^FTAS", 1.0)
      "FTSE"  -> ("^FTSE", 1.0)
      "AUG"   -> ("AUG?", rox1)
      "AUS"   -> ("AUS?", rox1)
      "AFUSO" -> ("AFUSO?", rox1)
      "FGF"   -> ("GB0003860789.L", rox1)
      "FGSS"  -> ("GB00B196XG23.L", rox1)
      "FSS"   -> ("GB0003875100.L", rox1)
      s       -> (s ++ ".L", 1.0)
    priceF = priceRaw * scale
      
getGoogles = makeTypes mkGoogle "P"


