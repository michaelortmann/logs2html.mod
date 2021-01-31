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
../logs2html.o: .././logs2html.mod/logs2html.c ../../../src/mod/module.h \
 ../../../src/main.h ../../../config.h ../../../eggint.h ../../../lush.h \
 ../../../src/lang.h ../../../src/eggdrop.h ../../../src/compat/in6.h \
 ../../../src/flags.h ../../../src/cmdt.h ../../../src/tclegg.h \
 ../../../src/tclhash.h ../../../src/chan.h ../../../src/users.h \
 ../../../src/compat/compat.h ../../../src/compat/base64.h \
 ../../../src/compat/inet_aton.h ../../../src/compat/snprintf.h \
 ../../../src/compat/gethostbyname2.h \
 ../../../src/compat/explicit_bzero.h ../../../src/compat/strlcpy.h \
 ../../../src/mod/modvals.h ../../../src/tandem.h \
 .././logs2html.mod/logs2html.h ../../../src/lang.h \
 .././logs2html.mod/language.h .././logs2html.mod/strlcpy.c \
 .././logs2html.mod/fileoperations.c .././logs2html.mod/htmloperations.c \
 .././logs2html.mod/tcllogs2html.c
