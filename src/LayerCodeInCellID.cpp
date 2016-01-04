#include "LayerCodeInCellID.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <sstream>

int LayerCodeInCellID::FindLayer(std::string const CellID_, int const StartBin_layers, int const LengthBin_layers){

  std::stringstream LayerID;

  for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers; ++i) {
    LayerID << CellID_.at(i);
  }
  std::cout << CellID_ << std::endl;
  std::cout << LayerID.str() << std::endl;
  std::bitset<64> LayerIDbit (LayerID.str());
  int tempLayer = -1;
  tempLayer = LayerIDbit.to_ulong();
  std::cout << tempLayer << std::endl;
  return tempLayer;
}

int LayerCodeInCellID::GetLayer(std::string const CellID, 
        int const StartBin_layers, 
        int const LengthBin_layers){
  Layer = -1;
  Layer = FindLayer(CellID, StartBin_layers, LengthBin_layers);
  if (Layer >= 0){
    return Layer;
  }
  else throw std::runtime_error("You didn't initialize the class with an ID!");
}				

int LayerCodeInCellID::GetLayer(int const cellid,
        int const StartBin_layers, 
        int const LengthBin_layers){
  std::bitset<64> cellidbit(cellid);

  Layer = -1;
  Layer = FindLayer(cellidbit.to_string(), StartBin_layers, LengthBin_layers);
  if (Layer >= 0){
    return Layer;
  }
  else throw std::runtime_error("You didn't initialize the class with an ID!");
}

