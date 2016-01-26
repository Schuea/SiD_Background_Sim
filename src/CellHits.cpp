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
    std::cout << "Position_Radius.at(" << j << ") = " << Position_Radius.at(j) << std::endl;
		if (Position_Radius.at(j) < 250)
			AverageOccupancy_Rad[250].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 250 && Position_Radius.at(j) < 500)
			AverageOccupancy_Rad[500].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 500 && Position_Radius.at(j) < 750)
			AverageOccupancy_Rad[750].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 750 && Position_Radius.at(j) < 1000)
			AverageOccupancy_Rad[1000].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 1000 && Position_Radius.at(j) < 1250)
			AverageOccupancy_Rad[1250].first.push_back(HitCount.at(j));
		if (Position_Radius.at(j) >= 1250 && Position_Radius.at(j) < 1500)
			AverageOccupancy_Rad[1500].first.push_back(HitCount.at(j));
	}
	Calculate_Average(AverageOccupancy_Rad);
}
void CellHits::Check_Phi_Position() {
	for (int j = 0; j < Position_Phi.size(); ++j) {
    std::cout << "Position_Phi.at(" << j << ") = " << Position_Phi.at(j) << std::endl;
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
    std::cout << "AverageMap.size = " << AverageMap.size() << std::endl;
	for (auto iterator = AverageMap.begin(); iterator != AverageMap.end(); iterator++) {
    std::cout << "AverageMap.iterator.first = " << iterator->first << std::endl;
		float average = 0;
		for (int i = 0; i < iterator->second.first.size(); ++i) {
			average += iterator->second.first.at(i);
		}
		average /= float(iterator->second.first.size());
    std::cout << "Average = " << average << std::endl;
		iterator->second.second = average;
	}
}
void CellHits::Set_BunchNumber(int const bunchnumber) {
	BunchNumber = bunchnumber;
}
