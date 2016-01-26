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
#include <utility>
#include <map>

#include "FunctionsForDrawingMacro.h"
#include "CreateCellID.h"
#include "Subdetector.h"
#include "LayerCodeInCellID.h"
#include "CellHits.h"
#include "Time.h"


class DrawingHistograms{
  public:
    DrawingHistograms(std::string _outputname, std::vector<std::string> _inputnames, std::vector<std::string> _argument_subdetectors, int NUMBER_OF_FILES, int first_layer_to_be_compared, int last_layer_to_be_compared);
    void DrawingMacro();
  
  private:
    int number_of_files;
    int first_layer_to_be_compared;
    int last_layer_to_be_compared;

    bool YesNo_TrackerHistograms;
    std::string outputname;
    std::vector< std::string > inputnames;
    std::vector< std::string > argument_subdetectors;
    TFile* inputfile;
    TFile *output_rootfile;
    std::string Canvas_name;
    TCanvas *Files_Canvas_;
    TCanvas* Hits_Canvas_;
    TH1D *DeadCells;
    TH1D *Hits;
    TH1D *Particles;
    TH2D *ParticlesVSEvent;
    TF1 *gausfit_Particles;
    std::vector< TH2D* > ParticleOrigins_2D_;
    std::vector< TH1D* > Hits_PerLayer_;
    std::vector< TH1D* > Hits_Histo_;
    std::vector< TH2D* > Hits_2D_;
    std::vector< TH3D* > Hits_3D_;
    std::vector< TH1D* > Hits_Energy_Histo_;
    std::vector< TH2D* > Hits_Energy_2D_;
    std::vector< TH3D* > Hits_Energy_3D_;
    std::vector< TH1D* > Hits_Time_;
    std::vector< TH2D* > Hits_Time_rtime_2D_;
    std::vector< TH2D* > Hits_Time_ztime_2D_;
    std::vector< TH3D* > Hits_Time_3D_;

    std::vector< int > BunchNumbers_for_TotDeadCells;   
    int* TotDeadCells_x;
    int* TotDeadCells_y;
    
    float time_interval_bunchspacing;
    std::pair < int, int> Number_train_bunch;

    std::vector< float > axis_range_occupancy_plot; //xbins, xlow, xup
    std::vector< float > axis_range_plot_1D; //xbins, xlow, xup
    std::vector< float > axis_range_plot_2D; //xbins, xlow, xup, ybins, ylow, yup
    std::vector< float > axis_range_plot_3D; //zbins, zlow, zup, xbins, xlow, xup, ybins, ylow, yup
    std::vector< float > axis_range_plot_energy_1D; //xbins, xlow, xup
    std::vector< float > axis_range_plot_time; //timebins, timelow, timeup
    std::vector< float > axis_range_plot_rtime_2D; //timebins, timelow, timeup, rbins, rlow, rup
    std::vector< float > axis_range_plot_ztime_2D; //timebins, timelow, timeup, zbins, zlow, zup
    std::vector< float > axis_range_plot_time_3D; //timebins, timelow, timeup, zbins, zlow, zup, xbins, xlow, xup

    std::vector<Subdetector*> * SubDetectors;
    std::string subdetector_name;
  
    std::vector< int > hitLayers;
    int MaxNumberLayers;
 
    std::map< unsigned long, int > HitMap;  //cellid, count of all hits per cell
    std::map< int, std::vector<int> > HitsPerLayerMap;
  
    void Initialize();
    void SetupGeneralHistograms();
    void Setup_SubDetector_vector();
    void Setup_HitsPerLayerMap(int layer);
    int Find_MaxNumberLayers();
    void SetupLayerHistograms(std::string layerstring);
    std::pair< int, int > Set_train_bunch_number(int number_of_file);
    void Open_inputfile(int number_of_file);
    void Filling_Data_for_SubDetectors(int subdetector_iterator);
    void Setup_for_inputfiles(int file_iterator, int subdetector_iterator, Time & time, std::vector< CellHits* > & AllHitCounts);
    void Filling_Data_of_hits(int file_iterator, int subdetector_iterator, Data* data, CellHits* HitCount, Time & time,
        std::map< std::pair<int, int>, std::vector<float> > &HitMapEnergy2D, std::map< std::pair<int, int>, std::vector<float> > &HitMapEnergy3D);
    void Define_Data_Variables(Data* data, Time & time, float & energy, float & absolutetime, float & x, float & y, float & z, std::array< double, 3> & vertex);
    void Fill_Histo(std::vector< TH1D* > Histo, int Layerno, float variable1);
    void Fill_Histo(std::vector< TH2D* > Histo, int Layerno, float variable1, float variable2);
    void Fill_Histo(std::vector< TH3D* > Histo, int Layerno, float variable1, float variable2, float variable3);
};

#endif /*DRAWINGHISTOS*/
