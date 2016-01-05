#include "CreateCellID.h"

#include <iostream>

template<size_t sRes, size_t s1, size_t s2>
std::bitset<sRes> concatString(std::bitset<s1> const & bs1,
    std::bitset<s2> const & bs2){
  std::string const string1(bs1.to_string());
  std::string const string2(bs2.to_string());

  std::bitset<sRes> res(string1 + string2);
  return res;
}

void CellID64bits::CreateCellID() {
  //if ((ID0_ == 0 && ID1_ == 0) || ID0_ < 0 || ID1_ <0){
  if (ID0_ == 0 && ID1_ == 0){
    throw std::runtime_error("The CellID was not set. Invalid IDs!");
  }				
  else{
    std::bitset<32> const ID0_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID0_);
    std::bitset<32> const ID1_bit = std::bitset<std::numeric_limits<unsigned int>::digits>(ID1_);
    CellID_bit_ = concatString<64, 32, 32>(ID1_bit, ID0_bit);
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
