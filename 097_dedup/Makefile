CXX=g++
CXXFLAGS=-Wall -ggdb3

all: dedup #doc

dedup: dedup.o
	$(CXX) -o $@ $<

dedup.cpp: dedup.w
	ctangle $< - $@

debug: dedup.w dedup-debug.ch
	ctangle dedup dedup-debug.ch debug.cpp
	$(CXX) $(CXXFLAGS) -o $@ debug.cpp

doc: dedup.pdf

dedup.pdf: dedup.tex
	tex "\let\pdf+\input $<"
	dvipdfm dedup.dvi

dedup.tex: dedup.w
	cweave $<

.PHONY: all doc
