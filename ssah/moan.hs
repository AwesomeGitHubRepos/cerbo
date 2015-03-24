module Main where


--import System.Locale (defaultTimeLocale)
--import Data.Time

--import Data.Time.Format (formatTime)
--import Data.Time.LocalTime (getCurrentTimeZone)

import System.IO


justThis j err =
  case j of
    Just x -> x
    Nothing -> error err



justThis1 (Just a) err = return a
justThis1 Nothing  err = error err


main = do
  let x = justThis (Just 10) "This shouldn't fail"
  print x
  let y = justThis Nothing "Oh no, spaghhtie-o"
  print y
  return 1

  
