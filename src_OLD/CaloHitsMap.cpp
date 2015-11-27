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
using namespace std;

void Usage();
void DrawingMacro(string outputname, std::vector<std::string> inputnames, std::vector<std::string> subdetectors);
//void DrawingCaloHitsMap(string subdetector, string Canvas_name);
//void DrawingHitsPerEvent(string Canvas_name);

int NUMBER_OF_FILES = 0;
int first_layer_to_be_compared = -999;
int last_layer_to_be_compared = 0;

TFile* inputfile;
TCanvas* Hits_Canvas_;
vector<TH1D*> Hits_PerLayer_;
vector<TH1D*> Hits_Histo_;
vector<TH2D*> Hits_2D_;
vector<TH3D*> Hits_3D_;
vector<TH1D*> Hits_Energy_Histo_;
vector<TH2D*> Hits_Energy_2D_;
vector<TH3D*> Hits_Energy_3D_;

vector<TH2D*> ParticleOrigins_2D_;

int main(int argc, char *argv[]) {

				if (argc < 2) {
								Usage();
				}

				std::vector<string> inputfilenames;
				std::vector<string> subdetectors;
				string outputfilename;
				bool NUMBER_OF_FILES_set = false;
				bool inputfile_set = false;
				bool outputfile_set = false;
				bool subdetector_set = false;

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
								//std::cerr	<< "There is no TTree for a subdetector with the given name in the ROOT files.\n";
								//std::cerr << "You probably misspelled the name of the subdetector."<< std::endl;
				}

				return 0;
}

void Setup_BinningArrays(std::vector<CaloIDs> SubDetectors, std::array<int, 3> *axis_range_plot_1D,
								std::array<float, 3> *axis_range_plot_energy_1D, std::array<int, 9> *axis_ranges_plot) {
				if (SubDetectors.size() == 1) {

								std::array<int, 3> temp1D = { SubDetectors.at(0).getBinning1D().at(0), SubDetectors.at(0).getBinning1D().at(1),
												SubDetectors.at(0).getBinning1D().at(2) };
								*axis_range_plot_1D = temp1D;
								std::array<float, 3> tempenergy1D = { SubDetectors.at(0).getBinningEnergy1D().at(0),
												SubDetectors.at(0).getBinningEnergy1D().at(1), SubDetectors.at(0).getBinningEnergy1D().at(2) };
								*axis_range_plot_energy_1D = tempenergy1D;
								std::array<int, 9> temp3D = { SubDetectors.at(0).getBinning3D().at(0), SubDetectors.at(0).getBinning3D().at(1),
												SubDetectors.at(0).getBinning3D().at(2), SubDetectors.at(0).getBinning3D().at(3),
												SubDetectors.at(0).getBinning3D().at(4), SubDetectors.at(0).getBinning3D().at(5),
												SubDetectors.at(0).getBinning3D().at(6), SubDetectors.at(0).getBinning3D().at(7),
												SubDetectors.at(0).getBinning3D().at(8) };
								*axis_ranges_plot = temp3D;
				} else {
								float maxEnergy1D = 0;
								float minEnergy1D = 0;
								float binsEnergy1D = 0;

								int max1D = 0;
								int min1D = 0;
								int bins1D = 0;

								int xmax = 0;
								int xmin = 0;
								int xbins = 0;
								int ymax = 0;
								int ymin = 0;
								int ybins = 0;
								int zmax = 0;
								int zmin = 0;
								int zbins = 0;

								for (int s = 0; s < SubDetectors.size(); ++s) {

												if (SubDetectors.at(s).getBinningEnergy1D().at(2) > maxEnergy1D) {
																maxEnergy1D = SubDetectors.at(s).getBinningEnergy1D().at(2);
												}
												if (SubDetectors.at(s).getBinningEnergy1D().at(1) > minEnergy1D) {
																minEnergy1D = SubDetectors.at(s).getBinningEnergy1D().at(1);
												}
												if (SubDetectors.at(s).getBinningEnergy1D().at(0) > binsEnergy1D) {
																binsEnergy1D = SubDetectors.at(s).getBinningEnergy1D().at(0);
												}

												if (SubDetectors.at(s).getBinning1D().at(2) > max1D) {
																max1D = SubDetectors.at(s).getBinning1D().at(2);
												}
												if (SubDetectors.at(s).getBinning1D().at(1) > min1D) {
																min1D = SubDetectors.at(s).getBinning1D().at(1);
												}
												if (SubDetectors.at(s).getBinning1D().at(0) > bins1D) {
																bins1D = SubDetectors.at(s).getBinning1D().at(0);
												}

												if (SubDetectors.at(s).getBinning3D().at(5) > xmax) {
																xmax = SubDetectors.at(s).getBinning3D().at(5);
												}
												if (SubDetectors.at(s).getBinning3D().at(4) < xmin) {
																xmin = SubDetectors.at(s).getBinning3D().at(4);
												}
												if (SubDetectors.at(s).getBinning3D().at(3) > xbins) {
																xbins = SubDetectors.at(s).getBinning3D().at(3);
												}
												if (SubDetectors.at(s).getBinning3D().at(8) > ymax) {
																ymax = SubDetectors.at(s).getBinning3D().at(8);
												}
												if (SubDetectors.at(s).getBinning3D().at(7) < ymin) {
																ymin = SubDetectors.at(s).getBinning3D().at(7);
												}
												if (SubDetectors.at(s).getBinning3D().at(6) > ybins) {
																ybins = SubDetectors.at(s).getBinning3D().at(6);
												}
												if (SubDetectors.at(s).getBinning3D().at(2) > zmax) {
																zmax = SubDetectors.at(s).getBinning3D().at(2);
												}
												if (SubDetectors.at(s).getBinning3D().at(1) < zmin) {
																zmin = SubDetectors.at(s).getBinning3D().at(1);
												}
												if (SubDetectors.at(s).getBinning3D().at(0) > zbins) {
																zbins = SubDetectors.at(s).getBinning3D().at(0);
												}
								}
								std::array<float, 3> tempEnergy1D = { binsEnergy1D, minEnergy1D, maxEnergy1D };
								*axis_range_plot_energy_1D = tempEnergy1D;

								std::array<int, 3> temp1D = { bins1D, min1D, max1D };
								*axis_range_plot_1D = temp1D;
								std::array<int, 9> temp = { zbins, zmin, zmax, xbins, xmin, xmax, ybins, ymin, ymax };
								*axis_ranges_plot = temp;
				}
}
void Setup_Histos1D_HitsPerLayer(std::array<int, 3> axis_range_plot_1D, string subdetector_name, int layer) {

				stringstream histo_name, histo_title;
				histo_name << "HitsPerLayer_" << subdetector_name << "_Layer_" << layer;
				histo_title << "Hits for " << subdetector_name << " layer " << layer;
				//Hits_PerLayer_.emplace_back(
				//	new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_1D[0], axis_range_plot_1D[1],axis_range_plot_1D[2]));
				/*EcalBarrel*/Hits_PerLayer_.emplace_back(
												new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_1D[0]*2, axis_range_plot_1D[1],75));
				///*MuonEndcap*/	Hits_PerLayer_.emplace_back(
				//		new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), 50, 0,1500));
				///*BeamCal*/	Hits_PerLayer_.emplace_back(new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_1D[0]*1/4, 4200000,4850000));
				///*ALL:*/Hits_PerLayer_.emplace_back(new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_1D[0], 0, 1500));
				Hits_PerLayer_.at(layer)->SetMinimum(0.1);
				///*ALL*/Hits_PerLayer_.at(layer)->SetMaximum(10000);
				Hits_PerLayer_.at(layer)->SetLineColor(kViolet);
				Hits_PerLayer_.at(layer)->GetYaxis()->SetTitle("Count");
				Hits_PerLayer_.at(layer)->GetXaxis()->SetTitle("Hits per bunch crossing");
				Hits_PerLayer_.at(layer)->GetXaxis()->CenterTitle();
}
void Setup_Histos1D(std::array<int, 3> axis_range_plot_1D, string subdetector_name, int layer) {

				stringstream histo_name, histo_title;
				histo_name << "Hits_" << subdetector_name << "_Layer_" << layer;
				histo_title << "Hit occupancy per cell for " << subdetector_name << " layer " << layer;
				Hits_Histo_.emplace_back(
												new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_1D[0], axis_range_plot_1D[1],
																axis_range_plot_1D[2]));
				///*ALL:*/Hits_Histo_.emplace_back(new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), 150, 0, 2000000));
				Hits_Histo_.at(layer)->SetMinimum(0.1);
				Hits_Histo_.at(layer)->SetLineColor(kViolet);
				Hits_Histo_.at(layer)->GetYaxis()->SetTitle("Occupancy");
				Hits_Histo_.at(layer)->GetXaxis()->SetTitle("Hits per cell");
				Hits_Histo_.at(layer)->GetXaxis()->CenterTitle();
}
void Setup_Histos2D(std::array<int, 9> axis_ranges_plot, string subdetector_name, int layer) {
				stringstream histo_2d_name, histo_2d_title;
				histo_2d_name << "Hits_2D_" << subdetector_name << "_Layer_" << layer;
				histo_2d_title << "HitMap for " << subdetector_name << " layer " << layer;
				Hits_2D_.emplace_back(
												new TH2D(histo_2d_name.str().c_str(), histo_2d_title.str().c_str(), axis_ranges_plot[3],
																axis_ranges_plot[4], axis_ranges_plot[5], axis_ranges_plot[6], axis_ranges_plot[7],
																axis_ranges_plot[8]));

				Hits_2D_.at(layer)->SetContour(100);
				Hits_2D_.at(layer)->GetYaxis()->SetTitle("y (mm)");
				Hits_2D_.at(layer)->GetYaxis()->CenterTitle();
				Hits_2D_.at(layer)->GetXaxis()->SetTitle("x (mm)");
				Hits_2D_.at(layer)->GetXaxis()->CenterTitle();
				Hits_2D_.at(layer)->GetZaxis()->SetTitle("Hit count");
}
void Setup_Histos3D(std::array<int, 9> axis_ranges_plot, string subdetector_name, int layer) {
				stringstream histo_3d_name, histo_3d_title;
				histo_3d_name << "Hits_3D_" << subdetector_name << "_Layer_" << layer;
				histo_3d_title << "HitMap for " << subdetector_name << " layer " << layer;
				Hits_3D_.emplace_back(
												new TH3D(histo_3d_name.str().c_str(), histo_3d_title.str().c_str(), axis_ranges_plot[0],
																axis_ranges_plot[1], axis_ranges_plot[2], axis_ranges_plot[3], axis_ranges_plot[4],
																axis_ranges_plot[5], axis_ranges_plot[6], axis_ranges_plot[7], axis_ranges_plot[8]));
				Hits_3D_.at(layer)->SetLineColor(kViolet);
				Hits_3D_.at(layer)->GetXaxis()->SetTitle("z (mm)");
				Hits_3D_.at(layer)->GetXaxis()->CenterTitle();
				Hits_3D_.at(layer)->GetZaxis()->SetTitle("y (mm)");
				Hits_3D_.at(layer)->GetZaxis()->CenterTitle();
				Hits_3D_.at(layer)->GetYaxis()->SetTitle("x (mm)");
				Hits_3D_.at(layer)->GetYaxis()->CenterTitle();
}

void Setup_EnergyHistos1D(std::array<float, 3> axis_range_plot_energy_1D, string subdetector_name, int layer) {

				stringstream histo_name, histo_title;
				histo_name << "Energy_" << subdetector_name << "_Layer_" << layer;
				histo_title << "Deposited hit energy for " << subdetector_name << " layer " << layer;
				Hits_Energy_Histo_.emplace_back(
												new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot_energy_1D[0],
																axis_range_plot_energy_1D[1], axis_range_plot_energy_1D[2]));
				///*ALL:*/Hits_Energy_Histo_.emplace_back(new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), 40., 0, 3.));
				Hits_Energy_Histo_.at(layer)->SetMinimum(0.1);
				Hits_Energy_Histo_.at(layer)->SetLineColor(kViolet);
				Hits_Energy_Histo_.at(layer)->GetYaxis()->SetTitle("Count");
				Hits_Energy_Histo_.at(layer)->GetXaxis()->SetTitle("Deposited hit energy (GeV)");
				Hits_Energy_Histo_.at(layer)->GetXaxis()->CenterTitle();
}
void Setup_EnergyHistos2D(std::array<int, 9> axis_ranges_plot, string subdetector_name, int layer) {
				stringstream histo_2d_name, histo_2d_title;
				histo_2d_name << "Hits_Energy_2D_" << subdetector_name << "_Layer_" << layer;
				histo_2d_title << "HitMap with the average hit energy per bin for " << subdetector_name << " layer " << layer;
				Hits_Energy_2D_.emplace_back(
												new TH2D(histo_2d_name.str().c_str(), histo_2d_title.str().c_str(), axis_ranges_plot[3],
																axis_ranges_plot[4], axis_ranges_plot[5], axis_ranges_plot[6], axis_ranges_plot[7],
																axis_ranges_plot[8]));

				Hits_Energy_2D_.at(layer)->SetContour(100);
				Hits_Energy_2D_.at(layer)->GetYaxis()->SetTitle("y (mm)");
				Hits_Energy_2D_.at(layer)->GetYaxis()->CenterTitle();
				Hits_Energy_2D_.at(layer)->GetXaxis()->SetTitle("x (mm)");
				Hits_Energy_2D_.at(layer)->GetXaxis()->CenterTitle();
				Hits_Energy_2D_.at(layer)->GetZaxis()->SetTitle("Hit energy (eV)");
}
void Setup_EnergyHistos3D(std::array<int, 9> axis_ranges_plot, string subdetector_name, int layer) {
				stringstream histo_3d_name, histo_3d_title;
				histo_3d_name << "Hits_Energy_3D_" << subdetector_name << "_Layer_" << layer;
				histo_3d_title << "HitMap with the hit energy for " << subdetector_name << " layer " << layer;
				Hits_Energy_3D_.emplace_back(
												new TH3D(histo_3d_name.str().c_str(), histo_3d_title.str().c_str(), axis_ranges_plot[0],
																axis_ranges_plot[1], axis_ranges_plot[2], axis_ranges_plot[3], axis_ranges_plot[4],
																axis_ranges_plot[5], axis_ranges_plot[6], axis_ranges_plot[7], axis_ranges_plot[8]));
				Hits_Energy_3D_.at(layer)->SetLineColor(kViolet);
				Hits_Energy_3D_.at(layer)->GetXaxis()->SetTitle("z (mm)");
				Hits_Energy_3D_.at(layer)->GetXaxis()->CenterTitle();
				Hits_Energy_3D_.at(layer)->GetZaxis()->SetTitle("y (mm)");
				Hits_Energy_3D_.at(layer)->GetZaxis()->CenterTitle();
				Hits_Energy_3D_.at(layer)->GetYaxis()->SetTitle("x (mm)");
				Hits_Energy_3D_.at(layer)->GetYaxis()->CenterTitle();
}
void Setup_ParticleOrigins_2D(std::array<int, 9> axis_ranges_plot, string subdetector_name, int layer) {
				stringstream histo_2d_name, histo_2d_title;
				histo_2d_name << "ParticleOrigins_" << subdetector_name << "_Layer_" << layer;
				histo_2d_title << "Origins of pair background particles for " << subdetector_name << " layer " << layer;
				ParticleOrigins_2D_.emplace_back(
												new TH2D(histo_2d_name.str().c_str(), histo_2d_title.str().c_str(), 150, -4000, 4000, axis_ranges_plot[6], 0,sqrt(pow(axis_ranges_plot[5],2)+pow(axis_ranges_plot[8],2))));
				//axis_ranges_plot[1], axis_ranges_plot[2], axis_ranges_plot[6], 0,sqrt(pow(axis_ranges_plot[5],2)+pow(axis_ranges_plot[8],2))));

				/*
					 for(int i = 0; i <= 8; ++i){
					 cout << axis_ranges_plot[i] << endl;
					 }
					 cout << "sqrt(pow(axis_ranges_plot[5],2)+pow(axis_ranges_plot[8],2))"<< sqrt(pow(axis_ranges_plot[5],2)+pow(axis_ranges_plot[8],2))<< endl;
				 */
				ParticleOrigins_2D_.at(layer)->SetContour(100);
				ParticleOrigins_2D_.at(layer)->GetYaxis()->SetTitle("r (mm)");
				ParticleOrigins_2D_.at(layer)->GetYaxis()->CenterTitle();
				ParticleOrigins_2D_.at(layer)->GetXaxis()->SetTitle("z (mm)");
				ParticleOrigins_2D_.at(layer)->GetXaxis()->CenterTitle();
				ParticleOrigins_2D_.at(layer)->GetZaxis()->SetTitle("Hit count");
}

void Fill_2D_Histogram(map<std::pair<int, long>, vector<float> > HitMap, vector<TH2D*> *Hits) {
				for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
								int temp_layer = iterator->first.first;
								float average = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												average += iterator->second.at(i);
								}
								average /= iterator->second.size();
								float stddev = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												stddev += iterator->second.at(i) - average;
								}
								stddev /= iterator->second.size();
								stddev = sqrt(stddev);
								Hits->at(temp_layer)->SetBinContent(iterator->first.second, average);
								Hits->at(temp_layer)->SetBinError(iterator->first.second, stddev);
				}
}
void Fill_Energy_Histogram(map<std::pair<int, long>, vector<float> > HitMapEnergy, vector<TH2D*> *Hits_Energy) {
				for (auto iterator = HitMapEnergy.begin(); iterator != HitMapEnergy.end(); iterator++) {
								int temp_layer = iterator->first.first;
								float average = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												average += iterator->second.at(i);
								}
								average /= iterator->second.size();
								float stddev = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												stddev += iterator->second.at(i) - average;
								}
								stddev /= iterator->second.size();
								stddev = sqrt(stddev);
								Hits_Energy->at(temp_layer)->SetBinContent(iterator->first.second, average * 1000000000);
								Hits_Energy->at(temp_layer)->SetBinError(iterator->first.second, stddev * 1000000000);
				}
}
void Fill_Energy_Histogram(map<std::pair<int, long>, vector<float> > HitMapEnergy, vector<TH3D*> *Hits_Energy) {
				for (auto iterator = HitMapEnergy.begin(); iterator != HitMapEnergy.end(); iterator++) {
								int temp_layer = iterator->first.first;
								float average = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												average += iterator->second.at(i);
								}
								average /= iterator->second.size();
								float stddev = 0;
								for (size_t i = 0; i < iterator->second.size(); ++i) {
												stddev += iterator->second.at(i) - average;
								}
								stddev /= iterator->second.size();
								stddev = sqrt(stddev);
								Hits_Energy->at(temp_layer)->SetBinContent(iterator->first.second, average * 1000000000);
								Hits_Energy->at(temp_layer)->SetBinError(iterator->first.second, stddev * 1000000000);
				}
}

/*
	 std::vector<TTree*> Get_TTrees(string subdetector_name) {
	 std::vector<TTree *> TTrees;
	 std::vector<std::string> Tree_names;
	 if (subdetector_name == std::string("all")) {
	 std::string subdetector1;
	 subdetector1 = "Tree_EcalBarrel";
	 std::string subdetector2;
	 subdetector2 = "Tree_EcalEndcap";
	 std::string subdetector3;
	 subdetector3 = "Tree_HcalBarrel";
	 std::string subdetector4;
	 subdetector4 = "Tree_HcalEndcap";
	 std::string subdetector5;
	 subdetector5 = "Tree_MuonBarrel";
	 std::string subdetector6;
	 subdetector6 = "Tree_MuonEndcap";
	 std::string subdetector7;
	 subdetector7 = "Tree_BeamCal";
	 std::string subdetector8;
	 subdetector8 = "Tree_LumiCal";
	 Tree_names.push_back(subdetector1);
	 Tree_names.push_back(subdetector2);
	 Tree_names.push_back(subdetector3);
	 Tree_names.push_back(subdetector4);
	 Tree_names.push_back(subdetector5);
	 Tree_names.push_back(subdetector6);
	 Tree_names.push_back(subdetector7);
	 Tree_names.push_back(subdetector8);
	 } else {
	 stringstream temp;
	 temp << "Tree_" << subdetector_name;
	 Tree_names.push_back(temp.str());
	 }

	 for (int t = 0; t < Tree_names.size(); ++t) {
	 TTree* Tree = nullptr;
	 inputfile->GetObject(Tree_names.at(t).c_str(), Tree);
	 if (!Tree) {
	 throw std::exception();
	 }

	 TTrees.push_back(Tree);
	 }
	 return TTrees;
	 }
 */
TTree* Get_TTree(string subdetector_name) {
				stringstream temp;
				temp << "Tree_" << subdetector_name;

				TTree* Tree = nullptr;
				inputfile->GetObject(temp.str().c_str(), Tree);
				if (!Tree) {
								throw std::exception();
				}

				return Tree;
}

void DrawingMacro(string outputname, std::vector<string> inputnames, std::vector<std::string> subdetectors) {
				TH1::SetDefaultSumw2();

				TFile * output_rootfile = new TFile(outputname.c_str(), "RECREATE");
				std::size_t found = outputname.find_last_of(".");
				string Canvas_name = outputname.substr(0, found);
				//std::cout << __LINE__ << std::endl;
				//Defining the histograms for DrawingHitsPerEvent
				TCanvas* Files_Canvas = new TCanvas((Canvas_name + "_HitsPerFile").c_str());

				TH2D* ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event", 50, 0, NUMBER_OF_FILES, 50, 190000,
												210000);
				TH1D* Particles = new TH1D("Particles", "Particles", 100, 180000, 220000);
				TF1* gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);

				//std::cout << __LINE__ << std::endl;

				//Defining the histograms for DrawingCaloHitsMap
				std::string subdetector_name;
				stringstream several_subdetector_names;

				std::vector<CaloIDs> SubDetectors;
				for (size_t s = 0; s < subdetectors.size(); ++s) {
								if (subdetectors.size() == 1) {
												several_subdetector_names.str("");
												several_subdetector_names << subdetectors.at(0);
												//std::cout << __LINE__ << std::endl;
												if (subdetectors.at(0) == std::string("all")) {
																CaloIDs SubDetector1("EcalBarrel");
																CaloIDs SubDetector2("EcalEndcap");
																CaloIDs SubDetector3("HcalBarrel");
																CaloIDs SubDetector4("HcalEndcap");
																CaloIDs SubDetector5("MuonBarrel");
																CaloIDs SubDetector6("MuonEndcap");
																CaloIDs SubDetector7("BeamCal");
																CaloIDs SubDetector8("LumiCal");
																SubDetectors.push_back(SubDetector1);
																SubDetectors.push_back(SubDetector2);
																SubDetectors.push_back(SubDetector3);
																SubDetectors.push_back(SubDetector4);
																SubDetectors.push_back(SubDetector5);
																SubDetectors.push_back(SubDetector6);
																SubDetectors.push_back(SubDetector7);
																SubDetectors.push_back(SubDetector8);
												} else {
																CaloIDs SubDetector(subdetectors.at(0));
																SubDetectors.push_back(SubDetector);
												}
								} else {

												if (subdetectors.at(s) == std::string("all")) {
																several_subdetector_names.str("");
																several_subdetector_names << subdetectors.at(s);

																SubDetectors.clear();
																CaloIDs SubDetector1("EcalBarrel");
																CaloIDs SubDetector2("EcalEndcap");
																CaloIDs SubDetector3("HcalBarrel");
																CaloIDs SubDetector4("HcalEndcap");
																CaloIDs SubDetector5("MuonBarrel");
																CaloIDs SubDetector6("MuonEndcap");
																CaloIDs SubDetector7("BeamCal");
																CaloIDs SubDetector8("LumiCal");
																SubDetectors.push_back(SubDetector1);
																SubDetectors.push_back(SubDetector2);
																SubDetectors.push_back(SubDetector3);
																SubDetectors.push_back(SubDetector4);
																SubDetectors.push_back(SubDetector5);
																SubDetectors.push_back(SubDetector6);
																SubDetectors.push_back(SubDetector7);
																SubDetectors.push_back(SubDetector8);
																break;

												} else {
																several_subdetector_names << subdetectors.at(s);
																CaloIDs SubDetector(subdetectors.at(s));
																SubDetectors.push_back(SubDetector);
												}
								}
				}
				subdetector_name = several_subdetector_names.str();
				//std::cout << __LINE__ << std::endl;
				std::array<int, 3> axis_range_plot_1D = { }; //xbins, xlow, xup
				std::array<float, 3> axis_range_plot_energy_1D = { }; //xbins, xlow, xup
				std::array<int, 9> axis_ranges_plot = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup

				Setup_BinningArrays(SubDetectors, &axis_range_plot_1D, &axis_range_plot_energy_1D, &axis_ranges_plot);

				TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*3, axis_range_plot_1D[1], axis_range_plot_1D[2]*40);
				///*MuonBarrel*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/5, 100, axis_range_plot_1D[2]);
				///*MuonEndcap*/TH1D* Hits = new TH1D("Hits", "Hits", 35,5500, 7300);
				///*BeamCal:*/ TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/4, 4200000, 4850000);
				///*ALL:*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*2/3, 4000000, 5000000);
				Hits->GetYaxis()->SetTitle("Count");
				Hits->GetXaxis()->SetTitle(("Hits per bunch crossing in " + subdetector_name).c_str());
				Hits->GetXaxis()->CenterTitle();
				//Scaling up the histogram ranges from the subdetector specific single layer hits plot, so that data fit on plot
				//TF1* gausfit_Hits = new TF1("gausfit", "gaus", 0, 150);


				//std::cout << __LINE__ << std::endl;
				int id0 = 0;
				int id1 = 0;
				double cell_posx = 0;
				double cell_posy = 0;
				float x = 0;
				float y = 0;
				float z = 0;
				double vertex_x = 0;
				double vertex_y = 0;
				double vertex_z = 0;
				float energy = 0;
				int MaxNumberLayers = 0;
				int Layer_no = 0;
				long CellIDkey = 0.;

				std::map<long, vector<int>> HitsPerLayerMap;

				//Find the largest number of layers from all the subdetectors that are to be plotted
				for (int s = 0; s < SubDetectors.size(); ++s) {
								if (SubDetectors.at(s).Number_layers > MaxNumberLayers) {
												MaxNumberLayers = SubDetectors.at(s).Number_layers;
								}
				}
				//std::cout << __LINE__ << std::endl;
				for (int l = 0; l < MaxNumberLayers; ++l) {

								for(int files = 1; files <= NUMBER_OF_FILES; ++files){
												HitsPerLayerMap[l].push_back(0);
								}

								std::stringstream layer;
								layer << l;
								Hits_Canvas_ = new TCanvas((Canvas_name + "_Hits_" + subdetector_name).c_str());

								gROOT->ForceStyle();
								gStyle->SetPalette(1);

								//std::cout << __LINE__ << std::endl;
								Setup_Histos1D_HitsPerLayer(axis_range_plot_1D, subdetector_name, l);
								Setup_ParticleOrigins_2D(axis_ranges_plot, subdetector_name, l);
								Setup_Histos1D(axis_range_plot_1D, subdetector_name, l);
								Setup_Histos2D(axis_ranges_plot, subdetector_name, l);
								Setup_Histos3D(axis_ranges_plot, subdetector_name, l);
								//std::cout << __LINE__ << std::endl;
								Setup_EnergyHistos1D(axis_range_plot_energy_1D, subdetector_name, l);
								Setup_EnergyHistos2D(axis_ranges_plot, subdetector_name, l);
								Setup_EnergyHistos3D(axis_ranges_plot, subdetector_name, l);
				}
				//std::cout << __LINE__ << std::endl;
				std::vector<int> hitLayers;
				for (int s = 0; s < SubDetectors.size(); ++s) {

								std::map<long, int> HitMap;
								std::map<std::pair<int, long>, vector<float> > HitMap2D; //layer, bin, hits

								//Getting the inputfile and its TTrees
								for (int j = 0; j < NUMBER_OF_FILES; ++j) {
												inputfile = TFile::Open((inputnames.at(j)).c_str());
												//std::cout << __LINE__ << std::endl;
												if (!inputfile) {
																throw std::exception();
												}

												std::cout << inputfile->GetName() << std::endl;

												TTree* Tree_MCP;
												inputfile->GetObject("Tree_MCP", Tree_MCP);
												cout << "Accessed TTrees.." << endl;

												int number_of_hits = 0;
												number_of_hits = Get_TTree(SubDetectors.at(s).GetName())->GetEntries();

												//Filling the histograms

												//DrawingCaloHitsMap:

												std::cout << "The TTree " << Get_TTree(SubDetectors.at(s).GetName())->GetName() << " has " << number_of_hits
																<< " entries." << std::endl;

												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("*", kFALSE); // disable all
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitCellID0", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitCellID1", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitPosition_x", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitPosition_y", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitPosition_z", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitVertex_x", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitVertex_y", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitVertex_z", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchStatus("HitEnergy", kTRUE);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitCellID0", &id0);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitCellID1", &id1);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitPosition_x", &x);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitPosition_y", &y);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitPosition_z", &z);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitVertex_x", &vertex_x);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitVertex_y", &vertex_y);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitVertex_z", &vertex_z);
												Get_TTree(SubDetectors.at(s).GetName())->SetBranchAddress("HitEnergy", &energy);
												//std::cout << __LINE__ << std::endl;

												map<std::pair<int, long>, vector<float> > HitMapEnergy2D; //layer, bin, energies
												map<std::pair<int, long>, vector<float> > HitMapEnergy3D; //layer, bin, energies

												for (std::size_t i = 0; i < number_of_hits; i++) {
																CellIDkey = 0.;

																Get_TTree(SubDetectors.at(s).GetName())->GetEntry(i);
																SubDetectors.at(s).GetCellID(id0, id1);
																cell_posx = SubDetectors.at(s).GetCellPos(id1)[0];
																cell_posy = SubDetectors.at(s).GetCellPos(id1)[1];
																//cout << "Cell with unique ID " << SubDetector.CellID << " is at x = " << cell_posx << " and y = " << cell_posy << endl;

																Layer_no = SubDetectors.at(s).GetLayer(id0);
																HitsPerLayerMap[Layer_no].at(j) += 1;

																HitMapEnergy2D[std::pair<int, long>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(cell_posx, cell_posy))].push_back(
																								energy);
																HitMapEnergy3D[std::pair<int, long>(Layer_no, Hits_Energy_3D_.at(Layer_no)->FindBin(z, cell_posx, cell_posy))].push_back(
																								energy);//DON'T KNOW IF CORRECT WITH GLOBAL Z AND LOCAL CELL_POSX AND CELL_POSY


																//cout << "The cell is in Layer " << Layer_no << endl;
																if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no) == hitLayers.end()) {
																				hitLayers.push_back(Layer_no);
																}

																CellIDkey = SubDetectors.at(s).CellID.to_ulong();

																if (HitMap.find(CellIDkey) == HitMap.end()) {
																				HitMap[CellIDkey] = 1;
																} else {
																				HitMap[CellIDkey] += 1;
																				//Hits_Histo_.at(Layer_no)->Fill(HitMap[CellIDkey]);
																}
																Hits_Energy_Histo_.at(Layer_no)->Fill(energy);

																ParticleOrigins_2D_.at(Layer_no)->Fill(vertex_z, sqrt(pow(vertex_x,2)+pow(vertex_y,2)));
																//Hits_2D_.at(Layer_no)->Fill(x, y);
																Hits_3D_.at(Layer_no)->Fill(z, x, y);
																//Hits_Energy_2D_.at(Layer_no)->Fill(x, y, energy);
																//Hits_Energy_3D_.at(Layer_no)->Fill(z, x, y, energy);
																//std::cout << __LINE__ << std::endl;

												}
												Fill_Energy_Histogram(HitMapEnergy2D, &Hits_Energy_2D_);
												Fill_Energy_Histogram(HitMapEnergy3D, &Hits_Energy_3D_);

												//	DrawingHitsPerEvent:
												int number_of_particles = 0;
												number_of_particles = Tree_MCP->GetEntries();
												ParticlesVSEvent->Fill(j, number_of_particles);
												Particles->Fill(number_of_particles);
												Hits->Fill(number_of_hits);

												inputfile->Close();
												delete inputfile;
												//std::cout << __LINE__ << std::endl;
								}
								float threshold = 0.0001;
								TH1D *Hits_temp = new TH1D("HitsTemp",
																Hits->GetTitle(),
																Hits->GetNbinsX(),
																Hits->GetBinCenter(Hits->FindFirstBinAbove(Hits->GetEntries()*threshold)),
																Hits->GetBinCenter(Hits->FindLastBinAbove(Hits->GetEntries()*threshold))
																);
								//Hits = Hits_temp;
								int cnt = 0;
								for (auto iterator = HitsPerLayerMap.begin(); iterator != HitsPerLayerMap.end(); iterator++) {
												//for(auto e :iterator->second){
												for(auto e = iterator->second.begin(); e != iterator->second.end(); ++e){
																if (*e > 0) {
																				Hits_PerLayer_.at(iterator->first)->Fill(*e);
																}
												}
												//stringstream number;
												//number << cnt;
												//TH1D *Hits_PerLayer_temp = new TH1D(("HitsperLayerTemp"+number.str()).c_str(),
												//								Hits_PerLayer_.at(iterator->first)->GetTitle(),
												//								Hits_PerLayer_.at(iterator->first)->GetNbinsX(),
												//								Hits_PerLayer_.at(iterator->first)->GetBinCenter(Hits_PerLayer_.at(iterator->first)->FindFirstBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold)),
												//								Hits_PerLayer_.at(iterator->first)->GetBinCenter(Hits_PerLayer_.at(iterator->first)->FindLastBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold))
												//							);
												//cout << "Entries = " << Hits_PerLayer_.at(iterator->first)->GetEntries() << endl;
												//cout << "LowBin = " << Hits_PerLayer_.at(iterator->first)->FindFirstBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold) << endl;
												//cout << "HighBin = " << Hits_PerLayer_.at(iterator->first)->FindLastBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold) << endl;
												//cout << Hits_PerLayer_.at(iterator->first)->GetBinCenter(Hits_PerLayer_.at(iterator->first)->FindFirstBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold)) << ",";
												//cout << Hits_PerLayer_.at(iterator->first)->GetBinCenter(Hits_PerLayer_.at(iterator->first)->FindLastBinAbove(Hits_PerLayer_.at(iterator->first)->GetEntries()*threshold)) << endl;
												//for(auto e = iterator->second.begin(); e != iterator->second.end(); ++e){
												//		if (*e > 0) {
												//				Hits_PerLayer_temp->Fill(*e);
												//		}
												//}
												//Hits_PerLayer_.at(iterator->first) = Hits_PerLayer_temp;
												//cnt++;
								}
								for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
												if (iterator->second > 0) {
																Hits_Histo_.at(SubDetectors.at(s).GetLayer(iterator->first))->Fill(iterator->second);
																HitMap2D[std::pair<int, long>(SubDetectors.at(s).GetLayer(iterator->first), Hits_2D_.at(Layer_no)->FindBin(SubDetectors.at(s).GetCellPos(iterator->first)[0],SubDetectors.at(s).GetCellPos(iterator->first)[1]))].push_back(iterator->second);
												}
								}
								Fill_2D_Histogram(HitMap2D, &Hits_2D_);
								}

								gStyle->SetOptStat(1);
								//gStyle->SetOptStat(111111);
								TCanvas* PDF_Canvas_1D2D_Hits_Layers = new TCanvas(); 
								PDF_Canvas_1D2D_Hits_Layers->Print("PDFCanvas_1D2D_Hits_Layers.pdf[");

								output_rootfile->cd();
								for (signed int l = 0; l < hitLayers.size(); ++l) {

												Hits_3D_.at(hitLayers.at(l))->Write();
												Hits_Energy_3D_.at(hitLayers.at(l))->Write();

												stringstream HitsCanvasName_eps, HitsCanvasName_C;
												Hits_Canvas_->cd();

												{
																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																Hits_Canvas_->SetLogy(0);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(0);
																Hits_Canvas_->SetRightMargin(0.15);
																Hits_2D_.at(hitLayers.at(l))->Draw("colz");
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << Hits_2D_.at(hitLayers.at(l))->GetName() << ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_2D_.at(hitLayers.at(l))->GetName() << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");

																Hits_Canvas_->Update();
																Hits_Canvas_->SetLogy(0);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(0);
																Hits_Canvas_->SetRightMargin(0.15);
																Hits_Energy_2D_.at(hitLayers.at(l))->Draw("colz");
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << Hits_Energy_2D_.at(hitLayers.at(l))->GetName()
																				<< ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_Energy_2D_.at(hitLayers.at(l))->GetName()
																				<< ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");

																Hits_Canvas_->Update();
																Hits_Canvas_->SetLogy(0);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(1);
																Hits_Canvas_->SetRightMargin(0.15);
																ParticleOrigins_2D_.at(hitLayers.at(l))->Draw("colz");
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << ParticleOrigins_2D_.at(hitLayers.at(l))->GetName()
																				<< ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << ParticleOrigins_2D_.at(hitLayers.at(l))->GetName()
																				<< ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");


																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																gROOT->ForceStyle();
																Hits_Canvas_->SetLogy(0);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(0);
																Hits_PerLayer_.at(hitLayers.at(l))->Draw();
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << Hits_PerLayer_.at(hitLayers.at(l))->GetName()
																				<< ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_PerLayer_.at(hitLayers.at(l))->GetName() << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");

																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																gROOT->ForceStyle();
																Hits_Canvas_->SetLogy(1);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(0);
																Hits_Histo_.at(hitLayers.at(l))->Draw();
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << Hits_Histo_.at(hitLayers.at(l))->GetName()
																				<< ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_Histo_.at(hitLayers.at(l))->GetName() << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");

																Hits_Canvas_->Update();
																Hits_Canvas_->SetLogy(1);
																Hits_Canvas_->SetLogx(0);
																Hits_Canvas_->SetLogz(0);
																Hits_Energy_Histo_.at(hitLayers.at(l))->Draw();
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_" << Hits_Energy_Histo_.at(hitLayers.at(l))->GetName()
																				<< ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_Energy_Histo_.at(hitLayers.at(l))->GetName()
																				<< ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_1D2D_Hits_Layers.pdf");
												}
												//std::cout << __LINE__ << std::endl;
								}
								PDF_Canvas_1D2D_Hits_Layers->Print("PDFCanvas_1D2D_Hits_Layers.pdf]");
								delete PDF_Canvas_1D2D_Hits_Layers;
								//Compare different layers:

								if (first_layer_to_be_compared >= 0) {

												TCanvas* PDF_Canvas_Hits_CompareLayers = new TCanvas(); 
												PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf[");

												int end_of_range = 0;
												if (last_layer_to_be_compared != 0) {
																end_of_range = last_layer_to_be_compared;
												} else {
																end_of_range = hitLayers.size() - 1;
												}

												int n = end_of_range - first_layer_to_be_compared + 1;//Otherwise the number of layers are wrong
												float layer_numbers[n];
												float layer_numbers_Errors[n];
												std::fill(layer_numbers_Errors,layer_numbers_Errors + n, 0);
												float MeanHits[n];
												float MeanHitErrors[n];
												for(size_t i = 0; i <= n; ++i){
																layer_numbers[i] = first_layer_to_be_compared + i;
																MeanHits[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetMean(1);
																MeanHitErrors[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetRMS(1);
												}

												TGraphErrors* MeanHits_PerLayer = new TGraphErrors(n, layer_numbers, MeanHits, layer_numbers_Errors, MeanHitErrors);
												MeanHits_PerLayer->SetTitle("Mean hits for different layers; Layer number; Mean hits per bunch crossing");
												MeanHits_PerLayer->SetMarkerColor(kViolet);
												MeanHits_PerLayer->SetMarkerStyle(21);
												MeanHits_PerLayer->GetXaxis()->SetRangeUser(first_layer_to_be_compared-0.5, end_of_range+0.5);
												MeanHits_PerLayer->GetXaxis()->CenterTitle();


												if (std::find(hitLayers.begin(), hitLayers.end(), first_layer_to_be_compared) != hitLayers.end()
																				&& std::find(hitLayers.begin(), hitLayers.end(), first_layer_to_be_compared) != hitLayers.end()) {

																stringstream HitsCanvasName_eps, HitsCanvasName_C;

																Hits_Canvas_->cd();
																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																Hits_Canvas_->SetLogy(0);
																MeanHits_PerLayer->Draw("AP");
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_MeanHits_" << first_layer_to_be_compared << "-" << end_of_range << ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_MeanHits_" << first_layer_to_be_compared 
																				<< "-" << end_of_range << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_Hits_CompareLayers.pdf");

																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																Hits_Canvas_->SetLogy(1);

																stringstream new_histo_perlayer_title;
																new_histo_perlayer_title << "Hits per layer for " << subdetector_name << " layers "
																				<< first_layer_to_be_compared << " - " << end_of_range;
																Hits_PerLayer_.at(first_layer_to_be_compared)->SetTitle(new_histo_perlayer_title.str().c_str());
																Hits_PerLayer_.at(first_layer_to_be_compared)->SetLineColor(2);
																Hits_PerLayer_.at(first_layer_to_be_compared)->SetMarkerStyle(20);
																Hits_PerLayer_.at(first_layer_to_be_compared)->SetMarkerColor(2);
																Hits_PerLayer_.at(first_layer_to_be_compared)->Draw();
																TPaveStats *st1 = (TPaveStats*) Hits_PerLayer_.at(first_layer_to_be_compared)->FindObject("stats");
																double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
																st1->SetTextColor(2);
																st1->SetY1NDC(0.78);
																st1->SetY2NDC(0.78 + statboxsize);

																for (signed int l = first_layer_to_be_compared + 1; l <= end_of_range; ++l) {

																				if (std::find(hitLayers.begin(), hitLayers.end(), l) == hitLayers.end()) {
																								// If one layer has no hits, it isn't mentioned in the vector hitlayers.
																								//This layer is then skipped.
																								continue;
																				}

																				Hits_PerLayer_.at(l)->SetMarkerStyle(l+20);
																				if(l<3){
																								Hits_PerLayer_.at(l)->SetLineColor(l+2);
																								Hits_PerLayer_.at(l)->SetMarkerColor(l+2);
																				}
																				else{
																								Hits_PerLayer_.at(l)->SetLineColor(l+3);
																								Hits_PerLayer_.at(l)->SetMarkerColor(l+3);
																				}
																				Hits_PerLayer_.at(l)->Draw("sames");
																				TPaveStats *st = (TPaveStats*) Hits_PerLayer_.at(l)->FindObject("stats");
																				if(l<3) st->SetTextColor(l+2);
																				else st->SetTextColor(l+3);
																				st->SetY2NDC(((TPaveStats*) Hits_PerLayer_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
																				st->SetY1NDC(st->GetY2NDC() - statboxsize); //new x end position
																}
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_"
																				<< Hits_PerLayer_.at(first_layer_to_be_compared)->GetName() << "-" << end_of_range << ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_PerLayer_.at(first_layer_to_be_compared)->GetName()
																				<< "-" << end_of_range << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_Hits_CompareLayers.pdf");


																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																Hits_Canvas_->SetLogy(1);

																stringstream new_histo_title;
																new_histo_title << "Hit occupancy per cell for " << subdetector_name << " layers "
																				<< first_layer_to_be_compared << " - " << end_of_range;
																Hits_Histo_.at(first_layer_to_be_compared)->SetTitle(new_histo_title.str().c_str());
																Hits_Histo_.at(first_layer_to_be_compared)->GetYaxis()->SetTitle("Normalized count");
																Hits_Histo_.at(first_layer_to_be_compared)->SetLineColor(2);
																Hits_Histo_.at(first_layer_to_be_compared)->SetMarkerStyle(20);
																Hits_Histo_.at(first_layer_to_be_compared)->SetMarkerColor(2);
																Hits_Histo_.at(first_layer_to_be_compared)->Scale(1.0/Hits_Histo_.at(first_layer_to_be_compared)->Integral());
																Hits_Histo_.at(first_layer_to_be_compared)->Draw();
																TPaveStats *st2 = (TPaveStats*) Hits_Histo_.at(first_layer_to_be_compared)->FindObject("stats");
																double statboxsize2 = st2->GetY2NDC() - st2->GetY1NDC();
																st2->SetTextColor(2);
																st2->SetY1NDC(0.78);
																st2->SetY2NDC(0.78 + statboxsize2);

																for (signed int l = first_layer_to_be_compared + 1; l <= end_of_range; ++l) {

																				if (std::find(hitLayers.begin(), hitLayers.end(), l) == hitLayers.end()) {
																								// If one layer has no hits, it isn't mentioned in the vector hitlayers.
																								//This layer is then skipped.
																								continue;
																				}

																				Hits_Histo_.at(l)->SetMarkerStyle(l+20);
																				if(l<3){
																								Hits_Histo_.at(l)->SetLineColor(l+2);
																								Hits_Histo_.at(l)->SetMarkerColor(l+2);
																				}
																				else{
																								Hits_Histo_.at(l)->SetLineColor(l+3);
																								Hits_Histo_.at(l)->SetMarkerColor(l+3);
																				}
																				Hits_Histo_.at(l)->GetYaxis()->SetTitle("Normalized count");
																				Hits_Histo_.at(l)->Scale(1.0/Hits_Histo_.at(l)->Integral());
																				Hits_Histo_.at(l)->Draw("sames");
																				TPaveStats *st = (TPaveStats*) Hits_Histo_.at(l)->FindObject("stats");
																				if(l<3) st->SetTextColor(l+2);
																				else st->SetTextColor(l+3);
																				st->SetY2NDC(((TPaveStats*) Hits_Histo_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
																				st->SetY1NDC(st->GetY2NDC() - statboxsize2); //new x end position
																}
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_"
																				<< Hits_Histo_.at(first_layer_to_be_compared)->GetName() << "-" << end_of_range << ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_" << Hits_Histo_.at(first_layer_to_be_compared)->GetName()
																				<< "-" << end_of_range << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_Hits_CompareLayers.pdf");

																Hits_Canvas_->Clear();
																Hits_Canvas_->Update();
																gStyle->SetStatX(0.87);
																Hits_Canvas_->SetLogy(1);

																stringstream new_Energy_histo_title;
																new_Energy_histo_title << "Deposited hit energy for " << subdetector_name << " layers "
																				<< first_layer_to_be_compared << " - " << end_of_range;
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->SetTitle(new_Energy_histo_title.str().c_str());
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->SetLineColor(2);
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->SetMarkerColor(2);
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->SetMarkerStyle(20);
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->GetYaxis()->SetTitle("Normalized count");
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->Scale(1.0/Hits_Energy_Histo_.at(first_layer_to_be_compared)->Integral());
																Hits_Energy_Histo_.at(first_layer_to_be_compared)->Draw();
																TPaveStats *stE1 = (TPaveStats*) Hits_Energy_Histo_.at(first_layer_to_be_compared)->FindObject("stats");
																double statboxsize_E = stE1->GetY2NDC() - stE1->GetY1NDC();
																stE1->SetTextColor(2);
																stE1->SetY1NDC(0.78);
																stE1->SetY2NDC(0.78 + statboxsize_E);

																for (signed int l = first_layer_to_be_compared + 1; l <= end_of_range; ++l) {

																				if (std::find(hitLayers.begin(), hitLayers.end(), l) == hitLayers.end()) {
																								// If one layer has no hits, it isn't mentioned in the vector hitlayers.
																								//This layer is then skipped.
																								continue;
																				}

																				Hits_Energy_Histo_.at(l)->SetMarkerStyle(l+20);
																				if(l<3){
																								Hits_Energy_Histo_.at(l)->SetLineColor(l+2);
																								Hits_Energy_Histo_.at(l)->SetMarkerColor(l+2);
																				}
																				else{
																								Hits_Energy_Histo_.at(l)->SetLineColor(l+3);
																								Hits_Energy_Histo_.at(l)->SetMarkerColor(l+3);
																				}
																				Hits_Energy_Histo_.at(l)->GetYaxis()->SetTitle("Normalized count");
																				Hits_Energy_Histo_.at(l)->Scale(1.0/Hits_Energy_Histo_.at(l)->Integral());
																				Hits_Energy_Histo_.at(l)->Draw("sames");
																				TPaveStats *st = (TPaveStats*) Hits_Energy_Histo_.at(l)->FindObject("stats");
																				if(l<3) st->SetTextColor(l+2);
																				else st->SetTextColor(l+3);
																				st->SetY2NDC(((TPaveStats*) Hits_Energy_Histo_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
																				st->SetY1NDC(st->GetY2NDC() - statboxsize_E); //new x end position
																}
																HitsCanvasName_eps << Hits_Canvas_->GetName() << "_"
																				<< Hits_Energy_Histo_.at(first_layer_to_be_compared)->GetName() << "-" << end_of_range << ".eps";
																HitsCanvasName_C << Hits_Canvas_->GetName() << "_"
																				<< Hits_Energy_Histo_.at(first_layer_to_be_compared)->GetName() << "-" << end_of_range << ".C";
																Hits_Canvas_->Write();
																Hits_Canvas_->Print(HitsCanvasName_eps.str().c_str());
																Hits_Canvas_->Print(HitsCanvasName_C.str().c_str());
																HitsCanvasName_eps.str("");
																HitsCanvasName_C.str("");
																Hits_Canvas_->Print("PDFCanvas_Hits_CompareLayers.pdf");
												} else {
																std::cerr << "The first or the last (or both) layer of your given range has no hits!\n"
																				"There will be no comparison of the layers." << std::endl;
												}
												PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf]");
												delete PDF_Canvas_Hits_CompareLayers;
								}

								for (signed int l = 0; l < MaxNumberLayers; ++l) {
												delete Hits_PerLayer_.at(l), Hits_Histo_.at(l), Hits_2D_.at(l), Hits_3D_.at(l);
												delete Hits_Energy_Histo_.at(l), Hits_Energy_2D_.at(l), Hits_Energy_3D_.at(l);
								}
								delete Hits_Canvas_;
								output_rootfile->Write();
								//output_rootfile->Close();
								//delete output_rootfile;
								TCanvas* PDF_Canvas_ParticlesHits_per_File = new TCanvas(); 
								PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf[");
								//Files_Canvas->Write();
								stringstream FilesCanvasName_eps, FilesCanvasName_C;

								Files_Canvas->cd();

								gStyle->SetOptStat(1);
								//gStyle->SetOptStat(111111);

								Files_Canvas->SetLogy(0);
								ParticlesVSEvent->Draw();
								FilesCanvasName_eps << Files_Canvas->GetName() << "_ParticlesVSEvent.eps";
								FilesCanvasName_C << Files_Canvas->GetName() << "_ParticlesVSEvent.C";
								Files_Canvas->Print(FilesCanvasName_eps.str().c_str());
								Files_Canvas->Print(FilesCanvasName_C.str().c_str());
								FilesCanvasName_eps.str("");
								FilesCanvasName_C.str("");
								Files_Canvas->Print("PDFCanvas_ParticlesHits_perFile.pdf");

								//gStyle->SetOptFit(0100);
								//gROOT->ForceStyle();

								Files_Canvas->SetLogy(0);
								Particles->Draw();
								Particles->Fit(gausfit_Particles, "R");
								FilesCanvasName_eps << Files_Canvas->GetName() << "_Particles.eps";
								FilesCanvasName_C << Files_Canvas->GetName() << "_Particles.C";
								Files_Canvas->Print(FilesCanvasName_eps.str().c_str());
								Files_Canvas->Print(FilesCanvasName_C.str().c_str());
								FilesCanvasName_eps.str("");
								FilesCanvasName_C.str("");
								Files_Canvas->Print("PDFCanvas_ParticlesHits_perFile.pdf");

								Files_Canvas->SetLogy(1);
								Hits->Draw();
								//Hits->Fit(gausfit_Hits, "R");
								FilesCanvasName_eps << Files_Canvas->GetName() << "_Hits.eps";
								FilesCanvasName_C << Files_Canvas->GetName() << "_Hits.C";
								Files_Canvas->Print(FilesCanvasName_eps.str().c_str());
								Files_Canvas->Print(FilesCanvasName_C.str().c_str());
								Files_Canvas->Print("PDFCanvas_ParticlesHits_perFile.pdf");
								//Files_Canvas->Close();
								PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf]");

								delete PDF_Canvas_ParticlesHits_per_File;
								delete Files_Canvas;
								//delete ParticlesVSEvent, Hits, Particles;
								//delete gausfit_Hits, gausfit_Particles;

				}

				void Usage() {
								//explain how to use program
								cerr << "Usage:" << endl;
								cerr
												<< "Type in the name of the subdetector you are interested in, as well as the output filename, the number of input files and the input root filenames after their correspondent flags!"
												<< endl;
								cerr << "e.g. ./DrawHistograms -s EcalBarrel -o output.root -n 2 -i file1.root file2.root" << endl;
								cerr
												<< "You can also give a range of layers you want to compare in the 1D histograms: -l 2-5  or  -l 5- (from layer 5 on all the layers that are there)"
												<< endl;
								cerr << "e.g. ./DrawHistograms -l 2-5 -s EcalBarrel -o output.root -n 2 -i file1.root file2.root" << endl;
								terminate();
				}
