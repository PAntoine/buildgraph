OBJECTS=build_graph.o bg_utilities.o bg_constants.o bg_output.o

$(info -- $(OBJECTS))

all: buildgraph buildgraph.a 

clean: 
	@$(RM) *.lib buildgraph.exe *.o 

buildgraph : $(OBJECTS) 
	@echo $*
	@$(CC) -o buildgraph $* $(OBJECTS)

buildgraph.a: bg_utilities.o
	@$(AR) -r buildgraph.a bg_utilities.o

$(OBJECTS): build_graph.h
	@echo --- $*
	$(CC) -c $*.c 

