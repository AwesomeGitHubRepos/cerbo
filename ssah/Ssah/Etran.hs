module Ssah.Etran where

import Data.List
import Data.Maybe
import Data.Tuple.Select

import Ssah.Comm
import Ssah.Utils


data EtranDerived = EtranDerived
                    { --deDstamp::Dstamp
                    deDuring:: Bool -- was the flow during the period?
                    --, deCbd::Pennies -- cost b/d
                    --, dePbd:: Pennies -- profit b/d
                    --, deVbd::Pennies -- valuation b/d
                    --, deFdp:: Pennies -- flow during period
                    --, dePdp::Pennies -- profit during period
                    --, deVcd::Pennies -- valuation carried down
                    , deComm::Comm
                    } deriving (Show)
                    --, dePennies Pennies Pennies Comm deriving (Show)

{-
etranDerivedTuple (EtranDerived odstamp startValue flow profit endValue comm) =
  (odstamp, startValue, flow, profit, endValue, comm)
-}

--data Etran = Etran Dstamp String Folio Sym Qty Pennies (Maybe EtranDerived) deriving (Show)

data Etran = Etran
             { etDstamp::Dstamp
             , etIsBuy::Bool
             , etFolio::Folio
             , etSym::Sym
             , etQty::Qty
             , etAmount::Pennies
             , etDerived::Maybe EtranDerived
             } deriving (Show)

mkEtran :: [[Char]] -> Etran
mkEtran ["etran", dstamp, way, folio, sym, qty, amount] =
    Etran dstamp etIsBuy folio sym qtyF amountP Nothing
    where
      etIsBuy = way == "B"
      sgn1 = if etIsBuy then 1.0 else -1.0
      qtyF = (asFloat qty) * sgn1
      amountP = enPennies (sgn1 * (asFloat amount))




etranTuple (Etran dstamp way acc sym qty amount derived) =
  (dstamp, way, acc, sym, qty, amount, derived)
{-
etranDstamp :: Etran -> Dstamp
etranDstamp e = sel1 $ etranTuple e

etranWay :: Etran -> String
etranWay e = sel2 $ etranTuple e

etranFolio :: Etran -> Folio
etranFolio e = sel3 $ etranTuple e

etranSym :: Etran -> Sym
etranSym e = sel4 $ etranTuple e

qty :: Etran -> Qty
qty e = sel5 $ etranTuple e
etranQty = qty

etranAmount :: Etran -> Pennies
etranAmount e = sel6 $ etranTuple e
-}

--etranDerived :: Etran -> EtranDerived
--etranDerived e = etranDerivedTuple $ fromJust $ sel7 $ etranTuple e
etranDerived e = fromJust $ etDerived e

--etranOdstamp e = deDstamp $ etranDerived e
--etranStartValue e = deVbd $ etranDerived e
--etranFlow e = sel3 $ etranDerived e
--etranProfit e = sel4 $ etranDerived e
--etranEndValue e = deVcd $ etranDerived e

qtys :: [Etran] -> Float
qtys es = sum $ map etQty es

getEtrans = makeTypes mkEtran "etran"

deriveEtran start comms e =
  e { etDerived = de }
  where
    during = start <= etDstamp e
    theComm = findComm comms (etSym e)
    de = Just $ EtranDerived during theComm
{-
-- | See data.c line 495
deriveEtran start comms etran =
  Etran  dstamp way acc sym qty amount derived
  where
    (dstamp, way, acc, sym, qty, amount, _) = etranTuple etran
    comm = findComm comms sym -- FIXME I think etran should already have Comm(?)
    value f =
      let p = fromMaybe 0.0 (f comm) in  enPennies (0.01 * qty * p)
    (odstamp, flow, startValue) =
      if dstamp < start then (start, Pennies 0, value commStartPrice)
      else (dstamp, amount, Pennies 0)   
    endValue = value commEndPrice
    profit = endValue |-| startValue |-| flow
    derived = Just (EtranDerived odstamp startValue flow profit endValue comm)
-}

deriveEtrans start comms etrans = map (deriveEtran start comms) etrans

cerl :: Etran -> String
cerl etran = -- create etran report line
  text
  where
    (dstamp, way, acc, sym, qty, amount, _) = etranTuple etran
    unit = 100.0 * (unPennies amount) / qty
    wayStr = if qty > 0.0 then "B" else "S" -- FIXME use isBuy rather than qty > 0.0
    fields = [psr 7 sym, dstamp, wayStr, psr 3 acc
             , f3 qty, show amount, f4 unit]
    text = intercalate " " fields
             
createEtranReport :: [Etran] -> [String]
createEtranReport etrans =
  [hdr] ++ eLines
  where    
    hdr = "SYM     DSTAMP     W FOLIO        QTY       AMOUNT         UNIT"
    sortedEtrans = sortOn (\e -> (etSym e, etDstamp e)) etrans
    eLines = map cerl sortedEtrans

etComm e = deComm $ fromJust $ etDerived e
etDuring e = deDuring $ fromJust $ etDerived e

-- | Profit during period
etPdp e = (etVcd e) |-| (if etDuring e then  (etAmount e) else (etVbd e))

etStartPrice e = fromJust $ commStartPrice $ etComm e

-- | value brought down
etVbd e = if etDuring e then Pennies 0 else enPennies  (etStartPrice e * 0.01 * etQty e)
etEndPrice e = fromJust $ commEndPrice $ etComm e

-- | value carried down
etVcd e = enPennies (etEndPrice e * 0.01 * etQty e)

-- | profit brought down
etPbd e = if etDuring e then Pennies 0 else (etVbd e) |-| (etAmount e)

-- | flow during period
etFlow e = (etVcd e) |-| (etVbd e) |-| (etPdp e)
