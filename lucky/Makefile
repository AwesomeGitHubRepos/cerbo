.PHONY : run install

run :
	python3 lucky.py

install :
	cp lucky.cfg /mnt/cubie-root/etc/nginx/sites-enabled
	ssh vivo sudo service nginx restart
