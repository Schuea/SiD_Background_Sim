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
  std::array<int, 6> axis_range_plot_2D = { }; //xbins, xlow, xup, ybins, ylow, yup
  std::array<int, 9> axis_range_plot_3D = { }; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup
  std::array<float, 3> axis_range_plot_energy_1D = { }; //xbins, xlow, xup

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
  long CellIDkey = 0.;

  std::map<long, vector<int>> HitsPerLayerMap;

  //Find the largest number of layers from all the subdetectors that are to be plotted
  for (int s = 0; s < SubDetectors.size(); ++s) {
    MaxNumberLayers = FindMax(SubDetectors.at(s).Number_layers, MaxNumberLayers); 
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

  SetupHistoTitles(histo_name1D, histo_title1D,
                histo_name2D, histo_title2D,
                histo_name3D, histo_title3D,
                energyhisto_name1D, energyhisto_title1D,
                energyhisto_name2D, energyhisto_title2D,
                energyhisto_name3D, energyhisto_title3D,
                hitsperlayerhisto_name, hitsperlayerhisto_title,
                particleoriginshisto_name, particleoriginshisto_title);

  Setup_Histos1D_HitsPerLayer(axis_range_plot_1D, subdetector_name, l);
    Setup_ParticleOrigins_2D(axis_ranges_plot, subdetector_name, l);
    Setup_Histos1D(Histos_1D, axis_range_plot_1D, histo_name, histo_title);
    Setup_Histos2D(axis_ranges_plot, subdetector_name, l);
    Setup_Histos3D(axis_ranges_plot, subdetector_name, l);
    Setup_EnergyHistos1D(axis_range_plot_energy_1D, subdetector_name, l);
    Setup_EnergyHistos2D(axis_ranges_plot, subdetector_name, l);
    Setup_EnergyHistos3D(axis_ranges_plot, subdetector_name, l);
  }

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
void Setup_BinningArrays(std::vector<Subdetector*> * SubDetectors, 
    std::array<int, 3> *axis_range_plot_1D, 
    std::array<int, 6> *axis_range_plot_2D, 
    std::array<int, 9> *axis_range_plot_3D,
    std::array<float, 3> *axis_range_plot_energy_1D) {

  if (SubDetectors.size() == 1) {

    std::array<int, 3> temp1D = { SubDetectors.at(0).GetROOTHisto_binning1D().at(0), SubDetectors.at(0).GetROOTHisto_binning1D().at(1),
      SubDetectors.at(0).GetROOTHisto_binning1D().at(2) };
    *axis_range_plot_1D = temp1D;
    
    std::array<float, 3> tempenergy1D = { 
      SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(0),
      SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(1), SubDetectors.at(0).GetROOTEnergyHisto_binning1D().at(2) };
    *axis_range_plot_energy_1D = tempenergy1D;
    
    std::array<int, 6> temp2D = { 
      SubDetectors.at(0).GetROOTHisto_binning2D().at(0), SubDetectors.at(0).GetROOTHisto_binning2D().at(1),
      SubDetectors.at(0).GetROOTHisto_binning2D().at(2), SubDetectors.at(0).GetROOTHisto_binning2D().at(3),
      SubDetectors.at(0).GetROOTHisto_binning2D().at(4), SubDetectors.at(0).GetROOTHisto_binning2D().at(5)};
    *axis_ranges_plot_2D = temp2D;

    std::array<int, 9> temp3D = { 
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
    std::array<float, 3> tempEnergy1D = { binsEnergy1D, minEnergy1D, maxEnergy1D };
    *axis_range_plot_energy_1D = tempEnergy1D;
    std::array<int, 3> temp1D = { bins1D, min1D, max1D };
    *axis_range_plot_1D = temp1D;
    std::array<int, 6> temp2D = { xbins, xmin, xmax, ybins, ymin, ymax };
    *axis_range_plot_2D = temp2D;
    std::array<int, 9> temp3D = { zbins, zmin, zmax, xbins, xmin, xmax, ybins, ymin, ymax };
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
void Setup_Histo(std::vector<TH1*> HistoVector, std::vector<float> axis_range_plot, stringstream histo_name, stringstream histo_title, string coodinate_system) {
  if (axis_range_plot.size() == 3){
    
    HistoVector.emplace_back(
          new TH1D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot.at(0), axis_range_plot.at(1),
            axis_range_plot.at(2)));
    HistoVector.at(layer)->SetMinimum(0.1);
    HistoVector.at(layer)->SetLineColor(kViolet);
  }

  if (axis_range_plot.size() == 6){
      HistoVector.emplace_back(
          new TH2D(histo_name.str().c_str(), histo_title.str().c_str(), axis_range_plot[3],
            axis_range_plot[4], axis_range_plot[5], axis_range_plot[6], axis_range_plot[7],
            axis_range_plot[8]));
    HistoVector.at(layer)->SetContour(100);
  }    

  if (axis_range_plot.size() == 9){
    std::vector<float> axis_vector;
    if (coordinate_system == std::string("kartesian")) axis_vector = axis_range_plot;
    if (coordinate_system == std::string("cylindrical")){
        float r = sqrt(pow(axis_range_plot[5],2)+pow(axis_range_plot[8],2));
        float phi = arcos(axis_range_plot[5]/r);
        float z = axis_range_plot[2];
    }  
    if (coordinate_system == std::string("spherical")){
        float r = sqrt(pow(axis_ranges_plot[5],2)+pow(axis_ranges_plot[8],2)+pow(axis_ranges_plot[2],2));
        float theta = arcos(axis_range_plot[2]/r);
        float phi = arcos(axis_range_plot[5]/(r*sin(theta)));
      }  
      HistoVector.emplace_back(
          new TH3D(histo_name.str().c_str(), histo_title.str().c_str(), axis_vector[0],
            axis_vector[1], axis_vector[2], axis_vector[3], axis_vector[4],
            axis_vector[5], axis_vector[6], axis_vector[7], axis_vector[8]));
    HistoVector.at(layer)->GetZaxis()->CenterTitle();
  }    
  HistoVector.at(layer)->GetXaxis()->CenterTitle();
  HistoVector.at(layer)->GetYaxis()->CenterTitle();
}

#endif /*DRAWINGHISTOS*/
