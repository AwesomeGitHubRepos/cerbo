module Ssah.Portfolio where

import Data.List
import Text.Printf

import Ssah.Comm
import Ssah.Utils

-- FIXME LOW - use calculated values of mine/b ... computed in Etb rather than working them out here

myPorts = ["hal", "hl", "tdn", "tdi"]

fmtName :: String -> String
fmtName name = printf "%5s" name

fmtRet :: Float -> String
fmtRet v = printf "%7.2f" v

getPline :: String -> [Pennies] -> String
getPline name values =
  text
  where
    [vbefore, _, vprofit, _] = values
    ret = (unPennies vprofit) / (unPennies vbefore) * 100.00
    nameStr = fmtName  name
    retStr = fmtRet ret
    text = nameStr ++ (concatMap show values) ++ retStr
          
calcPort getpe port =
  [vbefore, vflow, vprofit, vto]
  where
    vbefore = negp $ getpe (port ++ "/b")
    vprofit = negp $ getpe (port ++ "/g")
    vto     = getpe (port ++ "/c")
    vflow   = vto |-| vprofit |-| vbefore

    
createEquityPortfolios etb =
  ["PORTFOLIOS:", hdr] ++ mineLines -- FIXME this part really belongs in createPortfolios
  ++ [spacer1, mineSumsLine, utLine, spacer1, totalLine, spacer2]
  where
    hdr = "FOLIO     VBEFORE       VFLOW     VPROFIT         VTO   VRET"
    getpe = getp etb
    mine = map (calcPort getpe) myPorts
    mineLines = map2 getPline myPorts mine

    spacer1 = "----- ----------- ----------- ----------- ----------- ------"
    mineSums = map countPennies $ transpose mine
    mineSumsLine = getPline "mine" mineSums
    
    ut = calcPort getpe "ut"
    utLine = getPline "ut" ut
    
    totalSums = map countPennies $ transpose [mineSums, ut]
    totalLine = getPline "total" totalSums
    spacer2 = "===== =========== =========== =========== =========== ======"
    --totalSums = mineSums ++ ut

createIndexLine comms sym =
  text
  where
    --c = findComm comms comm
    fmtVal v = (printf "%12.2f" v)::String
    startPrice = commStartPriceOrDie comms sym
    endPrice = commEndPriceOrDie comms sym
    profit = endPrice - startPrice
    retStr = fmtRet (profit/startPrice * 100.0)
    text = (fmtName sym) ++ (concatMap fmtVal [startPrice, 0.0, profit,  endPrice]) ++ retStr
    
createIndices comms =  map (createIndexLine comms) ["FTAS", "FTSE", "FTMC"]


createPortfolios  etb comms =
  (createEquityPortfolios etb) ++ (createIndices comms) ++ ["."]
  
