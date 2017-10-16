CC=gcc
CFLAGS=-Wall -Wextra -g

all: prog # doc is not made by default

prog: mystat

mystat: mystat.c
	$(CC) $(CFLAGS) -o $@ $<

mystat.c: mystat.w mystat.ch
	ctangle $< mystat.ch

mystat.ch: mystat.w mystat-step*.ch
	ctie -c $@ mystat.w mystat-step*.ch

doc: mystat.pdf

%.dvi: %.tex
	tex $<

%.pdf: %.tex
	pdftex $<

mystat.tex: mystat.w mystat.ch
	cweave $< mystat.ch

clean:
	rm -f *.idx *.log *.scn *.toc

.PHONY: all prog doc clean
