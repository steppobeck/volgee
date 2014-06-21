default:
	cd src && make

clean:
	cd src && make clean

realclean:
	cd src && make realclean
	@rm -f *~ bin/*~ bin/volgee

