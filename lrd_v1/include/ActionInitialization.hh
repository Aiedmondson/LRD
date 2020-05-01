#include "G4VUserActionInitialization.hh"

class DetectorConstruction;

/// Action initialization class.
///

class ActionInitialization : public G4VUserActionInitialization
{
	public:
		ActionInitialization(DetectorConstruction*);
		virtual ~ActionInitialization();

		virtual void BuildForMaster() const;
		virtual void Build() const;

	private:
		DetectorConstruction* fDetConstruction;
};
