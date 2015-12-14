/*
 * DataClass.h
 *
 *  Created on: Dec 3, 2015
 *      Author: schuea
 */

#ifndef DATACLASS_H_
#define DATACLASS_H_

class Data {

public:
	Data(){}

	int event_id;
	float x_hit;
	float y_hit;
	float z_hit;

	virtual void SetBranchStatus(TTree* const tree) = 0;
	virtual void SetBranchAddresses(TTree* const tree) = 0;
	void SetBranches(TTree* const tree);
};

class DataSimCalorimeterHit: public Data {

public:
	DataSimCalorimeterHit(){}

	int id0;
	int id1;
	float energy_hit;
	int number_contributionsToHit;
	float energy_contribution;
	int PDG_contribution;
	float time_contribution;

	int LCIO_id_mother;
	float time_creation_mother;
	double vertex_x_mother;
	double vertex_y_mother;
	double vertex_z_mother;
	double endpoint_x_mother;
	double endpoint_y_mother;
	double endpoint_z_mother;
	double momentum_x_mother;
	double momentum_y_mother;
	double momentum_z_mother;
	int PDG_mother;
	float charge_mother;
	double energy_mother;

	void SetBranchStatus(TTree* const tree);
	void SetBranchAddresses(TTree* const tree);
};

class DataSimTrackerHit: public Data {

public:
	DataSimTrackerHit(){}

	int id;
	float dEdx_hit;
	float time_hit;
	float momentum_x_hit;
	float momentum_y_hit;
	float momentum_z_hit;

	int LCIO_id_particle;
	float time_creation_particle;
	double vertex_x_particle;
	double vertex_y_particle;
	double vertex_z_particle;
	double endpoint_x_particle;
	double endpoint_y_particle;
	double endpoint_z_particle;
	double momentum_x_particle;
	double momentum_y_particle;
	double momentum_z_particle;
	int PDG_particle;
	float charge_particle;
	float energy_particle;

	void SetBranchStatus(TTree* const tree);
	void SetBranchAddresses(TTree* const tree);
};


#endif /* DATACLASS_H_ */
