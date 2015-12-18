#ifndef LAYERCODEINCELLID_H_
#define LAYERCODEINCELLID_H_

#include <bitset>

class LayerCodeInCellID {

  public:
    LayerCodeInCellID(std::string const CellID, int const LayerInfoStart, int const LayerInfoLength) : CellID_(CellID), StartBin_layers(LayerInfoStart), LengthBin_layers(LayerInfoLength), Layer(-1) {}

    virtual ~LayerCodeInCellID() {}

    int GetLayer();				
    int ObtainLayerfromCellID(int const cellid,  int const StarBin_layers, int const LengthBin_layers);

  private:
    LayerCodeInCellID(): CellID_(""), StartBin_layers(-1), LengthBin_layers(-1), Layer(-1) {}
    LayerCodeInCellID(int ID): CellID_(""), StartBin_layers(-1), LengthBin_layers(-1), Layer(-1) {}
    LayerCodeInCellID(long ID): CellID_(""), StartBin_layers(-1), LengthBin_layers(-1), Layer(-1) {}
    LayerCodeInCellID(float ID): CellID_(""), StartBin_layers(-1), LengthBin_layers(-1), Layer(-1) {}
    LayerCodeInCellID(double ID): CellID_(""), StartBin_layers(-1), LengthBin_layers(-1), Layer(-1) {}

    int Layer;
    int StartBin_layers;
    int LengthBin_layers;

    std::string CellID_;

    int FindLayer(std::string const CellID_, int const StarBin_layers, int const LengthBin_layers);
};
#endif /*LAYERCODEINCELLID_H_*/
