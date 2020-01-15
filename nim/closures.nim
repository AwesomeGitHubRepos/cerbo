#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

proc ioto(): proc(): int =
    var i = 0
    proc fn (): int =
        i = i + 1
        return i
    result = fn


var j = iota()
echo j() , j()
var k = iota()
echo k(), k(), k()
echo j()
