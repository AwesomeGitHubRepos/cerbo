module Etran where

import Data.List
import Data.Maybe
import Data.Tuple.Select

import Comm
import Utils


data EtranDerived = EtranDerived
                    { --deDstamp::Dstamp
                    deDuring:: Bool -- was the flow during the period?
                    , deComm::Comm
                    } deriving (Show)



data Etran = Etran
             { etDstamp::Dstamp
             , etIsBuy::Bool
             , etFolio::Folio
             , etSym::Sym
             , etQty::Qty
             , etAmount::Pennies
             , etDerived::Maybe EtranDerived
             } deriving (Show)

etIsSell = not . etIsBuy

etBetween :: Etran -> Bool
etBetween e =
  inPeriod
  where
    de = etDerived e
    inPeriod = case de of
      Nothing -> False
      (Just x) -> deDuring x

etCommA :: Etran ->  Comm
etCommA e = c where
  de = etDerived e
  oops = "etComm couldn't find Comm of Etran:" ++ (show e)
  c = case de of
    Nothing -> error oops
    (Just x) -> deComm x



mkEtran :: [[Char]] -> Etran
mkEtran fields =
    Etran dstamp etIsBuy folio sym qtyF amountP Nothing
    where
      ["etran", dstamp, way, folio, sym, qty, amount] = fields
      getFloat field name = --FIXME this should be abstracted (e.g. also in Yahoo.hs)
        case asEitherFloat field of
          Left msg -> error $ unlines ["mkEtran parseError", name, show fields, msg]
          Right v -> v
      etIsBuy = way == "B"
      sgn1 = if etIsBuy then 1.0 else -1.0
      qtyF = (getFloat qty "qty") * sgn1
      amountP = enPennies (sgn1 * (getFloat amount "amount"))




etranTuple (Etran dstamp way acc sym qty amount derived) =
  (dstamp, way, acc, sym, qty, amount, derived)

etranDerived e = fromJust $ etDerived e

qtys :: [Etran] -> Float
qtys es = sum $ map etQty es

getEtrans = makeTypes mkEtran "etran"

deriveEtran start comms e =
  e { etDerived = de }
  where
    during = start <= etDstamp e
    theComm = findComm comms (etSym e)
    de = Just $ EtranDerived during theComm

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
    -- sortedEtrans = sortOnMc (\e -> (etSym e, etDstamp e)) etrans
    eLines = map cerl etrans

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
