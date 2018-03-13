' testing cut, TAB, lmatch, FmtNum
let s := "foo	bar"
print(cut(s, 1)) ' s/b foo
print(cut(s, 2)) ' s/b bar
print(lmatch("ntran", "ntranfoo"))
print(lmatch("ntran", "xntranfoo"))
print("TAB" + TAB() + "TAB")
print(lmatch("ntran", "ntranfoo"))
print(FmtNum(12, "%04.0f"))
