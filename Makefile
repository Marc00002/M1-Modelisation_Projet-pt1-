#SYSTEM     = x86-64_osx
#LIBFORMAT  = static_pic
#CPLEXDIR=/Users/renaudchicoisne/Applications/IBM/ILOG/CPLEX_Studio1262/cplex
#CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
#CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
#CPLEXINCDIR   = $(CPLEXDIR)/include


# CHANGEME: This should be the name of your executable
EXE_TP1 = TP1
EXE_GEN = TP1GenInstance
EXE_COURBE = TP1CourbeTemps

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
#OBJS = TP1Functions.o TP1.o 

OBJS_TP1 = TP1Functions.o TP1.o
OBJS_GEN = TP1GenInstance.o
OBJS_COURBE = TP1Functions.o TP1CourbeTemps.o

# CHANGEME: Additional libraries
#ADDLIBS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -m64 -lm -lpthread

# CHANGEME: Additional flags for compilation (e.g., include flags)
#DEFINES= -DSYS_UNIX=1 

#INCL = -I$(CPLEXINCDIR) -g $(DEFINES)

# C Compiler command
CC = gcc

# C Compiler options
CFLAGS = -m64 -fPIC -fexceptions -O3 -pipe -DNDEBUG -Wall -g

CYGPATH_W = echo

#all: $(EXE)
all: $(EXE_TP1) $(EXE_GEN) $(EXE_COURBE)

.SUFFIXES: .c .o .obj

$(EXE_TP1): $(OBJS_TP1)
	bla=;\
	for file in $(OBJS_TP1); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CC) $(CFLAGS) -o $@ $$bla $(LIBS) $(ADDLIBS)

$(EXE_GEN): $(OBJS_GEN)
	bla=;\
	for file in $(OBJS_GEN); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CC) $(CFLAGS) -o $@ $$bla $(LIBS) $(ADDLIBS)

$(EXE_COURBE): $(OBJS_COURBE)
	bla=;\
	for file in $(OBJS_COURBE); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CC) $(CFLAGS) -o $@ $$bla $(LIBS) $(ADDLIBS)

clean:
#	rm -f $(EXE) $(OBJS)
	rm -f $(EXE_TP1) $(EXE_COURBE) $(EXE_GEN) *.o


.c.o:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ $<


.c.obj:
	$(CC) $(CFLAGS) $(INCL) -c -o $@ `$(CYGPATH_W) '$<'`
