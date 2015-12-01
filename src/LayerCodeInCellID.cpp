#include "LayerCodeInCellID.h"

#include <limits>
#include <stdexcept>
#include <sstream>

int LayerCodeInCellID::FindLayer(std::string const CellID_, int const StarBin_layers, int const LengthBin_layers){

  std::stringstream LayerID;

  for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers; ++i) {
    LayerID << CellID_.at(i);
  }

  std::bitset<64> LayerIDbit (LayerID.str());
  int Layer = 0;
  Layer = LayerIDbit.to_ulong();

  return Layer;
}

int LayerCodeInCellID::GetLayer(){
  if (Layer >= 0){
    return FindLayer(CellID_, StartBin_layers, LengthBin_layers);
  }
  else throw std::runtime_error("You didn't initialize the class with an ID!");
}				

int LayerCodeInCellID::ObtainLayerfromCellID(int const cellid,  int const StarBin_layers, int const LengthBin_layers){
  std::bitset<64> cellidbit(cellid);
  return FindLayer(cellidbit.to_string(), StarBin_layers, LengthBin_layers);
}

