.SUFFIXES: .tex .dvi .pdf .obj .eps .aux .bb .tgz .tar .gz .out \
       .bz2 .tbz .zip .jpg .jpeg .bmp .png .log .toc .lof .lot \
       .idx .glo .ilg .glg .gls .ltx .dtx .bib .bbl .blg

VERSION := $(shell date +%Y-%m-%d)
ID := $(shell whoami)

# required sources
MAIN   = main
SRC    = section1.tex section2.tex
ETC    = 

# program setting
EDITOR =       openemacs
TEX    =       platex -kanji=utf8 -src-specials -no-shell-escape 
BIBTEX =       jbibtex -kanji=utf8
DVIPDF =       dvipdfmx
XDVI   =       open -a Mxdvi.app
XPDF   =       open -a Preview.app
REFGREP        =       grep "^LaTeX Warning: Label(s) may have changed."
EBB    =       ebb

# sub-directory setting
# JPEG, PNG などのビットマップ画像の保存先
IMGDIR =       img
# PDF の保存先
PDFDIR =       pdf
# OmniGraffle 等で作成したオリジナルファイル（dvipdfmx で未対応）
PICTDIR        =       pict
# 表の原稿ファイル
TABLEDIR       =       table
# Gnuplot のプロットデータや CSV ファイルの格納場所
PLOTDIR        =       plt
#
ETCDIR =       tex
# バックアップファイルの名前
DISTDIR        =       $(ID)-$(VERSION)

# default target
all:   dvi

# aliases
dvi:   $(MAIN).dvi
pdf:   $(MAIN).pdf
bib:   $(MAIN).bbl

# indipendency
$(MAIN).dvi:   $(MAIN).tex $(SRC)
$(MAIN).pdf:   $(MAIN).dvi
$(MAIN).aux:   $(MAIN).tex
$(MAIN).bbl:   $(MAIN).bib
$(MAIN).bib:   $(MAIN).aux

# translation rules
.dvi.pdf:
	$(DVIPDF) $<
.tex.dvi:
	$(TEX) $<
	(while grep "^No file $*.lo" $*.log; do $(TEX) $<; done)
	(while $(REFGREP) $*.log; do $(TEX) $<; done)
.bib.bbl:
	$(BIBTEX) $*
.tex.aux:
	$(TEX) $<
#
#
fast:
	$(TEX) -interaction=batchmode $(MAIN) 
view: $(MAIN).dvi
	$(XDVI) $< &
viewpdf: $(MAIN).pdf
	$(XPDF) $< &

edit:
	$(EDITOR) $(MAIN).tex & 

dist: clean
	mkdir $(DISTDIR)
	cp -r $(IMGDIR) $(PDFDIR) $(PICTDIR) $(TABLEDIR) \
	    $(PLOTDIR) $(ETCDIR) $(DISTDIR)
	cp $(SRC) $(MAIN).tex $(ETC) Makefile $(DISTDIR)
	tar cjf ../$(DISTDIR).tar.bz2 $(DISTDIR)
	rm -fr $(DISTDIR)

clean:
	rm -f *~
	rm -f *.{aux,bbl,blg} *.log $(MAIN).{lof,lot,toc,dvi,pdf,out}

help:
	@echo -e "\nusage: make [<target>]\n"
	@echo -e "all:\tDVI ファイル $(MAIN).dvi を作成する"
	@echo -e "bib:\t文献ファイル $(MAIN).bbl を作成する"
	@echo -e "clean:\t一時ファイルを全て削除する"
	@echo -e "dist:\tバックアップ $(DISTDIR).tar.bz2 を一つの上のフォルダに作成する"
	@echo -e "dvi:\tDVI ファイル $(MAIN).dvi を作成する"
	@echo -e "edit:\tテキストエディッタ $(EDITOR) で $(MAIN).tex を開く\t"
	@echo -e "fast:\t依存関係を無視して $(MAIN).tex をコンパイルする"
	@echo -e "help:\tこのメッセージを表示する"
	@echo -e "pdf:\tPDF ファイル $(MAIN).pdf を作成する"
	@echo -e "view:\tDVI を $(XDVI) で開く"
	@echo -e "viewpdf:\tPDF を $(XPDF) で開く\n"

