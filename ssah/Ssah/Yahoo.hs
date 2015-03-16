module Ssah.Yahoo where

import Control.Concurrent.Async
import Control.Monad
import Data.List.Split
import Data.String.Utils
import Network.HTTP

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
    post = "&f=snl1c1p2&e=.csv"


getUrl :: String -> IO String
getUrl url = do
    response <- simpleHTTP $ getRequest url
    getResponseBody response

fetchSym sym = getUrl $ symUrl $ replace "^" "%5E" sym

fetchSyms symList = mapConcurrently fetchSym symList


testYahoo = fetchSym "%5EFTAS"
testHyh = (liftM decodeFetch) (fetchSym "HYH")

   

data Yfetch = Yfetch String String Float Float Float deriving (Show)




chgPc :: [Char] -> Float
chgPc s = asFloat $ stripChars "\"%\n+" s

decodeFetch :: [Char] -> Yfetch
decodeFetch f =
  Yfetch ticker name price chg chgpc
  where
    ticker:name:priceStr:chgStr:chgpcStr:[] = splitStr "," f
    price = asFloat(priceStr)
    chg  = asFloat(chgStr)
    chgpc = asFloat(chgpcStr)


fetchAndDecode urls = fmap (liftM decodeFetch) (fetchSyms urls)

fetchAndSave urls = do
  fetches <- fmap (liftM (stripChars "\n")) (fetchSyms urls)
  writeFile "yahoo.csv" (unlines fetches)

loadSaves = do
  let txt = readFile "yahoo.csv"
  --ls <-  (fmap lines) (readFile "yahoo.csv")
  let ls = (liftM lines) txt
  fmap (liftM decodeFetch) ls
