-- Calculate CGT for 2014/5
module Ssah.Cgt where

import Data.List
import Data.Maybe
import Data.Set (Set)
import qualified Data.Set as Set

import Ssah.Comm  
import Ssah.Etran
import Ssah.Portfolio
import Ssah.Utils


commSymSold :: [Etran] -> [Sym]
commSymSold es =
  cs4
  where
    -- identify the comms which have sales during the period
    --es2 = filter isJust . etDerived es1
    es3 = filter etBetween $ filter etIsSell es -- sells during period
    cs1 = map (commSym . etCommA) es3
    cs2 = Set.fromList cs1 -- to remove dupes
    cs3 = Set.toList cs2
    cs4 = sort cs3
          
-- | create the CGT spreadsheet
mkCgt etrans =
  x
  where
    es1 = filter (isJust . etDerived) etrans
    es2 = feopn "tdi" (/=) es1 -- completely ignore the ISA
    cs = commSymSold es2
    
    x = cs

createCgtReport etrans = do
  putStrLn "FIXME NOW createCgtReport "
  putStrLn $ show $ mkCgt etrans
