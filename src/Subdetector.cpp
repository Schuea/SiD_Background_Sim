#include "Subdetector.h"

void EcalBarrel:Initialize(){
  LengthLayerBin_ = 6;
  StartLayerBin_ = 13;
  NumberOfLayers_ = 31;

  ROOTEnergyHisto_binning = {40., 0., 0.01};
  ROOTHisto_binning1D = {10, 0, 10};                                           
  ROOTHisto_binning2D = {70, -1500, 1500, 70, -1500, 1500};                    
  ROOTHisto_binning3D = {100, -2000, 2000, 100, -1500, 1500, 100, -1500, 1500};
}								
void EcalEndcap:Initialize(){
  LengthLayerBin_ = 6;
  StartLayerBin_ = 13;
  NumberOfLayers_ = 31;

  ROOTEnergyHisto_binning = {40., 0., 0.01};
  ROOTHisto_binning1D = {15, 0, 15};                                           
  ROOTHisto_binning2D = {70, -1500, 1500, 70, -1500, 1500};                    
  ROOTHisto_binning3D = {70, -2000, 2000, 70, -1500, 1500, 70, -1500, 1500};
}								
void HcalBarrel:Initialize(){
  LengthLayerBin_ = 6;
  StartLayerBin_ = 13;
  NumberOfLayers_ = 40;

  ROOTEnergyHisto_binning = {40., 0., 0.00002};
  ROOTHisto_binning1D = {6, 0, 6};                                           
  ROOTHisto_binning2D = {70, -2500, 2500, 70, -2500, 2500};                    
  ROOTHisto_binning3D = {100, -3100, 3100, 70, -2500, 2500, 70, -2500, 2500};
}								
void HcalEndcap:Initialize(){
  LengthLayerBin_ = 8;
  StartLayerBin_ = 13;
  NumberOfLayers_ = 45;

  ROOTEnergyHisto_binning = {250., 0., 0.00045};
  ROOTHisto_binning1D = {50, 0, 50};                                           
  ROOTHisto_binning2D = {70, -1500, 1500, 70, -1500, 1500};                    
  ROOTHisto_binning3D = {100, -3100, 3100, 70, -1500, 1500, 70, -1500, 1500};
}								
void MuonBarrel:Initialize(){
  LengthLayerBin_ = 6;
  StartLayerBin_ = 15;
  NumberOfLayers_ = 11;

  ROOTEnergyHisto_binning = {25., 0., 0.00003};
  ROOTHisto_binning1D = {10, 0, 10};                                           
  ROOTHisto_binning2D = {150, -6300, 6300, 150, -6300, 6300};                    
  ROOTHisto_binning3D = {100, -3100, 3100, 150, -6300, 6300, 150, -6300, 6300};
}								
void MuonEndcap:Initialize(){
  LengthLayerBin_ = 6;
  StartLayerBin_ = 15;
  NumberOfLayers_ = 11;

  ROOTEnergyHisto_binning = {35., 0., 0.0075};
  ROOTHisto_binning1D = {50, 0, 15000};                                           
  ROOTHisto_binning2D = {150, -6300, 6300, 150, -6300, 6300};                    
  ROOTHisto_binning3D = {150, -5700, 5700, 150, -6300, 6300, 150, -6300, 6300};
}								
void BeamCal:Initialize(){
  LengthLayerBin_ = 8;
  StartLayerBin_ = 8;
  NumberOfLayers_ = 50;

  ROOTEnergyHisto_binning = {45., 0., 2.5};
  ROOTHisto_binning1D = {150, 0, 2000000};                                           
  ROOTHisto_binning2D = {100, -140, 140, 100, -140, 140};                    
  ROOTHisto_binning3D = {150, -3200, 3200, 100, -140, 140, 100, -140, 140};
}								
void LumiCal:Initialize(){
  LengthLayerBin_ = 8;
  StartLayerBin_ = 8;
  NumberOfLayers_ = 30;

  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}								
void SiVertexEndcap:Initialize(){
  LengthLayerBin_ = 4;
  StartLayerBin_ = 9;
  NumberOfLayers_ = 4;

  //TO BE ADJUSTED:
  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}
void SiVertexBarrel:Initialize(){
  LengthLayerBin_ = 4;
  StartLayerBin_ = 9;
  NumberOfLayers_ = 5;

  //TO BE ADJUSTED:
  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}
void SiTrackerEndcap:Initialize(){
  LengthLayerBin_ = 4;
  StartLayerBin_ = 9;
  NumberOfLayers_ = 4;

  //TO BE ADJUSTED:
  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}
void SiTrackerBarrel:Initialize(){
  LengthLayerBin_ = 4;
  StartLayerBin_ = 9;
  NumberOfLayers_ = 5;

  //TO BE ADJUSTED:
  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}
void SiTrackerForward:Initialize(){
  LengthLayerBin_ = 4;
  StartLayerBin_ = 9;
  NumberOfLayers_ = 3;

  //TO BE ADJUSTED:
  ROOTEnergyHisto_binning = {40., 0., 0.045};
  ROOTHisto_binning1D = {100, 0, 210};                                           
  ROOTHisto_binning2D = {100, -230, 230, 100, -230, 230};                    
  ROOTHisto_binning3D = {150, -1900, 1900, 100, -230, 230, 100, -230, 230};
}
