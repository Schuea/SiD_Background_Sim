//============================================================================
// Name        : CaloHitsMap.cpp
// Author      : Anne Schuetz
// Description : Executable ROOT macro for drawing std::maps of the hits in certain
//				 calorimeter layers
//============================================================================

#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "DrawingHistograms.h"

void Usage();
void CheckArguments(int argc, char * argv[], std::vector<std::string> *inputfilenames, std::vector<std::string> *subdetectors, std::string *outputfilename);
extern int NUMBER_OF_FILES;
extern int first_layer_to_be_compared;
extern int last_layer_to_be_compared;
extern bool NUMBER_OF_FILES_set = false;
extern bool inputfile_set = false;
extern bool outputfile_set = false;
extern bool subdetector_set = false;


  int main(int argc, char * argv[]){

  std::vector<std::string> *inputfilenames = new std::vector<std::string>();
  std::vector<std::string> *subdetectors = new std::vector<std::string>();
  std::string *outputfilename = new std::string("");
  NUMBER_OF_FILES = 0;

  CheckArguments(argc, argv, inputfilenames, subdetectors, outputfilename);

  if (!inputfile_set || !outputfile_set || !subdetector_set || !NUMBER_OF_FILES_set) {
    std::cerr
      << "You didn't give the name for the subdector, the outputfile, the inputfiles or the amount of files. Please try again!"
      << std::endl;
    Usage();
  }

  try {
    DrawingMacro(*outputfilename, *inputfilenames, *subdetectors);
  } catch (std::exception& e) {
    std::cerr << "Something went wrong with drawing or saving the histograms.\n" << e.what();
  }

  return 0;
}

void CheckArguments(int argc, char * argv[],  std::vector<std::string> *inputfilenames, std::vector<std::string> *subdetectors, std::string *outputfilename){
  if (argc < 2) {
    Usage();
  }
  //First look for "-h", and store the number of files ("-n"):
  for (int i = 1; i < argc; i++) {
    if (argv[i] == std::string("-h") || argv[i] == std::string("--help=")) {
      Usage();
    }
    if (argv[i] == std::string("-n")) {
      if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-s")
          && argv[i + 1] != std::string("-o") && argv[i + 1] != std::string("-i")
          && argv[i + 1] != std::string("-l")) {
        NUMBER_OF_FILES = std::stoi(argv[i + 1]);
        std::cout << "Number of input files = " << NUMBER_OF_FILES << std::endl;
        NUMBER_OF_FILES_set = true;
      } else {
        std::cerr << "You didn't give an argument for the number of files!" << std::endl;
        Usage();
      }
    }
  }
  //Find the input and output file, and the subdetectors:
  for (int i = 1; i < argc; i++) {
    if (argv[i] == std::string("-i")) {
      if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-o")
          && argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-s")
          && argv[i + 1] != std::string("-l")) {
        int j = 1;
        do {
          if (argv[i + j] != std::string("-h") && argv[i + j] != std::string("-o")
              && argv[i + j] != std::string("-n") && argv[i + j] != std::string("-s")
              && argv[i + j] != std::string("-l")) {
            inputfilenames->push_back(argv[i + j]);
          } else {
            break;
          }
          ++j;
        } while (j <= NUMBER_OF_FILES);
        inputfile_set = true;
      } else {
        std::cerr << "You didn't give an argument for the inputfile(s)!" << std::endl;
        Usage();
      }
    }
    if (argv[i] == std::string("-o")) {
      if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-s")
          && argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-i")
          && argv[i + 1] != std::string("-l")) {
        (*outputfilename) = argv[i + 1];
        outputfile_set = true;
      } else {
        std::cerr << "You didn't give an argument for the outputfile!" << std::endl;
        Usage();
      }
    }
    if (argv[i] == std::string("-s")) {
      if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-o")
          && argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-i")
          && argv[i + 1] != std::string("-l")) {
        int j = 1;
        while (argv[i + j] != NULL && argv[i + j] != std::string("-h") && argv[i + j] != std::string("-o")
            && argv[i + j] != std::string("-n") && argv[i + j] != std::string("-i")
            && argv[i + j] != std::string("-l")) {
          subdetectors->push_back(argv[i + j]);
          std::cout << "Subdetectors = " << subdetectors->at(j - 1) << std::endl;
          ++j;
        }
        subdetector_set = true;

      } else {
        std::cerr << "You didn't give an argument for the subdetector!" << std::endl;
        Usage();
      }
    }
    if (argv[i] == std::string("-l")) {
      if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-o")
          && argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-i")
          && argv[i + 1] != std::string("-s")) {

        std::string layer_range = argv[i + 1];
        if (layer_range.find("-") != std::string::npos) {
          std::size_t found = layer_range.find_last_of("-");

          if (layer_range.substr(0, found) != std::string("")) {
            first_layer_to_be_compared = std::stoi(layer_range.substr(0, found));
          } else {
            std::cerr << "You didn't give the start point of the range for the layers you want to compare!"
              << std::endl;
            Usage();
          }
          if (layer_range.substr(found) != std::string("")) {
            last_layer_to_be_compared = std::stoi(layer_range.substr(found + 1));
            std::cout << "last_layer_to_be_compared = " << last_layer_to_be_compared << std::endl;
          } else {
            std::cout << "You didn't give an end of the range for the layers you want to compare.\n"
              "So I assume you want to look at all the layers from layer "
              << first_layer_to_be_compared << " on." << std::endl;
          }
        } else {
          std::cerr << "You didn't give a proper range for the layers you want to compare!\n"
            "Please give a range like: 4-6" << std::endl;
          Usage();
        }

      } else {
        std::cerr << "You didn't give an argument for the layers you want to compare!" << std::endl;
        Usage();
      }
    }
  }
}
void Usage() {
  //explain how to use program
  std::cerr << "Usage:" << std::endl;
  std::cerr
    << "Type in the name of the subdetector you are interested in, as well as the output filename, the number of input files and the input root filenames after their correspondent flags!"
    << std::endl;
  std::cerr << "e.g. ./DrawHistograms -s EcalBarrel -o output.root -n 2 -i file1.root file2.root" << std::endl;
  std::cerr
    << "You can also give a range of layers you want to compare in the 1D histograms: -l 2-5  or  -l 5- (from layer 5 on all the layers that are there)"
    << std::endl;
  std::cerr << "e.g. ./DrawHistograms -l 2-5 -s EcalBarrel -o output.root -n 2 -i file1.root file2.root" << std::endl;
  std::terminate();
}
