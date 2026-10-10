// This script simulates a p-p collision in different ranges of energy, but for a fixed cut in
// eta and pT.
// Number of events, eta and pT are mutable variables, as well as the values of energy.
// It's highly recommended to change the name of the files according to the eta cut and the 
// number of generated events.

#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8/AlphaClusterModel.h"
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main()
{
    // root files
    TFile *output = new TFile("../results/p_O_Collisions/Multiplicity/pO_ACMultiplicity_9p62TeVTESTE.root", "recreate");

    // Histograms
    TH1D* eta0p5 = new TH1D("Multiplicity 9p62 TeV eta 0.5", "Multiplicity 9p62 TeV eta 0.5; N_{ch}; P(N_{ch})", 500, -0.5, 499.5);
    TH1D* eta1p0 = new TH1D("Multiplicity 9p62 TeV eta 1.0", "Multiplicity 9p62 TeV eta 1.0; N_{ch}; P(N_{ch})", 500, -0.5, 499.5);
    //TH1D* eta2p4 = new TH1D("Multiplicity 5.02 TeV eta 2.0", "Multiplicity 5.02 TeV eta 2.0; N_{ch}; P(N_{ch})", 500, -0.5, 499.5);
    //TH1D* eta3p4 = new TH1D("Multiplicity 5.02 TeV eta 3.4", "Multiplicity 5.02 TeV eta 3.4; N_{ch}; P(N_{ch})", 500, -0.5, 499.5);
    //TH1D* eta5p0 = new TH1D("Multiplicity 5.02 TeV eta 5.0", "Multiplicity 5.02 TeV eta 5.0; N_{ch}; P(N_{ch})", 500, -0.5, 499.5);
    //TH1D* Ncoll0p5 = new TH1D("Ncoll 5.02 TeV", "Ncoll 5.02 TeV; N_{coll}; P(N_{coll})", 500, -0.5, 499.5);
    //TH1D* Npart = new TH1D("Npart 5.02 TeV", "Npart 5.02 TeV; N_{part}; P(N_{part})", 500, -0.5, 499.5);
    //TH2D* NcollMult0p5 = new TH2D("Ncoll vs Mult 5p02 TeV eta 0.5", "Ncoll vs Mult 5p02 TeV eta 0.5; N_{ch}; N_{coll}", 500, -0.5, 499.5, 500, -0.5, 499.5);
    //TH2D* NcollMult1p0 = new TH2D("Ncoll vs Mult 5p02 TeV eta 1.0", "Ncoll vs Mult 5p02 TeV eta 1.0; N_{ch}; N_{coll}", 500, -0.5, 499.5, 500, -0.5, 499.5);
    //TH1D* eta2p0 = new TH1D("Multiplicity 5p02 TeV eta 2.0", "Multiplicity 5p02 TeV eta 2.0; N_{ch}; N_{ch}", 500, -0.5, 499.5);
    //TH1D* eta3p0 = new TH1D("Multiplicity 5p02 TeV eta 3.0", "Multiplicity 5p02 TeV eta 3.0; N_{ch}; N_{ch}", 500, -0.5, 499.5);

    // Relevant Variables
    int nEvents = 1e5; // 10.000.000 events 
    //double pT_min = 0.15, pT_max = 10; //GeV
    double eta1 = 0.5, eta2 = 1.0;//, eta3 = 2.0, eta4 = 3.4, eta5 = 5.0; // pseudorapidity cuts
    //double eta3 = 2.0, eta4 = 3.4, eta5 = 5.0;
    // Number of charged particles found in each eta cut
    int nCharged_1p0, nCharged_0p5, nCharged_2p4 = 0, nCharged_3p4 = 0, nCharged_5p0 = 0;
    // Energy to be simulated
    double energy = 5020; // GeV

        // Define Pythia instance
        Pythia pythia;

        // Setup the beams
        pythia.readString("Beams:idA = 2212");
        pythia.readString("Beams:idB = 2212");
        //pythia.readString("Beams:idB = 1000822080");    // Pb Nucleus   
        //pythia.readString("Beams:idB = 1000080160");     // O Nucleus 1000080160
        pythia.readString("Beams:eCM = 9620");             // GeV
        pythia.readString("SoftQCD:all = on");
        pythia.readString("HeavyIon:mode = 1");          // Activates Angantyr
        // monash tune
        //pythia.readString("Tune:ee = 7");
        //pythia.readString("Tune:pp = 14");
        //pythia.readString("HeavyIon:AlphaCluster = on");   // Activates Alpha Cluster Geometry
        // Defining Woods-Saxon parameters
        //pythia.readString("Angantyr:NucleusModelB = 2");
        //pythia.readString("HeavyIonB:WSR = 0.964");      // R parameter for O Alpha Cluster Distribution
        //pythia.readString("HeavyIonB:WSa = 0.322");      // a parameter for O Alpha Cluster Distribution
        //pythia.readString("HeavyIonB:ACw = 0.517");        // w parameter for Alpha Cluster Distribution
        //pythia.readString("HeavyIonB:WSR = 2.608");      // R parameter for O Woods-Saxon Distribution
        //pythia.readString("HeavyIonB:WSa = 0.513");      // a parameter for O Woods-Saxon Distribution
        //pythia.readString("HeavyIonB:WSR = 6.620");       /// R parameter for Pb Woods-Saxon Distribution
        //pythia.readString("HeavyIonB:WSa = 0.550");       /// a parameter for Pb Woods-Saxon Distribution

        // Pythia initialization
        if (!pythia.init()) {
            std::cerr << "Error: Pythia initialization failed." << std::endl;
            return 1;
        }

        // Loop over events
        for(int i = 0; i < nEvents; i++)
        {
            if(!pythia.next()) continue;

            // Exhibits how many events already ran
            if (i % 1000 == 0 )
            {
                std::cout << "Events : " << i << "/" << nEvents << std::endl;
            }

            // Set counters of charged particles as 0 at the beggining of each event
            nCharged_0p5 = 0, nCharged_1p0 = 0, nCharged_2p4 = 0, nCharged_3p4 = 0, nCharged_5p0 = 0;

            // Defining the entries of each event
            int entries = pythia.event.size();

            // Number of collisions and number of participants in the event
            //double nColl = pythia.info.hiInfo -> nCollTot(); //  HIInfo::nCollTot();
            //double nPartProj = pythia.info.hiInfo -> nPartProj();
            //double nPartTarg = pythia.info.hiInfo -> nPartTarg();
            //double nPart = nPartProj + nPartTarg;
            
            //Ncoll0p5 -> Fill(nColl);
            //Npart -> Fill(nPart);

            // Loop over entries
            for(int j = 0; j < entries; j++)
            {
                // Flags for final event and if it is a charged particle
                if(pythia.event[j].isFinal() && pythia.event[j].isCharged());// && pythia.event[j].pT() > pT_min && pythia.event[j].pT() < pT_max)
                {
                    // Evaluates charged particle for each eta cut
                    //if (pythia.event[j].eta() < eta5 && pythia.event[j].eta() > -eta4) nCharged_5p0++;
                    //if (abs(pythia.event[j].eta()) < eta4) nCharged_3p4++;
                    //if (abs(pythia.event[j].eta()) < eta3) nCharged_2p4++;
                    if (abs(pythia.event[j].eta()) < eta2) nCharged_1p0++;
                    if (abs(pythia.event[j].eta()) < eta1) nCharged_0p5++;
                    
                }
            }

            // Stocking values in the histograms
            //if (nCharged_5p0 > 0) eta5p0 -> Fill(nCharged_5p0);
            //if (nCharged_3p4 > 0) eta3p4 -> Fill(nCharged_3p4);
            //if (nCharged_2p4 > 0) eta2p4 -> Fill(nCharged_2p4);
            if (nCharged_1p0 > 0) eta1p0 -> Fill(nCharged_1p0);
            if (nCharged_0p5 > 0) eta0p5 -> Fill(nCharged_0p5);
            //NcollMult0p5 -> Fill(nCharged_0p5, nColl);
            //NcollMult1p0 -> Fill(nCharged_1p0, nColl);
        
        }


    // make clone
    //TH1D* Ncoll0p5_norm = (TH1D*)Ncoll0p5 -> Clone("Ncoll0p5_norm");
    //TH1D* Npart_norm = (TH1D*)Npart -> Clone("Npart_norm");
    TH1D* eta0p5_norm = (TH1D*)eta0p5 -> Clone("eta0p5_norm");
    TH1D* eta1p0_norm = (TH1D*)eta1p0 -> Clone("eta1p0_norm");
    //TH1D* eta2p4_norm = (TH1D*)eta2p4 -> Clone("eta2p4_norm");
    //TH1D* eta3p4_norm = (TH1D*)eta3p4 -> Clone("eta3p4_norm");
    //TH1D* eta5p0_norm = (TH1D*)eta5p0 -> Clone("eta5p0_norm");

    // After all the events, normalize histograms
    //eta5p0_norm -> Scale(1.0/ eta5p0 -> GetEntries(), "width");
    //eta3p4_norm -> Scale(1.0/ eta3p4 -> GetEntries(), "width");
    //eta2p4_norm -> Scale(1.0/ eta2p4 -> GetEntries(), "width");
    eta1p0_norm -> Scale(1.0/ eta1p0 -> GetEntries(), "width");
    eta0p5_norm -> Scale(1.0/ eta0p5 -> GetEntries(), "width");
    //Ncoll0p5_norm -> Scale(1.0 / Ncoll0p5 -> GetEntries(), "width");
    //Npart_norm -> Scale(1.0 / Npart -> GetEntries(), "width");

    // Writing information
    //eta3p0 -> Write();
    //eta2p0 -> Write();
    eta0p5 -> Write();
    eta1p0 -> Write();
    //eta2p4 -> Write();
    //eta3p4 -> Write();
    //eta5p0 -> Write();
    //Ncoll0p5 -> Write();
    //Npart -> Write();
    //NcollMult0p5 -> Write();
    //NcollMult1p0 -> Write();
    //eta3p0_norm -> Write();
    //eta2p0_norm -> Write();
    eta0p5_norm -> Write();
    eta1p0_norm -> Write();
    //eta2p4_norm -> Write();
    //eta3p4_norm -> Write();
    //eta5p0_norm -> Write();
    //Ncoll0p5_norm -> Write();
    //Npart_norm -> Write();
    output -> Close();
    
    return 0;
}