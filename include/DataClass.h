/*
 * DataClass.h
 *
 *  Created on: Dec 3, 2015
 *      Author: schuea
 */

#ifndef DATACLASS_H_
#define DATACLASS_H_

#include "TTree.h"
#include "TBranch.h"

class Data {

public:
	Data() {}

	std::vector<int> ids;
	int Get_event_id() const {
		return event_id;
	}
	float Get_x_hit() const {
		return x_hit;
	}
	float Get_y_hit() const {
		return y_hit;
	}
	float Get_z_hit() const {
		return z_hit;
	}
	virtual float Get_energy_hit() const {return 0.0;}
	virtual int Get_number_contributionsToHit() const {return 0;}
	virtual float Get_energy_contribution() const {return 0.0;}
	virtual int Get_PDG_contribution() const {return 0;}
	virtual float Get_time_contribution() const {return 0.0;}
	virtual int Get_LCIO_id_mother() const {return 0;}
	virtual float Get_time_creation_mother() const {return 0.0;}
	virtual double Get_vertex_x_mother() const {return 0.0;}
	virtual double Get_vertex_y_mother() const {return 0.0;}
	virtual double Get_vertex_z_mother() const {return 0.0;}
	virtual std::array<double, 3> Get_vertex_mother() const {return {0.,0.,0.};}
	virtual double Get_endpoint_x_mother() const {return 0.0;}
	virtual double Get_endpoint_y_mother() const {return 0.0;}
	virtual double Get_endpoint_z_mother() const {return 0.0;}
	virtual std::array<double, 3> Get_endpoint_mother() const {return {0.,0.,0.};}
	virtual double Get_momentum_x_mother() const {return 0.0;}
	virtual double Get_momentum_y_mother() const {return 0.0;}
	virtual double Get_momentum_z_mother() const {return 0.0;}
	virtual std::array<double, 3> Get_momentum_mother() const {return {0.,0.,0.};}
	virtual int Get_PDG_mother() const {return 0;}
	virtual float Get_charge_mother() const {return 0.0;}
	virtual double Get_energy_mother() const {return 0.0;}

	virtual float Get_dEdx_hit() const {return 0.0;}
	virtual float Get_time_hit() const {return 0.0;}
	virtual float Get_momentum_x_hit() const {return 0.0;}
	virtual float Get_momentum_y_hit() const {return 0.0;}
	virtual float Get_momentum_z_hit() const  {return 0.0;}
	virtual std::array<double, 3> Get_momentum_hit() const {return {0.,0.,0.};}
	virtual int Get_LCIO_id_particle() const {return 0;}
	virtual float Get_time_creation_particle() const {return 0.0;}
	virtual double Get_vertex_x_particle() const {return 0.0;}
	virtual double Get_vertex_y_particle() const {return 0.0;}
	virtual double Get_vertex_z_particle() const {return 0.0;}
	virtual std::array<double, 3> Get_vertex_particle() const {return {0.,0.,0.};}
	virtual double Get_endpoint_x_particle() const {return 0.0;}
	virtual double Get_endpoint_y_particle() const {return 0.0;}
	virtual double Get_endpoint_z_particle() const {return 0.0;}
	virtual std::array<double, 3> Get_endpoint_particle() const {return {0.,0.,0.};}
	virtual double Get_momentum_x_particle() const {return 0.0;}
	virtual double Get_momentum_y_particle() const {return 0.0;}
	virtual double Get_momentum_z_particle() const {return 0.0;}
	virtual std::array<double, 3> Get_momentum_particle() const {return {0.,0.,0.};}
	virtual int Get_PDG_particle() const {return 0;}
	virtual float Get_charge_particle() const {return 0.0;}
	virtual double Get_energy_particle() const {return 0.0;}

	virtual void Fill_ids() = 0;

	virtual void SetBranchStatus(TTree* const tree) = 0;
	virtual void SetBranchAddresses(TTree* const tree) = 0;
	void SetBranches(TTree* const tree);

protected:
	int event_id;
	float x_hit;
	float y_hit;
	float z_hit;
}
;

class DataSimCalorimeterHit: public Data {

public:
	DataSimCalorimeterHit() {}

	float Get_energy_hit() const {
		return energy_hit;
	}
	int Get_number_contributionsToHit() const {
		return number_contributionsToHit;
	}
	float Get_energy_contribution() const {
		return energy_contribution;
	}
	int Get_PDG_contribution() const {
		return PDG_contribution;
	}
	float Get_time_contribution() const {
		return time_contribution;
	}
	int Get_LCIO_id_mother() const {
		return LCIO_id_mother;
	}
	float Get_time_creation_mother() const {
		return time_creation_mother;
	}
	double Get_vertex_x_mother() const {
		return vertex_x_mother;
	}
	double Get_vertex_y_mother() const {
		return vertex_y_mother;
	}
	double Get_vertex_z_mother() const {
		return vertex_z_mother;
	}
	std::array<double, 3> Get_vertex_mother() const {
		std::array<double, 3> vertex;
		vertex[0] = Get_vertex_x_mother();
		vertex[1] = Get_vertex_y_mother();
		vertex[2] = Get_vertex_z_mother();
		return vertex;
	}
	double Get_endpoint_x_mother() const {
		return endpoint_x_mother;
	}
	double Get_endpoint_y_mother() const {
		return endpoint_y_mother;
	}
	double Get_endpoint_z_mother() const {
		return endpoint_z_mother;
	}
	std::array<double, 3> Get_endpoint_mother() const {
		std::array<double, 3> endpoint;
		endpoint[0] = Get_endpoint_x_mother();
		endpoint[1] = Get_endpoint_y_mother();
		endpoint[2] = Get_endpoint_z_mother();
		return endpoint;
	}
	double Get_momentum_x_mother() const {
		return momentum_x_mother;
	}
	double Get_momentum_y_mother() const {
		return momentum_y_mother;
	}
	double Get_momentum_z_mother() const {
		return momentum_z_mother;
	}
	std::array<double, 3> Get_momentum_mother() const {
		std::array<double, 3> momentum;
		momentum[0] = Get_momentum_x_mother();
		momentum[1] = Get_momentum_y_mother();
		momentum[2] = Get_momentum_z_mother();
		return momentum;
	}
	int Get_PDG_mother() const {
		return PDG_mother;
	}
	float Get_charge_mother() const {
		return charge_mother;
	}
	double Get_energy_mother() const {
		return energy_mother;
	}

	void Fill_ids() {
		ids.clear();
		ids.push_back(id0);
		ids.push_back(id1);
	}

	void SetBranchStatus(TTree* const tree);
	void SetBranchAddresses(TTree* const tree);

private:
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

};

class DataSimTrackerHit: public Data {

public:
	DataSimTrackerHit() {}

	float Get_dEdx_hit() const {
		return dEdx_hit;
	}
	float Get_time_hit() const {
		return time_hit;
	}
	float Get_momentum_x_hit() const {
		return momentum_x_hit;
	}
	float Get_momentum_y_hit() const {
		return momentum_y_hit;
	}
	float Get_momentum_z_hit() const {
		return momentum_z_hit;
	}
	std::array<double, 3> Get_momentum_hit() const {
		std::array<double, 3> momentum;
		momentum[0] = Get_momentum_x_hit();
		momentum[1] = Get_momentum_y_hit();
		momentum[2] = Get_momentum_z_hit();
		return momentum;
	}
	int Get_LCIO_id_particle() const {
		return LCIO_id_particle;
	}
	float Get_time_creation_particle() const {
		return time_creation_particle;
	}
	double Get_vertex_x_particle() const {
		return vertex_x_particle;
	}
	double Get_vertex_y_particle() const {
		return vertex_y_particle;
	}
	double Get_vertex_z_particle() const {
		return vertex_z_particle;
	}
	std::array<double, 3> Get_vertex_particle() const {
		std::array<double, 3> vertex;
		vertex[0] = Get_vertex_x_particle();
		vertex[1] = Get_vertex_y_particle();
		vertex[2] = Get_vertex_z_particle();
		return vertex;
	}
	double Get_endpoint_x_particle() const {
		return endpoint_x_particle;
	}
	double Get_endpoint_y_particle() const {
		return endpoint_y_particle;
	}
	double Get_endpoint_z_particle() const {
		return endpoint_z_particle;
	}
	std::array<double, 3> Get_endpoint_particle() const {
		std::array<double, 3> endpoint;
		endpoint[0] = Get_endpoint_x_particle();
		endpoint[1] = Get_endpoint_y_particle();
		endpoint[2] = Get_endpoint_z_particle();
		return endpoint;
	}
	double Get_momentum_x_particle() const {
		return momentum_x_particle;
	}
	double Get_momentum_y_particle() const {
		return momentum_y_particle;
	}
	double Get_momentum_z_particle() const {
		return momentum_z_particle;
	}
	std::array<double, 3> Get_momentum_particle() const {
		std::array<double, 3> momentum;
		momentum[0] = Get_momentum_x_particle();
		momentum[1] = Get_momentum_y_particle();
		momentum[2] = Get_momentum_z_particle();
		return momentum;
	}
	int Get_PDG_particle() const {
		return PDG_particle;
	}
	float Get_charge_particle() const {
		return charge_particle;
	}
	double Get_energy_particle() const {
		return energy_particle;
	}

	void Fill_ids() {
		ids.clear();
		ids.push_back(id);
	}

	void SetBranchStatus(TTree* const tree);
	void SetBranchAddresses(TTree* const tree);

private:
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
	double energy_particle;

};

#endif /* DATACLASS_H_ */
