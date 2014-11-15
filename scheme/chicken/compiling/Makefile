.PHONY : clean

foo : foo.scm bar.scm
	csc -c bar.scm
	csc -c foo.scm
	csc foo.o bar.o -o foo

clean :
	rm -f *o foo
