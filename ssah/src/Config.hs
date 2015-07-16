{-# LANGUAGE TemplateHaskell #-}
{-# LANGUAGE OverloadedStrings #-}

module Config where

import Control.Exception
import Control.Monad
import Data.ByteString.Char8 as B
import Data.FileEmbed
--import Data.Text (Text, unpack, pack)
import Data.Text as T
import System.Environment (getEnv)
import System.IO.Error
import System.Info (os)
import System.Path.Glob

--import Control.Monad.Error
--import Data.ConfigFile
import Data.Configurator
import Data.Configurator.Types

isLinux = os == "linux"

iops :: IO String -> String -> IO String
iops iostr str = do
  str1 <- iostr
  return (str1 ++ str)

dcb :: ByteString
dcb = $(embedFile "resources/hssa.cfg")
defaultConfig :: String
defaultConfig = B.unpack dcb
  


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



strapp :: String -> String -> String
strapp a b = a ++ b


readConf :: IO [String]
readConf = do
  rc <- rcFile
  cfg <- load [Optional rc]
  batch <- lookupDefault "" cfg "prefix"
  let globname = T.pack $ strapp batch  "globs"
  globs <- lookupDefault ["*"] cfg globname :: IO ([String])
  return globs
