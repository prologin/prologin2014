all: subject.pdf

finale.tex: ../doc/finale.rst
	pandoc -o finale.tex ../doc/finale.rst

api.tex: ../prologin2014.yml
	generator apidoc ../prologin2014.yml .
	mv file.tex api.tex

subject.pdf: subject.tex subject_content.tex useapi.tex finale.tex api.tex prologin2014.pdf image.pdf
	xelatex subject.tex

clean:
	rm -f *.toc
	rm -f *.log
	rm -f *.out
	rm -f *.idx
	rm -f *.aux

distclean: clean
	rm -f api.tex
	rm -f finale.tex
	rm -f *.pdf
