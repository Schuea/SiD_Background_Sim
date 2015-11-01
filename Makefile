CC=g++ #compiler 
CFLAGS=-g -O2 -std=c++1y $(shell root-config --cflags) -Wfatal-errors #flags for compiler
INCLUDES=-I src/
#INCLUDES=-I ~/phd/code/tinyxml/ -I ~/atlas_style/atlasstyle-00-03-05/ $(EGAMMA_UTILS_PATH) -I ../include #folders to include
LIBS=$(shell root-config --libs) -L ~/phd/code/photons/tmva/TMVA-v4.2.0/lib -l TMVA -l ASImage -l GLEW -l FTGL -l RGL

#OBJECT FILES
MAIN=objects/CaloHitsMap.o
IDDECODER=objects/CaloIDs.o

all: Run 

Run: $(MAIN) $(IDDECODER)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LIBS) -o $@

objects/CaloHitsMap.o: src/CaloHitsMap.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/CaloIDs.o: src/CaloIDs.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-rm -f objects/*.o ./Run
