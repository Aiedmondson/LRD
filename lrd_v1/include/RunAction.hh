#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///

class RunAction : public G4UserRunAction
{
	public:
		RunAction();
		virtual ~RunAction();

		virtual void BeginOfRunAction(const G4Run*);
		virtual void   EndOfRunAction(const G4Run*);
};

