#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
	const DetectorConstruction* detectorConstruction, EventAction* eventAction)
	:G4UserSteppingAction(),
	fDetConstruction(detectorConstruction),
	fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step
	// Get volume of the current step
	auto volume1 = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	//auto edep_op = step->GetPostStepPoint()->GetTotalEnergyDeposit();
	
	// Energy deposit
	auto edep = step->GetTotalEnergyDeposit();
	
		if (volume1 == fDetConstruction->GetDet1()) {
			fEventAction->AddDet1(edep);
		}
		
		if (volume1 == fDetConstruction->GetDet2()) {
			fEventAction->AddDet2(edep);
		}
}


