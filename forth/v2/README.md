# bbf - barebones forth v2

Indirect threading is used in this version. That is to say that it uses pointers to locations that in turn point to machine code. This is in contrast to
v1, which stored the address of the dictionary header.

## Glossary

**cfa** - code field address
