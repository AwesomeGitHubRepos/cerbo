module Ssah.Utils where

stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v
