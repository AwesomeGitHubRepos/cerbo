brown : brown.c
	gcc $^ -o $@
	
white: white.c
	gcc $^ -o $@

noise.wav : noise.raw
	sox -t raw -e unsigned -b 8 -r 44100 -c 1 noise.raw  noise.wav

spectrum.png : spectrum.txt plotspec.py
	python3 plotspec.py

clean:	
	rm -f spectrum.txt 	\
		noise.aux noise.bbl noise.blg noise.log \
		noise.out noise.pdf noise.synctex.gz

