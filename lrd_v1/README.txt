-------------------------------------------------------------------

     =========================================================
     Geant4 - an Object-Oriented Toolkit for Simulation in HEP
     =========================================================

                        Project LRD v1.1.1 
                        ------------------

 This project simulates a simple dual detector NaI(Tl) setup to
 detect low level radiation without noise from background radiation.
 Hence the name is the Low Radiation Detetector (LRD).
	
 1- GEOMETRY DEFINITION
 
   The geometry is constructed in DetectorConstruction class.
   The scintillating crystals are cylinders made of NaI(Tl) and 
   have the same dimensions and properties.

   |<------Detector 1------>|                      |<------Detector 2------>|
   |                        |     Gamma Source     |                        |
   ==========================                      ==========================
   ||                      ||                      ||                      ||
   ||                      ||                      ||                      ||
   ||                      ||           o          ||                      ||
   ||                      ||                      ||                      ||
   ||                      ||                      ||                      ||
   ==========================                      ==========================

 2- PHYSICS LIST
 
   The physic processes which will be available in this example are set in 
   the FTFP_BERT physics list.
  
 3- ACTION INITALIZATION

   A new class, ActionInitialization, instantiates and registers 
   to Geant4 kernel all user action classes.
   
   While in sequential mode the action classes are instatiated just once,
   via invoking the method:
      ActionInitialization::Build() 
   in multi-threading mode the same method is invoked for each thread worker
   and so all user action classes are defined thread-local.

   A run action class is instantiated both thread-local 
   and global that's why its instance is created also in the method
   ActionInitialization::BuildForMaster()
   which is invoked only in multi-threading mode.

 4- PRIMARY GENERATOR
 
   The primary form of particle generation is through the General
   Particle Source and is harnessed through the run1.mac macro in
   order to simulate a Co-60 source. It produces photons of specific
   energies in order to simulate the radioactive decay of Co-60. To
   change the energies, simply go into co60Source.mac and change the
   energy under either beam. You can also copy or remove beams as you
   see fit by using the same format already provided. To change the
   number of particles emitted, go into run1.mac and change the value
   for the beamOn command.
 
 5- RUNS and EVENTS
 
   A run is a set of events. Progress that is recorded in ROOT histograms
   are reported at the end of each run. This file will be stored where
   the program is compiled as "LRD.root".
   
 6- DETECTOR RESPONSE
 
   The energy deposit photons are recorded on an event by event
   basis in the two distinct detectors
   
     These quantities are data members of the EventAction class.  
     They are collected step by step in 
     SteppingAction::UserSteppingAction(), and passed to the event action
     via two methods: EventAction::AddDet1() and EventAction::AddDet2().
   
     In EventAction::EndOfEventAction(), these quantities are printed and 
     filled in H1D histograms and ntuples to accumulate statistics and compute 
     dispersion.
       
  7- HISTOGRAMS
 
   The analysis tools are used to accumulate statistics and compute the dispersion
   of the energy deposit of the photons.
   H1D histograms are created in RunAction::RunAction() for the 
   following quantities:
   - Energy deposit in Det1
   - Energy deposit in Det2
   - Calibrated energy deposit in Det1
   - Calibrated energy deposit in Det2
   The same values are also saved in an ntuple. The calibration is to shift the energies to the correct values and is done when the values are
   stored. This is to counterbalance the positive shift the optical photons create when their energies are recorded.

   The histograms and the ntuple are saved in the output file in a format
   according to a technology selected in Analysis.hh, the default in this project
   is ROOT.

   The accumulated statistic and computed dispersion is printed at the end of 
   run, in RunAction::EndOfRunAction().
   When running in multi-threading mode, the histograms and the ntuple accumulated 
   on threads are merged in a single output file. While merging of histograms is
   performed by default, merging of ntuples is explicitly activated in the RunAction 
   constructor.

  8- VISUALIZATION TUTORIAL
  
   For details, see comment lines  described in the vis macro file.
   By default, vis.mac opens an OpenGL viewer (/vis/open OGL).
   The tracks are automatically drawn at the end of each event, accumulated
   for all events and erased at the beginning of the next run.
   
 9- HOW TO RUN
 
    This example is run after compilation with the following
    command once in the build directory. The example must first be built
    with cmake and make before the command listed below can be used. 
    $ ./lrdrun
    
    To Build:
    $ cd ../lrd_v1/build
    $ cmake ..
    $ make 			(OR optional 'make -jn' where n is core count)
    $ ./lrdrun 		(For visual)
    $ ./lrdrun run1.mac 	(For batch running)
