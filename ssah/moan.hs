import Data.Char

readConf = do
  print "I'm reading the file"
  x <- readFile "/etc/resolv.conf"
  return x

calc x = ( 

main = do
  x <- readConf
  print (length x)
  print (map toUpper x)
  
