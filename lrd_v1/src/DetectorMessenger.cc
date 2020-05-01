#include "DetectorConstruction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"
#include "G4RunManager.hh"
#include "G4UIcommand.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
  : G4UImessenger(), 
    fDetector(detector)
  {
    //Setup a command directory for detector controls with guidance
    fDetectorDir = new G4UIdirectory("/Lrd/");
    fDetectorDir->SetGuidance("UI simulation control");
    
    fVolumesDir = new G4UIdirectory("/Lrd/detector/");
    fVolumesDir->SetGuidance("Detectors geometry control");
 
    //Various commands for modifying detector geometry
    fSpacingCmd = new G4UIcmdWithADoubleAndUnit("/Lrd/detector/spacing",this);
    fSpacingCmd->SetGuidance("Set the thickness of the housing.");
    fSpacingCmd->SetParameterName("space",false);
    fSpacingCmd->SetDefaultUnit("cm");
    fSpacingCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fSpacingCmd->SetToBeBroadcasted(false);
    
    fDefaultsCmd = new G4UIcommand("/Lrd/detector/defaults",this);
    fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
    fDefaultsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    fDefaultsCmd->SetToBeBroadcasted(false);
  }
  
DetectorMessenger::~DetectorMessenger()
  {
    delete fDetectorDir;
    delete fVolumesDir;
    delete fSpacingCmd;
    delete fDefaultsCmd;
  }

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fSpacingCmd){
    fDetector->SetSpacing(fSpacingCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fDefaultsCmd){
    fDetector->SetDefaults();
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}






