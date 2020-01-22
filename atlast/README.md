# atlast

John Walker's excellent Forth, based on version 2.0.

# Examples

### FREAD

```
13 constant BLEN
blen string SSIN
stdin blen ssin  fread ssin  + 0 swap !
ssin type cr
```

