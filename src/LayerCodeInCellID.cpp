#include "LayerCodeInCellID.h"

int LayerCodeInCellID::FindLayer(std::string const CellID_, int const StarBin_layers, int const LengthBin_layers){

  std::stringstream LayerID;

  for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers; ++i) {
    LayerID << CellID_.at(i);
  }

  std::bitset<64> LayerIDbit (LayerID);
  int Layer = 0;
  Layer = LayerIDbit.to_ulong();

  return Layer;
}

int LayerCodeInCellID::GetLayer() const {
  if (Layer >= 0){
    return FindLayer(CellID_, StarBin_layers, LengthBin_layers);
  }
  else throw std::runtime_error("You didn't initialize the class with an ID!");
}				

int LayerCodeInCellID::ObtainLayerfromCellID(int const cellid,  int const StarBin_layers, int const LengthBin_layers) const{
  std::bitset<64> cellidbit = std::bitset<std::numeric_limits<unsigned int>::digits>(cellid);
  return FindLayer(cellidbit, StarBin_layers, LengthBin_layers);
}

