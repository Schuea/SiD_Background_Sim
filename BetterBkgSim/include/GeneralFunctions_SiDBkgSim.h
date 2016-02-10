#ifndef GENERALFUNCTIONSFORSIDBKGSIM_H_
#define GENERALFUNCTIONSFORSIDBKGSIM_H_

void SetupSubDetectorsVector(std::vector<Subdetector*> * SubDetectors, std::string *several_subdetector_names,
		std::vector<std::string> argument_subdetectors);
void InitializeAllCaloSubdetectors(std::vector<Subdetector*> * SubDetectors);
void InitializeAllTrackerSubdetectors(std::vector<Subdetector*> * SubDetectors);
void InitializeWhichSubdetector(std::string SubdetectorName, std::vector<Subdetector*> * SubDetectors);
void InitializeAllSubdetectors(std::vector<Subdetector*> * SubDetectors);

TTree* Get_TTree(TFile* inputfile, std::string subdetector_name);

std::pair<int, int> Set_train_bunch_number(int number_of_file);

#endif //GENERALFUNCTIONSFORSIDBKGSIM_H_
