CC=gcc
CFLAGS=-Wall -Wextra -g

all: prog doc

prog: mystat

mystat: mystat.c
	$(CC) $(CFLAGS) -o $@ $<

mystat.c: mystat.w
	ctangle $<

doc: mystat.dvi

%.dvi: %.tex
	tex $<

%.tex: %.w
	cweave $<

clean:
	rm -f *.idx *.log *.scn *.toc

.PHONY: all prog doc clean
