--module Ssah.Stack (Stack, empty, isEmpty, push, top, pop) where
module Stack where

-- https://wiki.haskell.org/Abstract_data_type#Stack


{-

empty :: Stack a
isEmpty :: Stack a -> Bool
push :: a -> Stack a -> Stack a
top :: Stack a -> a
pop :: Stack a -> (a,Stack a)

newtype Stack a = StackImpl [a] -- opaque!
empty = StackImpl []
isEmpty (StackImpl s) = null s
push x (StackImpl s) = StackImpl (x:s)
top (StackImpl s) = head s
pop (StackImpl (s:ss)) = (s,StackImpl ss)
-}
