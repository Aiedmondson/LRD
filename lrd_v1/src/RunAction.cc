#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
	:G4UserRunAction()
{
	G4RunManager::GetRunManager()->SetPrintProgress(1);
	
	// Create analysis manager
	// The choice of analysis technology is done via selectin of a namespace
	// in Analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using " << analysisManager->GetType() << G4endl;
	
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);

	// Creating histograms
	//
	analysisManager->CreateH1("EDet1","Edep in Det1", 800, 0.0, 2.85*MeV);
	analysisManager->CreateH1("EDet2","Edep in Det2", 800, 0.0, 2.85*MeV);
	analysisManager->CreateH1("EDet1cal","Calibrated Edep in Det2", 800, 0.0, 2.85*MeV);
	analysisManager->CreateH1("EDet2cal","Calibrated Edep in Det2", 800, 0.0, 2.85*MeV);
 
	// Creating ntuple
	//
	analysisManager->CreateNtuple("LRD", "Edep");
	analysisManager->CreateNtupleDColumn("EDet1");
	analysisManager->CreateNtupleDColumn("EDet2");
	analysisManager->CreateNtupleDColumn("EDet1cal");
	analysisManager->CreateNtupleDColumn("EDet2cal");
	analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete G4AnalysisManager::Instance(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* )
{ 
	
	auto analysisManager = G4AnalysisManager::Instance();
	
	// Open an output file
	//
	G4String fileName = "LRD";
	analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* )
{
  
	// Print histogram statistics
	//
	auto analysisManager = G4AnalysisManager::Instance();
	if ( analysisManager->GetH1(1) ) {
		G4cout << "\n ----> print histograms statistic \n" << G4endl;
		
		G4cout 
			<< "EDet1: mean = " << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
			<< "rms = " << G4BestUnit(analysisManager->GetH1(0)->rms(),	"Energy") 
			<< G4endl;
		G4cout 				 
			<< "EDet2: mean = " << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
			<< "rms = " << G4BestUnit(analysisManager->GetH1(1)->rms(),	"Energy") 
			<< G4endl;
	}
	
	// Save histograms 
	//
	analysisManager->Write();
	analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
