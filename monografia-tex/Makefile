
all: pdf

pdf: 
	pdflatex monografia_bellator.tex > /dev/null
	bibtex monografia_bellator.aux
	pdflatex monografia_bellator.tex > /dev/null
	pdflatex monografia_bellator.tex | grep Warning

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
