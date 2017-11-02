CXX=g++
CXXFLAGS=-Wall -ggdb3

all: dedup #doc

dedup: dedup.o
	$(CXX) -o $@ $<

dedup.cpp: dedup.w
	ctangle $< - $@

doc: dedup.pdf

dedup.pdf: dedup.tex
	tex "\let\pdf+\input $<"
	dvipdfm dedup.dvi

dedup.tex: dedup.w
	cweave $<

.PHONY: all doc
