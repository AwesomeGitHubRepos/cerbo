import Control.Concurrent
import Control.Concurrent.Async


calc x = do
  d<- threadDelay (round (x * 1000000.0)) -- x seconds
  return  (10.0 + x)
          
main = do
  let m1 = map calc [3.5, 3.2, 3.6]
  t3 <- mapM async m1
  t4 <- mapM wait t3
  print(t4)

