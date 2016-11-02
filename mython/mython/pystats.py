#import pdb
import unittest

#from pudb import set_trace
from pyparsing import *


block = Word(alphanums + ".")
#block.ignore(",")
block.setParseAction(lambda t: t[0])

blocks = ZeroOrMore(block)
#blocks.setParseAction(lambda t: t)

def scan(text):
    global blocks
    text = text.replace(",", "")
    tree = blocks.parseString(text)
    #print(type(tree))
    #print(tree)
    #set_trace()
    #print(tree.__dict__)
    toklist =  tree._ParseResults__toklist
    #for v in tree.values: print(v)
    return toklist


def scoop(text):
    
class PystatsTest(unittest.TestCase):

    def assertNear(self, a, b): 
        self.assertTrue(abs(a-b) < 0.001)

    def test_number(self):
        result = scan("12.3")
        self.assertNear(12.3, float(result[0]))

    def test_two(self):
        result = scan("12.3 13.4")
        a, b = result
        #print(a, b)
        self.assertNear(12.3, float(a))
        self.assertNear(13.4, float(b))

    def test_comma(self):
        result = scan("1,234.5")
        #print(result)
        self.assertNear(1234.5, float(result[0]))

    def test_command(self):
        result = scoop("10 12 g")
        self.assertNear(11, result)

if __name__ == "__main__":
    scan("12.3 13.6")
