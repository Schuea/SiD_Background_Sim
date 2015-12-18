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

	tree->Branch("event_id", &event_id, "event_id/I");
	tree->Branch("HitCellID0", &id0, "HitCellID0/I");
	tree->Branch("HitCellID1", &id1, "HitCellID1/I");
	tree->Branch("HitEnergy", &energy_hit, "HitEnergy/F");
	tree->Branch("HitPosition_x", &x_hit, "HitPosition_x/F");
	tree->Branch("HitPosition_y", &y_hit, "HitPosition_y/F");
	tree->Branch("HitPosition_z", &z_hit, "HitPosition_z/F");

	tree->Branch("Number_Contr", &number_contributionsToHit, "Number_Contr/I");
	tree->Branch("HitContrEnergy", &energy_contribution, "HitContrEnergy/F");
	tree->Branch("HitContrPDG", &PDG_contribution,"HitContrPDG/I");
	tree->Branch("HitContrTime", &time_contribution, "HitContrTime/F");

	tree->Branch("HitMotherLCIO_id", &LCIO_id_mother, "HitMotherLCIO_id/I");
	tree->Branch("HitMotherCreationTime", &time_creation_mother, "HitMotherCreationTime/F");

	tree->Branch("HitMotherVertex_x", &vertex_x_mother, "HitMotherVertex_x/D");
	tree->Branch("HitMotherVertex_y", &vertex_y_mother, "HitMotherVertex_y/D");
	tree->Branch("HitMotherVertex_z", &vertex_z_mother, "HitMotherVertex_z/D");

	tree->Branch("HitMotherEndpoint_x", &endpoint_x_mother, "HitMotherEndpoint_x/D");
	tree->Branch("HitMotherEndpoint_y", &endpoint_y_mother, "HitMotherEndpoint_y/D");
	tree->Branch("HitMotherEndpoint_z", &endpoint_z_mother, "HitMotherEndpoint_z/D");

	tree->Branch("HitMotherMomentum_x", &momentum_x_mother, "HitMotherMomentum_x/D");
	tree->Branch("HitMotherMomentum_y", &momentum_y_mother, "HitMotherMomentum_y/D");
	tree->Branch("HitMotherMomentum_z", &momentum_z_mother, "HitMotherMomentum_z/D");

	tree->Branch("HitMotherParticle_ID", &PDG_mother, "HitMotherParticle_ID/I");
	tree->Branch("HitMotherCharge", &charge_mother, "HitMotherCharge/F");
	tree->Branch("HitMotherParticleEnergy", &energy_mother, "HitMotherParticleEnergy/D");
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

	tree->Branch("event_id", &event_id, "event_id/I");
	tree->Branch("HitCellID", &id, "HitCellID/I");
	tree->Branch("HitdEdx", &dEdx_hit, "HitdEdx/F");
	tree->Branch("HitTime", &time_hit, "HitTime/F");

	tree->Branch("HitPosition_x", &x_hit, "HitPosition_x/F");
	tree->Branch("HitPosition_y", &y_hit, "HitPosition_y/F");
	tree->Branch("HitPosition_z", &z_hit, "HitPosition_z/F");

	tree->Branch("HitMomentum_x", &momentum_x_hit, "HitMomentum_x/F");
	tree->Branch("HitMomentum_y", &momentum_y_hit, "HitMomentum_y/F");
	tree->Branch("HitMomentum_z", &momentum_z_hit, "HitMomentum_z/F");

	tree->Branch("HitParticleLCIO_ID", &LCIO_id_particle, "HitParticleLCIO_ID/I");
	tree->Branch("HitParticleCreationTime", &time_creation_particle, "HitParticleCreationTime/F");

	tree->Branch("HitParticleVertex_x", &vertex_x_particle, "HitParticleVertex_x/D");
	tree->Branch("HitParticleVertex_y", &vertex_y_particle, "HitParticleVertex_y/D");
	tree->Branch("HitParticleVertex_z", &vertex_z_particle, "HitParticleVertex_z/D");

	tree->Branch("HitParticleEndpoint_x", &endpoint_x_particle, "HitParticleEndpoint_x/D");
	tree->Branch("HitParticleEndpoint_y", &endpoint_y_particle, "HitParticleEndpoint_y/D");
	tree->Branch("HitParticleEndpoint_z", &endpoint_z_particle, "HitParticleEndpoint_z/D");

	tree->Branch("HitParticleMomentum_x", &momentum_x_particle, "HitParticleMomentum_x/D");
	tree->Branch("HitParticleMomentum_y", &momentum_y_particle, "HitParticleMomentum_y/D");
	tree->Branch("HitParticleMomentum_z", &momentum_z_particle, "HitParticleMomentum_z/D");

	tree->Branch("HitParticle_ID", &PDG_particle, "HitParticle_ID/I");
	tree->Branch("HitParticleCharge", &charge_particle, "HitParticleCharge/F");
	tree->Branch("HitParticleEnergy", &energy_particle, "HitParticleEnergy/F");
}
