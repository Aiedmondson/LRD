#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;

/// Stepping action class.

class SteppingAction : public G4UserSteppingAction
{
	public:
		SteppingAction(const DetectorConstruction* detectorConstruction,
					   EventAction* eventAction);
		virtual ~SteppingAction();

		virtual void UserSteppingAction(const G4Step* step);
	
	private:
		const DetectorConstruction* fDetConstruction;
		EventAction*  fEventAction;  
};

