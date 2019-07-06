import numpy as np
import matplotlib.pyplot as plt
lines = open('spectrum.txt').readlines()
xs = np.array([])
ys = np.array([])
for line in lines[1:]:
    x, y = map(float, line.split("\t"))
    #xs = np.append(xs, np.log10(x))
    xs = np.append(xs, x)
    ys = np.append(ys, y)

fig = plt.figure()    
ax = fig.add_subplot(111)
ax.grid(which='major')
ax.set_xscale('log')
def log_10_product(x, pos):
    """The two args are the value and tick position.
    Label ticks with the product of the exponentiation"""
    return '%1i' % (x)

formatter = plt.FuncFormatter(log_10_product)
ax.xaxis.set_major_formatter(formatter)

plt.xlabel('Frequency (Hz)')
plt.ylabel('Level (dB)')
ax.set_xlim([50,20000])
ax.fill_between(xs, ys, -60)
ax.set_ylim([-60,0])
ax.plot(xs, ys)
plt.savefig('spectrum.png')
