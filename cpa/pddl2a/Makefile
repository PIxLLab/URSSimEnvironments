CC := g++

CFLAGS := -c -std=c++11 -Wall -O3
LFLAGS := -lfl

OBJS := lcp.tab.o lex.yy.o main.o define.o reader.o action.o planner.o state.o
HDR := action.h define.h planner.h reader.h state.h timer.h utility.h

cpa: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o cpa.pddl2pl

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) $<

lex.yy.c: lcp.lex
	flex lcp.lex

lcp.tab.o: lcp.tab.c
	$(CC) $(CFLAGS) $<

lcp.tab.c: lcp.y
	bison -d -v lcp.y
	mv lcp.tab.h lcp.tab.hpp

%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o *~ lex.yy.c lcp.tab.c lcp.tab.hpp lcp.output cpa.pddl2pl
