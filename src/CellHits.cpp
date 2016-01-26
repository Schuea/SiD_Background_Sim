/*
 * CellHits.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: schuea
 */

#include "CellHits.h"

std::vector<int> CellHits::Get_CellID() const {
	return CellID;
}
std::vector<int> CellHits::Get_HitCount() const {
	return HitCount;
}
std::vector<std::pair<float, float> > CellHits::Get_CellPosition() const {
	return CellPosition;
}
std::vector<float> CellHits::Get_Position_Radius() const {
	return Position_Radius;
}
std::vector<float> CellHits::Get_Position_Phi() const {
	return Position_Phi;
}
std::map<int, std::pair<std::vector<int>, float> > CellHits::Get_AverageOccupancy_Rad() const {
	return AverageOccupancy_Rad;
}
std::map<int, std::pair<std::vector<int>, float> > CellHits::Get_AverageOccupancy_Phi() const {
	return AverageOccupancy_Phi;
}
int CellHits::Get_BunchNumber() const {
	return BunchNumber;
}

void CellHits::CheckCellID(int const id, float const x, float const y) {
	bool cell_exists(false);
	int vector_element(-1);
	for (int i = 0; i < CellID.size(); ++i) {
		if (CellID.at(i) == id) {
			cell_exists = true;
			vector_element = i; //Check at which position in vector the ID is stored
			break;
		}
	}
	if (cell_exists) {
		HitCount.at(vector_element) += 1;
	} else {
		CellID.push_back(id);
		HitCount.push_back(1);
		CellPosition.push_back(std::pair<float, float>(x, y));
		Position_Radius.push_back(sqrt(pow(x, 2) + pow(y, 2)));
		Position_Phi.push_back(acos(x / sqrt(pow(x, 2) + pow(y, 2))));
	}
}
void CellHits::Check_Rad_Position() {
	for (int j = 0; j < Position_Radius.size(); ++j) {
		if (Position_Radius.at(j) < 10)
			AverageOccupancy_Rad[10].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 10 && Position_Radius.at(j) < 20)
			AverageOccupancy_Rad[20].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 20 && Position_Radius.at(j) < 30)
			AverageOccupancy_Rad[30].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 30 && Position_Radius.at(j) < 40)
			AverageOccupancy_Rad[40].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 40 && Position_Radius.at(j) < 50)
			AverageOccupancy_Rad[50].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 50 && Position_Radius.at(j) < 60)
			AverageOccupancy_Rad[60].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 60 && Position_Radius.at(j) < 70)
			AverageOccupancy_Rad[70].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 70 && Position_Radius.at(j) < 80)
			AverageOccupancy_Rad[80].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 80 && Position_Radius.at(j) < 90)
			AverageOccupancy_Rad[90].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 90 && Position_Radius.at(j) < 100)
			AverageOccupancy_Rad[100].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 100 && Position_Radius.at(j) < 110)
			AverageOccupancy_Rad[110].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 110 && Position_Radius.at(j) < 120)
			AverageOccupancy_Rad[120].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 120 && Position_Radius.at(j) < 130)
			AverageOccupancy_Rad[130].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 130 && Position_Radius.at(j) < 140)
			AverageOccupancy_Rad[140].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 140 && Position_Radius.at(j) < 150)
			AverageOccupancy_Rad[150].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 150)
			AverageOccupancy_Rad[160].first.push_back(HitCount.at(j));
	}
	Calculate_Average(AverageOccupancy_Rad);
}
void CellHits::Check_Phi_Position() {
	for (int j = 0; j < Position_Phi.size(); ++j) {
		if (Position_Phi.at(j) < 1)
			AverageOccupancy_Phi[1].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 1 && Position_Phi.at(j) < 2)
			AverageOccupancy_Phi[2].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 2 && Position_Phi.at(j) < 3)
			AverageOccupancy_Phi[3].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 3 && Position_Phi.at(j) < 4)
			AverageOccupancy_Phi[4].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 4 && Position_Phi.at(j) < 5)
			AverageOccupancy_Phi[5].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 5 && Position_Phi.at(j) < 6)
			AverageOccupancy_Phi[6].first.push_back(HitCount.at(j));
		if (Position_Phi.at(j) >= 6 && Position_Phi.at(j) < 7)
			AverageOccupancy_Phi[7].first.push_back(HitCount.at(j));
	}
	Calculate_Average(AverageOccupancy_Phi);
}
void CellHits::Calculate_Average(std::map< int, std::pair< std::vector<int>, float> > & AverageMap) {
	for (auto iterator = AverageMap.begin(); iterator != AverageMap.end(); iterator++) {
		float average = 0;
		for (int i = 0; i < iterator->second.first.size(); ++i) {
			average += iterator->second.first.at(i);
		}
		average /= iterator->second.first.size();
		iterator->second.second = average;
	}
}
void CellHits::Set_BunchNumber(int const bunchnumber) {
	BunchNumber = bunchnumber;
}
