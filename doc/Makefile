XELATEXARGS = -file-line-error -halt-on-error
IMAGES = remotecontrol.jpeg remote-top.jpeg remote-bottom.jpeg remote-main.jpeg shockcommand.png shock.png auto-shutdown-timeout-hack.jpeg level-hack-shock-unit.jpeg

.PHONY: clean all

all: cockshock.pdf

cockshock.pdf: cockshock.tex $(IMAGES) cockshock.bbl
	xelatex $(XELATEXARGS) cockshock.tex; xelatex $(XELATEXARGS) cockshock.tex

cockshock.bbl: cockshock.tex $(IMAGES) cockshock.bib
	xelatex $(XELATEXARGS) cockshock.tex; bibtex lospre; rm cockshock.pdf

