ETC = /mnt/cubie-root/etc/nginx/sites-enabled
.PHONY : config

config :
	rm -f $(ETC)/default
	cp main/main.cfg $(ETC)
	cp pdf/pdf.cfg $(ETC)
	ssh cubie "sudo service nginx restart"
