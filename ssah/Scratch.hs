module Scratch where

data FooBase = FooBase { b :: Int } deriving Show
data FooAug = FooAug { a :: Int } deriving Show

data Foo = Foo { c:: FooBase,  q::FooAug} deriving  Show

