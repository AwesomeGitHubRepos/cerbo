-- {-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE OverloadedStrings #-}

module Config where

import Control.Exception
import Control.Monad
import Data.Text (Text, unpack, pack)
import System.IO.Error
import System.Path.Glob

--import Control.Monad.Error
--import Data.ConfigFile
import Data.Configurator
import Data.Configurator.Types


--rcFile = "/home/mcarter/.hssarc"
rcFile = "$(HOME)/.hssarc"

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

readConfNice = do
  r <- tryJust (guard . isDoesNotExistError) $ readFile rcFile
  print $ show r

readConf :: IO [String]
readConf = do
-- FIXME NOW
  cfg <- load [Optional rcFile]

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
