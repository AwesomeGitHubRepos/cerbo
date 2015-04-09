module Ssah.Portfolio where

import Data.List
import Text.Printf

import Ssah.Comm
import Ssah.Etran
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

{-
calcPort getpe port =
  [vbefore, vflow, vprofit, vto]
  where
    vbefore = negp $ getpe (port ++ "/b")
    vprofit = negp $ getpe (port ++ "/g")
    vto     = getpe (port ++ "/c")
    vflow   = vto |-| vprofit |-| vbefore

    
createEquityPortfoliosXXX etb =
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
-}
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
{-

createPortfoliosXXX  etb comms =
  (createEquityPortfolios etb) ++ (createIndices comms) ++ ["."]
  
-- originally 76 lines before cleanup
-}

pfHdr     = "FOLIO     VBEFORE       VFLOW     VPROFIT         VTO   VRET"
pfSpacer1 = "----- ----------- ----------- ----------- ----------- ------"
pfSpacer2 = "===== =========== =========== =========== =========== ======"


pfCalc title subEtrans =
  getPline title sums
  where
    --subset = filter isIn etrans
    sumField field = countPennies $ map field subEtrans
    sums = map sumField [etVbd, etFlow, etPdp, etVcd]

{-
pfStd etrans folio =
  pfCalc folio 
  where
    isIn e = folio == etFolio e
-}

createEquityPortfolios etrans =
  [pfHdr, hal, hl, tdn, tdi, pfSpacer1, mine, ut, pfSpacer1, all, pfSpacer2]
  where
    pfStd folio = pfCalc folio $ filter (\e -> folio == etFolio e) etrans
    [hal, hl, tdn, tdi, ut] = map pfStd ["hal", "hl", "tdn", "tdi", "ut"]
    mine = pfCalc "mine" $ filter (\e -> "ut" /= etFolio e) etrans
    all = pfCalc "total" etrans
    --hal = pf "hal" ets
    --hl  = pfStd "hl"  et
    --"tdn" = pfStd "tdn" ets
    --isIn e = ("hal" == etFolio e)
    --res = [getPline "hal" sums]
           
createPortfolios etrans comms =
  (createEquityPortfolios etrans) ++ (createIndices comms)

    
