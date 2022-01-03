LANG=minor
EXT=min# file extension: .$(EXT)
LIB=lib# compiler library directory
UTIL=util# compiler library: lib$(LIB).a
RUN=run# runtime directory
#EXS=exs# examples directory
CC=gcc
CFLAGS=-g -DYYDEBUG
AS=nasm -felf32
LD=ld -m elf_i386
LDLIBS=lib$(LANG).a

$(LANG): $(LANG).y $(LANG).l $(LANG).brg
	make -C $(LIB)
	byacc -dv $(LANG).y
	flex -dl $(LANG).l
	pburg -T $(LANG).brg
	$(LINK.c) -o $(LANG) $(ARCH) -I$(LIB) lex.yy.c y.tab.c yyselect.c -L$(LIB) -l$(UTIL)
	make -C $(RUN)
	-cp $(RUN)/librun.a $(LDLIBS)

out: out.asm $(LANG)
	$(AS) out.asm -o out.o
	$(LD) out.o $(LDLIBS) -o out
	./out

arm::
	$(MAKE) $(MFLAGS) CC=gcc ARCH='-DpfARM' RUN=arm AS=as LD=ld

#examples:: $(LANG)
#	make -C $(EXS)

clean::
	make -C $(LIB) clean
	make -C $(RUN) clean
	#make -C $(EXS) clean
	rm -f *.o $(LANG) $(LDLIBS) lex.yy.c y.tab.c y.tab.h y.output yyselect.c *.asm *~
