#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "DetectorMessenger.hh"

class G4VPhysicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction();
		virtual ~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();
		const G4VPhysicalVolume* GetDet1() const;
		const G4VPhysicalVolume* GetDet2() const;
		void SetDefaults();
		void SetSpacing(G4double);
		G4double GetSpacing() const {return fSpace;}
		
	private:
		DetectorMessenger* fDetectorMessenger;
		G4double fSpace;          //Space between detectors
		DetectorMessenger* fMessenger;
		G4bool	CheckOverlaps;  //Checks volumes for overlap
		G4VPhysicalVolume* fDet1; //Sensitive detector 1
		G4VPhysicalVolume* fDet2; //Sensitive detector 2
};
/*
class Materials{

	public:
		~Materials();
		static Materials* GetInstance();
		G4Material* GetMaterial(const G4String);
	
	private:
		// Methods
		Materials();
		void SetOpticalProperties();

		// Fields
		static Materials* instance;
		G4NistManager* nistMan;
	};
*/
inline const G4VPhysicalVolume* DetectorConstruction::GetDet1() const { 
	return fDet1; //returns physical volume for fdet1
}

inline const G4VPhysicalVolume* DetectorConstruction::GetDet2() const { 
	return fDet2; //returns physical volume for fdet2
}



