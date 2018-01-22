all: lex.yy.cc tiger-grammar.tab.cc

tiger-grammar.tab.cc: tiger-grammar.yy
	bison tiger-grammar.yy

# I could get a .cc by using "%option c++", but that isn't what's done in the example
#  and it breaks a bunch of other stuff that I can't fix tonight :-(
lex.yy.cc: tiger-lex.ll
	flex tiger-lex.ll
	mv lex.yy.c lex.yy.cc

# Something weird is causing Eclipse to be sad after a clean, but a 2nd build does the trick
# Possibly this has to do with renaming lex.yy.c, but I can't figure it out tonight.
# Cleaning here, then "Project -> Clean", does a build that doesn't compile lex.yy.cc
#  (the second build does just that, then succeeds.)
# Thus, tried a bogus line:
#  	touch -d"Jan 1 1970" lex.yy.cc
#  but no luck, it just got weirder.

clean:
	-rm stack.hh tiger-grammar.tab.hh tiger-grammar.tab.cc lex.yy.cc lex.yy.c *~
	@echo " "
	@echo " "
	@echo " ===> CAUTION: First Eclipse build may fail; just do a second <==="
	@echo " "
	@echo " "
