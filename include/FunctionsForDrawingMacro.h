#ifndef FUNCTIONSFORDRAWINGMACRO
#define FUNCTIONSFORDRAWINGMACRO

#include "TGraph.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveStats.h"

#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>

#include "DataClass.h"
#include "UsefulFunctions.h"
#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"

bool DecideIfTrackerHistograms(std::vector< std::string > argument_subdetectors);

Data* SetBranches(TTree* const tree);
CellID * InitializeCellIDClass(std::string SubdetectorName, Data* data);

void SetupSubDetectorsVector(std::vector< Subdetector* > * SubDetectors, std::string *several_subdetector_names, std::vector< std::string > argument_subdetectors);

void InitializeAllCaloSubdetectors(std::vector< Subdetector* > * SubDetectors);
void InitializeAllTrackerSubdetectors(std::vector< Subdetector* > * SubDetectors);
void InitializeWhichSubdetector(std::string SubdetectorName, std::vector< Subdetector* > * SubDetectors);
void InitializeAllSubdetectors(std::vector< Subdetector* > * SubDetectors);

template< class T > void WritePrintComparedHistogram(TCanvas* Canvas_, std::vector< T > Histos_, std::string new_histo_title,		std::vector< int > hitLayers, int firstlayer, int lastlayer, bool Normalizing, std::string drawingoption,		std::string PDFName);
template< class T > void WritePrintComparedHistogram(TCanvas* Canvas_, T Histo, int firstlayer, int lastlayer, std::string drawingoption,		std::string PDFName);
template< class T > void WritePrintHistogram(TCanvas* Canvas_, T Histos_, bool Normalizing, std::string drawingoption, std::string PDFName);
std::string Setup_HitsTime3D_PlotsTitle(float timelow, float timehigh, std::string subdetector_name);
void Fill_HitsTime3D_Plots(float time, float timelow, float timehigh, TH3D* histo, float x, float y, float z);

template< class T > void Fill_Histogram_from_Map(std::map< std::pair< int, int >, std::vector< float > > HitMap, std::vector< T > *Hits,	int weight);
TTree* Get_TTree(TFile* inputfile, std::string subdetector_name);

void Setup_BinningArrays(std::vector< Subdetector* > * SubDetectors, std::vector<float> *axis_range_occupancy_plot_,
		std::vector<float> *axis_range_occupancy_r_plot_, std::vector<float> *axis_range_occupancy_phi_plot_,
		std::vector< float > *axis_range_plot_1D, std::vector< float > *axis_range_plot_2D, std::vector< float > *axis_range_plot_3D,
		std::vector< float > *axis_range_plot_energy_1D, float time_interval, std::vector< float > *axis_range_plot_time,
		std::vector< float > *axis_range_plot_rtime_2D, std::vector< float > *axis_range_plot_ztime_2D,
		std::vector< float > *axis_range_plot_time_3D);
void SetupHistoTitles(std::string subdetector_name, std::string layer,
		std::string & histo_name1D,	std::string & histo_title1D,
		std::string & histo_name2D, std::string & histo_title2D,
		std::string & histo_name3D, std::string & histo_title3D,
		std::string & energyhisto_name1D, std::string & energyhisto_title1D,
		std::string & energyhisto_name2D, std::string & energyhisto_title2D,
		std::string & energyhisto_name3D, std::string & energyhisto_title3D,
		std::string & hitsperlayerhisto_name, std::string & hitsperlayerhisto_title,
		std::string & particleoriginshisto_name, std::string & particleoriginshisto_title,
		std::string & histo_name_time, std::string & histo_title_time,
		std::string & histo_name_rtime2D, std::string & histo_title_rtime2D,
		std::string & histo_name_ztime2D, std::string & histo_title_ztime2D);

void Setup_ParticleOriginsHisto(std::vector< TH2D* > & HistoVector, std::vector< float > axis_range_plot,	std::string histo_name, std::string histo_title, std::string coordinate_system);

void Setup_Histo(std::vector< TH1D* > & HistoVector, std::vector< float > axis_range_plot, std::string histo_name, std::string histo_title);
void Setup_Histo(std::vector< TH2D* > & HistoVector, std::vector< float > axis_range_plot, std::string histo_name, std::string histo_title);
void Setup_Histo(std::vector< TH3D* > & HistoVector, std::vector< float > axis_range_plot, std::string histo_name, std::string histo_title);

template<class T>
void WritePrintComparedHistogram(TCanvas* Canvas_, std::vector<T> Histos_, std::string new_histo_title,
		std::vector<int> hitLayers, int firstlayer, int lastlayer, bool Normalizing, std::string drawingoption,
		std::string PDFName) {
	Canvas_->Update();
	Histos_.at(firstlayer)->SetTitle(new_histo_title.c_str());
	Histos_.at(firstlayer)->SetLineColor(2);
	Histos_.at(firstlayer)->SetMarkerStyle(20);
	Histos_.at(firstlayer)->SetMarkerColor(2);
	if (Normalizing){
		Histos_.at(firstlayer)->GetYaxis()->SetTitle("Normalized entries");
		Histos_.at(firstlayer)->Scale(1.0 / Histos_.at(firstlayer)->Integral());
  }
	Histos_.at(firstlayer)->Draw(drawingoption.c_str());
  TPaveStats *st1 = (TPaveStats*) Histos_.at(firstlayer)->FindObject("stats");
	double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
	st1->SetTextColor(2);
	st1->SetY1NDC(0.78);
	st1->SetY2NDC(0.78 + statboxsize);
	for (signed int l = firstlayer + 1; l <= lastlayer; ++l) {

		if (std::find(hitLayers.begin(), hitLayers.end(), l) == hitLayers.end()) {
			// If one layer has no hits, it isn't mentioned in the std::vector hitlayers.
			//This layer is then skipped.
			continue;
		}

		Histos_.at(l)->SetMarkerStyle(l + 20);
		if (l < 3) {
			Histos_.at(l)->SetLineColor(l + 2);
			Histos_.at(l)->SetMarkerColor(l + 2);
		} else {
			Histos_.at(l)->SetLineColor(l + 3);
			Histos_.at(l)->SetMarkerColor(l + 3);
		}
		if (Normalizing){
			Histos_.at(l)->GetYaxis()->SetTitle("Normalized entries");
			Histos_.at(l)->Scale(1.0 / Histos_.at(l)->Integral());
    }
		Histos_.at(l)->Draw("sames");
		TPaveStats *st = (TPaveStats*) Histos_.at(l)->FindObject("stats");
		if (l < 3)
			st->SetTextColor(l + 2);
		else
			st->SetTextColor(l + 3);
		st->SetY2NDC(((TPaveStats*) Histos_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
		st->SetY1NDC(st->GetY2NDC() - statboxsize); //new x end position
	}
	Canvas_->Update();
	Canvas_->Write();
	std::stringstream CanvasName_eps, CanvasName_C;
	CanvasName_eps << Canvas_->GetName() << "_" << Histos_.at(firstlayer)->GetName() << "_" << firstlayer << "-"
			<< lastlayer << ".eps";
	CanvasName_C << Canvas_->GetName() << "_" << Histos_.at(firstlayer)->GetName() << "_" << firstlayer << "-"
			<< lastlayer << ".C";

	Canvas_->Print(CanvasName_eps.str().c_str());
	Canvas_->Print(CanvasName_C.str().c_str());
	Canvas_->Print(PDFName.c_str());
}

template<class T>
void WritePrintComparedHistogram(TCanvas* Canvas_, T Histo, int firstlayer, int lastlayer, std::string drawingoption,
		std::string PDFName) {
	Canvas_->Update();
	Histo->Draw(drawingoption.c_str());
	Canvas_->Write();
	std::stringstream CanvasName_eps, CanvasName_C;
	CanvasName_eps << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".eps";
	CanvasName_C << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".C";

	Canvas_->Print(CanvasName_eps.str().c_str());
	Canvas_->Print(CanvasName_C.str().c_str());
	Canvas_->Print(PDFName.c_str());
}

template<class T>
void WritePrintHistogram(TCanvas* Canvas_, T Histos_, std::string drawingoption, std::string PDFName) {
	Canvas_->Update();
	Canvas_->SetRightMargin(0.15);
	Histos_->Draw(drawingoption.c_str());
	Canvas_->Write();
	std::stringstream CanvasName_eps, CanvasName_C;
	CanvasName_eps << Canvas_->GetName() << "_" << Histos_->GetName() << ".eps";
	CanvasName_C << Canvas_->GetName() << "_" << Histos_->GetName() << ".C";

	Canvas_->Print(CanvasName_eps.str().c_str());
	Canvas_->Print(CanvasName_C.str().c_str());
	Canvas_->Print(PDFName.c_str());
}

template<class T>
void WritePrintHistogram(TCanvas* Canvas_, T Histos_, bool Normalizing, std::string drawingoption, std::string PDFName) {
	Canvas_->Update();
	Canvas_->SetRightMargin(0.15);
	if (Normalizing){
			Histos_->GetYaxis()->SetTitle("Normalized entries");
			Histos_->Scale(1.0 / Histos_->Integral());
	}
	Histos_->Draw(drawingoption.c_str());
	Canvas_->Write();
	std::stringstream CanvasName_eps, CanvasName_C;
	CanvasName_eps << Canvas_->GetName() << "_" << Histos_->GetName() << ".eps";
	CanvasName_C << Canvas_->GetName() << "_" << Histos_->GetName() << ".C";

	Canvas_->Print(CanvasName_eps.str().c_str());
	Canvas_->Print(CanvasName_C.str().c_str());
	Canvas_->Print(PDFName.c_str());
}

template<class T>
void Fill_Histogram_from_Map(std::map<std::pair<int, int>, std::vector<float> > HitMap, std::vector<T> *Hits,
		int weight) {

	for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
		int temp_layer = iterator->first.first;

		float average = 0;
		for (size_t i = 0; i < iterator->second.size(); ++i) {
			average += iterator->second.at(i);
		}
		average /= iterator->second.size();

		float stddev = 0;
		for (size_t i = 0; i < iterator->second.size(); ++i) {
			stddev += pow(iterator->second.at(i) - average,2);
		}
		stddev /= float(iterator->second.size());
		stddev = sqrt(stddev);
		Hits->at(temp_layer)->SetBinContent(iterator->first.second, average * weight);
		Hits->at(temp_layer)->SetBinError(iterator->first.second, stddev * weight);
	}

	int NbinsX = Hits->at(Hits->size()-1)->GetNbinsX();
	int NbinsY = Hits->at(Hits->size()-1)->GetNbinsY();
	int totalbins = (NbinsX + 2) * (NbinsY + 2); //+2 overflowbins per axis
	std::vector< int > overflowbins;
	for (int i = 0; i < totalbins; ++i) {
		if ((i <= NbinsX + 1) || (i % (NbinsX + 2) == 0) || (i % (NbinsX + 2) == (NbinsX + 1))
				|| (i >= (NbinsX + 2) * (NbinsY + 1))) {
			overflowbins.push_back(i);
		}
	}
	for (int bin_iterator = 0; bin_iterator< totalbins; ++bin_iterator){
		if (std::find(overflowbins.begin(),overflowbins.end(), bin_iterator) != overflowbins.end()) continue;
		float total_average = 0;
		float total_stddev = 0;
		for (int vector_iterator = 0; vector_iterator < Hits->size()-2; ++vector_iterator) {//Only sum up the histograms for the single layers
			 total_average += Hits->at(vector_iterator)->GetBinContent(bin_iterator);
			 total_stddev += Hits->at(vector_iterator)->GetBinError(bin_iterator);
		}
		total_average /= float(Hits->size()-2);
		total_stddev /= float(Hits->size()-2);
		Hits->at(Hits->size()-1)->SetBinContent(bin_iterator, total_average);
		Hits->at(Hits->size()-1)->SetBinError(bin_iterator, total_stddev);
	}

}

#endif
