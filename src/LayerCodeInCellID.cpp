#include "LayerCodeInCellID.h"

int LayerCodeInCellID::FindLayer(int ID_) {

  std::bitset<32> ID_bit;
  ID_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID_);

  for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers;
      ++i) {
    if (LengthBin_layers == 6)
      LayerID6[i - StartBin_layers] = ID_bit[i];
    if (LengthBin_layers == 8)
      LayerID8[i - StartBin_layers] = ID_bit[i];
  }

  int Layer = 0;
  if (LengthBin_layers == 6)
    Layer = LayerID6.to_ulong();
  if (LengthBin_layers == 8)
    Layer = LayerID6.to_ulong();

  return Layer;
}

int LayerCodeInCellID::FindLayer(long CellID_){
  std::bitset<64> ID_bit;
  ID_bit = LongToBitsetConversion(CellID);

  for (int i = StartBin_layers; i <= StartBin_layers + LengthBin_layers;
      ++i) {
    if (LengthBin_layers == 6)
      LayerID6[i - StartBin_layers] = ID_bit[i];
    if (LengthBin_layers == 8)
      LayerID8[i - StartBin_layers] = ID_bit[i];
  }

  //std::bitset<6> which_one (std::string("111111"));
  //std::bitset<6> Layer = Layer_bit & which_one;
  //int Layer = (ID0_bit >> 13)&which_one;
  //int Layer = (ID0_bit >> 13)&0x3f;
  int Layer = 0;
  if (LengthBin_layers == 6)
    Layer = LayerID6.to_ulong();
  if (LengthBin_layers == 8)
    Layer = LayerID6.to_ulong();

  //std::cout << "Layer_bit = " << Layer_bit << std::endl;
  //std::cout << "Layer_bit & which_one = " << Layer << std::endl;
  //std::cout << "Layer = " << Layer << std::endl;
  //std::cout << "Layer (CellID) = " << Layer << std::endl;
  return Layer;
}
