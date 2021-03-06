#ifndef LAYERCODEINCELLID_H_
#define LAYERCODEINCELLID_H_

#include <bitset>

class LayerCodeInCellID {

  public:
    LayerCodeInCellID() {}

    virtual ~LayerCodeInCellID() {}

    int GetLayer(std::string const CellID, int const StartBin_layers, int const LengthBin_layers);
    int GetLayer(int const cellid, int const StartBin_layers, int const LengthBin_layers);
    //int GetLayer(unsigned long long const cellid, int const StartBin_layers, int const LengthBin_layers);

  private:
    //unsigned long long Layer;
    int Layer;

    int FindLayer(std::string const CellID_, int const StartBin_layers, int const LengthBin_layers);
};
#endif /*LAYERCODEINCELLID_H_*/
