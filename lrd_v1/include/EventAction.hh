#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class EventAction : public G4UserEventAction
{
	public:
		EventAction();
		virtual ~EventAction();

		virtual void BeginOfEventAction(const G4Event* event);
		virtual void EndOfEventAction(const G4Event* event);

		void AddDet1(G4double de); //Declares function to add energy
		void AddDet2(G4double de);

	private:
		G4double fEnergyDet1; //records energy deposition
		G4double fEnergyDet2;
};

inline void EventAction::AddDet1(G4double de) {
	fEnergyDet1 += de; //records change in energy in fdet1
}

inline void EventAction::AddDet2(G4double de) {
	fEnergyDet2 += de; //records change in energy in fdet2
}

	
