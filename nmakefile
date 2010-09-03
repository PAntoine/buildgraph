OBJECTS= build_graph.obj bg_utilities.obj bg_constants.obj bg_output.obj

all: buildgraph.exe buildgraph.lib 

clean: 
	@del *.lib *.exe *.obj 

buildgraph.exe : $(OBJECTS) 
	@link /DEBUG /NOLOGO /out:buildgraph.exe $** ws2_32.lib

buildgraph.lib: bg_utilities.obj
	@lib /NOLOGO /out:buildgraph.lib bg_utilities.obj

$(OBJECTS): $*.c build_graph.h
	cl /nologo /Zi /c $*.c 

