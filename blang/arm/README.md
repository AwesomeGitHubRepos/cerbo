# Arm compiler for BASIC

## Prerequisites

```
zef install Template::Mustache
```

On Ubuntu:
```
sudo apt install gcc-arm-linux-gnueabi
#sudo apt install qemu-system-arm # possibly unnecessary
sudo apt install qemu-user # 

```


## Example

```
perl6 blang.p6 <test.bas >test.S
arm-linux-gnueabi-gcc -static test.S -o test
./test

```

For the `test.bas` file, you can do:
```
make
./test
```

## References

* [My blog article](https://mcturra2000.wordpress.com/2019/12/26/poc-perl6-raku-basic-assembler/)

