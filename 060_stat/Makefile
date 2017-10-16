CC=gcc
CFLAGS=-Wall -Wextra -g

all: prog doc

prog: mystat

mystat: mystat.c
	$(CC) $(CFLAGS) -o $@ $<

mystat.c: mystat.w *.ch
	ctangle $< *.ch

doc: mystat.dvi

%.dvi: %.tex
	tex $<

%.tex: %.w *.ch
	cweave $< *.ch

clean:
	rm -f *.idx *.log *.scn *.toc

.PHONY: all prog doc clean
