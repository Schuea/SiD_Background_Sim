/*
 * CellHits.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#include "CellHits.h"

int CellHits::Get_CellID() const{
	return CellID;
}
int CellHits::Get_HitCount() const{
	return HitCount;
}
int CellHits::Get_BunchNumber() const{
	return BunchNumber;
}

void CellHits::Set_CellID(int cellid) {
	CellID = cellid;
}
void CellHits::Set_HitCount(int hitcount) {
	HitCount = hitcount;
}
void CellHits::Set_BunchNumber(int bunchnumber) {
	BunchNumber = bunchnumber;
}
