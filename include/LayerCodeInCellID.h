#ifndef LAYERCODEINCELLID_H_
#define LAYERCODEINCELLID_H_

#include <bitset>

class LayerCodeInCellID {

  public:
    LayerCodeInCellID(std::string const CellID, int const LayerInfoStart, int const LayerInfoLength) : CellID_(CellID), StartBin_layers(LayerInfoStart), LengthBin_layers(LayerInfoLength) {}

    virtual ~LayerCodeInCellID() {}

    int GetLayer() const {
      if (Layer >= 0){
        return FindLayer(CellID_, StarBin_layers, LengthBin_layers);
      }
      else throw std::runtime_error("You didn't initialize the class with an ID!");
    }				
    int ObtainLayerfromCellID(int const cellid,  int const StarBin_layers, int const LengthBin_layers) const{
	std::bitset<64> cellidbit = std::bitset<std::numeric_limits<unsigned int>::digits>(cellid);
	return FindLayer(cellidbit, StarBin_layers, LengthBin_layers);
    }

  private:
    LayerCodeInCellID() {}
    LayerCodeInCellID(int ID) {}
    LayerCodeInCellID(long ID) {}
    LayerCodeInCellID(float ID) {}
    LayerCodeInCellID(double ID) {}

    int Layer;
    int StartBin_layers;
    int LengthBin_layers;

    std::string CellID_;

    int FindLayer(std::string const CellID_, int const StarBin_layers, int const LengthBin_layers);
};
#endif /*LAYERCODEINCELLID_H_*/
