import Data.Char
--import Data.Text
import System.Directory
--import System.FilePath.Glob
import System.Path.Glob

filterInputs inputs =
  filter (\x -> isAlpha (x !! 0)) nonblanks
  where all = (lines . unlines) inputs
        nonblanks = filter (\x -> length x > 0) all





readInputs = do
  files <- glob "/home/mcarter/redact/docs/accts2014/data/*.txt"
  contents <- mapM readFile files
  let allLines = filterInputs contents
  let commands = map foldLine allLines
  --let commands = allLines
  mapM_ print commands

-- main = readInputs




--eof = [chr(0)]




eatWhite str = snd (span isSpace str)

-- TODO fix bug where this is not a termination by a "
getQuoted str =
  (h, rest)
  where (h, t) = break (\x -> x == '"') (tail str)
        rest = drop 1 t
        --body =  init all
        --len = 2 + length body

getUnquoted str = (break isSpace str)
--  (len, body)
--  where body = fst (break isSpace str)
--        len = length body

lexeme str
  | length nonWhite == 0 = ("", "")
  | nonWhite !! 0 == '"' = (getQuoted nonWhite)
  | otherwise = (getUnquoted nonWhite)
  where nonWhite = eatWhite str

foldLine' acc str
  | length lex == 0 = acc
  | otherwise = foldLine' (acc ++ [lex]) rest
  where (lex, rest) = lexeme str
    
foldLine str = foldLine' [] str




        
main = readInputs

--  print (foldLine "etran 2010-12-30 B  ut  FSS   1221.900 16469.06")
  --readInputs
  --let inputLine = readInputs
  
  --print (chunkify "etran 2014-07-14 B tdi 234.23")
  --print (chunkify "foo bar")
  -- print (foldLine "\"foo  baz \" bar")
  -- print (foldLine "happy  days")
  --  print (tricky 4)
  --  print (lexeme "")

