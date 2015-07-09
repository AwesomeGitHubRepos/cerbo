module Scratch where

data Foo = Foo { f1::Int , f2 :: DerBar} deriving Show
data Bar = Bar { b ::Int } deriving Show


data DerBar = Underived | Found Bar | Failed deriving Show

f = Foo 12 Underived

--newtype Baz = Foo | Bar
