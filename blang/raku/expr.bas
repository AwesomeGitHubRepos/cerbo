print "expect 42:"
push 30+10+2 # expect 42
call print

print "Expect 158:"
push 30 + 40 -5 - 7 + 100 # expect 158
call print

print "Expect 7:"
print 1+2*3

print "Expect 2.666667:"
print 5/3+1
