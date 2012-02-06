
all: pdf

pdf: 
	pdflatex monografia_bellator.tex
	bibtex monografia_bellator.aux
	pdflatex monografia_bellator.tex
	pdflatex monografia_bellator.tex

clean:
	rm *.aux
	rm *.bbl
	rm *.blg
	rm *.dvi
	rm *.lof
	rm *.log
	rm *.lot
	rm *.lsb
	rm *.lsg
	rm *.toc
