# Makefile for src/mod/logs2html.mod/

srcdir = .


doofus:
	@echo ""
	@echo "Let's try this from the right directory..."
	@echo ""
	@cd ../../../ && make

static: ../logs2html.o

modules: ../../../logs2html.$(MOD_EXT)

../logs2html.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -DMAKING_MODS -c $(srcdir)/logs2html.c
	@rm -f ../logs2html.o
	mv logs2html.o ../

../../../logs2html.$(MOD_EXT): ../logs2html.o
	$(LD) -o ../../../logs2html.$(MOD_EXT) ../logs2html.o $(XLIBS) $(MODULE_XLIBS) 
	$(STRIP) ../../../logs2html.$(MOD_EXT)

depend:
	$(CC) $(CFLAGS) -MM $(srcdir)/logs2html.c -MT ../logs2html.o > .depend

clean:
	@rm -f .depend *.o *.$(MOD_EXT) *~
distclean: clean

#safety hash
../logs2html.o: .././logs2html.mod/logs2html.c \
 ../../../src/mod/module.h ../../../src/main.h ../../../src/lang.h \
 ../../../src/eggdrop.h ../../../src/flags.h ../../../src/proto.h \
 ../../../lush.h ../../../src/misc_file.h ../../../src/cmdt.h \
 ../../../src/tclegg.h ../../../src/tclhash.h ../../../src/chan.h \
 ../../../src/users.h ../../../src/compat/compat.h \
 ../../../src/compat/inet_aton.h \
 ../../../src/compat/snprintf.h ../../../src/compat/memset.h \
 ../../../src/compat/memcpy.h ../../../src/compat/strcasecmp.h \
 ../../../src/compat/strftime.h ../../../src/mod/modvals.h \
 .././logs2html.mod/language.h .././logs2html.mod/tcllogs2html.c \
 .././logs2html.mod/htmloperations.c .././logs2html.mod/fileoperations.c 
