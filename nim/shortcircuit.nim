#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

# does shortcircuiting, so bar() isn't called. Cool

proc foo(): bool =
    echo "hello from foo"
    return false;

proc bar(): bool =
    echo "hello from bar"
    return true;



let res = foo() and bar();
echo res
