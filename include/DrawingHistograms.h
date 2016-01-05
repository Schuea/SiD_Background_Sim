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
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
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

#include <map>

#include "FunctionsForDrawingMacro.h"
#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"

TFile* inputfile;
TCanvas* Hits_Canvas_;
std::vector<TH2D*> ParticleOrigins_2D_;
std::vector<TH1D*> Hits_PerLayer_;
std::vector<TH1D*> Hits_Histo_;
std::vector<TH2D*> Hits_2D_;
std::vector<TH3D*> Hits_3D_;
std::vector<TH1D*> Hits_Energy_Histo_;
std::vector<TH2D*> Hits_Energy_2D_;
std::vector<TH3D*> Hits_Energy_3D_;

int NUMBER_OF_FILES;
int first_layer_to_be_compared;
int last_layer_to_be_compared;

void DrawingMacro(std::string outputname, std::vector<std::string> inputnames,
		std::vector<std::string> argument_subdetectors) {
	TH1::SetDefaultSumw2();

	TFile * output_rootfile = new TFile(outputname.c_str(), "RECREATE");
	std::size_t found = outputname.find_last_of(".");
	std::string Canvas_name = outputname.substr(0, found);

	std::cout << __FILE__ << ": " << __LINE__ << std::endl;

	TCanvas* Files_Canvas = new TCanvas((Canvas_name + "_HitsPerFile").c_str());
	TH2D* ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event", 50, 0, NUMBER_OF_FILES, 50, 190000,
			210000);
	TH1D* Particles = new TH1D("Particles", "Particles", 100, 180000, 220000);
	TF1* gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);

	bool YesNo_TrackerHistograms = DecideIfTrackerHistograms(argument_subdetectors);

	std::string * subdetector_name2 = new std::string("");
	std::vector<Subdetector*> * SubDetectors = new std::vector<Subdetector*>();
	SetupSubDetectorsVector(SubDetectors, subdetector_name2, argument_subdetectors);
	std::string subdetector_name = *subdetector_name2;

	std::cout << "Subdetectors: " << std::endl;
	for (int i = 0; i < SubDetectors->size(); ++i) {
		std::cout << SubDetectors->at(i)->GetName() << std::endl;
	}
	std::vector<float> axis_range_plot_1D = { }; //xbins, xlow, xup
	std::vector<float> axis_range_plot_2D = { }; //xbins, xlow, xup, ybins, ylow, yup
	std::vector<float> axis_range_plot_3D = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup
	std::vector<float> axis_range_plot_energy_1D = { }; //xbins, xlow, xup

	Setup_BinningArrays(SubDetectors, &axis_range_plot_1D, &axis_range_plot_2D, &axis_range_plot_3D,
			&axis_range_plot_energy_1D);
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0] * 3, axis_range_plot_1D[1], axis_range_plot_1D[2] * 40);
	///*MuonBarrel*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/5, 100, axis_range_plot_1D[2]);
	///*MuonEndcap*/TH1D* Hits = new TH1D("Hits", "Hits", 35,5500, 7300);
	///*BeamCal:*/ TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/4, 4200000, 4850000);
	///*ALL:*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*2/3, 4000000, 5000000);
	Hits->GetYaxis()->SetTitle("Count");
	Hits->GetXaxis()->SetTitle(("Hits per bunch crossing in " + subdetector_name).c_str());
	Hits->GetXaxis()->CenterTitle();
	//Scaling up the histogram ranges from the subdetector specific single layer hits plot, so that data fit on plot
	//TF1* gausfit_Hits = new TF1("gausfit", "gaus", 0, 150);

	/*
	int id = 0;
	int id0 = 0;
	int id1 = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	double vertex_x = 0;
	double vertex_y = 0;
	double vertex_z = 0;
	float energy = 0;
	*/
	int MaxNumberLayers = 0;
	int Layer_no = 0;
	int CellIDkey = 0.;

	std::map<int, std::vector<int> > HitsPerLayerMap;

	//Find the largest number of layers from all the subdetectors that are to be plotted
	for (int s = 0; s < SubDetectors->size(); ++s) {
		MaxNumberLayers = FindMax(SubDetectors->at(s)->GetNumberOfLayers(), MaxNumberLayers);
	}
	for (int l = 0; l < MaxNumberLayers; ++l) {
		for (int files = 1; files <= NUMBER_OF_FILES; ++files) {
			HitsPerLayerMap[l].push_back(0);
      std::cout << "Pushing back into layer " << l << std::endl;
		}

		std::stringstream layer;
		layer << l;
		std::string layerstring = layer.str();
		Hits_Canvas_ = new TCanvas((Canvas_name + "_Hits_" + subdetector_name).c_str());

		gROOT->ForceStyle();
		gStyle->SetPalette(1);

		std::string histo_name1D;
		std::string histo_title1D;
		std::string histo_name2D;
		std::string histo_title2D;
		std::string histo_name3D;
		std::string histo_title3D;
		std::string energyhisto_name1D;
		std::string energyhisto_title1D;
		std::string energyhisto_name2D;
		std::string energyhisto_title2D;
		std::string energyhisto_name3D;
		std::string energyhisto_title3D;
		std::string hitsperlayerhisto_name;
		std::string hitsperlayerhisto_title;
		std::string particleoriginshisto_name;
		std::string particleoriginshisto_title;

		SetupHistoTitles(subdetector_name, layerstring, histo_name1D, histo_title1D, histo_name2D, histo_title2D,
				histo_name3D, histo_title3D, energyhisto_name1D, energyhisto_title1D, energyhisto_name2D,
				energyhisto_title2D, energyhisto_name3D, energyhisto_title3D, hitsperlayerhisto_name,
				hitsperlayerhisto_title, particleoriginshisto_name, particleoriginshisto_title);
		Setup_ParticleOriginsHisto(ParticleOrigins_2D_, axis_range_plot_3D, particleoriginshisto_name,
				particleoriginshisto_title, "cylindrical");
		Setup_Histo(Hits_PerLayer_, axis_range_plot_1D, hitsperlayerhisto_name, hitsperlayerhisto_title);
		Setup_Histo(Hits_Histo_, axis_range_plot_1D, histo_name1D, histo_title1D);
		Setup_Histo(Hits_2D_, axis_range_plot_2D, histo_name2D, histo_title2D);
		Setup_Histo(Hits_3D_, axis_range_plot_3D, histo_name3D, histo_title3D);
		Setup_Histo(Hits_Energy_Histo_, axis_range_plot_energy_1D, energyhisto_name1D, energyhisto_title1D);
		Setup_Histo(Hits_Energy_2D_, axis_range_plot_2D, energyhisto_name2D, energyhisto_title2D);
		Setup_Histo(Hits_Energy_3D_, axis_range_plot_3D, energyhisto_name3D, energyhisto_title3D);
	}
	std::vector<int> hitLayers;
	for (int s = 0; s < SubDetectors->size(); ++s) {

		std::map<int, int> HitMap;  //cellid, count of hits per cell

		//Getting the inputfile and its TTrees
		for (int fileIterator = 0; fileIterator < NUMBER_OF_FILES; ++fileIterator) {
			inputfile = TFile::Open((inputnames.at(fileIterator)).c_str());
			if (!inputfile) {
				throw std::exception();
			}
			TTree* Tree_MCP;
			inputfile->GetObject("Tree_MCP", Tree_MCP);

			int number_of_hits = 0;
      TTree *SubdetectorTree = Get_TTree(inputfile, SubDetectors->at(s)->GetName());
			number_of_hits = SubdetectorTree->GetEntries();

			std::cout << "The TTree " << SubdetectorTree->GetName() << " has "
					<< number_of_hits << " entries." << std::endl;

			Data* data = SetBranches(SubdetectorTree);

			std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy2D; //layer, bin, energies
			std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy3D; //layer, bin, energies

			for (std::size_t i = 0; i < number_of_hits; i++) {
				SubdetectorTree->GetEntry(i);

				CellID *SubdetectorCells = InitializeCellIDClass(SubDetectors->at(s)->GetName(), data);
				std::cout << SubdetectorCells << std::endl;
				SubdetectorCells->CreateCellID();
				CellIDkey = 0.;
				CellIDkey = SubdetectorCells->CellID_ToINTconversion(SubdetectorCells->GetCellID());
				LayerCodeInCellID LayerInfo;
				Layer_no = LayerInfo.GetLayer(SubdetectorCells->GetCellID(), SubDetectors->at(s)->GetStartLayerBin(),
						SubDetectors->at(s)->GetLengthLayerBin());
				if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no) == hitLayers.end()) {
					hitLayers.push_back(Layer_no);
				}
        std::cout << "fileIterator = " << fileIterator << std::endl;
        std::cout << "Layer = " << Layer_no << std::endl;
        std::cout << "Mapvector size = " << HitsPerLayerMap[Layer_no].size() << std::endl;
				//Fill Maps:
				HitsPerLayerMap[Layer_no].at(fileIterator) += 1;
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;

				float energy = 0.;
				if (YesNo_TrackerHistograms) energy = data->Get_dEdx_hit();
				if (!YesNo_TrackerHistograms) energy = data->Get_energy_hit();
				std::array <double, 3> vertex = {0};
				if (YesNo_TrackerHistograms) vertex = data->Get_vertex_particle();
				if (!YesNo_TrackerHistograms) vertex = data->Get_vertex_mother();
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;


        std::cout << "Layer = " << Layer_no << std::endl;
        std::cout << "Hits_Energy_2D size = " << Hits_Energy_2D_.size() << std::endl;
        std::cout << "2Dhistobin = " << Hits_Energy_2D_.at(Layer_no)->FindBin(data->Get_x_hit(), data->Get_y_hit()) << std::endl;
        std::cout << "HitMapEnergy2Dvector size = " << HitMapEnergy2D[std::pair<int, int>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(data->Get_x_hit(), data->Get_y_hit()))].size() << std::endl;
				HitMapEnergy2D[std::pair<int, int>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(data->Get_x_hit(), data->Get_y_hit()))].push_back(energy);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
				HitMapEnergy3D[std::pair<int, int>(Layer_no, Hits_Energy_3D_.at(Layer_no)->FindBin(data->Get_z_hit(), data->Get_x_hit(), data->Get_y_hit()))].push_back(energy);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;

				if (HitMap.find(CellIDkey) == HitMap.end()) {
					HitMap[CellIDkey] = 1;
				} else {
					HitMap[CellIDkey] += 1;
				}
				//Fill histograms:
				Hits_Energy_Histo_.at(Layer_no)->Fill(energy);
				ParticleOrigins_2D_.at(Layer_no)->Fill(vertex[2], sqrt(pow(vertex[0], 2) + pow(vertex[1], 2)));
				Hits_2D_.at(Layer_no)->Fill(data->Get_x_hit(), data->Get_y_hit());
				Hits_3D_.at(Layer_no)->Fill(data->Get_z_hit(), data->Get_x_hit(), data->Get_y_hit());
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			}
			int const colorrangeweight = 1000000000;
			Fill_Histogram_from_Map<TH2D*>(HitMapEnergy2D, &Hits_Energy_2D_, colorrangeweight);
			Fill_Histogram_from_Map<TH3D*>(HitMapEnergy3D, &Hits_Energy_3D_, colorrangeweight);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			int number_of_particles = 0;
			number_of_particles = Tree_MCP->GetEntries();
			ParticlesVSEvent->Fill(fileIterator, number_of_particles);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			Particles->Fill(number_of_particles);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			Hits->Fill(number_of_hits);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			inputfile->Close();
			delete inputfile;
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
		} //End of loop through inputfiles

		for (auto iterator = HitsPerLayerMap.begin(); iterator != HitsPerLayerMap.end(); iterator++) {
			for (auto e = iterator->second.begin(); e != iterator->second.end(); ++e) {
				if (*e > 0) {
					Hits_PerLayer_.at(iterator->first)->Fill(*e);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
				}
			}
		}
		for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
			if (iterator->second > 0) {
				Hits_Histo_.at(SubDetectors->at(s)->GetLayer(iterator->first))->Fill(iterator->second);
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			}
		}
	} //End of SubDetectors loop

	gStyle->SetOptStat(1);
	//gStyle->SetOptStat(111111);
	TCanvas* PDF_Canvas_1D2D_Hits_Layers = new TCanvas();
	PDF_Canvas_1D2D_Hits_Layers->Print("PDFCanvas_1D2D_Hits_Layers.pdf[");

	output_rootfile->cd();
	for (signed int l = 0; l < hitLayers.size(); ++l) {

    std::cout << "Printing plots for layer number " << hitLayers.at(l) << std::endl;

		Hits_3D_.at(hitLayers.at(l))->Write();
		Hits_Energy_3D_.at(hitLayers.at(l))->Write();

		Hits_Canvas_->cd();

		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_1D2D_Hits_Layers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Energy_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_1D2D_Hits_Layers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(1);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, ParticleOrigins_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_1D2D_Hits_Layers.pdf");
		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		gROOT->ForceStyle();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_.at(hitLayers.at(l)), "",
				"PDFCanvas_1D2D_Hits_Layers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_.at(hitLayers.at(l)), "", "PDFCanvas_1D2D_Hits_Layers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_.at(hitLayers.at(l)), "",
				"PDFCanvas_1D2D_Hits_Layers.pdf");
	}
	PDF_Canvas_1D2D_Hits_Layers->Print("PDFCanvas_1D2D_Hits_Layers.pdf]");
	delete PDF_Canvas_1D2D_Hits_Layers;

	if (first_layer_to_be_compared >= 0) {

		int end_of_range = 0;
		if (last_layer_to_be_compared != 0) {
			end_of_range = last_layer_to_be_compared;
		} else {
			end_of_range = hitLayers.size() - 1;
		}
		//Drawing plots with mean values of hit distributions per bunchcrossing
		int n = end_of_range - first_layer_to_be_compared + 1; //Otherwise the number of layers are wrong
		float layer_numbers[n];
		float layer_numbers_Errors[n];
		std::fill(layer_numbers_Errors, layer_numbers_Errors + n, 0); //Fill layer_number_Errors array with zeros
		float MeanHits[n];
		float MeanHitErrors[n];
		for (size_t i = 0; i <= n; ++i) {
			layer_numbers[i] = first_layer_to_be_compared + i;
			MeanHits[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetMean(1);
			MeanHitErrors[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetRMS(1);
		}
		TGraphErrors* MeanHits_PerLayer = new TGraphErrors(n, layer_numbers, MeanHits, layer_numbers_Errors,
				MeanHitErrors);
		MeanHits_PerLayer->SetName("MeanHits");
		MeanHits_PerLayer->SetTitle("Mean hits for different layers; Layer number; Mean hits per bunch crossing");
		MeanHits_PerLayer->SetMarkerColor(kViolet);
		MeanHits_PerLayer->SetMarkerStyle(21);
		MeanHits_PerLayer->GetXaxis()->SetRangeUser(first_layer_to_be_compared - 0.5, end_of_range + 0.5);
		MeanHits_PerLayer->GetXaxis()->CenterTitle();

		TCanvas* PDF_Canvas_Hits_CompareLayers = new TCanvas();
		PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf[");

		if (std::find(hitLayers.begin(), hitLayers.end(), first_layer_to_be_compared) != hitLayers.end()
				&& std::find(hitLayers.begin(), hitLayers.end(), last_layer_to_be_compared) != hitLayers.end()) {

			Hits_Canvas_->cd();
			Hits_Canvas_->Clear();
			Hits_Canvas_->SetLogy(0);
			WritePrintComparedHistogram<TGraphErrors*>(Hits_Canvas_, MeanHits_PerLayer, first_layer_to_be_compared,
					end_of_range, "AP", "PDFCanvas_Hits_CompareLayers.pdf");
			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_histo_perlayer_title;
			new_histo_perlayer_title << "Hits per layer for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range;
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_, new_histo_perlayer_title.str(), hitLayers,
					first_layer_to_be_compared, end_of_range, false, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_histo_title;
			new_histo_title << "Hit occupancy per cell for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range;
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_, new_histo_title.str(), hitLayers,
					first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_Energy_histo_title;
			new_Energy_histo_title << "Deposited hit energy for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range;
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_, new_Energy_histo_title.str(),
					hitLayers, first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;
		} //End of if-to-be-compared loop
		else {
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

	TCanvas* PDF_Canvas_ParticlesHits_per_File = new TCanvas();
	PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf[");
	std::stringstream FilesCanvasName_eps, FilesCanvasName_C;

	Files_Canvas->cd();

	gStyle->SetOptStat(1);
	//gStyle->SetOptStat(111111);

	Files_Canvas->Clear();
	Files_Canvas->SetLogy(0);
	WritePrintHistogram(Files_Canvas, ParticlesVSEvent, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	Files_Canvas->Clear();
	Files_Canvas->SetLogy(0);
	WritePrintHistogram(Files_Canvas, Particles, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	Files_Canvas->Clear();
	Files_Canvas->SetLogy(1);
	WritePrintHistogram(Files_Canvas, Hits, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf]");

	delete PDF_Canvas_ParticlesHits_per_File;
	delete Files_Canvas;

} //End of function DrawingMacro 

#endif /*DRAWINGHISTOS*/
