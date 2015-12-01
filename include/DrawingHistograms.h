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

#include <map>

#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"

TFile* inputfile;
TCanvas* Hits_Canvas_;
vector<TH1*> ParticleOrigins_2D_;
vector<TH1*> Hits_PerLayer_;
vector<TH1*> Hits_Histo_;
vector<TH1*> Hits_2D_;
vector<TH1*> Hits_3D_;
vector<TH1*> Hits_Energy_Histo_;
vector<TH1*> Hits_Energy_2D_;
vector<TH1*> Hits_Energy_3D_;


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

  std::vector<float> axis_range_plot_1D = { }; //xbins, xlow, xup
  std::vector<float> axis_range_plot_2D = { }; //xbins, xlow, xup, ybins, ylow, yup
  std::vector<float> axis_range_plot_3D = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup
  std::vector<float> axis_range_plot_energy_1D = { }; //xbins, xlow, xup

  Setup_BinningArrays(SubDetectors, &axis_range_plot_1D, &axis_range_plot_2D, &axis_range_plot_3D, &axis_range_plot_energy_1D);

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
  int CellIDkey = 0.;

  std::map<int, vector<int>> HitsPerLayerMap;

  //Find the largest number of layers from all the subdetectors that are to be plotted
  for (int s = 0; s < SubDetectors->size(); ++s) {
    MaxNumberLayers = FindMax(SubDetectors->at(s).Number_layers, MaxNumberLayers); 
  }
  for (int l = 0; l < MaxNumberLayers; ++l) {

    for(int files = 1; files <= NUMBER_OF_FILES; ++files){
      HitsPerLayerMap[l].push_back(0);
    }

    std::stringstream layer;
    layer << l;
    Hits_Canvas_ = new TCanvas((Canvas_name + "_Hits_" + subdetector_name).c_str());

    gROOT->ForceStyle();
    gStyle->SetPalette(1);

    stringstream* histo_name1D, histo_title1D,
      histo_name2D, histo_title2D,
      histo_name3D, histo_title3D,
      energyhisto_name1D, energyhisto_title1D,
      energyhisto_name2D, energyhisto_title2D,
      energyhisto_name3D, energyhisto_title3D,
      hitsperlayerhisto_name, hitsperlayerhisto_title,
      particleoriginshisto_name, particleoriginshisto_title;

    SetupHistoTitles(subdetector_name, layer,
        histo_name1D, histo_title1D,
        histo_name2D, histo_title2D,
        histo_name3D, histo_title3D,
        energyhisto_name1D, energyhisto_title1D,
        energyhisto_name2D, energyhisto_title2D,
        energyhisto_name3D, energyhisto_title3D,
        hitsperlayerhisto_name, hitsperlayerhisto_title,
        particleoriginshisto_name, particleoriginshisto_title);

    Setup_ParticleOriginsHisto(ParticleOrigins_2D_, axis_range_plot_3D, particleoriginshisto_name, particleoriginshisto_title, "cylindrical");
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

    std::map<int, int> HitMap;//cellid, count of hits per cell

    //Getting the inputfile and its TTrees
    for (int fileIterator = 0; fileIterator < NUMBER_OF_FILES; ++fileIterator) {
      inputfile = TFile::Open((inputnames.at(fileIterator)).c_str());
      if (!inputfile) {
        throw std::exception();
      }

      TTree* Tree_MCP;
      inputfile->GetObject("Tree_MCP", Tree_MCP);

      int number_of_hits = 0;
      number_of_hits = Get_TTree(SubDetectors->at(s).GetName())->GetEntries();

      std::cout << "The TTree " << Get_TTree(inputfile, SubDetectors->at(s).GetName())->GetName() << " has " << number_of_hits
        << " entries." << std::endl;

      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("*", kFALSE); // disable all
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitCellID0", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitCellID1", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitPosition_x", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitPosition_y", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitPosition_z", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitVertex_x", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitVertex_y", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitVertex_z", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchStatus("HitEnergy", kTRUE);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitCellID0", &id0);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitCellID1", &id1);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitPosition_x", &x);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitPosition_y", &y);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitPosition_z", &z);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitVertex_x", &vertex_x);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitVertex_y", &vertex_y);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitVertex_z", &vertex_z);
      Get_TTree(inputfile, SubDetectors->at(s).GetName())->SetBranchAddress("HitEnergy", &energy);

      map<std::pair<int, int>, vector<float> > HitMapEnergy2D; //layer, bin, energies
      map<std::pair<int, int>, vector<float> > HitMapEnergy3D; //layer, bin, energies

      for (std::size_t i = 0; i < number_of_hits; i++) {
        Get_TTree(SubDetectors->at(s).GetName())->GetEntry(i);

        CellID SubdetectorCells;
        InitializeCellIDClass(SubDetectors->at(s).GetName(), id0, id1);
        SubdetectorCells.CreateCellID();
        CellIDkey = 0.;
        CellIDkey = SubdetectorCells.CellID_ToINTconversion(SubdetectorCells.GetCellID());

        LayerCodeInCellID LayerInfo(SubdetectorCells.GetCellID(),SubDetectors->at(s).GetStartLayerBin(),SubDetectors->at(s).GetLengthLayerBin());
        Layer_no = LayerInfo.GetLayer();
        if (std::find(hitLayers.begin(), hitLayers.end(), Layer_no) == hitLayers.end()) {
          hitLayers.push_back(Layer_no);
        }

        //Fill Maps:
        HitsPerLayerMap[Layer_no].at(fileIterator) += 1;

        HitMapEnergy2D[std::pair<int, int>(Layer_no, Hits_Energy_2D_.at(Layer_no)->FindBin(x, y))].push_back(
            energy);
        HitMapEnergy3D[std::pair<int, int>(Layer_no, Hits_Energy_3D_.at(Layer_no)->FindBin(z, x, y))].push_back(
            energy);

        if (HitMap.find(CellIDkey) == HitMap.end()) {
          HitMap[CellIDkey] = 1;
        } else {
          HitMap[CellIDkey] += 1;
        }

        //Fill histograms:
        Hits_Energy_Histo_.at(Layer_no)->Fill(energy);
        ParticleOrigins_2D_.at(Layer_no)->Fill(vertex_z, sqrt(pow(vertex_x,2)+pow(vertex_y,2)));
        Hits_2D_.at(Layer_no)->Fill(x, y);
        Hits_3D_.at(Layer_no)->Fill(z, x, y);

      }
      int const colorrangeweight = 1000000000;
      Fill_Histogram_from_Map(HitMapEnergy2D, &Hits_Energy_2D_, colorrangeweight);
      Fill_Histogram_from_Map(HitMapEnergy3D, &Hits_Energy_3D_, colorrangeweight);

      int number_of_particles = 0;
      number_of_particles = Tree_MCP->GetEntries();
      ParticlesVSEvent->Fill(fileIterator, number_of_particles);
      Particles->Fill(number_of_particles);
      Hits->Fill(number_of_hits);

      inputfile->Close();
      delete inputfile;
    }//End of loop through inputfiles

    for (auto iterator = HitsPerLayerMap.begin(); iterator != HitsPerLayerMap.end(); iterator++) {
      for(auto e = iterator->second.begin(); e != iterator->second.end(); ++e){
        if (*e > 0) {
          Hits_PerLayer_.at(iterator->first)->Fill(*e);
        }
      }
    }
    for (auto iterator = HitMap.begin(); iterator != HitMap.end(); iterator++) {
      if (iterator->second > 0) {
        Hits_Histo_.at(SubDetectors->at(s).ObtainLayerfromCellID(iterator->first, SubDetectors->at(s).GetStartLayerBin(), SubDetectors->at(s).GetLengthLayerBin()))->Fill(iterator->second);
      }
    }

  }//End of SubDetectors loop

  gStyle->SetOptStat(1);
  //gStyle->SetOptStat(111111);
  TCanvas* PDF_Canvas_1D2D_Hits_Layers = new TCanvas(); 
  PDF_Canvas_1D2D_Hits_Layers->Print("PDFCanvas_1D2D_Hits_Layers.pdf[");

  output_rootfile->cd();
  for (signed int l = 0; l < hitLayers.size(); ++l) {

    Hits_3D_.at(hitLayers.at(l))->Write();
    Hits_Energy_3D_.at(hitLayers.at(l))->Write();

    Hits_Canvas_->cd();

    Hits_Canvas_->Clear();
    gStyle->SetStatX(0.87);
    Hits_Canvas_->SetLogy(0);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(0);
    WritePrintHistogram(Hits_Canvas_, Hits_2D_.at(hitLayers.at(l)),"colz", "PDFCanvas_1D2D_Hits_Layers.pdf");

    Hits_Canvas_->Update();
    Hits_Canvas_->SetLogy(0);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(0);
    WritePrintHistogram(Hits_Canvas_, Hits_Energy_2D_.at(hitLayers.at(l)),"colz", "PDFCanvas_1D2D_Hits_Layers.pdf");

    Hits_Canvas_->Update();
    Hits_Canvas_->SetLogy(0);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(1);
    WritePrintHistogram(Hits_Canvas_, ParticleOrigins_2D_.at(hitLayers.at(l)),"colz", "PDFCanvas_1D2D_Hits_Layers.pdf");

    Hits_Canvas_->Clear();
    gStyle->SetStatX(0.87);
    gROOT->ForceStyle();
    Hits_Canvas_->SetLogy(0);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(0);
    WritePrintHistogram(Hits_Canvas_, Hits_PerLayer_.at(hitLayers.at(l)),"", "PDFCanvas_1D2D_Hits_Layers.pdf");

    Hits_Canvas_->Update();
    Hits_Canvas_->SetLogy(1);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(0);
    WritePrintHistogram(Hits_Canvas_, Hits_Histo_.at(hitLayers.at(l)),"", "PDFCanvas_1D2D_Hits_Layers.pdf");

    Hits_Canvas_->Update();
    Hits_Canvas_->SetLogy(1);
    Hits_Canvas_->SetLogx(0);
    Hits_Canvas_->SetLogz(0);
    WritePrintHistogram(Hits_Canvas_, Hits_Energy_Histo_.at(hitLayers.at(l)),"", "PDFCanvas_1D2D_Hits_Layers.pdf");

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
    int n = end_of_range - first_layer_to_be_compared + 1;//Otherwise the number of layers are wrong
    float layer_numbers[n];
    float layer_numbers_Errors[n];
    std::fill(layer_numbers_Errors,layer_numbers_Errors + n, 0);//Fill layer_number_Errors array with zeros
    float MeanHits[n];
    float MeanHitErrors[n];
    for(size_t i = 0; i <= n; ++i){
      layer_numbers[i] = first_layer_to_be_compared + i;
      MeanHits[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetMean(1);
      MeanHitErrors[i] = Hits_PerLayer_.at(first_layer_to_be_compared + i)->GetRMS(1);
    }

    TGraphErrors* MeanHits_PerLayer = new TGraphErrors(n, layer_numbers, MeanHits, layer_numbers_Errors, MeanHitErrors);
    MeanHits_PerLayer->SetName("MeanHits");
    MeanHits_PerLayer->SetTitle("Mean hits for different layers; Layer number; Mean hits per bunch crossing");
    MeanHits_PerLayer->SetMarkerColor(kViolet);
    MeanHits_PerLayer->SetMarkerStyle(21);
    MeanHits_PerLayer->GetXaxis()->SetRangeUser(first_layer_to_be_compared-0.5, end_of_range+0.5);
    MeanHits_PerLayer->GetXaxis()->CenterTitle();

    TCanvas* PDF_Canvas_Hits_CompareLayers = new TCanvas(); 
    PDF_Canvas_Hits_CompareLayers->Print("PDFCanvas_Hits_CompareLayers.pdf[");

    if (std::find(hitLayers.begin(), hitLayers.end(), first_layer_to_be_compared) != hitLayers.end()
        && std::find(hitLayers.begin(), hitLayers.end(), last_layer_to_be_compared) != hitLayers.end()) {

      Hits_Canvas_->cd();
      Hits_Canvas_->Clear();
      Hits_Canvas_->SetLogy(0);
      WritePrintComparedHistogram(Hit_Canvas_, MeanHits_PerLayer, first_layer_to_be_compared, end_of_range, "AP", "PDFCanvas_Hits_CompareLayers.pdf");

      Hits_Canvas_->Clear();
      gStyle->SetStatX(0.87);
      Hits_Canvas_->SetLogy(1);
      stringstream new_histo_perlayer_title;
      new_histo_perlayer_title << "Hits per layer for " << subdetector_name << " layers "
        << first_layer_to_be_compared << " - " << end_of_range;
      WritePrintComparedHistogram(Hit_Canvas_, Hits_PerLayer_,new_histo_perlayer_title.str(), first_layer_to_be_compared, end_of_range, "", "PDFCanvas_Hits_CompareLayers.pdf");

      Hits_Canvas_->Clear();
      gStyle->SetStatX(0.87);
      Hits_Canvas_->SetLogy(1);
      stringstream new_histo_title;
      new_histo_title << "Hit occupancy per cell for " << subdetector_name << " layers "
        << first_layer_to_be_compared << " - " << end_of_range;
      WritePrintComparedHistogram(Hit_Canvas_, Hits_Histo_,new_histo_title.str(), first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");

      Hits_Canvas_->Clear();
      gStyle->SetStatX(0.87);
      Hits_Canvas_->SetLogy(1);
      stringstream new_Energy_histo_title;
      new_Energy_histo_title << "Deposited hit energy for " << subdetector_name << " layers "
        << first_layer_to_be_compared << " - " << end_of_range;
      WritePrintComparedHistogram(Hit_Canvas_, Hits_Energy_Histo_,new_Energy_histo_title.str(), first_layer_to_be_compared, end_of_range, true, "", "PDFCanvas_Hits_CompareLayers.pdf");

    }//End of if-to-be-compared loop
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
  stringstream FilesCanvasName_eps, FilesCanvasName_C;

  Files_Canvas->cd();

  gStyle->SetOptStat(1);
  //gStyle->SetOptStat(111111);

  Files_Canvas->Clear();
  Files_Canvas->SetLogy(0);
  WritePrintHistogram(Files_Canvas_,ParticlesVSEvent, "", "PDFCanvas_ParticlesHits_perFile.pdf");

  Files_Canvas->Clear();
  Files_Canvas->SetLogy(0);
  WritePrintHistogram(Files_Canvas_,Particles,"", "PDFCanvas_ParticlesHits_perFile.pdf");

  Files_Canvas->Clear();
  Files_Canvas->SetLogy(1);
  WritePrintHistogram(Files_Canvas_,Hits,"", "PDFCanvas_ParticlesHits_perFile.pdf");

  PDF_Canvas_ParticlesHits_per_File->Print("PDFCanvas_ParticlesHits_perFile.pdf]");

  delete PDF_Canvas_ParticlesHits_per_File;
  delete Files_Canvas;

} //End of function DrawingMacro 
void WritePrintComparedHistogram(TCanvas* Canvas_, std::vector<TH1*> Histos_, std::string new_histo_title, int firstlayer, int lastlayer, bool Normalizing, std::string drawingoption, std::string PDFName){
  Canvas_->Update();
  Histos_.at(firstlayer)->SetTitle(new_histo_title.c_str());
  Histos_.at(firstlayer)->SetLineColor(2);
  Histos_.at(firstlayer)->SetMarkerStyle(20);
  Histos_.at(firstlayer)->SetMarkerColor(2);
  if (Normalizing) Histos_.at(firstlayer)->GetYaxis()->SetTitle("Normalized entries");
  if (Normalizing) Histos_.at(firstlayer)->Scale(1.0/Histos_.at(firstlayer)->Integral());
  Histos_.at(firstlayer)->Draw(drawingoption.c_str());
  TPaveStats *st1 = (TPaveStats*) Histos_.at(firstlayer)->FindObject("stats");
  double statboxsize = st1->GetY2NDC() - st1->GetY1NDC();
  st1->SetTextColor(2);
  st1->SetY1NDC(0.78);
  st1->SetY2NDC(0.78 + statboxsize);

  for (signed int l = firstlayer + 1; l <= lastlayer; ++l) {

    if (std::find(hitLayers.begin(), hitLayers.end(), l) == hitLayers.end()) {
      // If one layer has no hits, it isn't mentioned in the vector hitlayers.
      //This layer is then skipped.
      continue;
    }

    Histos_.at(l)->SetMarkerStyle(l+20);
    if(l<3){
      Histos_.at(l)->SetLineColor(l+2);
      Histos_.at(l)->SetMarkerColor(l+2);
    }
    else{
      Histos_.at(l)->SetLineColor(l+3);
      Histos_.at(l)->SetMarkerColor(l+3);
    }
    if (Normalizing) Histos_.at(l)->GetYaxis()->SetTitle("Normalized entries");
    if (Normalizing) Histos_.at(l)->Scale(1.0/Histos_.at(l)->Integral());
    Histos_.at(l)->Draw("sames");
    TPaveStats *st = (TPaveStats*) Histos_.at(l)->FindObject("stats");
    if(l<3) st->SetTextColor(l+2);
    else st->SetTextColor(l+3);
    st->SetY2NDC(((TPaveStats*) Histos_.at(l - 1)->FindObject("stats"))->GetY1NDC()); //new x start position
    st->SetY1NDC(st->GetY2NDC() - statboxsize); //new x end position
  } 
  Canvas_->Write();

  stringstream CanvasName_eps, CanvasName_C;
  CanvasName_eps << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".eps";
  CanvasName_C << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".C";

  Canvas_->Print(CanvasName_eps.str().c_str());
  Canvas_->Print(CanvasName_C.str().c_str());
  Canvas_->Print(PDFName.c_str());
}
void WritePrintComparedHistogram(TCanvas* Canvas_, TH1D* Histo, int firstlayer, int lastlayer, std::string drawingoption, std::string PDFName){
  Canvas_->Update();
  Histo->Draw(drawingoption.c_str());
  Canvas_->Write();

  stringstream CanvasName_eps, CanvasName_C;
      CanvasName_eps << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".eps";
      CanvasName_C << Canvas_->GetName() << "_" << Histo->GetName() << "_" << firstlayer << "-" << lastlayer << ".C";

  Canvas_->Print(CanvasName_eps.str().c_str());
  Canvas_->Print(CanvasName_C.str().c_str());
  Canvas_->Print(PDFName.c_str());
}
void WritePrintHistogram(TCanvas* Canvas_, TH1D* Histos_, std::string drawingoption, std::string PDFName){
  Canvas_->Update();
  Canvas_->SetRightMargin(0.15);
  Histos_->Draw(drawingoption.c_str());
  Canvas_->Write();

  stringstream CanvasName_eps, CanvasName_C;
  CanvasName_eps << Canvas_->GetName() << "_" << Histos_->GetName()<< ".eps";
  CanvasName_C << Canvas_->GetName() << "_" << Histos_->GetName()<< ".C";

  Canvas_->Print(CanvasName_eps.str().c_str());
  Canvas_->Print(CanvasName_C.str().c_str());
  Canvas_->Print(PDFName.c_str());
}

void Fill_Histogram_from_Map (map<std::pair<int, int>, vector<float> > HitMap, vector<TH1*> *Hits, int weight) {
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
}

TTree* Get_TTree(TFile* inputfile, string subdetector_name) {
  stringstream temp;
  temp << "Tree_" << subdetector_name;

  TTree* Tree = nullptr;
  inputfile->GetObject(temp.str().c_str(), Tree);
  if (!Tree) {
    throw std::exception();
  }

  return Tree;
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
InitializeCellIDClass(std::string SubdetectorName, int id0, int id1){
  if (id0 >= 0 && id1 >= 0) SubdetectorCells = new CellID64bits(id0, id1);
  if (id0 >= 0 && id1 < 0) {
    if (SubdetectorName == std::string("SiVertexBarrel") || 
        SubdetectorName == std::string("SiVertexEndcap") || 
        SubdetectorName == std::string("SiTrackerForward")) { 
      SubdetectorCells = new CellID58bits(id0);
    }
    if (SubdetectorName == std::string("SiTrackerBarrel") || 
        SubdetectorName == std::string("SiTrackerEndcap")) { 
      SubdetectorCells = new CellID54bits(id0);
    }
    else {
      std::cerr << "This subdetector name is unkown. The CellID classes CellID58bits or CellID54bits could not be inititalized!" << std::endl;
      terminate();
    }
    if (id0 < 0 && id1 < 0) {
      std::cerr << "The cell ids are not valid! Initializing the CellID class is not possible!" << std::endl;
      terminate();
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
void Setup_BinningArrays(std::vector<Subdetector*> * SubDetectors, 
    std::vector<float> *axis_range_plot_1D, 
    std::vector<float> *axis_range_plot_2D, 
    std::vector<float> *axis_range_plot_3D,
    std::vector<float> *axis_range_plot_energy_1D) {

  if (SubDetectors.size() == 1) {

    std::vector<float> temp1D = { SubDetectors.at(0).GetROOTHisto_binning1D().at(0), SubDetectors.at(0).GetROOTHisto_binning1D().at(1),
      SubDetectors.at(0).GetROOTHisto_binning1D().at(2) };
    *axis_range_plot_1D = temp1D;

    std::vector<float> tempenergy1D = { 
      SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(0),
      SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(1), SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(2) };
    *axis_range_plot_energy_1D = tempenergy1D;

    std::vector<float> temp2D = { 
      SubDetectors.at(0).GetROOTHisto_binning2D().at(0), SubDetectors.at(0).GetROOTHisto_binning2D().at(1),
      SubDetectors.at(0).GetROOTHisto_binning2D().at(2), SubDetectors.at(0).GetROOTHisto_binning2D().at(3),
      SubDetectors.at(0).GetROOTHisto_binning2D().at(4), SubDetectors.at(0).GetROOTHisto_binning2D().at(5)};
    *axis_ranges_plot_2D = temp2D;

    std::vector<float> temp3D = { 
      SubDetectors.at(0).GetROOTHisto_binning3D().at(0), SubDetectors.at(0).GetROOTHisto_binning3D().at(1),
      SubDetectors.at(0).GetROOTHisto_binning3D().at(2), SubDetectors.at(0).GetROOTHisto_binning3D().at(3),
      SubDetectors.at(0).GetROOTHisto_binning3D().at(4), SubDetectors.at(0).GetROOTHisto_binning3D().at(5),
      SubDetectors.at(0).GetROOTHisto_binning3D().at(6), SubDetectors.at(0).GetROOTHisto_binning3D().at(7),
      SubDetectors.at(0).GetROOTHisto_binning3D().at(8) };
    *axis_range_plot_3D = temp3D;

  } else {
    float maxEnergy1D = 0;
    float minEnergy1D = 0;
    float binsEnergy1D = 0;

    int max1D = 0;
    int min1D = 0;
    int bins1D = 0;

    int xmax2D = 0;
    int xmin2D = 0;
    int xbins2D = 0;
    int ymax2D = 0;
    int ymin2D = 0;
    int ybins2D = 0;

    int xmax3D = 0;
    int xmin3D = 0;
    int xbins3D = 0;
    int ymax3D = 0;
    int ymin3D = 0;
    int ybins3D = 0;
    int zmax3D = 0;
    int zmin3D = 0;
    int zbins3D = 0;

    for (int s = 0; s < SubDetectors.size(); ++s) {

      minEnergy1D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning1D().at(1),minEnergy1D);
      maxEnergy1D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning1D().at(2),maxEnergy1D);
      binsEnergy1D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning1D().at(0),binsEnergy1D);

      min1D  = FindMin(SubDetectors.at(s).GetROOTHisto_binning1D().at(1),min1D);
      max1D  = FindMax(SubDetectors.at(s).GetROOTHisto_binning1D().at(2),max1D);
      bins1D = FindMax(SubDetectors.at(s).GetROOTHisto_binning1D().at(0),bins1D);

      xmin2D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(1),xmin2D);
      xmax2D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(2),xmax2D);
      xbins2D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(0),xbins2D);

      ymin2D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(4),ymin2D);
      ymax2D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(5),ymax2D);
      ybins2D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning2D().at(3),ybins2D);

      zmin3D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(1),zmin3D);
      zmax3D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(2),zmax3D);
      zbins3D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(0),zbins3D);

      xmin3D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(4),xmin3D);
      xmax3D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(5),xmax3D);
      xbins3D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(3),xbins3D);

      ymin3D  = FindMin(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(7),ymin3D);
      ymax3D  = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(8),ymax3D);
      ybins3D = FindMax(SubDetectors.at(s).GetROOTEnergyHisto_binning3D().at(6),ybins3D);

    }
    std::vector<float> tempEnergy1D = { binsEnergy1D, minEnergy1D, maxEnergy1D };
    *axis_range_plot_energy_1D = tempEnergy1D;
    std::vector<float> temp1D = { bins1D, min1D, max1D };
    *axis_range_plot_1D = temp1D;
    std::vector<float> temp2D = { xbins, xmin, xmax, ybins, ymin, ymax };
    *axis_range_plot_2D = temp2D;
    std::vector<float> temp3D = { zbins, zmin, zmax, xbins, xmin, xmax, ybins, ymin, ymax };
    *axis_range_plot_3D = temp3D;
  }
}
void SetupHistoTitles(std::string subdetector_name, std::stringstream layer,
    std::stringstream* histo_name1D,              std::stringstream* histo_title1D,
    std::stringstream* histo_name2D,              std::stringstream* histo_title2D,
    std::stringstream* histo_name3D,              std::stringstream* histo_title3D,
    std::stringstream* energyhisto_name1D,        std::stringstream* energyhisto_title1D,
    std::stringstream* energyhisto_name2D,        std::stringstream* energyhisto_title2D,
    std::stringstream* energyhisto_name3D,        std::stringstream* energyhisto_title3D,
    std::stringstream* hitsperlayerhisto_name,    std::stringstream* hitsperlayerhisto_title,
    std::stringstream* particleoriginshisto_name, std::stringstream* particleoriginshisto_title){

  *(histo_name1D) << "Hits_" << subdetector_name << "_Layer_" << layer;
  *(histo_title1D) << "Hit occupancy per cell for " << subdetector_name << " layer " << layer;
  *(histo_name2D) << "Hits_2D_" << subdetector_name << "_Layer_" << layer;
  *(histo_title2D) << "HitMap for " << subdetector_name << " layer " << layer;
  *(histo_name3D) << "Hits_3D_" << subdetector_name << "_Layer_" << layer;
  *(histo_title3D) << "HitMap for " << subdetector_name << " layer " << layer;
  *(energyhisto_name1D) << "Energy_" << subdetector_name << "_Layer_" << layer;
  *(energyhisto_title1D) << "Deposited hit energy for " << subdetector_name << " layer " << layer;
  *(energyhisto_name2D) << "Hits_Energy_2D_" << subdetector_name << "_Layer_" << layer;
  *(energyhisto_title2D) << "HitMap with the average hit energy per bin for " << subdetector_name << " layer " << layer;
  *(energyhisto_name3D) << "Hits_Energy_3D_" << subdetector_name << "_Layer_" << layer;
  *(energyhisto_title3D) << "HitMap with the hit energy for " << subdetector_name << " layer " << layer;

  *(hitsperlayerhisto_name) << "HitsPerLayer_" << subdetector_name << "_Layer_" << layer;
  *(hitsperlayerhisto_title) << "Hits for " << subdetector_name << " layer " << layer;
  *(particleoriginshisto_name) << "ParticleOrigins_" << subdetector_name << "_Layer_" << layer;
  *(particleoriginshisto_title) << "Origins of pair background particles for " << subdetector_name << " layer " << layer;
}
void Setup_ParticleOriginsHisto(std::vector<TH1*> HistoVector, std::vector<float> axis_range_plot, stringstream histo_name, stringstream histo_title, string coodinate_system) {
  if(axis_range_plot.size()<9) {
    std::cerr << "The plot of the particle origins can only be plotted with the vector of the 3 dimensional plot axes." << std::endl;
    terminate();
  }
  std::vector<float> axis_vector;
  if (coordinate_system == std::string("kartesian")) axis_vector = axis_range_plot;
  if (coordinate_system == std::string("cylindrical")){
    float rmax = sqrt(pow(axis_range_plot.at(5),2)+pow(axis_range_plot.at(8),2));
    float rmin = 0.;
    float rrange = rmax - rmin;
    float phimax = arcos(axis_range_plot.at(5)/r);
    float zmax = axis_range_plot.at(2);
    float zmin = -zmax;
    float zrange = rmax - zmin;
    axis_vector = {0,0,0,zrange/4,zmin,zmax,rrange/4,rmin,rmax};
  }  
  if (coordinate_system == std::string("spherical")){
    float rmax = sqrt(pow(axis_ranges_plot.at(5),2)+pow(axis_ranges_plot.at(8),2)+pow(axis_ranges_plot.at(2),2));
    float rmin = 0.;
    float rrange = rmax - rmin;
    float zmax = axis_range_plot.at(2);
    float zmin = -zmax;
    float zrange = rmax - zmin;
    float thetamax = arcos(axis_range_plot.at(2)/r);
    float thetamin = 0;
    float thetarange = thetamax - thetamin;
    float phimax = arcos(axis_range_plot.at(5)/(r*sin(theta)));
    axis_vector = {0,0,0,zrange/4,zmin,zmax,thetarange/4,thetamin,thetamax};
  }  
  HistoVector.emplace_back(
      new TH2D(histo_name.str().c_str(), histo_title.str().c_str(), axis_vector.at(3),
        axis_vector.at(4), axis_vector.at(5), axis_vector.at(6), axis_vector.at(7),
        axis_vector.at(8)));
  HistoVector.at(layer)->SetContour(100);
}
void Setup_Histo(std::vector<TH1*> HistoVector, std::vector<float> axis_range_plot, stringstream histo_name, stringstream histo_title) {
  if (axis_range_plot.size() == 3){

    HistoVector.emplace_back(
        new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot.at(0), axis_range_plot.at(1),
          axis_range_plot.at(2)));
    HistoVector.at(layer)->SetMinimum(0.1);
    HistoVector.at(layer)->SetLineColor(kViolet);
  }

  if (axis_range_plot.size() == 6){
    HistoVector.emplace_back(
        new TH2D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot.at(0),
          axis_range_plot.at(1), axis_range_plot.at(2), axis_range_plot.at(3), axis_range_plot.at(4),
          axis_range_plot.at(5)));
    HistoVector.at(layer)->SetContour(100);
  }    

  if (axis_range_plot.size() == 9){
    HistoVector.emplace_back(
        new TH3D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot.at(0),
          axis_range_plot.at(1), axis_range_plot.at(2), axis_range_plot.at(3), axis_range_plot.at(4),
          axis_range_plot.at(5), axis_range_plot.at(6), axis_range_plot.at(7), axis_range_plot.at(8)));
    HistoVector.at(layer)->GetZaxis()->CenterTitle();
  }    
  HistoVector.at(layer)->GetXaxis()->CenterTitle();
  HistoVector.at(layer)->GetYaxis()->CenterTitle();
}

#endif /*DRAWINGHISTOS*/
