# Makefile-c: An example Makefile for CMPUT 379 (incomplete)
#
# Usage: make -f Makefile-c starter  // compile program
#        make -f Makefile-c run      // run the program
#	      	 	     	      // Note: making this target may trigger
#				      // compiling the program	 
#        make -f Makefile-c tar      // create a 'tar.gz' archive of 'allFiles'
#        make -f Makefile-c scp  // send '$(target).tar.gz' by scp to 'scpDir'
#        make -f Makefile-c clean     // remove unneeded files
# ------------------------------------------------------------

# ------------------------------------------------------------
directory=src
filename=main
target=$(filename)

source=$(directory)/$(filename).cpp \
		$(directory)/game.cpp \
		$(directory)/union_find.cpp \
		$(directory)/utility.cpp

allFiles=Makefile $(source) *.h
compiler=g++ -Wall -Wextra -Werror -pedantic -g
# ------------------------------------------------------------

HexBot: $(source)
	$(compiler) $(source) -o $@

# Nothing to be done for clean
clean: