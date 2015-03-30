module Ssah.Etran where

import Data.List
import Data.Maybe
import Data.Tuple.Select

import Ssah.Comm
import Ssah.Utils


data EtranDerived = EtranDerived  Dstamp  Pennies Pennies Pennies Pennies Comm deriving (Show)

etranDerivedTuple (EtranDerived odstamp startValue flow profit endValue comm) =
  (odstamp, startValue, flow, profit, endValue, comm)

data Etran = Etran Dstamp String Folio Sym Qty Pennies (Maybe EtranDerived) deriving (Show)

mkEtran :: [[Char]] -> Etran
mkEtran ["etran", dstamp, way, folio, sym, qty, amount] =
    Etran dstamp way folio sym qtyF amountP Nothing
    where
        sgn1 = if way == "B" then 1.0 else -1.0
        qtyF = (asFloat qty) * sgn1
        amountP = enPennies (sgn1 * (asFloat amount ))




etranTuple (Etran dstamp way acc sym qty amount derived) =
  (dstamp, way, acc, sym, qty, amount, derived)

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

--etranDerived :: Etran -> EtranDerived
etranDerived e = etranDerivedTuple $ fromJust $ sel7 $ etranTuple e


etranOdstamp e = sel1 $ etranDerived e
etranStartValue e = sel2 $ etranDerived e
etranFlow e = sel3 $ etranDerived e
etranProfit e = sel4 $ etranDerived e
etranEndValue e = sel5 $ etranDerived e

qtys :: [Etran] -> Float
qtys es = sum $ map qty es

getEtrans = makeTypes mkEtran "etran"

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

deriveEtrans start comms etrans = map (deriveEtran start comms) etrans

cerl :: Etran -> String
cerl etran = -- create etran report line
  text ++ "\n"
  where
    (dstamp, way, acc, sym, qty, amount, _) = etranTuple etran
    unit = 100.0 * (unPennies amount) / qty
    wayStr = if qty > 0.0 then "B" else "S"
    fields = [psr 7 sym, dstamp, wayStr, psr 3 acc
             , f3 qty, show amount, f4 unit]
    text = intercalate " " fields
             
createEtranReport :: [Etran] -> String
createEtranReport etrans =
  "Section: Etrans\n" ++ hdr ++ eLines ++ ".\n"
  where    
    hdr = "SYM     DSTAMP     W FOLIO        QTY       AMOUNT         UNIT\n"
    sortedEtrans = sortOn (\e -> (etranSym e, etranDstamp e)) etrans
    eLines = concatMap cerl sortedEtrans
