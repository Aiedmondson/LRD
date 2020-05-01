#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include <iomanip>
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
	:G4UserEventAction(),
	fEnergyDet1(0.),
	fEnergyDet2(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* )
{

	// Set beginning deposition as 0
	fEnergyDet1 = 0.;
	fEnergyDet2 = 0.;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	auto analysisManager = G4AnalysisManager::Instance();
	//Set energy deposition to -1 eV if there was no detection
	if (fEnergyDet1 == 0*eV){
		fEnergyDet1 = -1*eV;
	}
	if (fEnergyDet2 == 0*eV){
		fEnergyDet2 = -1*eV;
	}
	
	// Fill histograms
	analysisManager->FillH1(0, fEnergyDet1);
	analysisManager->FillH1(1, fEnergyDet2);
	if (fEnergyDet1 == -1*eV){
		analysisManager->FillH1(2, fEnergyDet1);
	}
	else { //Calibrate data to shift energies to correct positioning
		analysisManager->FillH1(2, fEnergyDet1*0.8688-0.0139);
	}
	if (fEnergyDet2 == -1*eV){
		analysisManager->FillH1(3, fEnergyDet2);
	}
	else { //Calibrate data to shift energies to correct positioning
		analysisManager->FillH1(3, fEnergyDet2*0.8688-0.0139);
	}
	
	// Fill ntuple
	analysisManager->FillNtupleDColumn(0, fEnergyDet1);
	analysisManager->FillNtupleDColumn(1, fEnergyDet2);
	if (fEnergyDet1 == -1*eV){
		analysisManager->FillNtupleDColumn(2, fEnergyDet1);
	}
	else { //Calibrate data to shift energies to correct positioning
		analysisManager->FillNtupleDColumn(2, fEnergyDet1*0.8688-0.0139);
	}
	if (fEnergyDet2 == -1*eV){
		analysisManager->FillNtupleDColumn(3, fEnergyDet2);
	}
	else { //Calibrate data to shift energies to correct positioning
		analysisManager->FillNtupleDColumn(3, fEnergyDet2*0.8688-0.0139);
	}
	analysisManager->AddNtupleRow();	
	
	// Print per event
	//
	auto eventID = event->GetEventID();
	auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
	if ((printModulo > 0) && (eventID % printModulo == 0)) {
		G4cout << "---> End of event: " << eventID << G4endl;		 

		G4cout
			 << "Det1: total energy: " << G4BestUnit(fEnergyDet1,"Energy")
			 << G4endl
			 << "Det2: total energy: " << G4BestUnit(fEnergyDet2,"Energy")
			 << G4endl;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
