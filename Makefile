all:
	$(MAKE) -C introlib
	$(MAKE) -C intro
	$(MAKE) -C intro2

clean:
	$(MAKE) -C intro2 clean
	$(MAKE) -C intro clean
	$(MAKE) -C introlib clean

