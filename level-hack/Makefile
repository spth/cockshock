SDCC = sdcc
SDCCFLAGS = -mstm8 --opt-code-speed --max-allocs-per-node 100000

.PHONY: all

all: level-hack.ihx

level-hack.ihx: level-hack.c
	$(SDCC) $(SDCCFLAGS) $<

