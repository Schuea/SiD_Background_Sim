#ifndef CELLID_H_
#define CELLID_H_

#include <bitset>
#include <limits>
#include <stdexcept>
#include <iostream>

class CellID{

  public:
    CellID(int const ID) : ID_(ID), ID0_(-1), ID1_(-1){}
    CellID(int const ID0, int const ID1) : ID0_(ID0),ID1_(ID1), ID_(-1){}
  
    virtual ~CellID() {}
    virtual void CreateCellID(){std::cout << "This should not be called!" << std::endl;}
    virtual std::string GetCellID() const {return "";}

    int CellID_ToINTconversion(std::string const CellIDstring) const{
	std::bitset<64> temp (CellIDstring);
	return int(temp.to_ulong());
    }
  
  private:
    CellID() : ID0_(-1), ID1_(-1), ID_(-1) {}
    CellID(float const ID) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception

    CellID(float const ID0, float const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(int const ID0, float const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(float const ID0, int const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID0, double const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(int const ID0, double const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
    CellID(double const ID0, int const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception

  protected:
    int ID_;
    int ID0_;
    int ID1_;

};

class CellID64bits : public CellID {
  public:
    CellID64bits(int const ID0, int const ID1) : CellID(ID0, ID1) {}
    void CreateCellID();
    std::string GetCellID() const {
      //if ((ID0_ == 0 && ID1_ == 0) || ID0_ < 0 || ID1_ <0){
      if (ID0_ == 0 && ID1_ == 0){
        throw std::runtime_error("The CellID was not set. Invalid IDs!");
      }				
      else return CellID_bit_.to_string();
    }
/*
    std::bitset<64> ConvertToBitset (std::string CellID_){
      CellID_bit_ = std::bitset<64>(Cell_ID_);
      return CellID_bit_;
    }
*/
  private:
    std::bitset<64> CellID_bit_;

};
class CellID58bits : public CellID {
  public:
    CellID58bits(int const ID) : CellID(ID) {}
    void CreateCellID();
    std::string GetCellID() const {
      if (ID_ <= 0){
        throw std::runtime_error("The CellID was not set. Invalid IDs!");
      }				
      else return CellID_bit_.to_string();
    }

  private:
    std::bitset<58> CellID_bit_;

};
class CellID54bits : public CellID {
  public:
    CellID54bits(int const ID) : CellID(ID) {}
    void CreateCellID();
    std::string GetCellID() const {
      if (ID_ <= 0){
        throw std::runtime_error("The CellID was not set. Invalid IDs!");
      }				
      else return CellID_bit_.to_string();
    }

  private:
    std::bitset<54> CellID_bit_;

};
#endif /*CELLID_H_*/

