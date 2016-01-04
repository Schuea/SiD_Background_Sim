/*
 * DataClass.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: schuea
 */

#include "DataClass.h"

void Data::SetBranches(TTree* const tree){
	SetBranchStatus(tree);
	SetBranchAddresses(tree);
  Fill_ids();
}

void DataSimCalorimeterHit::SetBranchStatus(TTree* const tree) {

	tree->SetBranchStatus("*", kFALSE); // disable all

	tree->SetBranchStatus("event_id", kTRUE);
	tree->SetBranchStatus("HitCellID0", kTRUE);
	tree->SetBranchStatus("HitCellID1", kTRUE);
	tree->SetBranchStatus("HitEnergy", kTRUE);
	tree->SetBranchStatus("HitPosition_x", kTRUE);
	tree->SetBranchStatus("HitPosition_y", kTRUE);
	tree->SetBranchStatus("HitPosition_z", kTRUE);

	tree->SetBranchStatus("Number_Contr", kTRUE);
	tree->SetBranchStatus("HitContrEnergy", kTRUE);
	tree->SetBranchStatus("HitContrPDG", kTRUE);
	tree->SetBranchStatus("HitContrTime", kTRUE);

	tree->SetBranchStatus("HitMotherLCIO_id", kTRUE);
	tree->SetBranchStatus("HitMotherCreationTime", kTRUE);

	tree->SetBranchStatus("HitMotherVertex_x", kTRUE);
	tree->SetBranchStatus("HitMotherVertex_y", kTRUE);
	tree->SetBranchStatus("HitMotherVertex_z", kTRUE);

	tree->SetBranchStatus("HitMotherEndpoint_x", kTRUE);
	tree->SetBranchStatus("HitMotherEndpoint_y", kTRUE);
	tree->SetBranchStatus("HitMotherEndpoint_z", kTRUE);

	tree->SetBranchStatus("HitMotherMomentum_x", kTRUE);
	tree->SetBranchStatus("HitMotherMomentum_y", kTRUE);
	tree->SetBranchStatus("HitMotherMomentum_z", kTRUE);

	tree->SetBranchStatus("HitMotherParticle_ID", kTRUE);
	tree->SetBranchStatus("HitMotherCharge", kTRUE);
	tree->SetBranchStatus("HitMotherParticleEnergy", kTRUE);
}
void DataSimCalorimeterHit::SetBranchAddresses(TTree* const tree) {

	tree->SetBranchAddress("event_id", &event_id);
	tree->SetBranchAddress("HitCellID0", &id0);
	tree->SetBranchAddress("HitCellID1", &id1);
	tree->SetBranchAddress("HitEnergy", &energy_hit);
	tree->SetBranchAddress("HitPosition_x", &x_hit);
	tree->SetBranchAddress("HitPosition_y", &y_hit);
	tree->SetBranchAddress("HitPosition_z", &z_hit);

	tree->SetBranchAddress("Number_Contr", &number_contributionsToHit);
	tree->SetBranchAddress("HitContrEnergy", &energy_contribution);
	tree->SetBranchAddress("HitContrPDG", &PDG_contribution);
	tree->SetBranchAddress("HitContrTime", &time_contribution);

	tree->SetBranchAddress("HitMotherLCIO_id", &LCIO_id_mother);
	tree->SetBranchAddress("HitMotherCreationTime", &time_creation_mother);

	tree->SetBranchAddress("HitMotherVertex_x", &vertex_x_mother);
	tree->SetBranchAddress("HitMotherVertex_y", &vertex_y_mother);
	tree->SetBranchAddress("HitMotherVertex_z", &vertex_z_mother);

	tree->SetBranchAddress("HitMotherEndpoint_x", &endpoint_x_mother);
	tree->SetBranchAddress("HitMotherEndpoint_y", &endpoint_y_mother);
	tree->SetBranchAddress("HitMotherEndpoint_z", &endpoint_z_mother);

	tree->SetBranchAddress("HitMotherMomentum_x", &momentum_x_mother);
	tree->SetBranchAddress("HitMotherMomentum_y", &momentum_y_mother);
	tree->SetBranchAddress("HitMotherMomentum_z", &momentum_z_mother);

	tree->SetBranchAddress("HitMotherParticle_ID", &PDG_mother);
	tree->SetBranchAddress("HitMotherCharge", &charge_mother);
	tree->SetBranchAddress("HitMotherParticleEnergy", &energy_mother);
}

void DataSimTrackerHit::SetBranchStatus(TTree* const tree) {

	tree->SetBranchStatus("*", kFALSE); // disable all

	tree->SetBranchStatus("event_id", kTRUE);
	tree->SetBranchStatus("HitCellID", kTRUE);
	tree->SetBranchStatus("HitdEdx", kTRUE);
	tree->SetBranchStatus("HitTime", kTRUE);

	tree->SetBranchStatus("HitPosition_x", kTRUE);
	tree->SetBranchStatus("HitPosition_y", kTRUE);
	tree->SetBranchStatus("HitPosition_z", kTRUE);

	tree->SetBranchStatus("HitMomentum_x", kTRUE);
	tree->SetBranchStatus("HitMomentum_y", kTRUE);
	tree->SetBranchStatus("HitMomentum_z", kTRUE);

	tree->SetBranchStatus("HitParticleLCIO_ID", kTRUE);
	tree->SetBranchStatus("HitParticleCreationTime", kTRUE);

	tree->SetBranchStatus("HitParticleVertex_x", kTRUE);
	tree->SetBranchStatus("HitParticleVertex_y", kTRUE);
	tree->SetBranchStatus("HitParticleVertex_z", kTRUE);

	tree->SetBranchStatus("HitParticleEndpoint_x", kTRUE);
	tree->SetBranchStatus("HitParticleEndpoint_y", kTRUE);
	tree->SetBranchStatus("HitParticleEndpoint_z", kTRUE);

	tree->SetBranchStatus("HitParticleMomentum_x", kTRUE);
	tree->SetBranchStatus("HitParticleMomentum_y", kTRUE);
	tree->SetBranchStatus("HitParticleMomentum_z", kTRUE);

	tree->SetBranchStatus("HitParticle_ID", kTRUE);
	tree->SetBranchStatus("HitParticleCharge", kTRUE);
	tree->SetBranchStatus("HitParticleEnergy", kTRUE);
	}

void DataSimTrackerHit::SetBranchAddresses(TTree* const tree) {

	tree->SetBranchAddress("event_id", &event_id);
	tree->SetBranchAddress("HitCellID", &id);
	tree->SetBranchAddress("HitdEdx", &dEdx_hit);
	tree->SetBranchAddress("HitTime", &time_hit);

	tree->SetBranchAddress("HitPosition_x", &x_hit);
	tree->SetBranchAddress("HitPosition_y", &y_hit);
	tree->SetBranchAddress("HitPosition_z", &z_hit);

	tree->SetBranchAddress("HitMomentum_x", &momentum_x_hit);
	tree->SetBranchAddress("HitMomentum_y", &momentum_y_hit);
	tree->SetBranchAddress("HitMomentum_z", &momentum_z_hit);

	tree->SetBranchAddress("HitParticleLCIO_ID", &LCIO_id_particle);
	tree->SetBranchAddress("HitParticleCreationTime", &time_creation_particle);

	tree->SetBranchAddress("HitParticleVertex_x", &vertex_x_particle);
	tree->SetBranchAddress("HitParticleVertex_y", &vertex_y_particle);
	tree->SetBranchAddress("HitParticleVertex_z", &vertex_z_particle);

	tree->SetBranchAddress("HitParticleEndpoint_x", &endpoint_x_particle);
	tree->SetBranchAddress("HitParticleEndpoint_y", &endpoint_y_particle);
	tree->SetBranchAddress("HitParticleEndpoint_z", &endpoint_z_particle);

	tree->SetBranchAddress("HitParticleMomentum_x", &momentum_x_particle);
	tree->SetBranchAddress("HitParticleMomentum_y", &momentum_y_particle);
	tree->SetBranchAddress("HitParticleMomentum_z", &momentum_z_particle);

	tree->SetBranchAddress("HitParticle_ID", &PDG_particle);
	tree->SetBranchAddress("HitParticleCharge", &charge_particle);
	tree->SetBranchAddress("HitParticleEnergy", &energy_particle);
}
