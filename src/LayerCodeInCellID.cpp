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
