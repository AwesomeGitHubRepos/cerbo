module Main where

import Lexer

doScan str = do
  putStrLn $ "Scanning: '" ++ str ++ "'"
  print $ alexScanTokens str
  putStrLn "Done\n"
  
main = do
  --putStrLn "hello world"
  --print $ alexScanTokens "hel0lo world"
  doScan "hello world"
  doScan "hello \"world as a string\""
  doScan "this is \"a string with \\\" embedded\" inside it"

  putStrLn "For a finale: scan a file:"
  txt <- readFile "example.txt"
  doScan txt
