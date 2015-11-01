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
#include "TGraph.h"
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
void DrawingMacro(string outputname, std::vector<string> inputnames,
		string subdetector);
void DrawingCaloHitsMap(string subdetector, string Canvas_name);
void DrawingHitsPerEvent(string Canvas_name);

const int NUMBER_OF_BUNCHES = 5;//1312;

std::vector<TFile *> input_rootfiles;
std::vector<TTree *> Trees;
std::vector<TTree *> Trees_MCP;

int main(int argc, char *argv[]) {

	if (argc < 2) {
		Usage();
	}

	std::vector<string> inputfilenames;
	string outputfilename;
	string subdetector;
	bool inputfile_set = false;
	bool outputfile_set = false;
	bool subdetector_set = false;

	//Find the input and output file:
	for (int i = 1; i < argc; i++) {
		if (argv[i] == std::string("-h") || argv[i] == std::string("--help=")){
			Usage();
		}
		if (argv[i] == std::string("-i")) {
			if (argv[i + 1] != NULL){
				for (int j = 1; j <= NUMBER_OF_BUNCHES; ++j) {
					inputfilenames.push_back(argv[i + j]);
				}
				inputfile_set = true;
			}
			else{
				cerr << "You didn't give an argument for the inputfile(s)!" << endl;
				Usage();
			}
		}
		if (argv[i] == std::string("-o")) {
			if (argv[i + 1] != NULL){
				outputfilename = argv[i + 1];
				outputfile_set = true;
			}
			else{
				cerr << "You didn't give an argument for the outputfile!" << endl;
				Usage();
			}
		}
		if (argv[i] == std::string("-s")) {
			if (argv[i + 1] != NULL){
				subdetector = argv[i + 1];
				subdetector_set = true;
			}
			else{
				cerr << "You didn't give an argument for the subdetector!" << endl;
				Usage();
			}
		}
	}

	if (!inputfile_set || !outputfile_set || !subdetector_set) {
		cerr
				<< "You didn't give the name for the subdector, the inputfile or outputfile. Please try again!"
				<< endl;
		Usage();
	}
	try{
		DrawingMacro(outputfilename, inputfilenames, subdetector);
	}
	catch(std::exception& e){
		std::cerr << "There is no TTree for a subdetector with the given name in the ROOT files.\n";
		std::cerr << "You probably misspelled the name of the subdetector." << std::endl;
	}

	return 0;
}

void DrawingMacro(string outputname, std::vector<string> inputnames,
		string subdetector) {

	std::vector<std::string> Tree_names;
	if (subdetector == std::string("all")){
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
	}
	else{
		stringstream temp;
		temp << "Tree_" << subdetector;
		Tree_names.push_back(temp.str());
	}


	for (int j = 0; j < NUMBER_OF_BUNCHES; ++j) {
		TFile* File = TFile::Open((inputnames.at(j)).c_str());
		if(!File){
					throw std::exception();
				}
		input_rootfiles.push_back(File);

		std::cout << input_rootfiles.at(j)->GetName() << std::endl;

		for (int t = 0; t < Tree_names.size(); ++t){
			TTree* Tree = nullptr;
			File->GetObject(Tree_names.at(t).c_str(), Tree);
			if(!Tree){
				throw std::exception();
			}

			Trees.push_back(Tree);
		}
		TTree* TreeMCP;
		File->GetObject("Tree_MCP", TreeMCP);
		Trees_MCP.push_back(TreeMCP);
	}
	cout << "Accessed TTrees.." << endl;

	TFile * output_rootfile = new TFile(outputname.c_str(), "RECREATE");
	std::size_t found = outputname.find_last_of(".");
	string Canvas_name = outputname.substr(0, found);
	std::cout << "The name for the ouput is: " << Canvas_name << std::endl;

	DrawingCaloHitsMap(subdetector, Canvas_name);

	output_rootfile->Write();
	output_rootfile->Close();

	DrawingHitsPerEvent(Canvas_name);

	for (int j = 0; j < NUMBER_OF_BUNCHES; ++j) {
		input_rootfiles.at(j)->Close();
		delete input_rootfiles.at(j);
	}
	delete output_rootfile;
}

void DrawingHitsPerEvent(string Canvas_name) {
//	TCanvas* ParticlesPerEvent_Canvas = new TCanvas((Canvas_name + "_ParticlesPerEvent").c_str());
//	ParticlesPerEvent_Canvas->Divide(2,2);
	TCanvas* Files_Canvas = new TCanvas((Canvas_name + "_HitsPerFile").c_str());
	gStyle->SetOptStat(1);
	TH2D* ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event",
			50, 0, NUMBER_OF_BUNCHES, 50, 190000, 210000);
	TH1D* Hits = new TH1D("Hits", "Hits", 50, 20, 120);
	TH1D* Particles = new TH1D("Particles", "Particles", 100, 100000, 280000);

	//std::vector<TH1D*> ParticlesPerEvent;
	//std::vector<TF1*> gausfit_ParticlesPerEvent;
	//TH2D* Spread_ParticlesPerEvent = new TH2D("ParticlesSpread","Spread of particle per event", 50, 0, NUMBER_OF_BUNCHES, 10, 0,10);

	int number_of_particles = 0;
	int number_of_hits = 0;

	for (int n = 0; n < NUMBER_OF_BUNCHES; ++n) {
		number_of_particles = Trees_MCP.at(n)->GetEntries();
		number_of_hits = Trees.at(n)->GetEntries();
		ParticlesVSEvent->Fill(n, number_of_particles);
		Particles->Fill(number_of_particles);
		Hits->Fill(number_of_hits);
		/*
		 ParticlesPerEvent.emplace_back(new TH1D("", "", 50, 190000, 210000));
		 ParticlesPerEvent.at(n)->Fill(number_of_particles);

		 ParticlesPerEvent_Canvas->cd(n+1);
		 ParticlesPerEvent.at(n)->Draw();
		 gausfit_ParticlesPerEvent.emplace_back(
		 new TF1("gausfit", "gaus", 190000, 210000));
		 ParticlesPerEvent.at(n)->Fit(gausfit_ParticlesPerEvent.at(n), "R");
		 Spread_ParticlesPerEvent->Fill(n,
		 2 * gausfit_ParticlesPerEvent.at(n)->GetParameter(2));
		 */
	}
	/*
	 stringstream ParticlesPerEventCanvasName_eps, ParticlesPerEventCanvasName_C;
	 ParticlesPerEventCanvasName_eps << ParticlesPerEvent_Canvas->GetName() << ".eps";
	 ParticlesPerEventCanvasName_C << ParticlesPerEvent_Canvas->GetName() << ".C";
	 ParticlesPerEvent_Canvas->Write();
	 ParticlesPerEvent_Canvas->Print(ParticlesPerEventCanvasName_eps.str().c_str());
	 ParticlesPerEvent_Canvas->Print(ParticlesPerEventCanvasName_C.str().c_str());
	 ParticlesPerEvent_Canvas->Close();
	 for (int n = 0; n < NUMBER_OF_BUNCHES; ++n) {
	 delete ParticlesPerEvent.at(n);
	 }
	 */
	Files_Canvas->Divide(2, 2);
	Files_Canvas->cd(1);
	ParticlesVSEvent->Draw();

	Files_Canvas->cd(2);
	Particles->Draw();
	TF1* gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);
	Particles->Fit(gausfit_Particles, "R");

	Files_Canvas->cd(3);
	Hits->Draw();
	TF1* gausfit_Hits = new TF1("gausfit", "gaus", 20, 120);
	Hits->Fit(gausfit_Hits, "R");

//	Files_Canvas->cd(4);
//	Spread_ParticlesPerEvent->Draw();

	stringstream FilesCanvasName_eps, FilesCanvasName_C;
	FilesCanvasName_eps << Files_Canvas->GetName() << ".eps";
	FilesCanvasName_C << Files_Canvas->GetName() << ".C";

	Files_Canvas->Write();
	Files_Canvas->Print(FilesCanvasName_eps.str().c_str());
	Files_Canvas->Print(FilesCanvasName_C.str().c_str());
	Files_Canvas->Close();

	delete Files_Canvas;
	delete ParticlesVSEvent, Hits, Particles;
	delete gausfit_Hits, gausfit_Particles;
}

void DrawingCaloHitsMap(string subdetector, string Canvas_name) {
	try {
		std::vector<CaloIDs> SubDetectors;
		if (subdetector == std::string("all")){
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
		}
		else{
			CaloIDs SubDetector(subdetector);
			SubDetectors.push_back(SubDetector);
		}

		int id0 = 0;
		int id1 = 0;
		//double cell_posx = 0;
		//double cell_posy = 0;
		float x = 0;
		float y = 0;
		float z = 0;
		int Layer_no = 0;
		long CellIDkey = 0.;

		vector<TCanvas*> Hits_Canvas_;
		vector<TH1D*> Hits_Histo_;
		vector<TH2D*> Hits_2D_;
		vector<TH3D*> Hits_3D_;

for(int s = 0; s < SubDetectors.size(); ++s){
		for (int l = 0; l < SubDetectors.at(s).Number_layers; ++l) {

			std::stringstream layer;
			layer << l;
			Hits_Canvas_.emplace_back(new TCanvas(
							(Canvas_name + "_Hits_" + subdetector + "_Layer" + layer.str()).c_str()));

			stringstream histo_name, histo_title, histo_2d_name, histo_2d_title,
					histo_3d_name, histo_3d_title;
			histo_name << "Hits_" << subdetector << "_Layer_[" << l << "]";
			histo_title << "Hits per cells for " << subdetector << " layer " << l;
			Hits_Histo_.emplace_back(
					new TH1D(histo_name.str().c_str(),
							histo_title.str().c_str(), 50, 0., 50));
			Hits_Histo_.at(l)->SetLineColor(kBlue);
			Hits_Histo_.at(l)->GetXaxis()->SetTitle("Hits per cell");
			Hits_Histo_.at(l)->GetXaxis()->CenterTitle();


			gROOT->ForceStyle();
			gStyle->SetCanvasPreferGL(kTRUE);
			gStyle->SetPalette(1);

			std::array<int,9> axis_ranges_plot = {}; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup

			if (subdetector == std::string("EcalBarrel")) {
				std::array<int,9> temp3D = {50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500};
				axis_ranges_plot = temp3D;
			}
			else if (subdetector == std::string("EcalEndcap")) {
				std::array<int,9> temp = {50, -1850, 1850, 50, -1500, 1500, 50, -1500, 1500};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("HcalBarrel")) {
				std::array<int,9> temp = {50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("HcalEndcap")) {
				std::array<int,9> temp = {100, -3100, 3100, 50, -1350, 1350, 50, -1350, 1350};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("MuonBarrel")) {
				std::array<int,9> temp = {50, -1700, 1700, 50, -1500, 1500, 50, -1500, 1500};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("MuonEndcap")) {
				std::array<int,9> temp = {150, -5800, 5800, 50, -5700, 5700, 50, -5700, 5700};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("LumiCal")) {
				std::array<int,9> temp = {50, -1900, 1900, 50, -200, 200, 50, -200, 200};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("BeamCal")) {
				std::array<int,9> temp = {100, -3200, 3200, 50, -135, 135, 50, -135, 135};
				axis_ranges_plot = temp;
			}
			else if (subdetector == std::string("all")){
				std::array<int,9> temp = {100, -5600, 5600, 100, -7100, 7100, 100, -7100, 7100};
				axis_ranges_plot = temp;
			}

			histo_2d_name << "Hits_2D_" << subdetector << "_Layer_[" << l
					<< "]";
			histo_2d_title << "HitMap for " << subdetector << " layer " << l;
			Hits_2D_.emplace_back(
					new TH2D(histo_2d_name.str().c_str(),
							histo_2d_title.str().c_str(),
							axis_ranges_plot[3], axis_ranges_plot[4],
							axis_ranges_plot[5], axis_ranges_plot[6],
							axis_ranges_plot[7], axis_ranges_plot[8]));

			Hits_2D_.at(l)->SetLineColor(kBlue);
			Hits_2D_.at(l)->GetYaxis()->SetTitle("y (mm)");
			Hits_2D_.at(l)->GetYaxis()->CenterTitle();
			Hits_2D_.at(l)->GetXaxis()->SetTitle("x (mm)");
			Hits_2D_.at(l)->GetXaxis()->CenterTitle();

			histo_3d_name << "Hits_3D_" << subdetector << "_Layer_[" << l
					<< "]";
			histo_3d_title << "HitMap for " << subdetector << " layer " << l;
			Hits_3D_.emplace_back(
					new TH3D(histo_3d_name.str().c_str(),
							histo_3d_title.str().c_str(),
							axis_ranges_plot[0], axis_ranges_plot[1], axis_ranges_plot[2],
							axis_ranges_plot[3], axis_ranges_plot[4], axis_ranges_plot[5],
							axis_ranges_plot[6], axis_ranges_plot[7], axis_ranges_plot[8]));
			Hits_3D_.at(l)->SetLineColor(kBlue);
			Hits_3D_.at(l)->GetXaxis()->SetTitle("z (mm)");
			Hits_3D_.at(l)->GetXaxis()->CenterTitle();
			Hits_3D_.at(l)->GetZaxis()->SetTitle("y (mm)");
			Hits_3D_.at(l)->GetZaxis()->CenterTitle();
			Hits_3D_.at(l)->GetYaxis()->SetTitle("x (mm)");
			Hits_3D_.at(l)->GetYaxis()->CenterTitle();
		}

		std::map<long, int> HitMap;
		std::vector<int> hitLayers;
		std::vector<int> nentries;

		for (int j = 0; j < NUMBER_OF_BUNCHES; ++j) {

			nentries.push_back(Trees.at(j)->GetEntries());
			std::cout << "The TTree " << Trees.at(j)->GetName() << " has "
					<< nentries.at(j) << " entries." << std::endl;
			Trees.at(j)->SetBranchStatus("*", kFALSE); // disable all
			Trees.at(j)->SetBranchStatus("HitCellID0", kTRUE);
			Trees.at(j)->SetBranchStatus("HitCellID1", kTRUE);
			Trees.at(j)->SetBranchStatus("HitPosition_x", kTRUE);
			Trees.at(j)->SetBranchStatus("HitPosition_y", kTRUE);
			Trees.at(j)->SetBranchStatus("HitPosition_z", kTRUE);
			Trees.at(j)->SetBranchAddress("HitCellID0", &id0);
			Trees.at(j)->SetBranchAddress("HitCellID1", &id1);
			Trees.at(j)->SetBranchAddress("HitPosition_x", &x);
			Trees.at(j)->SetBranchAddress("HitPosition_y", &y);
			Trees.at(j)->SetBranchAddress("HitPosition_z", &z);

			for (int i = 0; i < nentries.at(j); i++) {
				CellIDkey = 0.;

				Trees.at(j)->GetEntry(i);

				SubDetectors.at(s).GetCellID(id0, id1);

				//cell_posx = SubDetector.GetCellPos(id1)[0];
				//cell_posy = SubDetector.GetCellPos(id1)[1];
				//cout << "Cell with unique ID " << SubDetector.CellID << " is at x = " << cell_posx << " and y = " << cell_posy << endl;

				Layer_no = SubDetectors.at(s).GetLayer(id0);
				//cout << "The cell is in Layer " << Layer_no << endl;

				CellIDkey = SubDetectors.at(s).CellID.to_ulong();

				if (HitMap.find(CellIDkey) == HitMap.end()) {
					HitMap[CellIDkey] = 0;
				} else
					HitMap[CellIDkey] += 1;

				if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no)
						== hitLayers.end()) {
					hitLayers.push_back(Layer_no);
				}
				if (HitMap[CellIDkey] != 0) {
					Hits_Histo_.at(Layer_no)->Fill(HitMap[CellIDkey]);
				}
				Hits_2D_.at(Layer_no)->Fill(x, y);
				Hits_3D_.at(Layer_no)->Fill(z, x, y,1);
			}

		}


		for (signed int l = 0; l < hitLayers.size(); ++l) {


			Hits_Canvas_.at(hitLayers.at(l))->Divide(1, 2);
			Hits_Canvas_.at(hitLayers.at(l))->cd(1);
			Hits_Histo_.at(hitLayers.at(l))->Draw();
			Hits_Canvas_.at(hitLayers.at(l))->cd(2);
			Hits_2D_.at(hitLayers.at(l))->Draw("colz");

			stringstream HitsCanvasName_eps, HitsCanvasName_C;
			HitsCanvasName_eps << Hits_Canvas_.at(hitLayers.at(l))->GetName()
					<< ".eps";
			HitsCanvasName_C << Hits_Canvas_.at(hitLayers.at(l))->GetName()
					<< ".C";

			Hits_Canvas_.at(hitLayers.at(l))->Write();
			Hits_Canvas_.at(hitLayers.at(l))->Print(HitsCanvasName_eps.str().c_str());
			Hits_Canvas_.at(hitLayers.at(l))->Print(HitsCanvasName_C.str().c_str());
			Hits_Canvas_.at(hitLayers.at(l))->Close();

		}
		for (signed int l = 0; l < SubDetectors.at(s).Number_layers; ++l) {
			delete Hits_Histo_.at(l), Hits_2D_.at(l), Hits_3D_.at(l), Hits_Canvas_.at(
					l);
		}
	}
	}catch (const std::bad_alloc&){
		std::cerr<< "You have a memory leak!";
		terminate();
	}
	catch (std::exception& ex) {
		std::cerr<< "The given subdetector is not known. Maybe you misspelled it?";
		terminate();
	}
}


void Usage() {
	//explain how to use program
	cerr << "Usage:" << endl;
	cerr
			<< "Type in the name of the subdetector you are interested in, as well as the output and the input root file after their correspondent flags!"
			<< endl;
	cerr << "e.g. ./DrawHistograms -s EcalBarrel -o output.root -i file.root"
			<< endl;
	terminate();
}
