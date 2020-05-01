#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Cons.hh"

DetectorConstruction::DetectorConstruction()
	:G4VUserDetectorConstruction(),
	fDet1(nullptr),
	fDet2(nullptr)
{
  SetDefaults();
  fMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
       
	// Get nist material manager
	auto	nist = G4NistManager::Instance();
	
	
	// Build Materials
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* Quartz_mat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
	G4Material* NaI_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
	G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");
	G4Material* SSteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

	// Index of Reflection (435.8 nm to 633.0 nm)
	const G4int nRINDEX = 7;
	G4double photonEnergyRINDEX[nRINDEX]=
	{1.9587*eV,2.1451*eV,2.2704*eV,2.4098*eV,2.5407*eV,2.8450*eV,5.*eV};
	G4double RefractiveIndexGlass[nRINDEX]=
	{1.778,1.786,1.799,1.804,1.814,1.839,1.85};

	// Absorption Length
	const G4int nABS=70;
	G4double photonEnergyABS[nABS]={2.0000*eV,2.2168*eV,2.2570*eV,2.2890*eV,
							  2.3200*eV,2.3559*eV,2.3929*eV,2.4290*eV,
							  2.4617*eV,2.4909*eV,2.5160*eV,2.5417*eV,
							  2.5608*eV,2.5777*eV,2.5948*eV,2.6147*eV,
							  2.6298*eV,2.6503*eV,2.6710*eV,2.6841*eV,
							  2.6974*eV,2.7135*eV,2.7271*eV,2.7518*eV,
							  2.7435*eV,2.7658*eV,2.7799*eV,2.7998*eV,
							  2.8230*eV,2.8465*eV,2.8704*eV,2.9100*eV,
							  2.9508*eV,3.0090*eV,3.0627*eV,3.1078*eV,
							  3.1469*eV,3.1796*eV,3.2055*eV,3.2318*eV,
							  3.2470*eV,3.2663*eV,3.2857*eV,3.3054*eV,
							  3.3294*eV,3.3496*eV,3.3742*eV,3.4034*eV,
							  3.4289*eV,3.4678*eV,3.5075*eV,3.5391*eV,
							  3.5713*eV,3.5992*eV,3.6325*eV,3.6614*eV,
							  3.6760*eV,3.6957*eV,3.7156*eV,3.7256*eV,
							  3.7408*eV,3.7509*eV,3.7662*eV,3.7817*eV,
							  3.7920*eV,3.8130*eV,3.8342*eV,3.8611*eV,
							  3.8994*eV,4.1300*eV};

	G4double AbsLengthGlass[nABS]={.00000,.00000,.00478,.01895,.04210,
							 .06143,.08570,.11805,.14434,.17466,
							 .19892,.23329,.26966,.30200,.33837,
							 .37677,.42325,.46568,.50812,.54449,
							 .58490,.62128,.66775,.70211,.72635,
							 .75262,.79304,.82739,.86175,.89610,
							 .93450,.96281,.98101,.97700,.96086,
							 .93462,.89828,.86799,.83366,.79731,
							 .76903,.74479,.71652,.68623,.65593,
							 .62766,.59332,.56101,.53476,.50447,
							 .47418,.44591,.41966,.38735,.35504,
							 .32474,.29445,.26819,.23991,.20961,
							 .18740,.16316,.13690,.11266,.09044,
							 .06419,.04197,.01976,.00000,.00000};
							 
	// Setting sctintillation to be the emission spectra
	const G4int nEM = 20;
	G4double photonEnergyEM[nEM]={2.1396*eV,2.2157*eV,2.2632*eV,2.3526*eV,
						     2.4233*eV,2.5191*eV,2.5759*eV,2.7796*eV,
						     2.8910*eV,2.9406*eV,2.9790*eV,3.0183*eV,
						     3.1073*eV,3.1905*eV,3.4214*eV,3.5453*eV,
						     3.6589*eV,3.8222*eV,3.8983*eV,4.1272*eV,};

	G4double emNaI[nEM]={0.,0.,.00608,.05878,.11149,.20068,.29189,.79865,
					 .95270,.98514,.98311,.94054,.85541,.55135,.45405,
					 .33041,.05473,0.,0.};

	// Creating the materials property table and adding entries into properties table
	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	MPT->AddProperty("RINDEX",photonEnergyRINDEX,RefractiveIndexGlass,nRINDEX);
	MPT->AddProperty("ABSLENGTH",photonEnergyABS,AbsLengthGlass,nABS);
	// Setting Scintillation Properties
	MPT->AddProperty("FASTCOMPONENT",photonEnergyEM,emNaI,nEM);
	MPT->AddConstProperty("FASTTIMECONSTANT",230*ns);
	MPT->AddConstProperty("SCINTILLATIONYIELD", 38000/MeV);
	MPT->AddConstProperty("YIELDRATIO", 1.0);
	MPT->AddConstProperty("RESOLUTIONSCALE", 61.7);
	
	//MPT->AddConstProperty("WLSTimeProfile","delta");
	//MPT->AddConstProperty("ScintillationExcitationRatio",0.0);
	//MPT->AddConstProperty("MaxNumPhotonsPerStep",100);
	
	NaI_mat->SetMaterialPropertiesTable(MPT);
	//NaI_mat->GetIonisation()->SetBirksConstant(.0052*mm/MeV);

	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;

	//		 
	// World
	//
	G4double world_sizeXY = 100*cm;
	G4double world_sizeZ = 100*cm;
	
	auto solidWorld =		
		new G4Box("World",									//its name
			  	world_sizeXY, world_sizeXY, world_sizeZ);		//its size
			
	auto logicWorld =												 
		new G4LogicalVolume(solidWorld,						//its solid
				    		world_mat,						//its material
				    		"World");							//its name
																	 
	auto physWorld = 
		new G4PVPlacement(0,								//no rotation
				  	   G4ThreeVector(),						//at (0,0,0)
				  	   logicWorld,							//its logical volume
					   "World",							//its name
				  	   0,								//its mother	volume
				  	   false,								//no boolean operation
				  	   0,								//copy number
				  	   checkOverlaps);						//overlaps checking
										 
	
	//Cylinder NaI Detectors
	G4double innerRadius = 0.*cm;
	G4double outerRadius = 7.6*cm;
	G4double hz = 5.1*cm;
	G4double startAngle = 0.*deg;
	G4double spanningAngle = 360.*deg;
	G4Tubs* solidNaIDetector = new G4Tubs("NaIDetector", innerRadius, outerRadius, 
								   hz, startAngle, spanningAngle);
	 
	auto logicShape1 =												 
		new G4LogicalVolume(solidNaIDetector,				//its solid
				    		NaI_mat,						//its material
				    		"NaIDetector");				//its name
						
	auto logicShape2 =												 
		new G4LogicalVolume(solidNaIDetector,				//its solid
				    		NaI_mat,						//its material
				    		"NaIDetector");				//its name
		
	G4ThreeVector pos1 = G4ThreeVector(-5.3*cm-fSpace, 0*cm, 0*cm);
	G4ThreeVector pos2 = G4ThreeVector(5.3*cm+fSpace, 0*cm, 0*cm);
	G4RotationMatrix* myRotation1 = new G4RotationMatrix();
	myRotation1->rotateX(0.*deg);
	myRotation1->rotateY(90.*deg);
	myRotation1->rotateZ(0.*deg);

	fDet1 = new G4PVPlacement(myRotation1,					//rotation
				  		pos1,						//at position
				  		logicShape1,					//its logical volume
				  		"NaIDetector1",				//its name
				  		logicWorld,					//its mother volume
				  		false,						//no boolean operation
				  		0,							//copy number
				  		checkOverlaps);				//overlaps checking

	fDet2 = new G4PVPlacement(myRotation1,					//rotation
				  		pos2,						//at position
				  		logicShape2,					//its logical volume
				 		"NaIDetector2",				//its name
				  		logicWorld,					//its mother volume
				  		false,						//no boolean operation
				  		0,							//copy number
				  		checkOverlaps);				//overlaps checking
	
	
	//Lead Box for setup
	auto leadOuter = new G4Box("leadOuter",50*cm,50*cm,50*cm);
	auto leadInner = new G4Box("leadInner",48*cm,48*cm,48*cm);
	auto solidLeadBox = new G4SubtractionSolid("LogicLeadBox",leadOuter,leadInner);
	auto logicLeadBox = new G4LogicalVolume(solidLeadBox,Pb,"logLeadBox");
	new G4PVPlacement(0,G4ThreeVector(),logicLeadBox,"LeadBox",logicWorld,false,0,checkOverlaps);
	
	auto leadBoxVis = new G4VisAttributes(G4Colour(1.,1.,1.));
	leadBoxVis->SetForceWireframe(true);
	logicLeadBox->SetVisAttributes(leadBoxVis);
	
	
	//NaI surface properties
	G4double mppcPolish=1.0; //sigma_alpha
	auto NaISurface = new G4OpticalSurface("NaISurface", //name
						      glisur, //NaISurface->SetModel(glisur);
						      polished, //NaISurface->SetFinish(polished);
						      dielectric_metal, //NaISurface->SetType(dielectric_metal);
						      mppcPolish); //NaISurface->SetSigmaAlpha(mppcPolish);

	auto NaISurfaceProperty = new G4MaterialPropertiesTable();

	const G4int nEntries = 8; 
	G4double p_mppc[nEntries] = { 2.364*eV, 2.482*eV,//525nm, 500nm 
				2.613*eV, 2.758*eV,//475nm,450nm
				2.92*eV, 3.103*eV,//425nm,400nm
				3.309*eV, 3.546*eV};//375nm,350nm

	G4double refl_mppc[nEntries] = {1., 1., 1., 1., 1., 1., 1., 1.};
	G4double effi_mppc[nEntries] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; //minimum efficiency (=no detection of hits)

	NaISurfaceProperty -> AddProperty("REFLECTIVITY",p_mppc,refl_mppc,nEntries);
	NaISurfaceProperty -> AddProperty("EFFICIENCY",p_mppc,effi_mppc,nEntries);

	NaISurface -> SetMaterialPropertiesTable(NaISurfaceProperty);
 
	new G4LogicalBorderSurface("NaISurface1",physWorld,fDet1,NaISurface);
	new G4LogicalBorderSurface("NaISurface2",physWorld,fDet2,NaISurface);
	
	//Casing for detector
	auto casingBot = new G4Tubs("casingBot",7.8*cm,8.0*cm,5.8*cm,0.*deg,360.*deg);
	auto casingEx1 = new G4Tubs("casingEx1",8.0*cm,9.5*cm,0.75*cm,0.*deg,360.*deg);
	auto casingMid = new G4Tubs("casingMid",6.9*cm,7.1*cm,3.55*cm,0.*deg,360.*deg);
	auto casingEx2 = new G4Tubs("casingEx2",7.1*cm,8.0*cm,0.45*cm,0.*deg,360.*deg);
	auto casingCon = new G4Cons("casingCon",2.7*cm,2.9*cm,6.9*cm,7.1*cm,2.1*cm,0.*deg,360*deg);
	auto casingTop = new G4Tubs("casingTop",2.7*cm,2.9*cm,5.3*cm,0.*deg,360.*deg);
	auto casingCap = new G4Tubs("casingCap",0.*cm,2.7*cm,0.1*cm,0.*deg,360.*deg);
	auto casingCvr = new G4Tubs("casingCvr",0.*cm,8.0*cm,0.1*cm,0.*deg,360.*deg);
	
	auto logicCasingBot = new G4LogicalVolume(casingBot,SSteel,"logicCasingBot");
	auto logicCasingEx1 = new G4LogicalVolume(casingEx1,SSteel,"logicCasingEx1");
	auto logicCasingMid = new G4LogicalVolume(casingMid,SSteel,"logicCasingMid");
	auto logicCasingEx2 = new G4LogicalVolume(casingEx2,SSteel,"logicCasingEx2");
	auto logicCasingCon = new G4LogicalVolume(casingCon,SSteel,"logicCasingCon");
	auto logicCasingTop = new G4LogicalVolume(casingTop,SSteel,"logicCasingTop");
	auto logicCasingCap = new G4LogicalVolume(casingCap,SSteel,"logicCasingCap");
	auto logicCasingCvr = new G4LogicalVolume(casingCvr,SSteel,"logicCasingCvr");
	
	G4RotationMatrix* rotation1 = new G4RotationMatrix();
	rotation1->rotateX(0.*deg);
	rotation1->rotateY(90.*deg);
	rotation1->rotateZ(0.*deg);
	G4RotationMatrix* rotation2 = new G4RotationMatrix();
	rotation2->rotateX(0.*deg);
	rotation2->rotateY(-90.*deg);
	rotation2->rotateZ(0.*deg);
	
	new G4PVPlacement(rotation1,G4ThreeVector(6.0*cm+fSpace, 0.*cm, 0.*cm),logicCasingBot,"casingBot1",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(11.05*cm+fSpace, 0.*cm, 0.*cm),logicCasingEx1,"casingEx11",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(15.35*cm+fSpace, 0.*cm, 0.*cm),logicCasingMid,"casingMid1",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(12.25*cm+fSpace, 0.*cm, 0.*cm),logicCasingEx2,"casingEx21",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(21.0*cm+fSpace, 0.*cm, 0.*cm),logicCasingCon,"casingCon1",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(28.4*cm+fSpace, 0.*cm, 0.*cm),logicCasingTop,"casingTop1",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(33.8*cm+fSpace, 0.*cm, 0.*cm),logicCasingCap,"casingCap1",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation1,G4ThreeVector(0.1*cm+fSpace, 0.*cm, 0.*cm),logicCasingCvr,"casingCvr1",logicWorld,false,0,checkOverlaps);
	
	new G4PVPlacement(rotation2,G4ThreeVector(-6.0*cm-fSpace, 0.*cm, 0.*cm),logicCasingBot,"casingBot2",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-11.05*cm-fSpace, 0.*cm, 0.*cm),logicCasingEx1,"casingEx12",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-15.35*cm-fSpace, 0.*cm, 0.*cm),logicCasingMid,"casingMid2",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-12.25*cm-fSpace, 0.*cm, 0.*cm),logicCasingEx2,"casingEx22",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-21.0*cm-fSpace, 0.*cm, 0.*cm),logicCasingCon,"casingCon2",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-28.4*cm-fSpace, 0.*cm, 0.*cm),logicCasingTop,"casingTop2",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-33.8*cm-fSpace, 0.*cm, 0.*cm),logicCasingCap,"casingCap2",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,G4ThreeVector(-0.1*cm-fSpace, 0.*cm, 0.*cm),logicCasingCvr,"casingCvr2",logicWorld,false,0,checkOverlaps);
	
	//Quartz Piece
	auto qrtzCrystal = new G4Tubs("qrtzCrystal",0*cm,6.9*cm,0.75*cm,0.*deg,360.*deg);
	auto logicQrtzCrystal = new G4LogicalVolume(qrtzCrystal,Quartz_mat,"logicQuartzCrystal");
	auto Quartz1 = new G4PVPlacement(rotation2,G4ThreeVector(-11.15*cm-fSpace, 0.*cm, 0.*cm),logicQrtzCrystal,"QrtzCrystal2",logicWorld,false,0,checkOverlaps);
	auto Quartz2 = new G4PVPlacement(rotation1,G4ThreeVector(11.15*cm+fSpace, 0.*cm, 0.*cm),logicQrtzCrystal,"QrtzCrystal1",logicWorld,false,0,checkOverlaps);
	
	G4double quartz_Energy[73]={0.086788957*eV,0.096707687*eV,0.106626373*eV,
			  0.116545113*eV,0.126463891*eV,0.136382593*eV,
			  0.146301277*eV,0.156220036*eV,0.166138735*eV,
			  0.176057634*eV,0.185976188*eV,0.195895138*eV,
			  0.205813628*eV,0.215732352*eV,0.225651036*eV,
			  0.235569862*eV,0.245488451*eV,0.25540737*eV,
			  0.265326002*eV,0.275245171*eV,0.285163374*eV,
			  0.295082413*eV,0.30500115*eV,0.314919666*eV,
			  0.324838902*eV,0.334757641*eV,0.344676262*eV,
			  0.354595131*eV,0.364513569*eV,0.374432051*eV,
			  0.384350558*eV,0.394270229*eV,0.404189065*eV,
			  0.414107459*eV,0.424025429*eV,0.43394509*eV,
			  0.44386277*eV,0.453782199*eV,0.463700305*eV,
			  0.473619785*eV,0.483538815*eV,0.493457619*eV,
			  0.503376657*eV,0.51329431*eV,0.523212884*eV,
			  0.533132326*eV,0.543049557*eV,0.552970084*eV,
			  0.562889022*eV,0.572807519*eV,0.582725564*eV,
			  0.592644475*eV,0.602564079*eV,0.612482339*eV,
			  0.622401206*eV,0.632320747*eV,0.642238733*eV,
			  0.652157061*eV,0.66207527*eV,0.671993731*eV,
			  0.681913713*eV,0.691833578*eV,0.701752269*eV,
			  0.71166935*eV,0.721589255*eV,0.731504658*eV,
			  0.741425797*eV,0.751341907*eV,0.761265012*eV,
			  0.771180227*eV,0.781101162*eV,0.791018167*eV,
			  0.800936617*eV};
	G4double quartz_RIND[73]={1.75483,1.78543,1.76132,2.13818,2.94379,0.84093,
				0.56717,0.56351,0.95629,1.10394,1.18289,1.23338,
				1.26877,1.29504,1.31532,1.33145,1.34457,1.35545,
				1.36459,1.37238,1.37908,1.3849,1.38999,1.39448,
				1.39847,1.40202,1.40521,1.40807,1.41067,1.41302,
				1.41517,1.41713,1.41893,1.42058,1.4221,1.42351,
				1.42481,1.42602,1.42714,1.42819,1.42917,1.43008,
				1.43093,1.43173,1.43249,1.43319,1.43386,1.43449,
				1.43508,1.43564,1.43617,1.43668,1.43715,1.4376,
				1.43803,1.43844,1.43883,1.4392,1.43955,1.43989,
				1.44021,1.44052,1.44081,1.44109,1.44136,1.44162,
				1.44187,1.4421,1.44233,1.44255,1.44276,1.44296,
				1.44315};
	G4double quartz_AbsLength[73]={14.39677512,4.71809389,8.24198467,
			     19.88466892,0.956992746,0.491963772,
			     1.248330358,5.467767511,84.67400508,
			     218.8183807,346.0207612,473.9336493,
			     609.7560976,757.5757576,909.0909091,
			     1086.956522,1265.822785,1470.588235,
			     1694.915254,1923.076923,2173.913043,
			     2439.02439,2702.702703,3030.30303,
			     3333.333333,3703.703704,4166.666667,
			     4545.454545,5000,5263.157895,5882.352941,
			     6250,6666.666667,7142.857143,7692.307692,
			     8333.333333,9090.909091,10000,11111.11111,
			     11111.11111,12500,12500,14285.71429,
			     14285.71429,14285.71429,16666.66667,
			     16666.66667,20000,20000,20000,20000,25000,
			     25000,25000,25000,25000,33333.33333,
			     33333.33333,33333.33333,33333.33333,
			     33333.33333,33333.33333,33333.33333,
			     33333.33333,50000,50000,50000,50000,50000,
			     50000,50000,50000,50000};
	G4MaterialPropertiesTable* quartz_mt = new G4MaterialPropertiesTable();
	quartz_mt->AddProperty("ABSLENGTH",quartz_Energy,quartz_AbsLength,73);
	quartz_mt->AddProperty("RINDEX",quartz_Energy,quartz_RIND,73);
	Quartz_mat->SetMaterialPropertiesTable(quartz_mt);

	G4double polish2=1.0; //sigma_alpha
	auto QuartzSurface = new G4OpticalSurface("QuartzSurface", //name
						      glisur,
						      polished,
						      dielectric_dielectric,
						      polish2);

	auto QuartzSurfaceProperty = new G4MaterialPropertiesTable();

	const G4int nEntries2 = 8; 
	G4double p_mppc2[nEntries] = { 2.364*eV, 2.482*eV,//525nm, 500nm 
				2.613*eV, 2.758*eV,//475nm,450nm
				2.92*eV, 3.103*eV,//425nm,400nm
				3.309*eV, 3.546*eV};//375nm,350nm

	G4double refl_mppc2[nEntries] = {0., 0., 0., 0., 0., 0., 0., 0.};
	G4double effi_mppc2[nEntries] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; //minimum efficiency (=no detection of hits)

	QuartzSurfaceProperty -> AddProperty("REFLECTIVITY",p_mppc2,refl_mppc2,nEntries2);
	QuartzSurfaceProperty -> AddProperty("EFFICIENCY",p_mppc2,effi_mppc2,nEntries2);

	QuartzSurface -> SetMaterialPropertiesTable(NaISurfaceProperty);
 
	new G4LogicalBorderSurface("QuartzSurface1",Quartz1,fDet1,QuartzSurface);
	new G4LogicalBorderSurface("QuartzSurface2",Quartz2,fDet2,QuartzSurface);

	G4double air_Energy[3]={2.0*eV,7.0*eV,7.14*eV};
	G4double air_RIND[3]={1.,1.,1.};
	G4MaterialPropertiesTable *air_mt = new G4MaterialPropertiesTable();
	air_mt->AddProperty("RINDEX", air_Energy, air_RIND,3);
	world_mat->SetMaterialPropertiesTable(air_mt);


	//
	//always return the physical World
	//
	return physWorld;
}

//Commands for modifying geometry
void DetectorConstruction::SetSpacing(G4double space) {
  fSpace=space;
  G4cout << "Spacing set to: " << space << G4endl;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetDefaults() {
  //Resets to default values
  fSpace=4.7*cm;
}


