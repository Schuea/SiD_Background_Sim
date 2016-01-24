#include "DrawingHistograms.h"

DrawingHistograms::DrawingHistograms(std::string _outputname, std::vector<std::string> _inputnames, std::vector<std::string> _argument_subdetectors) : 
  outputname(_outputname),
  inputnames(_inputnames),
  argument_subdetectors(_argument_subdetectors)
{
}

void DrawingHistograms::Setup(){
	output_rootfile = new TFile(outputname.c_str(), "RECREATE");
	Canvas_name = outputname.substr(0, outputname.find_last_of("."));
	Files_Canvas = new TCanvas((Canvas_name + "_HitsPerFile").c_str());
	ParticlesVSEvent = new TH2D("ParticlesVSEvent", "Particles per event", 50, 0, number_of_files, 50, 190000, 210000);
	Particles = new TH1D("Particles", "Particles", 100, 180000, 220000);
	gausfit_Particles = new TF1("gausfit", "gaus", 190000, 210000);
	DeadCells = new TH1D("DeadCells", "Dead cells (> 4 hits per cell) per bunch crossing", int((number_of_files+50)/10),0, number_of_files+50);
	DeadCells->GetYaxis()->SetTitle("Count");
	DeadCells->GetXaxis()->SetTitle("Number of bunch crossings");
	DeadCells->GetXaxis()->CenterTitle();
	YesNo_TrackerHistograms = DecideIfTrackerHistograms(argument_subdetectors);
}

void DrawingHistograms::DrawingMacro(){
  TH1::SetDefaultSumw2();

  Setup();

	std::string subdetector_name("");
	std::vector<Subdetector*> * SubDetectors = new std::vector<Subdetector*>();
	SetupSubDetectorsVector(SubDetectors, &subdetector_name, argument_subdetectors);

	std::cout << "Subdetectors: " << std::endl;
	for (int i = 0; i < SubDetectors->size(); ++i) {
		std::cout << SubDetectors->at(i)->GetName() << std::endl;
	}

	std::vector<float> axis_range_plot_1D = { }; //xbins, xlow, xup
	std::vector<float> axis_range_plot_2D = { }; //xbins, xlow, xup, ybins, ylow, yup
	std::vector<float> axis_range_plot_3D = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup
	std::vector<float> axis_range_plot_energy_1D = { }; //xbins, xlow, xup

	std::vector<float> axis_range_plot_time = { }; //timebins, timelow, timeup
	std::vector<float> axis_range_plot_rtime_2D = { }; //timebins, timelow, timeup, rbins, rlow, rup
	std::vector<float> axis_range_plot_ztime_2D = { }; //timebins, timelow, timeup, zbins, zlow, zup
	std::vector<float> axis_range_plot_time_3D = { }; //timebins, timelow, timeup, zbins, zlow, zup, xbins, xlow, xup

	Setup_BinningArrays(SubDetectors, &axis_range_plot_1D, &axis_range_plot_2D, &axis_range_plot_3D, &axis_range_plot_energy_1D,
			&axis_range_plot_time, &axis_range_plot_rtime_2D, &axis_range_plot_ztime_2D, &axis_range_plot_time_3D);
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

	int MaxNumberLayers = 0;
	int Layer_no = 0;
	int CellIDkey = 0.;

	std::map<int, std::vector<int> > HitsPerLayerMap; //Key is layer, value is the hits for a given bunch
	Hits_Canvas_ = new TCanvas((Canvas_name + "_Hits_" + subdetector_name).c_str());

	//Find the largest number of layers from all the subdetectors that are to be plotted
	for (int s = 0; s < SubDetectors->size(); ++s) {
		MaxNumberLayers = FindMax(SubDetectors->at(s)->GetNumberOfLayers(), MaxNumberLayers);
	}
	for (int l = 0; l <= MaxNumberLayers + 1; ++l) {//In the end, push back an additional histogram for all the layers together
		for (int files = 1; files <= number_of_files; ++files) {//For each file (bunch), add zero hits for this layer (so we can add to them later)
			HitsPerLayerMap[l].push_back(0);
		}

		std::stringstream layer;
		layer << l;
		std::string layerstring("");

		if (l ==  MaxNumberLayers+1){
			layerstring = "all";
		}
		else layerstring = layer.str();

		gROOT->ForceStyle();
		gStyle->SetPalette(1);

    //This next bit of code makes lots of strings for the various names and titles of the histograms and then fills them using the SetupHistoTitles function
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
		std::string histo_name_time3D, histo_title_time3D;

		SetupHistoTitles(subdetector_name, layerstring, histo_name1D, histo_title1D, histo_name2D, histo_title2D,
				histo_name3D, histo_title3D, energyhisto_name1D, energyhisto_title1D, energyhisto_name2D,
				energyhisto_title2D, energyhisto_name3D, energyhisto_title3D, hitsperlayerhisto_name,
				hitsperlayerhisto_title, particleoriginshisto_name, particleoriginshisto_title,
				histo_name_time, histo_title_time, histo_name_rtime2D, histo_title_rtime2D,
				histo_name_ztime2D, histo_title_ztime2D, histo_name_time3D, histo_title_time3D);

		Setup_ParticleOriginsHisto(ParticleOrigins_2D_, axis_range_plot_3D, particleoriginshisto_name,
				particleoriginshisto_title, "cylindrical");
		Setup_Histo(Hits_PerLayer_, axis_range_plot_1D, hitsperlayerhisto_name, hitsperlayerhisto_title);
		Setup_Histo(Hits_Histo_, axis_range_plot_1D, histo_name1D, histo_title1D);
		Setup_Histo(Hits_2D_, axis_range_plot_2D, histo_name2D, histo_title2D);
		Setup_Histo(Hits_3D_, axis_range_plot_3D, histo_name3D, histo_title3D);
		Setup_Histo(Hits_Energy_Histo_, axis_range_plot_energy_1D, energyhisto_name1D, energyhisto_title1D);
		Setup_Histo(Hits_Energy_2D_, axis_range_plot_2D, energyhisto_name2D, energyhisto_title2D);
		Setup_Histo(Hits_Energy_3D_, axis_range_plot_3D, energyhisto_name3D, energyhisto_title3D);

		Setup_Histo(Hits_Time_, axis_range_plot_time, histo_name_time, histo_title_time);
		Setup_Histo(Hits_Time_rtime_2D_, axis_range_plot_rtime_2D, histo_name_rtime2D, histo_title_rtime2D);
		Setup_Histo(Hits_Time_ztime_2D_, axis_range_plot_ztime_2D, histo_name_ztime2D, histo_title_ztime2D);
		Setup_Histo(Hits_Time_3D_, axis_range_plot_time_3D, histo_name_time3D, histo_title_time3D);

	} //End of layers

	std::vector<int> hitLayers;
	for(int s = 0; s < SubDetectors->size(); ++s) {
		std::map<int, int> HitMap;  //cellid, count of all hits per cell
    std::vector< CellHits* > AllHitCounts;
		//Getting the inputfile and its TTrees
		for (int fileIterator = 0; fileIterator < number_of_files; ++fileIterator) {
			inputfile = TFile::Open((inputnames.at(fileIterator)).c_str());
			if (!inputfile) {
				throw std::exception();
			}
      //Make the tree for accessing MC particles
			TTree* Tree_MCP;
			inputfile->GetObject("Tree_MCP", Tree_MCP);

      //Make the tree for the subdetector details
			int number_of_hits = 0;
			TTree *SubdetectorTree = Get_TTree(inputfile, SubDetectors->at(s)->GetName());
			number_of_hits = SubdetectorTree->GetEntries();

			std::cout << "The TTree " << SubdetectorTree->GetName() << " has "
					<< number_of_hits << " entries." << std::endl;

      //Set up the tree data in the Data class
			Data* data = SetBranches(SubdetectorTree);

			std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy2D; //layer, bin, energies
			std::map<std::pair<int, int>, std::vector<float> > HitMapEnergy3D; //layer, bin, energies

      CellHits *HitCount = new CellHits();
      HitCount->Set_BunchNumber(fileIterator+1);
			for (std::size_t i = 0; i < number_of_hits; i++) { //Go through each hit in the subdetector
				SubdetectorTree->GetEntry(i);
        //Set up the Cell ID
				CellID *SubdetectorCells = InitializeCellIDClass(SubDetectors->at(s)->GetName(), data);
				SubdetectorCells->CreateCellID();
				CellIDkey = SubdetectorCells->CellID_ToINTconversion(SubdetectorCells->GetCellID());

        //Get the layer number
				LayerCodeInCellID LayerInfo;
				Layer_no = LayerInfo.GetLayer(SubdetectorCells->GetCellID(), SubDetectors->at(s)->GetStartLayerBin(),	SubDetectors->at(s)->GetLengthLayerBin());

        //
				if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no) == hitLayers.end()) {
					hitLayers.push_back(Layer_no);
				}

        //This adds a hit to the Cell ID for a given bunch
				HitCount->CheckCellID(CellIDkey);

				//Fill Maps:
				HitsPerLayerMap[Layer_no].at(fileIterator) += 1;

				float energy = 0.;
				float time = 0.;
				float x = 0.;
				float y = 0.;
				float z = 0.;
				std::array <double, 3> vertex = {0};
				if (YesNo_TrackerHistograms){
					x = data-> Get_x_hit_particle();
					y = data-> Get_y_hit_particle();
					z = data-> Get_z_hit_particle();
					energy = data->Get_dEdx_hit();
					vertex = data->Get_vertex_particle();
					time = data->Get_time_hit();
				}
				if (!YesNo_TrackerHistograms){
					x = data-> Get_x_hit();
					y = data-> Get_y_hit();
					z = data-> Get_z_hit();
					energy = data->Get_energy_hit();
					vertex = data->Get_vertex_mother();
					time = data->Get_time_contribution();
				}

				HitMapEnergy2D[std::pair<int, int>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(x, y))].push_back(energy);
				HitMapEnergy3D[std::pair<int, int>(Layer_no, Hits_Energy_3D_.at(Layer_no)->FindBin(z, x, y))].push_back(energy);

				if (HitMap.find(CellIDkey) == HitMap.end()) {
					HitMap[CellIDkey] = 1;
				} else {
					HitMap[CellIDkey] += 1;
				}
				//Fill histograms:
				Hits_Energy_Histo_.at(Layer_no)->Fill(energy);
				Hits_Energy_Histo_.at(MaxNumberLayers+1)->Fill(energy);
				ParticleOrigins_2D_.at(Layer_no)->Fill(vertex[2], sqrt(pow(vertex[0], 2) + pow(vertex[1], 2)));
				ParticleOrigins_2D_.at(MaxNumberLayers+1)->Fill(vertex[2], sqrt(pow(vertex[0], 2) + pow(vertex[1], 2)));

				std::cout << "x_hit, y_hit = " << x << ", " << y << std::endl;
				Hits_Time_rtime_2D_.at(Layer_no)->Fill(time, sqrt(pow(x,2)+pow(y,2)));
				Hits_Time_rtime_2D_.at(MaxNumberLayers+1)->Fill(time, sqrt(pow(x,2)+pow(y,2)));
				Hits_Time_ztime_2D_.at(Layer_no)->Fill(time, z);
				Hits_Time_ztime_2D_.at(MaxNumberLayers+1)->Fill(time, z);
				Hits_Time_3D_.at(Layer_no)->Fill(time, z, sqrt(pow(x,2)+pow(y,2)));
				Hits_Time_3D_.at(MaxNumberLayers+1)->Fill(time, z, sqrt(pow(x,2)+pow(y,2)));
				Hits_Time_.at(Layer_no)->Fill(time);
				Hits_Time_.at(MaxNumberLayers+1)->Fill(time);

				Hits_2D_.at(Layer_no)->Fill(x, y);
				Hits_2D_.at(MaxNumberLayers+1)->Fill(x, y);
				Hits_3D_.at(Layer_no)->Fill(z, x, y);
				Hits_3D_.at(MaxNumberLayers+1)->Fill(z, x, y);
			}
      AllHitCounts.push_back(HitCount);
			int const colorrangeweight = 1000000000;
			Fill_Histogram_from_Map<TH2D*>(HitMapEnergy2D, &Hits_Energy_2D_, colorrangeweight);
			Fill_Histogram_from_Map<TH3D*>(HitMapEnergy3D, &Hits_Energy_3D_, colorrangeweight);

			int number_of_particles = 0;
			number_of_particles = Tree_MCP->GetEntries();
			ParticlesVSEvent->Fill(fileIterator, number_of_particles);
			Particles->Fill(number_of_particles);
			Hits->Fill(number_of_hits);
			inputfile->Close();
			delete inputfile;
		} //End of loop through inputfiles

		for (auto iterator = HitsPerLayerMap.begin(); iterator != HitsPerLayerMap.end(); iterator++) {
			for (auto e = iterator->second.begin(); e != iterator->second.end(); ++e) {
				if (*e > 0) {
					Hits_PerLayer_.at(iterator->first)->Fill(*e);
				}
			}
		}
    for(int i = 0; i < AllHitCounts.size(); ++i){
      for(int j = 0; j < AllHitCounts.at(i)->Get_CellID().size(); ++j){
        if(AllHitCounts.at(i)->Get_HitCount().at(j) > 4){
          DeadCells->Fill(AllHitCounts.at(i)->Get_BunchNumber());
        }
      }
    }
		for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
			if (iterator->second > 0) {
				Hits_Histo_.at(SubDetectors->at(s)->GetLayer(iterator->first))->Fill(iterator->second);
			}
		}
	} //End of SubDetectors loop
			std::cout << __FILE__ << ": " << __LINE__ << std::endl;

	gStyle->SetOptStat(1);
	//gStyle->SetOptStat(111111);
	TCanvas* PDF_Canvas_Hits_Layers = new TCanvas();
	PDF_Canvas_Hits_Layers->Print("PDFCanvas_Hits_Layers.pdf[");

	TCanvas* PDF_Canvas_Hits_allLayers = new TCanvas(); //For all layers together
	PDF_Canvas_Hits_allLayers->Print("PDFCanvas_Hits_allLayers.pdf[");

	output_rootfile->cd();
	for (signed int l = 0; l < hitLayers.size(); ++l) {

    std::cout << "Printing plots for layer number " << hitLayers.at(l) << std::endl;

		Hits_3D_.at(hitLayers.at(l))->Write();
		Hits_Energy_3D_.at(hitLayers.at(l))->Write();
		Hits_3D_.at(MaxNumberLayers+1)->Write();
		Hits_Energy_3D_.at(MaxNumberLayers+1)->Write();

		Hits_Canvas_->cd();

		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_2D_.at(MaxNumberLayers+1), "colz",
				"PDFCanvas_Hits_allLayers.pdf"); //Print the plot fro all layers only once!
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Energy_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Energy_2D_.at(MaxNumberLayers+1), "colz",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(1);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, ParticleOrigins_2D_.at(hitLayers.at(l)), "colz",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH2D*>(Hits_Canvas_, ParticleOrigins_2D_.at(MaxNumberLayers+1), "colz",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Clear();
		gStyle->SetStatX(0.87);
		gROOT->ForceStyle();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_PerLayer_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Energy_Histo_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");

		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(1);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Time_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH1D*>(Hits_Canvas_, Hits_Time_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_rtime_2D_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_rtime_2D_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_ztime_2D_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH2D*>(Hits_Canvas_, Hits_Time_ztime_2D_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
		Hits_Canvas_->Update();
		Hits_Canvas_->SetLogy(0);
		Hits_Canvas_->SetLogx(0);
		Hits_Canvas_->SetLogz(0);
		WritePrintHistogram<TH3D*>(Hits_Canvas_, Hits_Time_3D_.at(hitLayers.at(l)), "",
				"PDFCanvas_Hits_Layers.pdf");
		if (l == 0) WritePrintHistogram<TH3D*>(Hits_Canvas_, Hits_Time_3D_.at(MaxNumberLayers+1), "",
				"PDFCanvas_Hits_allLayers.pdf");
	}
	PDF_Canvas_Hits_Layers->Print("PDFCanvas_Hits_Layers.pdf]");
	delete PDF_Canvas_Hits_Layers;
	PDF_Canvas_Hits_allLayers->Print("PDFCanvas_Hits_allLayers.pdf]");
	delete PDF_Canvas_Hits_allLayers;


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

			Hits_Canvas_->Clear();
			gStyle->SetStatX(0.87);
			Hits_Canvas_->SetLogy(1);
			std::stringstream new_histo_title;
			new_histo_title << "Hit occupancy per cell for " << subdetector_name << " layers "
					<< first_layer_to_be_compared << " - " << end_of_range;
			WritePrintComparedHistogram<TH1D*>(Hits_Canvas_, Hits_Histo_, new_histo_title.str(), hitLayers,
					first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");

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
		delete Hits_Energy_Histo_.at(l), Hits_Energy_2D_.at(l), Hits_Energy_3D_.at(l), Hits_Time_ztime_2D_.at(l), Hits_Time_3D_.at(l);
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
	Files_Canvas->Clear();
	Files_Canvas->SetLogy(0);
	WritePrintHistogram(Files_Canvas, DeadCells, "", "PDFCanvas_ParticlesHits_perFile.pdf");
	std::cout << __FILE__ << ": " << __LINE__ << std::endl;
	PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf]");

	delete PDF_Canvas_ParticlesHits_per_File;
	delete Files_Canvas;

} //End of function DrawingMacro 

