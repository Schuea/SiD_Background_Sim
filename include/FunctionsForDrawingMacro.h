#ifndef FUNCTIONSFORDRAWINGMACRO
#define FUNCTIONSFORDRAWINGMACRO

#include "TH2.h"

#include <vector>
#include <cmath>
#include <iostream>

#include "DataClass.h"
#include "UsefulFunctions.h"
#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"

void InitializeAllCaloSubdetectors(std::vector<Subdetector*> * SubDetectors) {
	SubDetectors->push_back(new EcalBarrel());
	SubDetectors->push_back(new EcalEndcap());
	SubDetectors->push_back(new HcalBarrel());
	SubDetectors->push_back(new HcalEndcap());
	SubDetectors->push_back(new MuonBarrel());
	SubDetectors->push_back(new MuonEndcap());
	SubDetectors->push_back(new BeamCal());
	SubDetectors->push_back(new LumiCal());
}
void InitializeAllTrackerSubdetectors(std::vector<Subdetector*> * SubDetectors) {
	SubDetectors->push_back(new SiVertexBarrel());
	SubDetectors->push_back(new SiVertexEndcap());
	SubDetectors->push_back(new SiTrackerBarrel());
	SubDetectors->push_back(new SiTrackerEndcap());
	SubDetectors->push_back(new SiTrackerForward());
}
void InitializeWhichSubdetector(std::string SubdetectorName, std::vector<Subdetector*> * SubDetectors) {
	std::cout << "SubdetectorName = " << SubdetectorName << std::endl;
	if (SubdetectorName == std::string("EcalBarrel"))
		SubDetectors->push_back(new EcalBarrel());
	else if (SubdetectorName == std::string("EcalEndcap"))
		SubDetectors->push_back(new EcalEndcap());
	else if (SubdetectorName == std::string("HcalBarrel"))
		SubDetectors->push_back(new HcalBarrel());
	else if (SubdetectorName == std::string("HcalEndcap"))
		SubDetectors->push_back(new HcalEndcap());
	else if (SubdetectorName == std::string("MuonBarrel"))
		SubDetectors->push_back(new MuonBarrel());
	else if (SubdetectorName == std::string("MuonEndcap"))
		SubDetectors->push_back(new MuonEndcap());
	else if (SubdetectorName == std::string("BeamCal"))
		SubDetectors->push_back(new BeamCal());
	else if (SubdetectorName == std::string("LumiCal"))
		SubDetectors->push_back(new LumiCal());
	else if (SubdetectorName == std::string("SiVertexBarrel"))
		SubDetectors->push_back(new SiVertexBarrel());
	else if (SubdetectorName == std::string("SiVertexEndcap"))
		SubDetectors->push_back(new SiVertexEndcap());
	else if (SubdetectorName == std::string("SiTrackerBarrel"))
		SubDetectors->push_back(new SiTrackerBarrel());
	else if (SubdetectorName == std::string("SiTrackerEndcap"))
		SubDetectors->push_back(new SiTrackerEndcap());
	else if (SubdetectorName == std::string("SiTrackerForward"))
		SubDetectors->push_back(new SiTrackerForward());
	else {
		std::cerr << "The given name doesn't match any subdetector!" << std::endl;
		std::terminate();
	}
}
void InitializeAllSubdetectors(std::vector<Subdetector*> * SubDetectors) {
	InitializeAllCaloSubdetectors(SubDetectors);
	InitializeAllTrackerSubdetectors(SubDetectors);
}

template<class T>
void WritePrintComparedHistogram(TCanvas* Canvas_, std::vector<T> Histos_, std::string new_histo_title,
		std::vector<int> hitLayers, int firstlayer, int lastlayer, bool Normalizing, std::string drawingoption,
		std::string PDFName) {
	Canvas_->Update();
	Histos_.at(firstlayer)->SetTitle(new_histo_title.c_str());
	Histos_.at(firstlayer)->SetLineColor(2);
	Histos_.at(firstlayer)->SetMarkerStyle(20);
	Histos_.at(firstlayer)->SetMarkerColor(2);
	if (Normalizing)
		Histos_.at(firstlayer)->GetYaxis()->SetTitle("Normalized entries");
	if (Normalizing)
		Histos_.at(firstlayer)->Scale(1.0 / Histos_.at(firstlayer)->Integral());
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
		if (Normalizing)
			Histos_.at(l)->GetYaxis()->SetTitle("Normalized entries");
		if (Normalizing)
			Histos_.at(l)->Scale(1.0 / Histos_.at(l)->Integral());
		Histos_.at(l)->Draw("sames");
		TPaveStats *st = (TPaveStats*) Histos_.at(l)->FindObject("stats");
		if (l < 3)
			st->SetTextColor(l + 2);
		else
			st->SetTextColor(l + 3);
		st->SetY2NDC(((TPaveStats*) Histos_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
		st->SetY1NDC(st->GetY2NDC() - statboxsize); //new x end position
	}
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

std::string Setup_HitsTime3D_PlotsTitle(float timelow, float timehigh, std::string subdetector_name) {
	std::string title;
	title = "Hitmap of hits between " + std::to_string(timelow) + " and " + std::to_string(timehigh) + "ns for "
			+ subdetector_name + ";z [mm];x [mm];y [mm]";
	return title;
}

void Fill_HitsTime3D_Plots(float time, float timelow, float timehigh, TH3D* histo, float x, float y, float z) {
	if (time >= timelow && time < timehigh) {
		histo->Fill(z, x, y);
	}
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
			stddev += iterator->second.at(i) - average;
		}
		stddev /= iterator->second.size();
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
		total_average /= Hits->size()-2;
		total_stddev /= Hits->size()-2;
		Hits->at(Hits->size()-1)->SetBinContent(bin_iterator, total_average);
		Hits->at(Hits->size()-1)->SetBinError(bin_iterator, total_stddev);
	}

}

TTree* Get_TTree(TFile* inputfile, std::string subdetector_name) {
	std::stringstream temp;
	temp << "Tree_" << subdetector_name;

	TTree* Tree = nullptr;
	inputfile->GetObject(temp.str().c_str(), Tree);
	if (!Tree) {
		throw std::exception();
	}
	return Tree;
}

Data* SetBranches(TTree* const tree) {
	Data* data;
	if (tree->GetName() == std::string("Tree_EcalBarrel") || tree->GetName() == std::string("Tree_EcalEndcap")
			|| tree->GetName() == std::string("Tree_HcalBarrel") || tree->GetName() == std::string("Tree_HcalEndcap")
			|| tree->GetName() == std::string("Tree_MuonBarrel") || tree->GetName() == std::string("Tree_MuonEndcap")
			|| tree->GetName() == std::string("Tree_BeamCal") || tree->GetName() == std::string("Tree_LumiCal")) {
		data = new DataSimCalorimeterHit();
	} else if (tree->GetName() == std::string("Tree_SiVertexBarrel")
			|| tree->GetName() == std::string("Tree_SiVertexEndcap")
			|| tree->GetName() == std::string("Tree_SiTrackerBarrel")
			|| tree->GetName() == std::string("Tree_SiTrackerEndcap")
			|| tree->GetName() == std::string("Tree_SiTrackerForward")) {
		data = new DataSimTrackerHit();
	} else {
		std::cerr << "The given TTree name does not match any TTree in the inputfile!" << std::endl;
		std::terminate();
	}
	data->SetBranches(tree);
	return data;
}

bool DecideIfTrackerHistograms(std::vector<std::string> argument_subdetectors) {
	bool YesNo_TrackerHistograms = false;
	for (int iterator = 0; iterator < argument_subdetectors.size(); ++iterator) {
		if (argument_subdetectors.at(iterator) == std::string("SiVertexBarrel")
				|| argument_subdetectors.at(iterator) == std::string("SiVertexEndcap")
				|| argument_subdetectors.at(iterator) == std::string("SiTrackerBarrel")
				|| argument_subdetectors.at(iterator) == std::string("SiTrackerEndcap")
				|| argument_subdetectors.at(iterator) == std::string("SiTrackerForward")
				|| argument_subdetectors.at(iterator) == std::string("allTracker")
				|| argument_subdetectors.at(iterator) == std::string("all")) {
			YesNo_TrackerHistograms = true;
		}
	}
	return YesNo_TrackerHistograms;
}

void SetupSubDetectorsVector(std::vector<Subdetector*> * SubDetectors, std::string *several_subdetector_names,
		std::vector<std::string> argument_subdetectors) {
	for (size_t s = 0; s < argument_subdetectors.size(); ++s) {

		if (argument_subdetectors.size() == 1) {
			//(*several_subdetector_names).str("");
			(*several_subdetector_names) = argument_subdetectors.at(s);
			if (argument_subdetectors.at(s) == std::string("allCalo")) {

				SubDetectors->clear();
				InitializeAllCaloSubdetectors(SubDetectors);
			}

			else if (argument_subdetectors.at(s) == std::string("allTracker")) {
				//(*several_subdetector_names).str("");
				(*several_subdetector_names) = argument_subdetectors.at(s);

				SubDetectors->clear();
				InitializeAllTrackerSubdetectors(SubDetectors);
			}

			else if (argument_subdetectors.at(s) == std::string("all")) {
				//(*several_subdetector_names).str("");
				(*several_subdetector_names) = argument_subdetectors.at(s);

				SubDetectors->clear();
				InitializeAllSubdetectors(SubDetectors);
			} else {
				InitializeWhichSubdetector(argument_subdetectors.at(s), SubDetectors);

			}
		} else {
			if (argument_subdetectors.at(s) == std::string("all")) {
				//(*several_subdetector_names).str("");
				(*several_subdetector_names) = argument_subdetectors.at(s);

				SubDetectors->clear();
				InitializeAllSubdetectors(SubDetectors);
				break;
			} else if (argument_subdetectors.at(s) == std::string("allCalo")) {
				//(*several_subdetector_names).str("");
				(*several_subdetector_names) = argument_subdetectors.at(s);

				SubDetectors->clear();
				InitializeAllCaloSubdetectors(SubDetectors);

				if (argument_subdetectors.at(s + 1) == std::string("allTracker")) {
					std::cerr
							<< "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!"
							<< std::endl;
					std::terminate();
				} else
					break;
			} else if (argument_subdetectors.at(s) == std::string("allTracker")) {
				//(*several_subdetector_names).str("");
				(*several_subdetector_names) = argument_subdetectors.at(s);

				SubDetectors->clear();
				InitializeAllTrackerSubdetectors(SubDetectors);

				if (argument_subdetectors.at(s + 1) == std::string("allCalo")) {
					std::cerr
							<< "If you want to compare all subdetectors, just use the argument 'all' after the flag -s!"
							<< std::endl;
					std::terminate();
				} else
					break;

			} else {
				(*several_subdetector_names) += argument_subdetectors.at(s);
				InitializeWhichSubdetector(argument_subdetectors.at(s), SubDetectors);
			}
		}
	}
	std::cout << "Subdetectors: " << std::endl;
	for (int i = 0; i < SubDetectors->size(); ++i) {
		SubDetectors->at(i)->Initialize();
		std::cout << SubDetectors->at(i)->GetName() << std::endl;
	}
}
CellID * InitializeCellIDClass(std::string SubdetectorName, Data* data) {
	if (SubdetectorName == std::string("SiVertexBarrel") || SubdetectorName == std::string("SiVertexEndcap")
			|| SubdetectorName == std::string("SiTrackerForward")) {
		if (data->Get_id() == 0) {
			std::cerr << "The cell id is not valid! Initializing the CellID class is not possible!" << std::endl;
			std::terminate();
		} else
			return new CellID58bits(data->Get_id());
	} else if (SubdetectorName == std::string("SiTrackerBarrel") || SubdetectorName == std::string("SiTrackerEndcap")) {
		if (data->Get_id() == 0) {
			std::cerr << "The cell id is not valid! Initializing the CellID class is not possible!" << std::endl;
			std::terminate();
		} else
			return new CellID54bits(data->Get_id());
	} else if (SubdetectorName == std::string("EcalBarrel") || SubdetectorName == std::string("EcalEndcap")
			|| SubdetectorName == std::string("HcalBarrel") || SubdetectorName == std::string("HcalEndcap")
			|| SubdetectorName == std::string("MuonBarrel") || SubdetectorName == std::string("MuonEndcap")
			|| SubdetectorName == std::string("LumiCal") || SubdetectorName == std::string("BeamCal")) {

		std::cout << "id0 = " << data->Get_id0() << std::endl;
		std::cout << "id1 = " << data->Get_id1() << std::endl;
		if (data->Get_id0() == 0 || data->Get_id1() == 0) {
			std::cerr << "The cell ids are not valid! Initializing the CellID class is not possible!" << std::endl;
			std::terminate();
		} else {
			return new CellID64bits(data->Get_id0(), data->Get_id1());
		}
	} else {
		std::cerr << "This subdetector name is unkown. The CellID class could not be inititalized!" << std::endl;
		std::terminate();
	}
}

void Setup_BinningArrays(std::vector<Subdetector*> * SubDetectors, std::vector<float> *axis_range_plot_1D,
		std::vector<float> *axis_range_plot_2D, std::vector<float> *axis_range_plot_3D,
		std::vector<float> *axis_range_plot_energy_1D, float time_interval, std::vector<float> *axis_range_plot_time,
		std::vector<float> *axis_range_plot_rtime_2D, std::vector<float> *axis_range_plot_ztime_2D,
		std::vector<float> *axis_range_plot_time_3D) {

	float maxEnergy1D = 0;
	float minEnergy1D = 0;
	float binsEnergy1D = 0;

	float max1D = 0;
	float min1D = 0;
	float bins1D = 0;

	float xmax2D = 0;
	float xmin2D = 0;
	float xbins2D = 0;
	float ymax2D = 0;
	float ymin2D = 0;
	float ybins2D = 0;

	float xmax3D = 0;
	float xmin3D = 0;
	float xbins3D = 0;
	float ymax3D = 0;
	float ymin3D = 0;
	float ybins3D = 0;
	float zmax3D = 0;
	float zmin3D = 0;
	float zbins3D = 0;

	float maxtime = 0;
	float mintime = 0;
	float binstime = 0;

	for (int s = 0; s < SubDetectors->size(); ++s) {

		minEnergy1D = FindMin(SubDetectors->at(s)->GetROOTEnergyHisto_binning().at(1), minEnergy1D);
		maxEnergy1D = FindMax(SubDetectors->at(s)->GetROOTEnergyHisto_binning().at(2), maxEnergy1D);
		binsEnergy1D = FindMax(SubDetectors->at(s)->GetROOTEnergyHisto_binning().at(0), binsEnergy1D);
		min1D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning1D().at(1), min1D);
		max1D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning1D().at(2), max1D);
		bins1D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning1D().at(0), bins1D);
		xmin2D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning2D().at(1), xmin2D);
		xmax2D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning2D().at(2), xmax2D);
		xbins2D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning2D().at(0), xbins2D);
		ymin2D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning2D().at(4), ymin2D);
		ymax2D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning2D().at(5), ymax2D);
		ybins2D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning2D().at(3), ybins2D);
		zmin3D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning3D().at(1), zmin3D);
		zmax3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(2), zmax3D);
		zbins3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(0), zbins3D);
		xmin3D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning3D().at(4), xmin3D);
		xmax3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(5), xmax3D);
		xbins3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(3), xbins3D);
		ymin3D = FindMin(SubDetectors->at(s)->GetROOTHisto_binning3D().at(7), ymin3D);
		ymax3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(8), ymax3D);
		ybins3D = FindMax(SubDetectors->at(s)->GetROOTHisto_binning3D().at(6), ybins3D);

		mintime = FindMin(SubDetectors->at(s)->GetROOTHisto_time().at(1), mintime);
		maxtime = time_interval + FindMax(SubDetectors->at(s)->GetROOTHisto_time().at(2), maxtime);
		binstime = FindMax(SubDetectors->at(s)->GetROOTHisto_time().at(0), binstime);
	}
	std::vector<float> tempEnergy1D = { binsEnergy1D, minEnergy1D, maxEnergy1D };
	*axis_range_plot_energy_1D = tempEnergy1D;
	std::vector<float> temp1D = { bins1D, min1D, max1D };
	*axis_range_plot_1D = temp1D;
	std::vector<float> temp2D = { xbins2D, xmin2D, xmax2D, ybins2D, ymin2D, ymax2D };
	*axis_range_plot_2D = temp2D;
	std::vector<float> temp3D = { zbins3D, zmin3D, zmax3D, xbins3D, xmin3D, xmax3D, ybins3D, ymin3D, ymax3D };
	*axis_range_plot_3D = temp3D;

	float rmax = sqrt(pow(xmax3D, 2) + pow(ymax3D, 2));
	float rbins = rmax / 10;
	std::vector<float> temprtime2D = { binstime, mintime, maxtime, rbins, 0, rmax };
	*axis_range_plot_rtime_2D = temprtime2D;
	std::vector<float> tempztime2D = { binstime, mintime, maxtime, zbins3D, zmin3D, zmax3D };
	*axis_range_plot_ztime_2D = tempztime2D;
	std::vector<float> temptime3D = { binstime, mintime, maxtime, zbins3D, zmin3D, zmax3D, rbins, 0, rmax };
	*axis_range_plot_time_3D = temptime3D;
	std::vector<float> temptime = { binstime, mintime, maxtime };
	*axis_range_plot_time = temptime;

}
void SetupHistoTitles(std::string subdetector_name, std::string layer, std::string & histo_name1D,
		std::string & histo_title1D, std::string & histo_name2D, std::string & histo_title2D,
		std::string & histo_name3D, std::string & histo_title3D, std::string & energyhisto_name1D,
		std::string & energyhisto_title1D, std::string & energyhisto_name2D, std::string & energyhisto_title2D,
		std::string & energyhisto_name3D, std::string & energyhisto_title3D, std::string & hitsperlayerhisto_name,
		std::string & hitsperlayerhisto_title, std::string & particleoriginshisto_name,
		std::string & particleoriginshisto_title, std::string & histo_name_time, std::string & histo_title_time,
		std::string & histo_name_rtime2D, std::string & histo_title_rtime2D, std::string & histo_name_ztime2D,
		std::string & histo_title_ztime2D) {

	std::stringstream layercount;
	if (layer == std::string("all"))
		layercount << ", " << layer << " layers";
	else
		layercount << ", " << layer << ". layer";

	histo_name1D = "Hits_" + subdetector_name + "_Layer_" + layer;
	histo_title1D = "Hit occupancy per cell for " + subdetector_name + layercount.str()
			+ ";Number of hits per cell;Count";
	histo_name2D = "Hits_2D_" + subdetector_name + "_Layer_" + layer;
	histo_title2D = "HitMap for " + subdetector_name + layercount.str() + ";x [mm];y [mm]";
	histo_name3D = "Hits_3D_" + subdetector_name + "_Layer_" + layer;
	histo_title3D = "HitMap for " + subdetector_name + layercount.str() + ";z [mm];x[mm];y[mm]";
	energyhisto_name1D = "Energy_" + subdetector_name + "_Layer_" + layer;
	energyhisto_title1D = "Deposited hit energy for " + subdetector_name + layercount.str()
			+ ";Deposited hit energy [GeV];Count";
	energyhisto_name2D = "Hits_Energy_2D_" + subdetector_name + "_Layer_" + layer;
	energyhisto_title2D = "HitMap with the average hit energy per bin for " + subdetector_name + layercount.str()
			+ ";x [mm];y [mm]";
	energyhisto_name3D = "Hits_Energy_3D_" + subdetector_name + "_Layer_" + layer;
	energyhisto_title3D = "HitMap with the hit energy for " + subdetector_name + layercount.str()
			+ ";z [mm];x[mm];y[mm]";
	hitsperlayerhisto_name = "HitsPerLayer_" + subdetector_name + "_Layer_" + layer;
	hitsperlayerhisto_title = "Hits for " + subdetector_name + layercount.str()
			+ ";Number of hits per bunch crossing;Count";
	particleoriginshisto_name = "ParticleOrigins_" + subdetector_name + "_Layer_" + layer;
	particleoriginshisto_title = "Origins of pair background particles for " + subdetector_name + layercount.str()
			+ ";r [mm];z [mm]";

	histo_name_time = "HitsTime_" + subdetector_name + "_Layer_" + layer;
	histo_title_time = "Hit time for " + subdetector_name + layercount.str() + ";Hit time [ns];Count";
	histo_name_rtime2D = "HitsTime_rtime_2D_" + subdetector_name + "_Layer_" + layer;
	histo_title_rtime2D = "Radial position of hits over hit time for " + subdetector_name + layercount.str()
			+ ";Hit time [ns];r [mm]";
	histo_name_ztime2D = "HitsTime_ztime_2D_" + subdetector_name + "_Layer_" + layer;
	histo_title_ztime2D = "z position of hits over hit time for " + subdetector_name + layercount.str()
			+ ";Hit time [ns];z [mm]";
}

void Setup_ParticleOriginsHisto(std::vector<TH2D*> & HistoVector, std::vector<float> axis_range_plot,
		std::string histo_name, std::string histo_title, std::string coordinate_system) {
	if (axis_range_plot.size() < 9) {
		std::cerr
				<< "The plot of the particle origins can only be plotted with the std::vector of the 3 dimensional plot axes."
				<< std::endl;
		std::terminate();
	}
	std::vector<float> axis_vector;
	if (coordinate_system == std::string("kartesian"))
		axis_vector = axis_range_plot;
	else if (coordinate_system == std::string("cylindrical")) {
		float rmax = sqrt(pow(axis_range_plot.at(5), 2) + pow(axis_range_plot.at(8), 2));
		float rmin = 0.;
		float rrange = rmax - rmin;
		float phimax = acos(axis_range_plot.at(5) / rmax);
		float zmax = axis_range_plot.at(2);
		float zmin = -zmax;
		float zrange = rmax - zmin;
		axis_vector = {0,0,0,float(zrange/4.0),zmin,zmax,float(rrange/4.0),rmin,rmax};
	} else if (coordinate_system == std::string("spherical")) {
		float rmax = sqrt(
				pow(axis_range_plot.at(5), 2) + pow(axis_range_plot.at(8), 2) + pow(axis_range_plot.at(2), 2));
		float rmin = 0.;
		float rrange = rmax - rmin;
		float zmax = axis_range_plot.at(2);
		float zmin = -zmax;
		float zrange = rmax - zmin;
		float thetamax = acos(axis_range_plot.at(2) / rmax);
		float thetamin = 0;
		float thetarange = thetamax - thetamin;
		float phimax = acos(axis_range_plot.at(5) / (rmax * sin(thetamax)));
		axis_vector = {0,0,0,float(zrange/4.0),zmin,zmax,float(thetarange/4.0),thetamin,thetamax};
	}
	HistoVector.emplace_back(
			new TH2D(histo_name.c_str(), histo_title.c_str(), axis_vector.at(3), axis_vector.at(4), axis_vector.at(5),
					axis_vector.at(6), axis_vector.at(7), axis_vector.at(8)));
	HistoVector.at(HistoVector.size() - 1)->SetContour(100);
}
void Setup_Histo(std::vector<TH1D*> & HistoVector, std::vector<float> axis_range_plot, std::string histo_name,
		std::string histo_title) {
	HistoVector.emplace_back(
			new TH1D(histo_name.c_str(), histo_title.c_str(), axis_range_plot.at(0), axis_range_plot.at(1),
					axis_range_plot.at(2)));
	HistoVector.at(HistoVector.size() - 1)->SetMinimum(0.1);
	HistoVector.at(HistoVector.size() - 1)->SetLineColor(kViolet);
	HistoVector.at(HistoVector.size() - 1)->GetXaxis()->CenterTitle();
	HistoVector.at(HistoVector.size() - 1)->GetYaxis()->CenterTitle();
}

void Setup_Histo(std::vector<TH2D*> & HistoVector, std::vector<float> axis_range_plot, std::string histo_name,
		std::string histo_title) {
	HistoVector.emplace_back(
			new TH2D(histo_name.c_str(), histo_title.c_str(), axis_range_plot.at(0), axis_range_plot.at(1),
					axis_range_plot.at(2), axis_range_plot.at(3), axis_range_plot.at(4), axis_range_plot.at(5)));
	HistoVector.at(HistoVector.size() - 1)->SetContour(100);
	HistoVector.at(HistoVector.size() - 1)->GetXaxis()->CenterTitle();
	HistoVector.at(HistoVector.size() - 1)->GetYaxis()->CenterTitle();
}

void Setup_Histo(std::vector<TH3D*> & HistoVector, std::vector<float> axis_range_plot, std::string histo_name,
		std::string histo_title) {
	HistoVector.emplace_back(
			new TH3D(histo_name.c_str(), histo_title.c_str(), axis_range_plot.at(0), axis_range_plot.at(1),
					axis_range_plot.at(2), axis_range_plot.at(3), axis_range_plot.at(4), axis_range_plot.at(5),
					axis_range_plot.at(6), axis_range_plot.at(7), axis_range_plot.at(8)));
	HistoVector.at(HistoVector.size() - 1)->GetZaxis()->CenterTitle();
	HistoVector.at(HistoVector.size() - 1)->GetXaxis()->CenterTitle();
	HistoVector.at(HistoVector.size() - 1)->GetYaxis()->CenterTitle();
}

#endif
