TEX    := report.tex
PDFTMP := report.pdf
OUTPDF := report_Kernel_Rebooters_Vision_Division.pdf
LATEX  := pdflatex

.PHONY: all clean

all: $(OUTPDF)

$(PDFTMP): $(TEX)
	$(LATEX) -interaction=nonstopmode -halt-on-error $(TEX)
	$(LATEX) -interaction=nonstopmode -halt-on-error $(TEX)

$(OUTPDF): $(PDFTMP)
	mv $(PDFTMP) $(OUTPDF)

	$(MAKE) clean

clean:
	rm -f *.aux *.log *.out *.toc *.lof *.lot *.gz *.nav *.snm *.vrb
