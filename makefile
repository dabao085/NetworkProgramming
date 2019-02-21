SUBDIRS = client server
all: ${SUBDIRS}
${SUBDIRS}:ECHO
	make -C $@
ECHO:
	@echo $(SUBDIRS)
clean:
	rm -f */*.o