XELATEXARGS = -file-line-error -halt-on-error
IMAGES = remotecontrol.jpeg

.PHONY: clean all

all: cockshock.pdf

cockshock.pdf: cockshock.tex $(IMAGES) cockshock.bbl
	xelatex $(XELATEXARGS) cockshock.tex; xelatex $(XELATEXARGS) cockshock.tex

cockshock.bbl: cockshock.tex $(IMAGES) cockshock.bib
	xelatex $(XELATEXARGS) cockshock.tex; bibtex lospre; rm cockshock.pdf

