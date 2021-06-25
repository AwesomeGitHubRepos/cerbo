# sine wave (blocking)

A sine wave of 440Hz actually sounds pretty good at a measly sampling rate of 4kHz.

Computing 1000 blocks of 512 floats each takes approx 20,000 microseconds. 
So one block takes 20us. This is fast enough, even at 44kHz, not to require
threading.


## Status

2021-06-25 Started. Works
