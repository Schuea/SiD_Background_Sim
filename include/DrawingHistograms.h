#ifndef DRAWINGHISTOS
#define DRAWINGHISTOS

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

				std::vector<Subdetector*> SubDetectors;
				for (size_t s = 0; s < argument_subdetectors.size(); ++s) {
								if (argument_subdetectors.size() == 1) {
												several_subdetector_names.str("");
												several_subdetector_names << argument_subdetectors.at(0);
												if (argument_subdetectors.at(0) == std::string("allCalo")) {
																Subdetector* SubCalo1;
																Subdetector* SubCalo2;
																Subdetector* SubCalo3;
																Subdetector* SubCalo4;
																Subdetector* SubCalo5;
																Subdetector* SubCalo6;
																Subdetector* SubCalo7;
																Subdetector* SubCalo8;

																InitializeAllCaloSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8);

																SubDetectors.push_back(SubCalo1);
																SubDetectors.push_back(SubCalo2);
																SubDetectors.push_back(SubCalo3);
																SubDetectors.push_back(SubCalo4);
																SubDetectors.push_back(SubCalo5);
																SubDetectors.push_back(SubCalo6);
																SubDetectors.push_back(SubCalo7);
																SubDetectors.push_back(SubCalo8);
												}
												if (argument_subdetectors.at(s) == std::string("allTracker")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors.clear();
																Subdetector* SubTracker1;
																Subdetector* SubTracker2;
																Subdetector* SubTracker3;
																Subdetector* SubTracker4;
																Subdetector* SubTracker5;

																InitializeAllTrackerSubdetectors(SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5);

																SubDetectors.push_back(SubTracker1);
																SubDetectors.push_back(SubTracker2);
																SubDetectors.push_back(SubTracker3);
																SubDetectors.push_back(SubTracker4);
																SubDetectors.push_back(SubTracker5);
												}
												if (argument_subdetectors.at(s) == std::string("all")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors.clear();
																Subdetector* SubCalo1;
																Subdetector* SubCalo2;
																Subdetector* SubCalo3;
																Subdetector* SubCalo4;
																Subdetector* SubCalo5;
																Subdetector* SubCalo6;
																Subdetector* SubCalo7;
																Subdetector* SubCalo8;
																Subdetector* SubTracker1;
																Subdetector* SubTracker2;
																Subdetector* SubTracker3;
																Subdetector* SubTracker4;
																Subdetector* SubTracker5;

																InitializeAllSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8,SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5);

																SubDetectors.push_back(SubCalo1);
																SubDetectors.push_back(SubCalo2);
																SubDetectors.push_back(SubCalo3);
																SubDetectors.push_back(SubCalo4);
																SubDetectors.push_back(SubCalo5);
																SubDetectors.push_back(SubCalo6);
																SubDetectors.push_back(SubCalo7);
																SubDetectors.push_back(SubCalo8);
																SubDetectors.push_back(SubTracker1);
																SubDetectors.push_back(SubTracker2);
																SubDetectors.push_back(SubTracker3);
																SubDetectors.push_back(SubTracker4);
																SubDetectors.push_back(SubTracker5);

												} else {
																Subdetector* SubDetector;
																InitializeWhichSubdetector(argument_subdetectors.at(0));
																SubDetectors.push_back(SubDetector);
												}
								} else {
												if (argument_subdetectors.at(s) == std::string("all")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors.clear();
																Subdetector* SubCalo1;
																Subdetector* SubCalo2;
																Subdetector* SubCalo3;
																Subdetector* SubCalo4;
																Subdetector* SubCalo5;
																Subdetector* SubCalo6;
																Subdetector* SubCalo7;
																Subdetector* SubCalo8;
																Subdetector* SubTracker1;
																Subdetector* SubTracker2;
																Subdetector* SubTracker3;
																Subdetector* SubTracker4;
																Subdetector* SubTracker5;

																InitializeAllSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8,SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5);

																SubDetectors.push_back(SubCalo1);
																SubDetectors.push_back(SubCalo2);
																SubDetectors.push_back(SubCalo3);
																SubDetectors.push_back(SubCalo4);
																SubDetectors.push_back(SubCalo5);
																SubDetectors.push_back(SubCalo6);
																SubDetectors.push_back(SubCalo7);
																SubDetectors.push_back(SubCalo8);
																SubDetectors.push_back(SubTracker1);
																SubDetectors.push_back(SubTracker2);
																SubDetectors.push_back(SubTracker3);
																SubDetectors.push_back(SubTracker4);
																SubDetectors.push_back(SubTracker5);

																break;
												}
												if (argument_subdetectors.at(s) == std::string("allCalo")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors.clear();
																Subdetector* SubCalo1;
																Subdetector* SubCalo2;
																Subdetector* SubCalo3;
																Subdetector* SubCalo4;
																Subdetector* SubCalo5;
																Subdetector* SubCalo6;
																Subdetector* SubCalo7;
																Subdetector* SubCalo8;

																InitializeAllCaloSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8);

																SubDetectors.push_back(SubCalo1);
																SubDetectors.push_back(SubCalo2);
																SubDetectors.push_back(SubCalo3);
																SubDetectors.push_back(SubCalo4);
																SubDetectors.push_back(SubCalo5);
																SubDetectors.push_back(SubCalo6);
																SubDetectors.push_back(SubCalo7);
																SubDetectors.push_back(SubCalo8);

																if (argument_subdetectors.at(s+1) == std::string("allTracker")){
																				std::cerr << "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!" << std::endl;
																				terminate();
																}
																else break;
												}
												if (argument_subdetectors.at(s) == std::string("allTracker")) {
																several_subdetector_names.str("");
																several_subdetector_names << argument_subdetectors.at(s);

																SubDetectors.clear();
																Subdetector* SubTracker1;
																Subdetector* SubTracker2;
																Subdetector* SubTracker3;
																Subdetector* SubTracker4;
																Subdetector* SubTracker5;

																InitializeAllTrackerSubdetectors(SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5);

																SubDetectors.push_back(SubTracker1);
																SubDetectors.push_back(SubTracker2);
																SubDetectors.push_back(SubTracker3);
																SubDetectors.push_back(SubTracker4);
																SubDetectors.push_back(SubTracker5);
																if (argument_subdetectors.at(s+1) == std::string("allCalo")){
																				std::cerr << "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!" << std::endl;
																				terminate();
																}
																else break;

												} else {
																several_subdetector_names << argument_subdetectors.at(s);
																Subdetector* SubDetector;
																InitializeWhichSubdetector(argument_subdetectors.at(s));
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


}
void InitializeAllSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8,SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5){
				InitializeAllCaloSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8);
				InitializeAllTrackerSubdetectors(SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5);
}				
void InitializeAllCaloSubdetectors(SubCalo1,SubCalo2,SubCalo3,SubCalo4,SubCalo5,SubCalo6,SubCalo7,SubCalo8){
				SubCalo1 = new EcalBarrel();
				SubCalo2 = new EcalEndcap();
				SubCalo3 = new HcalBarrel();
				SubCalo4 = new HcalEndcap();
				SubCalo5 = new MuonBarrel();
				SubCalo6 = new MuonEndcap();
				SubCalo7 = new BeamCal();
				SubCalo8 = new LumiCal();
}
void InitializeAllTrackerSubdetectors(SubTracker1,SubTracker2,SubTracker3,SubTracker4,SubTracker5){
				SubTracker1 = new SiVertexBarrel();
				SubTracker2 = new SiVertexEndcap();
				SubTracker3 = new SiTrackerBarrel();
				SubTracker4 = new SiTrackerEndcap();
				SubTracker5 = new SiTrackerForward();
}
void InitializeWhichSubdetector(std::string SubdetectorName){
				if (SubdetectorName == std::string("EcalBarrel")){
								SubDetector = new EcalBarrel();
				}
				if (SubdetectorName == std::string("EcalEndcap")){
								SubDetector = new EcalEndcap();
				}
				if (SubdetectorName == std::string("HcalBarrel")){
								SubDetector = new HcalBarrel();
				}
				if (SubdetectorName == std::string("HcalEndcap")){
								SubDetector = new HcalEndcap();
				}
				if (SubdetectorName == std::string("MuonBarrel")){
								SubDetector = new MuonBarrel();
				}
				if (SubdetectorName == std::string("MuonEndcap")){
								SubDetector = new MuonEndcap();
				}
				if (SubdetectorName == std::string("BeamCal")){
								SubDetector = new BeamCal();
				}
				if (SubdetectorName == std::string("LumiCal")){
								SubDetector = new LumiCal();
				}
				if (SubdetectorName == std::string("SiVertexBarrel")){
								SubDetector = new SiVertexBarrel();
				}
				if (SubdetectorName == std::string("SiVertexEndcap")){
								SubDetector = new SiVertexEndcap();
				}
				if (SubdetectorName == std::string("SiTrackerBarrel")){
								SubDetector = new SiTrackerBarrel();
				}
				if (SubdetectorName == std::string("SiTrackerEndcap")){
								SubDetector = new SiTrackerEndcap();
				}
				if (SubdetectorName == std::string("SiTrackerForward")){
								SubDetector = new SiTrackerForward();
				}
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
