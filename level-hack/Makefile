SDCC = sdcc
SDCCFLAGS = -mstm8 --opt-code-speed --max-allocs-per-node 100000

.PHONY: all clean

all: level-hack.ihx

level-hack.ihx: level-hack.c
	$(SDCC) $(SDCCFLAGS) $<

clean:
	rm -f level-hack.asm level-hack.cdb level-hack.lk  level-hack.lst  level-hack.map  level-hack.rel  level-hack.rst  level-hack.sym

