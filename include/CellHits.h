/*
 * CellHits.h
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#ifndef CELLHITS_H_
#define CELLHITS_H_

#include <vector>

class CellHits {
public:
	CellHits(): CellID(), HitCount(), BunchNumber(0){}
	~CellHits(){}


	std::vector< int > Get_CellID() const;
	std::vector< int > Get_HitCount() const;
	int Get_BunchNumber() const;

	void CheckCellID(int const id);
	void Set_BunchNumber(int const bunchnumber);

protected:
	std::vector< int > CellID;
	std::vector< int > HitCount;
	int BunchNumber;
};

#endif /* CELLHITS_H_ */
