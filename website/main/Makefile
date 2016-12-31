HTDOCS = $(shell www main)
#DEST = /mnt/cubie/var/www/main/htdocs/                               


.PHONY : update

update:
	echo Updating main to $(HTDOCS)
	rmjunk
	rsync -avz --delete `pwd`/htdocs/ $(HTDOCS)/

#install :
#	rm $(ETC)/default
#	ln -s  `pwd`/main.cfg $(ETC)
#	service nginx restart
