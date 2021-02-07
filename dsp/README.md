
## brownian

`brown.c` implements Doney algorithm, which suffers from the problem that long wavelengths can be generated, making the sound
difficult to hear.

Consider, instead, my `choco` implementation.

Links:
* [ThinkDSP](https://www.reddit.com/r/DSP/comments/bx550i/thinkdsp_brownian_noise_and_audacity/) - discussing problems with Downey's algorithm


## chocolate noise

Well, that is just my daft choice of words. `choco.c` does seem to produce Brownian-like noise. The algorithm could
possibly be simplified. The trick is to use a 'leaky integrator':
```
y[n+1] = 0.999 * y[n] + x[n]
where
x is white noise
y is the signal value
```

It also implements the trick which Audacity seems to use, whereby if `x[n]` takes it outside the range of values,
use subtraction rather than addition.


## pink

Links:
* [Easy generation](http://www.firstpr.com.au/dsp/pink-noise/)


## sampling

* [sampling.ipynb](sampling.ipynb) - generate signals of various sorts


## Other references

* [Differentiation and integration of audio signals](http://pcfarina.eng.unipr.it/Differentiation_Integration.htm)
* [Simplest lowpass filter](https://www.dsprelated.com/freebooks/filters/Simplest_Lowpass_Filter_I.html) **excellent**

