module Types where

import Text.Printf

type Acc = String
type Desc = String -- description
type Dstamp = String
type Etb = [(String, Pennies)]
type Folio = String
type Percent = Double -- [0, 1]
type Period = (Dstamp, Dstamp)
type Qty = Double
type Rox = Double
type Sym = String
type Ticker = String
type Tstamp = String
newtype Pennies = Pennies Integer

-- use doubles instead of floats, as you'd have problems with e.g.: enPennies (-386440.77)
enPennies :: Double -> Pennies
enPennies pounds =
  Pennies i
  where
    --d = 100.0 * float2Double pounds -- use Double for awkward rounding
    d = 100.0 * pounds
    i = (round d :: Integer)

--penTest = enPennies $ asDouble "82301.87"

unPennies :: Pennies -> Double
unPennies (Pennies p) = (fromIntegral p) / 100.0







infixl 6 |+|
Pennies a |+| Pennies b = Pennies (a+b)

infixl 6 |-|
Pennies a |-| Pennies b = Pennies (a-b)


scalep :: Pennies -> Double -> Pennies
scalep p by = enPennies( by * (unPennies p))

negPennies :: Pennies -> Pennies -- unary negate pennies
negPennies p = (Pennies 0) |-| p


negp = negPennies
    
cumPennies :: [Pennies] -> [Pennies]
--cumPennies (p:[]) = p
--cumPennies (p:ps) = p: |+| (cumPennies ps)
cumPennies ps =
  fst resultTuple
  where
    f (pennies, tot)  p =
      (pennies ++ [newTot],  newTot)
      where
        newTot = tot |+| p
    resultTuple = foldl f ([], Pennies 0) ps
      
testCumPennies = cumPennies [Pennies 3, Pennies 4, Pennies 5]
  
countPennies :: [Pennies] -> Pennies
countPennies ([]) = (Pennies 0)
countPennies (p:ps) = p |+| (countPennies ps)

testCountPennies = countPennies [(Pennies 3), (Pennies 4)]


instance Show Pennies where
  show (Pennies p) = printf "%12.2f" (unPennies (Pennies p))


data Comm = Comm { cmSym :: Sym
                 , cmFetch :: Bool
                 , cmType :: String
                 , cmUnit :: String -- currency as string, e.g. USD P GBP NIL
                 , cmExch :: String
                 , cmGepic :: String
                 , cmYepic :: Ticker
                 , cmName :: String
                 , cmStartPrice :: Maybe Double
                 , cmEndPrice :: Maybe Double }
            deriving Show


data Dps = Dps { dpSym::Sym
               , dpDps::Double -- dividend per share in PENCE
               } deriving (Show)

data Etran = Etran { etDstamp::Dstamp
                   , etIsBuy::Bool
                   , etFolio::Folio
                   , etSym::Sym
                   , etQty::Qty
                   , etAmount::Pennies
                   , etDuring :: Maybe Bool
                   , etComm :: Maybe Comm }
             deriving Show

data Nacc = Nacc { ncAcc::Acc, ncAlt::Acc, ncDesc::String} deriving Show

data Ntran = Ntran { ntDstamp::Dstamp, ntDr ::Acc, ntCr:: Acc, ntP:: Pennies, ntClear:: String, ntDesc:: String}
             deriving Show


data Return = Return { idx::Int
                     , dstamp::Dstamp
                     , mine::Double
                     , asx::Double
                     } deriving (Show)


-- see Parser.hs for reading these items
data Record = RecComm Comm | RecDps Dps | RecEtran Etran | RecNacc Nacc | RecNtran Ntran
            | RecReturn Return
            deriving (Show)
