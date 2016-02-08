#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <bitset>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main(int const argc, char const * const * const argv){
  //Three occupancy plots
  //Two that are 1D histograms, y-axis is average occupancy and x-axis is radius/phi
  //The difficult plot is buffer depth plot: y-axis is probability of a specific cell occupancy occuring and x-axis is occupancy

  //The input is a TTree ROOT file(s)
  //The output is .pdf and .C files

  //Open the tree for making the plot
  string const filename("/home/schuea/BkgSim_data/1_simulated_pairs.root");
  string const tree_name("Tree_EcalEndcap");

  TFile *file = TFile::Open(filename.c_str());
  TTree *tree;
  file->GetObject(tree_name.c_str(),tree);
  
  //Set the branches
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus("HitCellID0",1);
  tree->SetBranchStatus("HitCellID1",1);
  tree->SetBranchStatus("HitPosition_x",1);
  tree->SetBranchStatus("HitPosition_y",1);
  
  int HitCellID0(0), HitCellID1(0);
  float HitPosition_x(0.0), HitPosition_y(0.0);

  tree->SetBranchAddress("HitCellID0",&HitCellID0);
  tree->SetBranchAddress("HitCellID1",&HitCellID1);
  tree->SetBranchAddress("HitPosition_x",&HitPosition_x);
  tree->SetBranchAddress("HitPosition_y",&HitPosition_y);

  //Now we loop through the tree
  //Combine the two Cell ID's into a single new Cell ID
  //See how often the new Cell ID occurs in total, this is the occupancy
  
  //long long int const entries = 500;
  long long int const entries = tree->GetEntries();
  map<string,int> hit_map;
  for(long long int i = 0; i < entries; ++i){
    tree->GetEntry(i);
    if(i%10000 == 0){
      cout << "Event " << i << endl;
    }
    //First, we combine the two cell IDs into one cell ID
    bitset<32> const id0bit(HitCellID0);
    string const id0string(id0bit.to_string());
    bitset<32> const id1bit(HitCellID1);
    string const id1string(id1bit.to_string());

    //Make a combined cell ID
    string combined_cell_id = id1string+id0string;
    //Test if the ID already exists in a map, either way add 1
    if(hit_map.find(combined_cell_id) == hit_map.end()){
      hit_map[combined_cell_id] = 1;
    }
    else{
      hit_map[combined_cell_id] = hit_map[combined_cell_id] + 1;
    }
  }

  //Make histogram for storing the information
  TH1D *histo = new TH1D("histo","title",20,0,20);
  //Fill histogram with the occupancies from the hit_map
  for(auto const &it : hit_map){
    histo->Fill(it.second);
  }
  if(histo->Integral() == 0){
    cerr << "Histogram not filled in the x-axis range you specified" << endl;
    cerr << "Underflow = " << histo->GetBinContent(0) << ", Overflow = " << histo->GetBinContent(histo->GetNbinsX()+1) << endl;
  }
  histo->Scale(1.0/histo->Integral());

  //Plot the histogram and save it
  TCanvas *canvas = new TCanvas("canvas","canvas",800,600);
  histo->Draw();
  canvas->Print("../output/buffer_depth.pdf");

  return 0;
}
