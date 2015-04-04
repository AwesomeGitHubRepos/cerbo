.PHONY : config cookies main

main:
	@echo Be more specific: make config, cookies

config:
	mkdir -p int/calcs
	mkdir -p int/cards
	mkdir -p int/fetches
	mkdir -p int/misc

cookies: # reduce the size of the cookies file
	#mv ~/Downloads/cookies.txt  int/misc/cookies.txt
	fookies | sed -e '/sharelock\|^#/!d' > int/misc/cookies.txt


