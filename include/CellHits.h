/*
 * CellHits.h
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#ifndef CELLHITS_H_
#define CELLHITS_H_

#include "Subdetector.h"

#include <iostream>

#include <vector>
#include <map>
#include <cmath>
#include <math.h>       /* atan2 */

class CellHits {
public:
	CellHits(Subdetector* SubDetector_) :
			SubDetector(SubDetector_), CellID(), HitCount(), CellPosition(), Position_Radius(), Position_Phi(), AverageOccupancy_Rad(), AverageOccupancy_Phi(), BunchNumber(
					0) {
	}
	~CellHits() {
	}

	std::vector<int> Get_CellID() const;
	std::vector<int> Get_HitCount() const;
	std::vector<std::pair<float, float> > Get_CellPosition() const;
	std::vector<float> Get_Layer() const;
	std::vector<float> Get_Position_Radius() const;
	std::vector<float> Get_Position_Phi() const;
	std::map<int, std::pair<std::vector<int>, std::pair<float, float> > > Get_AverageOccupancy_Rad() const;
	std::map<int, std::pair<std::vector<int>, std::pair<float, float> > > Get_AverageOccupancy_Phi() const;
	int Get_BunchNumber() const;

	void Check_CellID(unsigned long const id, float const x, float const y);
	void Check_Rad_Position();
	void Check_Phi_Position();
	void Calculate_Average(std::map<int, std::pair<std::vector<int>, std::pair<float, float> > > & AverageMap);
	void Set_BunchNumber(int const bunchnumber);

protected:
	Subdetector* SubDetector;
	std::vector<int> CellID;
	std::vector<int> HitCount;
	std::vector<std::pair<float, float> > CellPosition;
	std::vector<float> Layer;
	std::vector<float> Position_Radius;
	std::vector<float> Position_Phi;
	std::map<int, std::pair<std::vector<int>, std::pair< float, float > > > AverageOccupancy_Rad;
	std::map<int, std::pair<std::vector<int>, std::pair< float, float > > > AverageOccupancy_Phi;
	int BunchNumber;
};

#endif /* CELLHITS_H_ */
