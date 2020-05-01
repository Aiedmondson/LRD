#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcommand;

class DetectorMessenger: public G4UImessenger
{
  public:

    DetectorMessenger(DetectorConstruction*);
    virtual ~DetectorMessenger();
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    DetectorConstruction*        fDetector;
    G4UIdirectory*               fDetectorDir;
    G4UIdirectory*               fVolumesDir;
    G4UIcmdWithADoubleAndUnit*   fSpacingCmd;
    G4UIcommand*                 fDefaultsCmd;
};

