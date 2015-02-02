"""My blog discussion:
http://wp.me/p2eZvw-Nn
Discussion on Stackoverflow:
http://is.gd/oPKAtg
"""

from pyDatalog import pyDatalog
from pyDatalog.pyDatalog import create_terms as terms
from pyDatalog.pyDatalog import ask

pyDatalog.create_terms('scale') # the long way of doing it
terms('A, B, C, V')

scale['meter', 'inch'] = 39.3700787
scale['mile', 'inch'] = 63360.0
scale['feet', 'inch'] = 12.0

scale[A, B] =  1/scale[B, A]
scale[A,B] = scale[A,C] * scale[C, B]

print(scale['inch', 'meter'] == V)
print(scale['mile', 'meter'] == V)

scale['league', 'mile'] = 3.45233834
print(scale['league', 'meter'] == V)

terms('conv')
conv[V, A, B] = V * scale[A, B]
print(conv[3, 'mile', 'meter'] == V)
print(conv[1, 'meter', 'feet'] == V)

