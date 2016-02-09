#include "CreateCellID.h"

#include <string>
#include <cstring>
#include <iostream>

template<size_t sRes, size_t s1, size_t s2>
std::bitset<sRes> concatString(std::bitset<s1> const & bs1, std::bitset<s2> const & bs2){
  std::string const string1(bs1.to_string());
  std::string const string2(bs2.to_string());

  std::bitset<sRes> res(string1 + string2);
  return res;
}

void CellID64bits::CreateCellID() {
  if (ID0_ <= 0 || ID1_ == 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else{
	std::cout << "ID0 = " << ID0_ << std::endl;
	std::cout << "ID1 = " << ID1_ << std::endl;
    std::bitset<32> const ID0_bit = std::bitset<32>(ID0_);
    //std::bitset<32> const ID1_bit = std::bitset<32>();
    std::bitset<32> const ID1_bit = std::bitset<32>(ID1_);
    CellID_bit_ = concatString<64, 32, 32>(ID1_bit, ID0_bit);
	std::cout << "ID0_bit = " << ID0_bit << std::endl;
	std::cout << "ID1_bit = " << ID1_bit << std::endl;
	std::cout << "concatString = " << CellID_bit_ << std::endl;
  }
}

void CellID58bits::CreateCellID() {
  if (ID_ <= 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else{
    std::bitset<58> const ID_bit = std::bitset<58>(ID_);
    CellID_bit_ = ID_bit;
  }
}

void CellID54bits::CreateCellID() {
  if (ID_ <= 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else{
    std::bitset<54> const ID_bit = std::bitset<54>(ID_);
    CellID_bit_ = ID_bit;
  }
}

unsigned long long CellID::CellID_ToLONGconversion(std::string const CellIDstring) const{
  std::bitset<64> temp (CellIDstring);
  return temp.to_ullong();
}

CellID::CellID(int const ID) : ID_(ID), ID0_(-1), ID1_(-1){}
CellID::CellID(int const ID0, int const ID1) : ID0_(ID0),ID1_(ID1), ID_(-1){}
CellID::~CellID() {}
void CellID::CreateCellID(){
  std::cout << "This should not be called!" << std::endl;
}
std::string CellID::GetCellID() const {
  return "";
}
CellID::CellID() : ID0_(-1), ID1_(-1), ID_(-1) {}
CellID::CellID(float const ID) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(double const ID) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(float const ID0, float const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(int const ID0, float const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(float const ID0, int const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(double const ID0, double const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}	//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(int const ID0, double const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
CellID::CellID(double const ID0, int const ID1) : ID0_(-1), ID1_(-1), ID_(-1){}		//Float arguments are not allowed -> set them to negative to make it throw exception
 
CellID64bits::CellID64bits(int const ID0, int const ID1) : CellID(ID0, ID1) {}
std::string CellID64bits::GetCellID() const {
  if (ID0_ == 0 || ID1_ == 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else return CellID_bit_.to_string();
}

CellID58bits::CellID58bits(int const ID) : CellID(ID) {}
std::string CellID58bits::GetCellID() const {
  if (ID_ <= 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else return CellID_bit_.to_string();
}

CellID54bits::CellID54bits(int const ID) : CellID(ID) {}
std::string CellID54bits::GetCellID() const {
  if (ID_ <= 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else return CellID_bit_.to_string();
}
