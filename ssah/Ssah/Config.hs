-- {-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE OverloadedStrings #-}

module Config where

import Control.Exception
import Control.Monad
import Data.Text (Text, unpack, pack)
import System.Environment (getEnv)
import System.IO.Error
import System.Info (os)
import System.Path.Glob

--import Control.Monad.Error
--import Data.ConfigFile
import Data.Configurator
import Data.Configurator.Types

isLinux = os == "linux"

--data Cfg = Cfg {

iops :: IO String -> String -> IO String
iops iostr str = do
  str1 <- iostr
  return (str1 ++ str)

defaultConfigXXX = 
  case os of
  "linux" -> (getEnv "HOME", "", "/", ".hssarc")
  _ -> (getEnv "USERPROFILE", "\\AppData\\Local\\MarkCarter\\hssa", "\\", "hssa.cfg")


outDir :: IO String
outDir =
  case os of
  "linux" -> getEnv "HOME"
  _ -> iops (getEnv "USERPROFILE") "\\AppData\\Local\\MarkCarter\\hssa"
  
  --let (base, d, _, _) = defaultConfig
  --iops base d

fileSep = if isLinux then "/" else "\\"

rcFile = iops outDir (fileSep ++ cfgFile)
         where
           cfgFile = if isLinux then ".hssarc" else "hssa.cfg"

outFile :: String -> IO String
outFile name = iops outDir (fileSep ++ name)




{-
--rcFile = "/home/mcarter/.hssarc"
rcFileLinux = do
  root <- getEnv "HOME"
  let full = root ++ "/.hssarc"
  --return "$(HOME)/.hssarc"
  return full

--rcFileWin = "FIXME"

rcFileWin = do
  root <- getEnv "USERPROFILE" -- e.g. C:\Users\mcarter
  let full = root ++ "\\AppData\\Local\\MarkCarter\\hssa\\hssa.cfg"
  return full

rcFile = if isLinux then rcFileLinux else rcFileWin
-}

-- lup = Data.Configurator.lookup

strapp :: String -> String -> String
strapp a b = a ++ b
{-
readConf = do
  --cp <- readfile emptyCP 
  -- print cp
  --return cp
  rv <- runErrorT $
        do
          cp <- join $ liftIO $ readfile empty rcFile
          let x = cp
          liftIO $ putStrLn "In the test"
          nb <- get x "DEFAULT" "nobody"
          liftIO $ putStrLn nb
          foo <- get x "DEFAULT" "foo"
          liftIO $ putStrLn foo
          return "done"
  print rv
-}

{-
readConf = do
  strOrExc <- try $ readFile rcFile
  case strOrExc of
    Left except -> print except
    Right contents -> putStrLn contents

-}

{-
-- FIXME looks like it contains some important information
readConfNice = do
  r <- tryJust (guard . isDoesNotExistError) $ readFile rcFile
  print $ show r
-}

readConf :: IO [String]
readConf = do
-- FIXME NOW
  rc <- rcFile
  cfg <- load [Optional rc]

  --display cfg
  batch <- lookupDefault "" cfg "prefix"
  -- putStrLn batch
  let globname = pack $ strapp batch  "globs"
  globs <- lookupDefault ["*"] cfg globname :: IO ([String])
  return globs
  --globs <- lup cfg  "globs" -- :: Maybe Text
  {-
  print globs
  files <- glob $ head globs
  print files
  putStrLn "."
-}
