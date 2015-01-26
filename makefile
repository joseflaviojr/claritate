CC=gcc
CXX=g++
RM=rm -f

SRCS=Claritate.cpp Aplicacao.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: Claritate

Claritate: Claritate.o Aplicacao.o
	$(CXX) Claritate.o Aplicacao.o -o Claritate

Claritate.o: Claritate.h Claritate.cpp

Aplicacao.o: Aplicacao.cpp

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) Claritate
