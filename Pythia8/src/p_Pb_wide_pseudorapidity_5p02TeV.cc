// This script simulates a p-Pb collision at a certain energy (5.02 TeV) and considering four cuts in $$\eta$$:
// -3.4 < $\eta$ < 5.0 ; -3.4 < $\eta$ < 3.4 ; -3.0 < $\eta$ < 3.0 ; -2.4 < $\eta$ < 2.4 
// The purpose of this script is to generate the multiplicity distribution of this collision
// For this simulation, the Angantyr Model is being considered and data will be stored in .root files 
// (in order to generate graphics and make a fit according to some references) and it will also be stored
// in .hepmc files (in order to compare with results from the literature)

#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "Pythia8/Pythia.h"
//#include "Pythia8Plugins/HepMC3.h"

using namespace Pythia8;

int main()
{

    // Defining file names
    //Pythia8ToHepMC toHepMC("../results/p_Pb_widepseudorapidity.hepmc"); // HepMC file
    TFile *output = new TFile("../results/teste_p_O_widepseudorapidity.root", "RECREATE"); // root file

    // Defining Histograms names
    TH1D *eta_3p4_5p0 = new TH1D("-3.4 < eta 5.0","Multiplicity distribution -3.4 < #eta < 5.0; N_{ch}; P(N_{ch})", 500, 0, 500);
    TH1D *eta_3p4 = new TH1D("|eta| < 3.4","Multiplicity distribution |#eta|  < 3.4; N_{ch}; P(N_{ch})", 500, 0, 500);
    TH1D *eta_3p0 = new TH1D("|eta| < 3.0","Multiplicity distribution |#eta| < 3.0; N_{ch}; P(N_{ch})", 500, 0, 500);
    TH1D *eta_2p4 = new TH1D("|eta| < 2.4","Multiplicity distribution |#eta| < 2.4; N_{ch}; P(N_{ch})", 500, 0, 500);
    TH1D *dNdEta = new TH1D("dN/d#eta", "Pseudorapidity Density; #eta; dN_{ch}/d#eta", 100, -5.0, 5.0 );


    // Relevant variables 
    int nEvents = 9e3; 
    double pT = 0.1;
    double eta1 = 3.4, eta2 = 5.0, eta3 = 3.0, eta4 = 2.4;
    // Number of charged particles found in each eta cut
    int nCharged_3p4_5p0, nCharged_3p4, nCharged_3p0, nCharged_2p4;

    Pythia pythia;

    // Setup the beams
    pythia.readString("Beams:idA = 2212");
    // pythia.readString("Beams:idB = 1000822080"); // Pb Nucleus
    pythia.readString("Beams:idB = 1000080160"); // O Nucleus 1000080160
    pythia.readString("Beams:ecM = 5020"); 
    pythia.readString("SoftQCD:all = on");
    pythia.readString("HeavyIon:mode = 2"); // Activates Angantyr

    // Pythia initialization
    pythia.init();

    // Loop over events
    for (int i = 0; i <  nEvents; i++)
    {
        if(!pythia.next()) continue;

        // Exhibits how many events already ran
        if (i % 1000 == 0 )
        {
            std::cout << "Events : " << i << "/" << nEvents << std::endl;
        }

        // Set counters of charged particles as 0 at the beggining of each event
        nCharged_3p4_5p0 = 0, nCharged_3p4 = 0, nCharged_3p0 = 0, nCharged_2p4 = 0;

        // Define entries of each event
        int entries = pythia.event.size();

        // Loop over entries
        for (int j = 0; j < entries; j++)
        {
            // Criteria for selection
            if (pythia.event[j].isFinal() && pythia.event[j].isCharged() && pythia.event[j].pT() > pT)
            {

                dNdEta -> Fill(pythia.event[j].eta());
                // Evaluates charged particles for each #eta cut
                double current_eta = pythia.event[j].eta();
                if (current_eta > -eta1 && current_eta < eta2) nCharged_3p4_5p0++;
                if (abs(current_eta) < eta1) nCharged_3p4++;
                if (abs(current_eta) < eta3) nCharged_3p0++;
                if (abs(current_eta) < eta4) nCharged_2p4++;
            }
        }
        // After the loop of all entries of the event, stock Values in the histograms
        eta_3p4_5p0 -> Fill(nCharged_3p4_5p0);
        eta_3p4 -> Fill(nCharged_3p4);
        eta_3p0 -> Fill(nCharged_3p0);
        eta_2p4 -> Fill(nCharged_2p4);

        // At the end of the event, construct new empty HepMC event, fill it and write it out
        //toHepMC.writeNextEvent( pythia );
    }

    // After all the events, normalize histograms
    eta_3p4_5p0 -> Scale(1.0/ eta_3p4_5p0 -> GetEntries(), "width");
    eta_3p4 -> Scale(1.0/ eta_3p4 -> GetEntries(), "width");
    eta_3p0 -> Scale(1.0/ eta_3p0 -> GetEntries(), "width");
    eta_2p4 -> Scale(1.0/eta_2p4 -> GetEntries(), "width");
    dNdEta -> Scale(1.0/nEvents, "width"); // Normalize by number of events in order to get the mean per event

    // Writing information
    eta_3p4_5p0 -> Write();
    eta_3p4 -> Write();
    eta_3p0 -> Write();
    eta_2p4 -> Write();
    dNdEta -> Write();
    //gPad -> SetLogy();
    output -> Close();

    return 0;
}