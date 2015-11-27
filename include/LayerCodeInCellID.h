#ifndef LAYERCODEINCELLID_H_
#define LAYERCODEINCELLID_H_

#include <bitset>

class LayerCodeInCellID {

				public:
								LayerCodeInCellID() {}
								LayerCodeInCellID(int ID) : ID_(ID) {}
								LayerCodeInCellID(long ID) : CellID_(ID) {}
								LayerCodeInCellID(float ID) : ID_(-1) {}
								LayerCodeInCellID(double ID) : ID_(-1) {}
								LayerCodeInCellID(std::bitset<32> ID) : ID_bit(ID) {}
								LayerCodeInCellID(std::bitset<64> CellID) : CellID_bit(CellID){}

								virtual ~LayerCodeInCellID() {}


								int GetLayer() const {
												if (Layer >= 0){
																return Layer;
												}
												else throw std::runtime_error("You didn't initialize the class with an ID!");
								}				

				private:
								int Layer;
								int StartBin_layers;
								int LengthBin_layers;
								std::bitset<6> LayerID6;
								std::bitset<8> LayerID8;

								int ID_;
								int CellID_;
								std::bitset<32> ID_bit;
								std::bitset<64> CellID_bit;

								int FindLayer(int ID_);
								int FindLayer(int CellID_);
};
#endif /*LAYERCODEINCELLID_H_*/
