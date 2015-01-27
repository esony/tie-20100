# Makefile - tiraka, uds
# 12.10.2005 HJ - initial revision
# 27.10.2006 HJ - removed -O2 from eff and added rm in clean
# 24.06.2009 SK - added *~ in clean (removes now emacs' backup files)
# 26.04.2010 SK - added rule to run style with correct config
# 16.06.2011 SK - added -w in style and -Wextra for compiling with g++
# 17.06.2011 SK - allows only datastructure.cc, .hh and main.cc to be compiled
# 23.01.2015 MM - adapted to run tests directly                                

.PHONY: all

SRC=main.cc datastructure.cc
HDR=datastructure.hh
TESTDIR=../test_material
RESULTDIR=../results
EXERCISE=hw1
TESTSRC=$(CURDIR)/datastructure.cc
TESTHDR=$(CURDIR)/datastructure.hh

all: eff

prog: $(SRC) $(HDR)
	tutg++ -o prog $(SRC)

eff: $(SRC) $(HDR)
	g++ -std=c++11 -Wall -Wextra -pedantic -Werror $(SRC) -o eff

test: $(SRC)
	cd $(TESTDIR) && python3 run_tests.py $(TESTHDR) $(TESTSRC)

publish:
	[ -d ~/public_html ] || mkdir ~/public_html
	chmod o+rx ~/public_html
	cp -r $(RESULTDIR) ~/public_html
	chmod -R o+rX ~/public_html/results
	chmod -R o+r ~/public_html/results/
	@echo
	@echo Results are viewable in http://www.cs.tut.fi/~$(USER)/results/$(EXERCISE)/

clean:
	rm -f prog eff *~

style:
	/home/style/style -c /home/boss/style/tiraka -w $(SRC)
