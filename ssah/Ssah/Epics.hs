module Ssah.Epics where

import Data.List
import Text.Printf

import Ssah.Aggregate
import Ssah.Comm
import Ssah.Etran
import Ssah.Utils

data Epic = Epic { sym::Sym, eqty::Float , ucost::Float, uvalue::Float
                 , cost::Pennies, value::Pennies, ret::Float} deriving (Show)

showEpic :: Epic -> String
showEpic epic =
  printf fmt s q uc uv c v r
  where
    fmt = "%5s %8.2f %8.2f %8.2f %s %s %6.2f"
    s = sym epic
    q = eqty epic
    uc = ucost epic
    uv = uvalue epic
    c = show $ cost epic
    v = show $ value epic
    r = ret epic

epicHdr = "SYM        QTY    UCOST   UVALUE         COST        VALUE   RET%"  
every e = True

epicSum :: Pennies -> Pennies -> String
epicSum inCost inValue =
  printf "%32s %s %s" " " (show inCost) (show inValue)

{-
foldEtrans aSym aQty aUcost aUvalue aCost aValue  ([]) =
  Epic { sym=aSym, eqty= aQty, ucost = aUcost , uvalue = aUvalue
       , cost = aCost, value = aValue  }

foldEtrans aSym aQty aUcost aUvalue aCost aValue  (e:es) =
  foldEtrans aSym newQty newUcost newUvalue newCost newValue
  where
-}

foldEtrans inQty inCost  ([]) = (inQty, inCost)

foldEtrans inQty inCost  (e:es) =
  foldEtrans newQty newCost es
  where
    --isBuy = ((qty e) > 0.0)
    eQty = etQty e
    newQty = inQty + eQty
    incCost = if etIsBuy e -- incremental cost
              then (etAmount e)
              else (scalep inCost (eQty/ inQty))
    newCost = inCost |+| incCost


  
    
processSymGrp comms etrans =
  Epic { sym = theSym, eqty = theQty, ucost = theUcost, uvalue = theUvalue
       , cost = theCost, value = theValue, ret = theRet}

  where
    theSym = etSym $ head etrans
    sortedEtrans = sortOnMc etDstamp etrans
    (theQty, theCost) =  foldEtrans  0.0 (Pennies 0) sortedEtrans
    theUcost = 100.0 * (unPennies theCost) / theQty
    theUvalue = commEndPriceOrDie comms theSym
    theValue = enPennies (0.01 * theQty * theUvalue)
    theRet = gainpc (unPennies theValue)  (unPennies theCost)

        
  
reportOn title comms etrans =
  (fullTableLines, zeroLines)
  where
    symGrp = groupByKey etSym etrans
    epics = map (processSymGrp comms) symGrp
    (nonzeros, zeros) = partition (\e -> (eqty e) > 0.0) epics
    tableLines = map showEpic nonzeros
    
    tCost = countPennies $ map cost nonzeros
    tValue = countPennies $ map value nonzeros
    sumLine = epicSum tCost tValue

    fullTitle = "EPICS: " ++ title
    fullTableLines = [fullTitle, epicHdr] ++ tableLines ++ [sumLine]
    zeroLines = map sym zeros 
    

subEpicsReportXXX comms etrans aFolio =
  nzTab
  where
    fEtrans = filter (\e -> (etFolio e) == aFolio) etrans
    (nzTab, _) = reportOn aFolio comms fEtrans

subEpicsReportWithTitle title comms etrans cmp aFolio =   
  fst $ reportOn title comms fEtrans
  where
    fEtrans = filter (\e -> aFolio `cmp` etFolio e) etrans
    
subEpicsReport comms etrans cmp aFolio =
  subEpicsReportWithTitle aFolio comms etrans cmp aFolio
    
--matchFolio name = 
reportEpics comms etrans =
  nzTab ++ nonUts ++ zTab1 ++ subReports
  where
    etransBySym = sortOnMc etSym etrans --work around apparent groupBy bug
    (nzTab, zTab) = reportOn "ALL" comms etransBySym
    zTab1 = ["EPICS: ZEROS"] ++ zTab ++ [";"]
    folios = ["hal", "hl", "tdi", "tdn", "ut"]
    nonUts = subEpicsReportWithTitle "NON-UT" comms etransBySym (/=) "ut" -- not the Unit Trusts
    subReports = concatMap (subEpicsReport comms etransBySym (==)) folios
