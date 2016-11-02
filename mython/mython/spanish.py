#import string

dic = """
apagado:turned off, quiet, muffled, dull
descubrir:to discover, invent, reveal,uncover
despertar: to wake up, to arouse
dichoso: happy, blessed, annoying, darned
premiar:to award, to reward
regresar: to return
siglo: century, age, ages
tierra: earth, land
"""

top100 = """
el/la (def art) the
de (prep) of, from
que (cong) that, which
y (conj) and
a (prep) to, at
en (prep) in, on
un (indef art) a, an
ser (verb) to be
se (pron) -self, onself [reflexive marker]
no (adv) no
haber (adv) to have
por (prep) by, for, through
con (pre) with
su (adj) his, her, their, your (fam)
para (prep) for, to, in order to
como (conj) like, as
estar (verb) to be
tener (verb) to have
le (pron) [3rd peso indirect object pronoun]
lo (art) the (+ noun)
lo (prom) ]3rd pers masc direct object pronoun[
todo (adj) all, every
pero (conj) but, yet, except
ma's (adj) more
hacer (verb) to do, make
o (conj) or
poder (verb) to be able to, can
decid (verb) to tell, say
este (adj) this (m), esta (f)
ir (verb) to go
31:
"""
def pairs():
	global dic
	for line in dic.splitlines():
		if len(line) == 0: continue
		yield line.split(":", 1)

def all():
	for p in pairs():
		es, en = p
		print(es)
		print(en)
		print()

def main():
	all()

if __name__ == "__main__":
	main()
