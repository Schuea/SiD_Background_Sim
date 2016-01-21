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

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "FunctionsForDrawingMacro.h"
#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"
#include "CellHits.h"

class DrawingHistograms{
  public:
    DrawingHistograms(std::string _outputname, std::vector<std::string> _inputnames, std::vector<std::string> _argument_subdetectors);
    void DrawingMacro();
  private:
    std::string outputname;
    std::string Canvas_name;
    std::vector< std::string > inputnames;
    std::vector< std::string > argument_subdetectors;
    TFile *output_rootfile;
    TH2D *ParticlesVSEvent;
    TH1D *Particles;
    TF1 *gausfit_Particles;
    TH1D *DeadCells;
    TCanvas *Files_Canvas;
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
  std::vector<TH1D*> Hits_Time_;
  std::vector<TH2D*> Hits_Time_rtime_2D_;
  std::vector<TH2D*> Hits_Time_ztime_2D_;
  std::vector<TH3D*> Hits_Time_3D_;
    int first_layer_to_be_compared;
  int last_layer_to_be_compared;


    bool YesNo_TrackerHistograms;
    int number_of_files;
  
    void Setup();
};

#endif /*DRAWINGHISTOS*/
