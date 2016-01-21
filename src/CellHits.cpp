/*
 * CellHits.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#include "CellHits.h"

std::vector< int > CellHits::Get_CellID() const{
	return CellID;
}
std::vector< int > CellHits::Get_HitCount() const{
	return HitCount;
}
int CellHits::Get_BunchNumber() const{
	return BunchNumber;
}

void CellHits::CheckCellID(int const id){
  bool cell_exists(false);
  int cell_element(-1);
  for(int i = 0; i < CellID.size(); ++i){
    if(CellID.at(i) == id){
      cell_exists = true;
      cell_element = i;
      break;
    }
  }
  if(cell_exists){
    HitCount.at(cell_element) += 1;
  } else{
    CellID.push_back(id);
    HitCount.push_back(1);
  }
}
void CellHits::Set_BunchNumber(int const bunchnumber) {
	BunchNumber = bunchnumber;
}
