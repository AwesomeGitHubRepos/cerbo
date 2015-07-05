module Returns where

import Text.Printf

import Comm
import Etran
import Utils

data Return = Return { idx::Int
                     , dstamp::Dstamp
                     , mine::Double
                     , asx::Double
                     } deriving (Show)

mkReturn :: [String] -> Return
mkReturn ["return", arg2, arg3, arg4, arg5] =
  Return { idx = idxInt , dstamp = arg3
         , mine = (asDouble arg4), asx = (asDouble arg5) }
  where idxInt = (read arg2)::Int

getReturns inputs = makeTypes mkReturn "return" inputs

fmtReturn :: Int -> Dstamp -> Double -> Double -> Double -> Double -> Double -> String
fmtReturn aIdx aDstamp aMine minepc aAsx asxpc out =
  printf "%3d %11s %6.2f %6.2f %4.0f %6.2f %6.2f" aIdx aDstamp aMine minepc aAsx asxpc out

-- final 
foldReturns :: [String] -> Return -> [Return] -> [String]
foldReturns acc prev ([]) = acc
  
foldReturns acc prev (r:rs) =
  foldReturns (acc ++ [newReturnStr]) r rs
  where
    minepc = gainpc (mine r) (mine prev)
    asxpc  = gainpc (asx  r) (asx  prev)
    out    = minepc - asxpc
    newReturnStr = fmtReturn (idx r) (dstamp r) (mine r) minepc (asx r) asxpc out


summaryLine :: Double -> Double -> Double -> String
summaryLine minepa asxpa outpa =
  printf "%15s %6s %6.2f %4s %6.2f %6.2f\n" "AVG" " " minepa " " asxpa outpa

createReturns :: Dstamp -> [Etran] -> Double -> [Return] -> [String]
createReturns ds etrans asxNow returns =
  [hdr] ++ createdReturns ++ [summary]
  where
    hdr = "IDX      DSTAMP   MINE  MINE%  ASX   ASX%   OUT%"
    ret0 = head returns
    lastRet = last returns
    finIdx = 1 + (idx lastRet)
    {-
    lup x =
      unPennies f
      where
        msg = "createReturns couldn't lookup etb value:'" ++ x ++ "'"
        f = lookupOrDie x etb msg
-}
    --mine_g = lup "mine/g"
    --mine_bd = lup "mine/b"
    nonUt = filter (\e -> "ut" /= etFolio e) etrans
    mine_g = unPennies $ countPennies $ map etPdp nonUt
    mine_bd = unPennies $ countPennies $ map etVbd nonUt
    finMine = (mine lastRet) * (mine_g / mine_bd + 1.0)
    finRet = Return { idx = finIdx, dstamp = ds, mine = finMine, asx = asxNow }

    augReturns = returns ++ [finRet] 
    createdReturns = foldReturns [] ret0 augReturns

    -- summary line
    gpc v =
      gainpc power 1.0
      where
        fix = (fromIntegral finIdx)::Double
        inv = 1.0/ fix
        power =  v ** inv
        
    minepa = gpc (finMine/100.0)
    asx0 = asx ret0
    asxpa  = gpc (asxNow/asx0)
    outpa = gpc (finMine *asx0 / asxNow / 100.0)
    summary = summaryLine minepa asxpa outpa


