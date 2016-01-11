/*
 * CellHits.h
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#ifndef CELLHITS_H_
#define CELLHITS_H_

class CellHits {
public:
	CellHits(): CellID(0), HitCount(0), BunchNumber(0){}
	~CellHits(){}


	int Get_CellID() const;
	int Get_HitCount() const;
	int Get_BunchNumber() const;

	void Set_CellID(int cellid);
	void Set_HitCount(int hitcount);
	void Set_BunchNumber(int bunchnumber);

protected:
	int CellID;
	int HitCount;
	int BunchNumber;
};

#endif /* CELLHITS_H_ */
