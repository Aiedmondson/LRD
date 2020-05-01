#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"


#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	// Detect interactive mode (if no arguments) and define UI session
	//
	G4UIExecutive* ui = 0;
	if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}

	// Choose the Random engine
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	
	// Construct the default run manager
	//
	#ifdef G4MULTITHREADED
		auto runManager = new G4MTRunManager;
	#else
		auto runManager = new G4RunManager;
	#endif

	// Set mandatory initialization classes
	//

	auto detConstruction = new DetectorConstruction();
	runManager->SetUserInitialization(detConstruction);

	auto thePL = new FTFP_BERT;
	thePL->ReplacePhysics(new G4EmStandardPhysics_option4());
	auto opticalphysics = new G4OpticalPhysics();
//	opticalphysics->SetWLSTimeProfile("delta");
//	opticalphysics->SetScintillationYieldFactor(1.0);
//	opticalphysics->SetScintillationExcitationRatio(0.0);
//	opticalphysics->SetMaxNumPhotonsPerStep(100);
//	opticalphysics->SetMaxBetaChangePerStep(10.0);
//	opticalphysics->SetTrackSecondariesFirst(kScintillation,true);
	thePL->RegisterPhysics( opticalphysics );
	runManager->SetUserInitialization( thePL );

	auto actionInitialization = new ActionInitialization(detConstruction);
	runManager->SetUserInitialization(actionInitialization);

	// Initialize visualization
	//
	auto visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	auto UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//
	if ( ! ui ) { 
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else { 
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program !
	
	delete visManager;
	delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
