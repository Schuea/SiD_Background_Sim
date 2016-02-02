#include "DrawingHistograms.h"
#include "FunctionsForDrawingMacro.h"

DrawingHistograms::DrawingHistograms(std::string _outputname, std::vector<std::string> _inputnames,
		std::vector<std::string> _argument_subdetectors, int NUMBER_OF_FILES, int FIRST_LAYER_TO_BE_COMPARED,
		int LAST_LAYER_TO_BE_COMPARED) :
		outputname(_outputname), inputnames(_inputnames), argument_subdetectors(_argument_subdetectors), number_of_files(
				NUMBER_OF_FILES), first_layer_to_be_compared(FIRST_LAYER_TO_BE_COMPARED), last_layer_to_be_compared(
				LAST_LAYER_TO_BE_COMPARED), inputfile(nullptr), output_rootfile(nullptr), Canvas_name(""), Files_Canvas_(
				nullptr), Hits_Canvas_(nullptr), DeadCells(nullptr), Particles(nullptr), ParticlesVSEvent(nullptr), gausfit_Particles(
				nullptr), Occupancy_r_Histo_(nullptr), Occupancy_phi_Histo_(nullptr), ParticleOrigins_2D_(), Hits_PerLayer_(), Hits_Histo_(), Hits_2D_(), Hits_3D_(), Hits_Energy_Histo_(), Hits_Energy_2D_(), Hits_Energy_3D_(), Hits_Time_(), Hits_Time_rtime_2D_(), Hits_Time_ztime_2D_(), Hits_Time_3D_(), time_interval_bunchspacing(
				0), Number_train_bunch(), BunchNumbers_for_TotDeadCells(), TotDeadCells_x(nullptr), TotDeadCells_y(
				nullptr), YesNo_TrackerHistograms(false), axis_range_occupancy_plot(), axis_range_occupancy_r_plot(), axis_range_occupancy_phi_plot(), axis_range_plot_1D(), axis_range_plot_2D(), axis_range_plot_3D(), axis_range_plot_energy_1D(), axis_range_plot_time(), axis_range_plot_rtime_2D(), axis_range_plot_ztime_2D(), axis_range_plot_time_3D(), SubDetectors(
				nullptr), subdetector_name(""), hitLayers(), MaxNumberLayers(0), HitMap(), HitsPerLayerMap() {
}

DrawingHistograms::~DrawingHistograms() {
}

void DrawingHistograms::Initialize() {
	TH1::SetDefaultSumw2();
	gROOT->ForceStyle();
	gStyle->SetPalette(1);

	output_rootfile = new TFile(outputname.c_str(), "RECREATE");
	std::size_t found = outputname.find_last_of(".");
	Canvas_name = outputname.substr(0, found);
	Files_Canvas_ = new TCanvas((Canvas_name + "_HitsPerFile").c_str());
	Hits_Canvas_ = new TCanvas((Canvas_name + "_Hits_" + subdetector_name).c_str());

	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	YesNo_TrackerHistograms = DecideIfTrackerHistograms(argument_subdetectors);
	time_interval_bunchspacing = number_of_files * 554.0; //ns (one bunch spacing is 554 ns)
}

void DrawingHistograms::SetupGeneralHistograms() {
	ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event", 50, 0, number_of_files, 50, 190000, 210000);
	Particles = new TH1D("Particles", "Particles", 100, 180000, 220000);
	gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);

	DeadCells = new TH1D("DeadCells", "Dead cells (> 4 hits per cell) per bunch crossing",
			int((number_of_files + 50) / 10), 0, number_of_files + 50);
	DeadCells->GetYaxis()->SetTitle("Count");
	DeadCells->GetXaxis()->SetTitle("Number of bunch crossings");
	DeadCells->GetXaxis()->CenterTitle();

	for (int it = 1; it <= int(ceil(number_of_files / 100)); ++it) {
		//Fill the vector in steps of 100 up to the max number of bunches
		if (100 * it < number_of_files - 50)
			BunchNumbers_for_TotDeadCells.push_back(100 * it); //Number of bunches for which the total number of dead cells will be plotted
		//Close to the end (number_of_files-50) check and then fill only one last time:
		else if (100 * it > number_of_files - 50 || it == int(ceil(number_of_files / 100))) {
			BunchNumbers_for_TotDeadCells.push_back(number_of_files);
		}
	}
	TotDeadCells_x = new int[BunchNumbers_for_TotDeadCells.size()];
	TotDeadCells_y = new int[BunchNumbers_for_TotDeadCells.size()];

	Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0] * 3, axis_range_plot_1D[1], axis_range_plot_1D[2] * 40);
	///*MuonBarrel*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/5, 100, axis_range_plot_1D[2]);
	///*MuonEndcap*/TH1D* Hits = new TH1D("Hits", "Hits", 35,5500, 7300);
	///*BeamCal:*/ TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*1/4, 4200000, 4850000);
	///*ALL:*/TH1D* Hits = new TH1D("Hits", "Hits", axis_range_plot_1D[0]*2/3, 4000000, 5000000);
	Hits->GetYaxis()->SetTitle("Count");
	Hits->GetXaxis()->SetTitle(("Hits per bunch crossing in " + subdetector_name).c_str());
	Hits->GetXaxis()->CenterTitle();
	//Scaling up the histogram ranges from the subdetector specific single layer hits plot, so that data fit on plot
	//TF1* gausfit_Hits = new TF1("gausfit", "gaus", 0, 150);
	std::string occupancy_r_histo_name = "Occupancy_r_" + subdetector_name + "_all_layers";
	std::string occupancy_r_histo_title = "Hit occupancy per cell over r for all layers of " + subdetector_name
			+ ";r [mm];Average occupancy";
	std::string occupancy_phi_histo_name = "Occupancy_phi_" + subdetector_name + "_all_layers";
	std::string occupancy_phi_histo_title = "Hit occupancy per cell over phi for all layers of " + subdetector_name
			+ ";phi [Radians];Average occupancy";
	/*
	 Occupancy_r_Histo_ = new TH2D(occupancy_r_histo_name.c_str(), occupancy_r_histo_title.c_str(),
	 axis_range_occupancy_r_plot.at(0), axis_range_occupancy_r_plot.at(1), axis_range_occupancy_r_plot.at(2),
	 axis_range_occupancy_r_plot.at(3), axis_range_occupancy_r_plot.at(4), axis_range_occupancy_r_plot.at(5));
	 */
	Occupancy_r_Histo_ = new TH1D(occupancy_r_histo_name.c_str(), occupancy_r_histo_title.c_str(),
			axis_range_occupancy_r_plot.at(0), axis_range_occupancy_r_plot.at(1), axis_range_occupancy_r_plot.at(2));
	Occupancy_r_Histo_->GetXaxis()->CenterTitle();
	Occupancy_r_Histo_->GetYaxis()->CenterTitle();
	/*
	 Occupancy_phi_Histo_ = new TH2D(occupancy_phi_histo_name.c_str(), occupancy_phi_histo_title.c_str(),
	 axis_range_occupancy_phi_plot.at(0), axis_range_occupancy_phi_plot.at(1), axis_range_occupancy_phi_plot.at(2),
	 axis_range_occupancy_phi_plot.at(3), axis_range_occupancy_phi_plot.at(4),
	 axis_range_occupancy_phi_plot.at(5));
	 */
	Occupancy_phi_Histo_ = new TH1D(occupancy_phi_histo_name.c_str(), occupancy_phi_histo_title.c_str(),
			axis_range_occupancy_phi_plot.at(0), axis_range_occupancy_phi_plot.at(1),
			axis_range_occupancy_phi_plot.at(2));
	Occupancy_phi_Histo_->GetXaxis()->CenterTitle();
	Occupancy_phi_Histo_->GetYaxis()->CenterTitle();
	std::string histo_name_time3D, histo_title_time3D;
	histo_name_time3D = "HitsTime_3D_" + subdetector_name;
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(0, 2, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(2, 4, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(4, 6, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(6, 8, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(8, 12, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(12, 16, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(16, 20, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(20, 50, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(50, 100, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(100, 200, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(200, 300, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(300, 400, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(400, 500, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(500, 600, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(600, 700, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(700, 800, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(800, 900, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(900, 1000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(1000, 1250, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(1250, 1500, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(1500, 2000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(2000, 2500, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(2500, 3000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(3000, 4000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(4000, 6000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
	histo_title_time3D = Setup_HitsTime3D_PlotsTitle(6000, 10000, subdetector_name);
	Setup_Histo(Hits_Time_3D_, axis_range_plot_3D, histo_name_time3D, histo_title_time3D);
}

void DrawingHistograms::Setup_SubDetector_vector() {
	std::string * subdetector_name2 = new std::string("");
	SubDetectors = new std::vector<Subdetector*>();
	SetupSubDetectorsVector(SubDetectors, subdetector_name2, argument_subdetectors);
	subdetector_name = (*subdetector_name2);
}

void DrawingHistograms::Setup_HitsPerLayerMap(int layer) {
	for (int files = 1; files <= number_of_files; ++files) {
		HitsPerLayerMap[layer].push_back(0);
	}
}

int DrawingHistograms::Find_MaxNumberLayers() {
	//Find the largest number of layers from all the subdetectors that are to be plotted
	int temp_MaxNumberLayers = 0;
	for (int s = 0; s < SubDetectors->size(); ++s) {
		temp_MaxNumberLayers = FindMax(SubDetectors->at(s)->GetNumberOfLayers(), temp_MaxNumberLayers);
	}
	return temp_MaxNumberLayers;
}

void DrawingHistograms::SetupLayerHistograms(std::string layerstring) {
	std::string histo_name1D, histo_title1D;
	std::string histo_name2D, histo_title2D;
	std::string histo_name3D, histo_title3D;
	std::string energyhisto_name1D, energyhisto_title1D;
	std::string energyhisto_name2D, energyhisto_title2D;
	std::string energyhisto_name3D, energyhisto_title3D;
	std::string hitsperlayerhisto_name, hitsperlayerhisto_title;
	std::string particleoriginshisto_name, particleoriginshisto_title;

	std::string histo_name_time, histo_title_time;
	std::string histo_name_rtime2D, histo_title_rtime2D;
	std::string histo_name_ztime2D, histo_title_ztime2D;

	SetupHistoTitles(subdetector_name, layerstring, histo_name1D, histo_title1D, histo_name2D, histo_title2D,
			histo_name3D, histo_title3D, energyhisto_name1D, energyhisto_title1D, energyhisto_name2D,
			energyhisto_title2D, energyhisto_name3D, energyhisto_title3D, hitsperlayerhisto_name,
			hitsperlayerhisto_title, particleoriginshisto_name, particleoriginshisto_title, histo_name_time,
			histo_title_time, histo_name_rtime2D, histo_title_rtime2D, histo_name_ztime2D, histo_title_ztime2D);

	Setup_ParticleOriginsHisto(ParticleOrigins_2D_, axis_range_plot_3D, particleoriginshisto_name,
			particleoriginshisto_title, "cylindrical");
	Setup_Histo(Hits_PerLayer_, axis_range_plot_1D, hitsperlayerhisto_name, hitsperlayerhisto_title);
	Setup_Histo(Hits_Histo_, axis_range_occupancy_plot, histo_name1D, histo_title1D);
	Setup_Histo(Hits_2D_, axis_range_plot_2D, histo_name2D, histo_title2D);
	Setup_Histo(Hits_3D_, axis_range_plot_3D, histo_name3D, histo_title3D);
	Setup_Histo(Hits_Energy_Histo_, axis_range_plot_energy_1D, energyhisto_name1D, energyhisto_title1D);
	Setup_Histo(Hits_Energy_2D_, axis_range_plot_2D, energyhisto_name2D, energyhisto_title2D);
	Setup_Histo(Hits_Energy_3D_, axis_range_plot_3D, energyhisto_name3D, energyhisto_title3D);

	Setup_Histo(Hits_Time_, axis_range_plot_time, histo_name_time, histo_title_time);
	Setup_Histo(Hits_Time_rtime_2D_, axis_range_plot_rtime_2D, histo_name_rtime2D, histo_title_rtime2D);
	Setup_Histo(Hits_Time_ztime_2D_, axis_range_plot_ztime_2D, histo_name_ztime2D, histo_title_ztime2D);
}

void DrawingHistograms::Open_inputfile(int number_of_file) {
	inputfile = TFile::Open((inputnames.at(number_of_file)).c_str());
	if (!inputfile) {
		throw std::exception();
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
}

std::pair<int, int> DrawingHistograms::Set_train_bunch_number(int number_of_file) {
	int number_of_train = 0;
	int number_of_bunch = 0;
	if ((number_of_file + 1) <= 1312) {	//number of bunches starts with 1, not 0
		number_of_train = 1;
		number_of_bunch = number_of_file + 1;
	} else if ((number_of_file + 1) > 1312 && (number_of_file + 1) <= 2624) {
		number_of_train = 2;
		number_of_bunch = (number_of_file + 1) - 1312;
	}
	return std::pair<int, int>(number_of_train, number_of_bunch);
}

void DrawingHistograms::Filling_Data_for_SubDetectors(int subdetector_iterator) {
	Time PassedTime;
	std::vector<CellHits*> AllHitCounts;

	//Getting the inputfile and its TTrees
	for (int fileIterator = 0; fileIterator < number_of_files; ++fileIterator) {
		Setup_for_inputfiles(fileIterator, subdetector_iterator, PassedTime, AllHitCounts);
	} //End of loop through inputfiles
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	for (auto iterator = HitsPerLayerMap.begin(); iterator != HitsPerLayerMap.end(); ++iterator) {
		for (auto e = iterator->second.begin(); e != iterator->second.end(); ++e) {
			if (*e > 0) {
				Hits_PerLayer_.at(iterator->first)->Fill(*e);
				Hits_PerLayer_.at(MaxNumberLayers + 1)->Fill(*e);
			}
		}
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	for (auto iterator = HitMap.begin(); iterator != HitMap.end(); ++iterator) {
		if (iterator->second > 0) {
			Hits_Histo_.at(SubDetectors->at(subdetector_iterator)->GetLayer(iterator->first))->Fill(iterator->second);
			Hits_Histo_.at(MaxNumberLayers + 1)->Fill(iterator->second);
			if (iterator->second > 4)
				DeadCells->Fill(iterator->first);
		}
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	int totdead = 0;
	std::map<int, std::vector<float> > average_occupancy_r;
	std::map<int, std::vector<float> > stddev_occupancy_r;
	std::map<int, std::vector<float> > average_occupancy_phi;
	std::map<int, std::vector<float> > stddev_occupancy_phi;
	std::map<int, int> bunch_totdead;

	for (int i = 0; i < AllHitCounts.size(); ++i) {
		AllHitCounts.at(i)->Check_Rad_Position(SubDetectors->at(subdetector_iterator)->GetName());
		AllHitCounts.at(i)->Check_Phi_Position();

		for(auto const &iterator : AllHitCounts.at(i)->Get_AverageOccupancy_Rad()){
			std::cout << "AverageOccupancy_Rad.size() = " << AllHitCounts.at(i)->Get_AverageOccupancy_Rad().size()
					<< std::endl;
			std::cout << iterator.first << "," << AllHitCounts.at(i)->Get_AverageOccupancy_Rad()[iterator.first].second.first << std::endl;
			average_occupancy_r[iterator.first].push_back(AllHitCounts.at(i)->Get_AverageOccupancy_Rad()[iterator.first].second.first);
			stddev_occupancy_r[iterator.first].push_back(AllHitCounts.at(i)->Get_AverageOccupancy_Rad()[iterator.first].second.second);
		}

		for(auto const &iterator : AllHitCounts.at(i)->Get_AverageOccupancy_Phi()){
			std::cout << "AverageOccupancy_Phi.size() = " << AllHitCounts.at(i)->Get_AverageOccupancy_Phi().size()
					<< std::endl;
			std::cout << iterator.first << "," << AllHitCounts.at(i)->Get_AverageOccupancy_Phi()[iterator.first].second.first << std::endl;
			average_occupancy_phi[iterator.first].push_back(AllHitCounts.at(i)->Get_AverageOccupancy_Phi()[iterator.first].second.first);
			stddev_occupancy_phi[iterator.first].push_back(AllHitCounts.at(i)->Get_AverageOccupancy_Phi()[iterator.first].second.second);
		}
		/*
		 for (int j = 0; j < AllHitCounts.at(i)->Get_CellID().size(); ++j) {
		 Occupancy_r_Histo_->Fill(AllHitCounts.at(i)->Get_Position_Radius().at(j),AllHitCounts.at(i)->Get_HitCount().at(j));
		 Occupancy_phi_Histo_->Fill(AllHitCounts.at(i)->Get_Position_Phi().at(j),AllHitCounts.at(i)->Get_HitCount().at(j));
		 }
		 */

		for (int j = 0; j < AllHitCounts.at(i)->Get_CellID().size(); ++j) {
			if (AllHitCounts.at(i)->Get_HitCount().at(j) > 4) {
				DeadCells->Fill(AllHitCounts.at(i)->Get_BunchNumber());
				totdead++;
			}
		}
		for (int vector_it = 0; vector_it < BunchNumbers_for_TotDeadCells.size(); ++vector_it) {
			if (AllHitCounts.at(i)->Get_BunchNumber() == BunchNumbers_for_TotDeadCells.at(vector_it)) {
				bunch_totdead[BunchNumbers_for_TotDeadCells.at(vector_it)] = totdead;
			}
		}
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	for (auto const &iterator : average_occupancy_phi) {
		float average = 0;
		for (int v = 0; v < iterator.second.size(); ++v) {
			std::cout << "Occupancy phi average = " << average << std::endl;
			average += iterator.second.at(v);
		}
		average /= float(iterator.second.size());
		std::cout << "Occupancy_phi_Histo_->FindBin(iterator->first) = " << Occupancy_phi_Histo_->FindBin(float(iterator.first/10.0)) << std::endl;
		Occupancy_phi_Histo_->SetBinContent(Occupancy_phi_Histo_->FindBin(float(iterator.first/10.0)), average); //first = (phi*10)
	}
	for (auto const &iterator : stddev_occupancy_phi) {
		float stddev = 0;
		for (int v = 0; v < iterator.second.size(); ++v) {
			std::cout << "Occupancy phi stddev = " << stddev << std::endl;
			stddev += pow(iterator.second.at(v),2);
		}
		stddev = sqrt(stddev);
		stddev /= float(iterator.second.size());
		Occupancy_phi_Histo_->SetBinError(Occupancy_phi_Histo_->FindBin(float(iterator.first/10.0)), stddev); //first = (phi*10)
	}

	for (auto const &iterator : average_occupancy_r) {
		float average = 0;
		for (int v = 0; v < iterator.second.size(); ++v) {
			std::cout << "Occupancy r average = " << average << std::endl;
			average += iterator.second.at(v);
		}
		average /= float(iterator.second.size());
		std::cout << "Occupancy_r_Histo_->FindBin(iterator->first) = " << Occupancy_r_Histo_->FindBin(iterator.first) << std::endl;
		Occupancy_r_Histo_->SetBinContent(Occupancy_r_Histo_->FindBin(iterator.first), average); //first = radius bin
	}
	for (auto const &iterator : stddev_occupancy_r) {
		float stddev = 0;
		for (int v = 0; v < iterator.second.size(); ++v) {
			std::cout << "Occupancy r stddev = " << stddev << std::endl;
			stddev += pow(iterator.second.at(v),2);
		}
		stddev = sqrt(stddev);
		stddev /= float(iterator.second.size());
		Occupancy_r_Histo_->SetBinError(Occupancy_r_Histo_->FindBin(iterator.first), stddev); //first = phi bin
	}

	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	/*
	 TProfile *AverageOccupancyRadius = Occupancy_r_Histo_->ProfileX();
	 TProfile *AverageOccupancyPhi = Occupancy_phi_Histo_->ProfileX();
	 TCanvas phillcanvas;
	 AverageOccupancyRadius->Draw();
	 phillcanvas.Print("PhillRocks.pdf");
	 AverageOccupancyPhi->Draw();
	 phillcanvas.Print("PhillRocks2.pdf");
	 */

	int i = 0;
	for (auto iterator = bunch_totdead.begin(); iterator != bunch_totdead.end(); ++iterator) {
		TotDeadCells_x[i] = iterator->first;
		TotDeadCells_y[i] = iterator->second;
		i++;
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
}

void DrawingHistograms::Setup_for_inputfiles(int file_iterator, int subdetector_iterator, Time & PassedTime,
		std::vector<CellHits*> & AllHitCounts) {
	Open_inputfile(file_iterator);
	TTree* Tree_MCP;
	inputfile->GetObject("Tree_MCP", Tree_MCP);
	TTree *SubdetectorTree = Get_TTree(inputfile, SubDetectors->at(subdetector_iterator)->GetName());
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	int number_of_hits = SubdetectorTree->GetEntries();
	std::cout << "The TTree " << SubdetectorTree->GetName() << " has " << number_of_hits << " entries." << std::endl;

	Number_train_bunch = Set_train_bunch_number(file_iterator);
	PassedTime.Calculate_passedbytime(Number_train_bunch.first, Number_train_bunch.second);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	Data* data = SetBranches(SubdetectorTree);
	CellHits *HitCount = new CellHits();

	std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy2D; //layer, bin, energies
	std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy3D; //layer, bin, energies

	for (std::size_t i = 0; i < number_of_hits; i++) {
		SubdetectorTree->GetEntry(i);
		Filling_Data_of_hits(file_iterator, subdetector_iterator, data, HitCount, PassedTime, HitMapEnergy2D,
				HitMapEnergy3D);
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	AllHitCounts.push_back(HitCount);

	int const colorrangeweight = 1000000000;
	Fill_Histogram_from_Map<TH2D*>(HitMapEnergy2D, &Hits_Energy_2D_, colorrangeweight);
	Fill_Histogram_from_Map<TH3D*>(HitMapEnergy3D, &Hits_Energy_3D_, colorrangeweight);

	int number_of_particles = 0;
	number_of_particles = Tree_MCP->GetEntries();
	ParticlesVSEvent->Fill(file_iterator, number_of_particles);
	Particles->Fill(number_of_particles);
	Hits->Fill(number_of_hits);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	inputfile->Close();
	delete inputfile;
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
}

void DrawingHistograms::Define_Data_Variables(Data* data, Time & time, float & energy, float & absolutetime, float & x,
		float & y, float & z, std::array<double, 3> & vertex) {
	if (YesNo_TrackerHistograms) {
		x = data->Get_x_hit_particle();
		y = data->Get_y_hit_particle();
		z = data->Get_z_hit_particle();
		energy = data->Get_dEdx_hit();
		vertex = data->Get_vertex_particle();
		//absolute time = time in respect to the current bunch interaction + time passed by since first bunch interaction
		absolutetime = data->Get_time_hit() + time.Get_passedbytime();
	}
	if (!YesNo_TrackerHistograms) {
		x = data->Get_x_hit();
		y = data->Get_y_hit();
		z = data->Get_z_hit();
		energy = data->Get_energy_hit();
		vertex = data->Get_vertex_mother();
		//absolute time = time in respect to the current bunch interaction + time passed by since first bunch interaction
		absolutetime = data->Get_time_contribution() + time.Get_passedbytime();
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
}
void DrawingHistograms::Fill_Histo(std::vector<TH1D*> Histo, int Layerno, float variable1) {
	Histo.at(Layerno)->Fill(variable1);
	Histo.at(MaxNumberLayers + 1)->Fill(variable1);
}
void DrawingHistograms::Fill_Histo(std::vector<TH2D*> Histo, int Layerno, float variable1, float variable2) {
	Histo.at(Layerno)->Fill(variable1, variable2);
	Histo.at(MaxNumberLayers + 1)->Fill(variable1, variable2);
}
void DrawingHistograms::Fill_Histo(std::vector<TH3D*> Histo, int Layerno, float variable1, float variable2,
		float variable3) {
	Histo.at(Layerno)->Fill(variable1, variable2, variable3);
	Histo.at(MaxNumberLayers + 1)->Fill(variable1, variable2, variable3);
}

void DrawingHistograms::Filling_Data_of_hits(int file_iterator, int subdetector_iterator, Data* data,
		CellHits* HitCount, Time & PassedTime, std::map<std::pair<int, int>, std::vector<float> > &HitMapEnergy2D,
		std::map<std::pair<int, int>, std::vector<float> > &HitMapEnergy3D) {

	CellID *SubdetectorCells = InitializeCellIDClass(SubDetectors->at(subdetector_iterator)->GetName(), data);
	SubdetectorCells->CreateCellID();
	unsigned long const CellIDkey = SubdetectorCells->CellID_ToINTconversion(SubdetectorCells->GetCellID());
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	LayerCodeInCellID LayerInfo;
	int const Layer_no = LayerInfo.GetLayer(SubdetectorCells->GetCellID(),
			SubDetectors->at(subdetector_iterator)->GetStartLayerBin(),
			SubDetectors->at(subdetector_iterator)->GetLengthLayerBin());
	if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no) == hitLayers.end()) {
		hitLayers.push_back(Layer_no);
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	float energy = 0.;
	float absolutetime = 0.;
	float x = 0.;
	float y = 0.;
	float z = 0.;
	std::array<double, 3> vertex = { 0 };

	Define_Data_Variables(data, PassedTime, energy, absolutetime, x, y, z, vertex);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	//This adds a hit to the Cell ID for a given bunch
	HitCount->CheckCellID(CellIDkey, x, y);
	HitCount->Set_BunchNumber(file_iterator + 1);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	//Fill Maps:
	HitsPerLayerMap[Layer_no].at(file_iterator) += 1;
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	HitMapEnergy2D[std::pair<int, int>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(x, y))].push_back(energy);
	HitMapEnergy3D[std::pair<int, int>(Layer_no, Hits_Energy_3D_.at(Layer_no)->FindBin(z, x, y))].push_back(energy);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	if (HitMap.find(CellIDkey) == HitMap.end()) {
		HitMap[CellIDkey] = 1;
	} else {
		HitMap[CellIDkey] += 1;
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	//Fill histograms:
	Fill_Histo(Hits_2D_, Layer_no, x, y);
	Fill_Histo(Hits_3D_, Layer_no, z, x, y);
	Fill_Histo(Hits_Energy_Histo_, Layer_no, energy);
	Fill_Histo(ParticleOrigins_2D_, Layer_no, vertex[2], sqrt(pow(vertex[0], 2) + pow(vertex[1], 2)));
	//std::cout << "x_hit, y_hit = " << x << ", " << y << std::endl;
	Fill_Histo(Hits_Time_rtime_2D_, Layer_no, absolutetime, sqrt(pow(x, 2) + pow(y, 2)));
	Fill_Histo(Hits_Time_ztime_2D_, Layer_no, absolutetime, z);
	Fill_Histo(Hits_Time_, Layer_no, absolutetime);
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	if (file_iterator == 0) {
		std::cout << "Filling hits into 3D plot for the time " << std::to_string(absolutetime) << std::endl;
		//Hits_Time_3D_.at(floor(absolutetime/time_step))->Fill(z, x, y);
		Fill_HitsTime3D_Plots(absolutetime, 0, 2, Hits_Time_3D_.at(0), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 2, 4, Hits_Time_3D_.at(1), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 4, 6, Hits_Time_3D_.at(2), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 6, 8, Hits_Time_3D_.at(3), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 8, 12, Hits_Time_3D_.at(4), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 12, 16, Hits_Time_3D_.at(5), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 16, 20, Hits_Time_3D_.at(6), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 20, 50, Hits_Time_3D_.at(7), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 50, 100, Hits_Time_3D_.at(8), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 100, 200, Hits_Time_3D_.at(9), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 200, 300, Hits_Time_3D_.at(10), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 300, 400, Hits_Time_3D_.at(11), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 400, 500, Hits_Time_3D_.at(12), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 500, 600, Hits_Time_3D_.at(13), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 600, 700, Hits_Time_3D_.at(14), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 700, 800, Hits_Time_3D_.at(15), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 800, 900, Hits_Time_3D_.at(16), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 900, 1000, Hits_Time_3D_.at(17), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 1000, 1250, Hits_Time_3D_.at(18), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 1250, 1500, Hits_Time_3D_.at(19), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 1500, 2000, Hits_Time_3D_.at(20), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 2000, 2500, Hits_Time_3D_.at(21), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 2500, 3000, Hits_Time_3D_.at(22), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 3000, 4000, Hits_Time_3D_.at(23), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 4000, 6000, Hits_Time_3D_.at(24), x, y, z);
		Fill_HitsTime3D_Plots(absolutetime, 6000, 10000, Hits_Time_3D_.at(25), x, y, z);
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
}

void DrawingHistograms::DrawingMacro() {
	Setup_SubDetector_vector();
	Initialize();
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Setup_BinningArrays(SubDetectors, &axis_range_occupancy_plot, &axis_range_occupancy_r_plot,
			&axis_range_occupancy_phi_plot, &axis_range_plot_1D, &axis_range_plot_2D, &axis_range_plot_3D,
			&axis_range_plot_energy_1D, time_interval_bunchspacing, &axis_range_plot_time, &axis_range_plot_rtime_2D,
			&axis_range_plot_ztime_2D, &axis_range_plot_time_3D);
	SetupGeneralHistograms();
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	MaxNumberLayers = Find_MaxNumberLayers();
	for (int l = 0; l <= MaxNumberLayers + 1; ++l) { //In the end, push back an additional histogram for all the layers together

		Setup_HitsPerLayerMap(l);

		std::string layerstring;
		if (l == MaxNumberLayers + 1)
			layerstring = "all";
		else
			layerstring = std::to_string(l);

		SetupLayerHistograms(layerstring);
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	for (int s = 0; s < SubDetectors->size(); ++s) {
		Filling_Data_for_SubDetectors(s);
	} //End of SubDetectors loop
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	TGraph* TotDeadCells = new TGraph(BunchNumbers_for_TotDeadCells.size(), TotDeadCells_x, TotDeadCells_y);
	TotDeadCells->SetTitle("Dead cells (> 4 hits per cell) for given number of bunch crossings");
	TotDeadCells->SetMarkerStyle(20);
	TotDeadCells->GetYaxis()->SetTitle("Count");
	TotDeadCells->GetXaxis()->SetTitle("Number of bunch crossings");
	TotDeadCells->GetXaxis()->CenterTitle();

	gStyle->SetOptStat(1);
	//gStyle->SetOptStat(111111);
	TCanvas* PDF_Canvas_Hits_Layers = new TCanvas();
	PDF_Canvas_Hits_Layers->Print("PDFCanvas_Hits_Layers.pdf[");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	TCanvas* PDF_Canvas_Hits_allLayers = new TCanvas(); //For all layers together
	PDF_Canvas_Hits_allLayers->Print("PDFCanvas_Hits_allLayers.pdf[");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	output_rootfile->cd();
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	for (signed int t = 0; t < 20; ++t) {
		Hits_Time_3D_.at(t)->Write();
	}
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	for (signed int l = 0; l < hitLayers.size(); ++l) {

		std::cout << "Printing plots for layer number " << hitLayers.at(l) << std::endl;

		Hits_3D_.at(hitLayers.at(l))->Write();
		Hits_Energy_3D_.at(hitLayers.at(l))->Write();
		Hits_3D_.at(MaxNumberLayers + 1)->Write();
		Hits_Energy_3D_.at(MaxNumberLayers + 1)->Write();
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->cd();

		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_2D_.at(hitLayers.at(l)), "colz", "PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_2D_.at(MaxNumberLayers + 1), "colz",
					"PDFCanvas_Hits_allLayers.pdf"); //Print the plot fro all layers only once!
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Energy_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Energy_2D_.at(MaxNumberLayers + 1), "colz",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(1);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, ParticleOrigins_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(1);
			WritePrintHistogram<TH2D*>(Hits_Canvas_, ParticleOrigins_2D_.at(MaxNumberLayers + 1), "colz",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		gROOT->ForceStyle();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_.at(hitLayers.at(l)), "", "PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_.at(hitLayers.at(l)), "", "PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(1);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(1);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Time_.at(hitLayers.at(l)), "", "PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(1);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Time_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_rtime_2D_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_rtime_2D_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_ztime_2D_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) {
			Hits_Canvas_->Update();
			Hits_Canvas_->SetLogy(0);
			Hits_Canvas_->SetLogx(0);
			Hits_Canvas_->SetLogz(0);
			WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_ztime_2D_.at(MaxNumberLayers + 1), "",
					"PDFCanvas_Hits_allLayers.pdf");
		}
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

	}
	PDF_Canvas_Hits_Layers->Print("PDFCanvas_Hits_Layers.pdf]");
	delete PDF_Canvas_Hits_Layers;
	PDF_Canvas_Hits_allLayers->Print("PDFCanvas_Hits_allLayers.pdf]");
	delete PDF_Canvas_Hits_allLayers;
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

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
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;

		TCanvas* PDF_Canvas_Hits_CompareLayers = new TCanvas();
		PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf[");

		if (std::find(hitLayers.begin(), hitLayers.end(), first_layer_to_be_compared) != hitLayers.end()
				&& std::find(hitLayers.begin(), hitLayers.end(), last_layer_to_be_compared) != hitLayers.end()) {

			Hits_Canvas_->cd();
			Hits_Canvas_->Clear();
			Hits_Canvas_->SetLogy(0);
			WritePrintComparedHistogram<TGraphErrors*>(Hits_Canvas_, MeanHits_PerLayer, first_layer_to_be_compared,
					end_of_range, "AP", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ", " << __LINE__ << std::endl;

			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_histo_perlayer_title;
			new_histo_perlayer_title << "Hits per layer for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range
					<< ";Number of hits per bunch crossing;Count";
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_, new_histo_perlayer_title.str(), hitLayers,
					first_layer_to_be_compared, end_of_range, false, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ", " << __LINE__ << std::endl;

			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_histo_title;
			new_histo_title << "Hit occupancy per cell for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range << ";Number of hits per cell;Count";
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_, new_histo_title.str(), hitLayers,
					first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ", " << __LINE__ << std::endl;

			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_Energy_histo_title;
			new_Energy_histo_title << "Deposited hit energy for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range << ";Deposited hit energy;Count";
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_, new_Energy_histo_title.str(),
					hitLayers, first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");
			std::cout << __FILE__ << ", " << __LINE__ << std::endl;
		} //End of if-to-be-compared loop
		else {
			std::cerr << "The first or the last (or both) layer of your given range has no hits!\n"
					"There will be no comparison of the layers." << std::endl;
		}
		PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf]");
		delete PDF_Canvas_Hits_CompareLayers;
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	}
	for (signed int l = 0; l < MaxNumberLayers + 1; ++l) {
		delete Hits_PerLayer_.at(l), Hits_Histo_.at(l), Hits_2D_.at(l), Hits_3D_.at(l);
		delete Hits_Energy_Histo_.at(l), Hits_Energy_2D_.at(l), Hits_Energy_3D_.at(l), Hits_Time_ztime_2D_.at(l);
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	}
	for (signed int t = 0; t < 20; ++t) {
		delete Hits_Time_3D_.at(t);
		std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	}
	delete Hits_Canvas_;
	output_rootfile->Write();

	TCanvas* PDF_Canvas_ParticlesHits_per_File = new TCanvas();
	PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf[");
	std::stringstream FilesCanvasName_eps, FilesCanvasName_C;

	Files_Canvas_->cd();
	gStyle->SetOptStat(1);
	//gStyle->SetOptStat(111111);

	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(0);
	WritePrintHistogram(Files_Canvas_, ParticlesVSEvent, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(0);
	WritePrintHistogram(Files_Canvas_, Particles, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(1);
	WritePrintHistogram(Files_Canvas_, Hits, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(1);
	WritePrintHistogram(Files_Canvas_, Occupancy_r_Histo_, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(0);
	WritePrintHistogram(Files_Canvas_, Occupancy_phi_Histo_, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(0);
	WritePrintHistogram(Files_Canvas_, DeadCells, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;
	Files_Canvas_->Clear();
	Files_Canvas_->SetLogy(0);
	WritePrintHistogram(Files_Canvas_, TotDeadCells, "AP", "PDFCanvas_ParticlesHits_perFile.pdf");
	PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf]");

	delete PDF_Canvas_ParticlesHits_per_File;
	delete Files_Canvas_;
	std::cout << __FILE__ << ", " << __LINE__ << std::endl;

} //End of function DrawingMacro 
