//============================================================================
// Name        : CaloHitsMap.cpp
// Author      : Anne Schuetz
// Description : Executable ROOT macro for drawing maps of the hits in certain
//				 calorimeter layers
//============================================================================

#include "TFile.h"
#include "TAxis.h"
#include "TBrowser.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TEllipse.h"
#include "TPaveStats.h"
#include "TProfile.h"
#include "TMath.h"
#include "TChain.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>

#include <map>

#include "CaloIDs.h"
#include "CreateCellID.h"
#include "DrawingHistograms.h"

void Usage();
void CheckArguments();

int main(int argc, char * argv[]){

				std::vector<string> inputfilenames;
				std::vector<string> subdetectors;
				string outputfilename;
				bool NUMBER_OF_FILES_set = false;
				bool inputfile_set = false;
				bool outputfile_set = false;
				bool subdetector_set = false;

				CheckArguments(argc, argv);

				if (!inputfile_set || !outputfile_set || !subdetector_set || !NUMBER_OF_FILES_set) {
								cerr
												<< "You didn't give the name for the subdector, the outputfile, the inputfiles or the amount of files. Please try again!"
												<< endl;
								Usage();
				}
				try {
								DrawingMacro(outputfilename, inputfilenames, subdetectors);
				} catch (std::exception& e) {
								std::cerr << "Something went wrong with drawing or saving the histograms.\n" << e.what();
				}

				return 0;
}

void CheckArguments(int argc, char * argv[]){
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
																cerr << "You didn't give an argument for the number of files!" << endl;
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
																								inputfilenames.push_back(argv[i + j]);
																				} else {
																								break;
																				}
																				++j;
																} while (j <= NUMBER_OF_FILES);
																inputfile_set = true;
												} else {
																cerr << "You didn't give an argument for the inputfile(s)!" << endl;
																Usage();
												}
								}
								if (argv[i] == std::string("-o")) {
												if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-s")
																				&& argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-i")
																				&& argv[i + 1] != std::string("-l")) {
																outputfilename = argv[i + 1];
																outputfile_set = true;
												} else {
																cerr << "You didn't give an argument for the outputfile!" << endl;
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
																				subdetectors.push_back(argv[i + j]);
																				std::cout << "Subdetectors = " << subdetectors.at(j - 1) << std::endl;
																				++j;
																}
																subdetector_set = true;

												} else {
																cerr << "You didn't give an argument for the subdetector!" << endl;
																Usage();
												}
								}
								if (argv[i] == std::string("-l")) {
												if (argv[i + 1] != NULL && argv[i + 1] != std::string("-h") && argv[i + 1] != std::string("-o")
																				&& argv[i + 1] != std::string("-n") && argv[i + 1] != std::string("-i")
																				&& argv[i + 1] != std::string("-s")) {

																string layer_range = argv[i + 1];
																if (layer_range.find("-") != string::npos) {
																				std::size_t found = layer_range.find_last_of("-");

																				if (layer_range.substr(0, found) != std::string("")) {
																								first_layer_to_be_compared = std::stoi(layer_range.substr(0, found));
																				} else {
																								cerr << "You didn't give the start point of the range for the layers you want to compare!"
																												<< endl;
																								Usage();
																				}
																				if (layer_range.substr(found) != std::string("")) {
																								last_layer_to_be_compared = std::stoi(layer_range.substr(found + 1));
																								cout << "last_layer_to_be_compared = " << last_layer_to_be_compared << endl;
																				} else {
																								std::cout << "You didn't give an end of the range for the layers you want to compare.\n"
																												"So I assume you want to look at all the layers from layer "
																												<< first_layer_to_be_compared << " on." << std::endl;
																				}
																} else {
																				cerr << "You didn't give a proper range for the layers you want to compare!\n"
																								"Please give a range like: 4-6" << endl;
																				Usage();
																}

												} else {
																cerr << "You didn't give an argument for the layers you want to compare!" << endl;
																Usage();
												}
								}
				}
}

