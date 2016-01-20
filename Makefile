CC=g++ #compiler
OPTIMISE= -O2
CFLAGS=-g $(OPTIMISE) -std=c++1y $(shell root-config --cflags) #-Wfatal-errors #flags for compiler
INCLUDES=-I include/
LIBS=$(shell root-config --libs) -L ~/phd/code/photons/tmva/TMVA-v4.2.0/lib -l TMVA -l ASImage -l GLEW -l RGL

#OBJECT FILES
MAIN=objects/main.o
DATACLASS=objects/DataClass.o
CREATECELLID=objects/CreateCellID.o
SUBDETECTOR=objects/Subdetector.o
LAYERCODE=objects/LayerCodeInCellID.o
CELLHITS=objects/CellHits.o
USEFULFUNCTIONS=objects/UsefulFunctions.o
TIME=objects/Time.o
OBJECTS = $(DATACLASS) $(CREATECELLID) $(SUBDETECTOR) $(LAYERCODE) $(CELLHITS) $(USEFULFUNCTIONS) $(TIME)

all: Run

Run: $(MAIN) $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LIBS) -o $@

objects/main.o: src/main.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/DataClass.o: src/DataClass.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/LayerCodeInCellID.o: src/LayerCodeInCellID.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/Subdetector.o: src/Subdetector.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/CreateCellID.o: src/CreateCellID.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/CellHits.o: src/CellHits.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

objects/UsefulFunctions.o: src/UsefulFunctions.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/Time.o: src/Time.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-rm -f objects/*.o ./Run
