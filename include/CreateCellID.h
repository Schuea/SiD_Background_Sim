#ifndef CELLID_H_
#define CELLID_H_

#include <bitset>
#include <limits>
#include <stdexcept>

class CellID{

				public:
								CellID() : ID0_(-999), ID1_(-999) {}
								CellID(int ID) : ID_(ID){}
								CellID(int ID0, int ID1) : ID0_(ID0),ID1_(ID1){}

								CellID(float ID) : ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(double ID) : ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception

								CellID(float ID0, float ID1) : ID0_(-1),ID1_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(int ID0, float ID1) : ID0_(-1),ID1_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(float ID0, int ID1) : ID0_(-1),ID1_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(double ID0, double ID1) : ID0_(-1),ID1_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(int ID0, double ID1) : ID0_(-1),ID1_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
								CellID(double ID0, int ID1) : ID0_(-1),ID1_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception

								virtual ~CellID() {}
								virtual void CreateCellID();
								virtual char* GetCellID() const; 

				protected:
								int ID_;
								int ID0_;
								int ID1_;

};
class CellID64bits : public CellID {
				public:
								void CreateCellID();
								char* GetCellID() const {
												if ((ID0_ == 0 && ID1_ == 0) || ID0_ < 0 || ID1_ <0){
																throw std::runtime_error("The CellID was not set. Invalid IDs!");
												}				
												else return CellID_;
								}
								std::bitset<64> ConvertToBitset (char CellID_){
												CellID_bit_ = std::bitset<64>(Cell_ID_);
												return CellID_bit_;
								}

				private:
								char CellID_;
								std::bitset<64> CellID_bit_;

};
#endif /*CELLID_H_*/

