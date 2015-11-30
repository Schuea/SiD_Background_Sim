#ifndef DRAWINGHISTOS
#define DRAWINGHISTOS

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

#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"

void DrawingMacro(string outputname, std::vector<std::string> inputnames, std::vector<std::string> argument_subdetectors){
				TH1::SetDefaultSumw2();

				TFile * output_rootfile = new TFile(outputname.c_str(), "RECREATE");
				std::size_t found = outputname.find_last_of(".");
				string Canvas_name = outputname.substr(0, found);
				TCanvas* Files_Canvas = new TCanvas((Canvas_name + "_HitsPerFile").c_str());

				TH2D* ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event", 50, 0, NUMBER_OF_FILES, 50, 190000, 210000);
				TH1D* Particles = new TH1D("Particles", "Particles", 100, 180000, 220000);
				TF1* gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);

				std::string subdetector_name;
				stringstream several_subdetector_names;

				std::vector<Subdetector*> * SubDetectors;
				SetupSubDetectorsVector(SubDetectors, severeal_subdetector_names, argument_subdetectors);
			
				subdetector_name = several_subdetector_names.str();
				
				std::array<int, 3> axis_range_plot_1D = { }; //xbins, xlow, xup
				std::array<float, 3> axis_range_plot_energy_1D = { }; //xbins, xlow, xup
				std::array<int, 9> axis_ranges_plot = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup

				Setup_BinningArrays(SubDetectors, &axis_range_plot_1D, &axis_range_plot_energy_1D, &axis_ranges_plot);


}
void SetupSubDetectorsVector(std::vector<Subdetector*> * SubDetectors, stringstream severeal_subdetector_names, std::vector<std::string> argument_subdetectors){
				for (size_t s = 0; s < argument_subdetectors.size(); ++s) {
								if (argument_subdetectors.size() == 1) {
												several_subdetector_names.str("");
												several_subdetector_names << argument_subdetectors.at(0);

												if (argument_subdetectors.at(0) == std::string("allCalo")) {

																SubDetectors->clear();
																InitializeAllCaloSubdetectors(SubDetectors);
												}
												if (argument_subdetectors.at(s) == std::string("allTracker")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors->clear();
																InitializeAllTrackerSubdetectors(SubDetectors);
												}
												if (argument_subdetectors.at(s) == std::string("all")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors->clear();
																InitializeAllSubdetectors(SubDetectors);
												} else {
																InitializeWhichSubdetector(argument_subdetectors.at(0), SubDetectors);
												}
								} else {
												if (argument_subdetectors.at(s) == std::string("all")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors->clear();
																InitializeAllSubdetectors(SubDetectors);
																break;
												}
												if (argument_subdetectors.at(s) == std::string("allCalo")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors->clear();
																InitializeAllCaloSubdetectors(SubDetectors);

																if (argument_subdetectors.at(s+1) == std::string("allTracker")){
																				std::cerr << "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!" << std::endl;
																				terminate();
																}
																else break;
												}
												if (argument_subdetectors.at(s) == std::string("allTracker")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors->clear();
																InitializeAllTrackerSubdetectors(SubDetectors);

																if (argument_subdetectors.at(s+1) == std::string("allCalo")){
																				std::cerr << "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!" << std::endl;
																				terminate();
																}
																else break;

												} else {
																several_subdetector_names << argument_subdetectors.at(s);
																InitializeWhichSubdetector(argument_subdetectors.at(s), SubDetectors);
												}
								}
				}
}

void InitializeAllSubdetectors(std::vector< Subdetector* > * SubDetectors){
				InitializeAllCaloSubdetectors(SubDetectors);
				InitializeAllTrackerSubdetectors(SubDetectors);
}				
void InitializeAllCaloSubdetectors(std::vector< Subdetector* > * SubDetectors){
				SubDetectors->push_back(new EcalBarrel());
				SubDetectors->push_back(new EcalEndcap());
				SubDetectors->push_back(new HcalBarrel());
				SubDetectors->push_back(new HcalEndcap());
				SubDetectors->push_back(new MuonBarrel());
				SubDetectors->push_back(new MuonEndcap());
				SubDetectors->push_back(new BeamCal());
				SubDetectors->push_back(new LumiCal());
}
void InitializeAllTrackerSubdetectors(std::vector< Subdetector* > * SubDetectors){
				SubDetectors->push_back(new SiVertexBarrel());
				SubDetectors->push_back(new SiVertexEndcap());
				SubDetectors->push_back(new SiTrackerBarrel());
				SubDetectors->push_back(new SiTrackerEndcap());
				SubDetectors->push_back(new SiTrackerForward());
}       
void InitializeWhichSubdetector(std::string SubdetectorName, std::vector< Subdetector* > * SubDetectors){
				if (SubdetectorName == std::string("EcalBarrel")) SubDetectors->push_back(new EcalBarrel());
				if (SubdetectorName == std::string("EcalEndcap")) SubDetectors->push_back(new EcalEndcap());
				if (SubdetectorName == std::string("HcalBarrel")) SubDetectors->push_back(new HcalBarrel());
				if (SubdetectorName == std::string("HcalEndcap")) SubDetectors->push_back(new HcalEndcap());
				if (SubdetectorName == std::string("MuonBarrel")) SubDetectors->push_back(new MuonBarrel());
				if (SubdetectorName == std::string("MuonEndcap")) SubDetectors->push_back(new MuonEndcap());
				if (SubdetectorName == std::string("BeamCal"))    SubDetectors->push_back(new BeamCal());
				if (SubdetectorName == std::string("LumiCal"))    SubDetectors->push_back(new LumiCal());
				if (SubdetectorName == std::string("SiVertexBarrel"))   SubDetectors->push_back(new SiVertexBarrel());
				if (SubdetectorName == std::string("SiVertexEndcap"))   SubDetectors->push_back(new SiVertexEndcap());
				if (SubdetectorName == std::string("SiTrackerBarrel"))  SubDetectors->push_back(new SiTrackerBarrel());
				if (SubdetectorName == std::string("SiTrackerEndcap"))  SubDetectors->push_back(new SiTrackerEndcap());
				if (SubdetectorName == std::string("SiTrackerForward")) SubDetectors->push_back(new SiTrackerForward());
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

#endif /*DRAWINGHISTOS*/
