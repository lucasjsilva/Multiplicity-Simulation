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
#include "Pythia8/HeavyIons.h"
#include "Pythia8/AlphaClusterModel.h"
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main()
{
    // root files
    TFile *output = new TFile("../results/pO_AlphaCluster_NEWparamENERGY_1.root", "recreate");

    // Histograms
    TH1D* eta0p5 = new TH1D("Multiplicity 9.62 TeV eta 0.5", "Multiplicity 9.62 TeV eta 0.5; N_{ch}; N_{ch}", 500, -0.5, 499.5);
    TH1D* eta1p0 = new TH1D("Multiplicity 9.62 TeV eta 1.0", "Multiplicity 9.62 TeV eta 1.0; N_{ch}; N_{ch}", 500, -0.5, 499.5);
    //TH1D* eta2p0 = new TH1D("Multiplicity 9.62 TeV eta 2.0", "Multiplicity 9.62 TeV eta 2.0; N_{ch}; N_{ch}", 500, -0.5, 499.5);
    //TH1D* eta3p0 = new TH1D("Multiplicity 9.62 TeV eta 3.0", "Multiplicity 9.62 TeV eta 3.0; N_{ch}; N_{ch}", 500, -0.5, 499.5);

    // Relevant Variables
    int nEvents = 1e7; // 10.000.000 events 
    // double pT_min = 0.15, pT_max = 10; //GeV
    double eta1 = 0.5, eta2 = 1.0, eta3 = 2.0, eta4 = 3.0; // pseudorapidity cuts
    // Number of charged particles found in each eta cut
    int nCharged_1p0, nCharged_0p5, nCharged_3p0, nCharged_2p0;
    // Energy to be simulated
    double energy = 2360; // GeV

    // File creation
    //std::ofstream pO_simulation_eta0p5("../results/pO_" + std::to_string(energy) + "GeV_eta0p5.txt");
    //std::ofstream pO_simulation_eta1p0("../results/pO_" + std::to_string(energy) + "GeV_eta1p0.txt");
    //std::ofstream pO_simulation_eta2p0("../results/pO_" + std::to_string(energy) + "GeV_eta2p0.txt");
    //std::ofstream pO_simulation_eta3p0("../results/pO_" + std::to_string(energy) + "GeV_eta3p0.txt");
    
    // Definition of headers in .txt files
    //pO_simulation_eta0p5 << "Energy;N_ch;sigma_n";
    //pO_simulation_eta1p0 << "Energy;N_ch;sigma_n";
    //pO_simulation_eta2p0 << "Energy;N_ch;sigma_n";
    //pO_simulation_eta3p0 << "Energy;N_ch;sigma_n";

        // Define Pythia instance
        Pythia pythia;

        // Setup the beams
        pythia.readString("Beams:idA = 2212");
        pythia.readString("Beams:idB = 1000080160"); // O Nucleus 1000080160
        //pythia.readString("Beams:eCM = 9620"); // GeV
        pythia.readString("SoftQCD:all = on");
        pythia.readString("HeavyIon:mode = 1"); // Activates Angantyr

        pythia.readString("Beams:eA = 3400.0");
        pythia.readString("Beams:eB = 6800.0");
        pythia.readString("Beams:frameType = 2");

        pythia.readString("Random:setSeed = on");
        pythia.readString("Random:seed = 0");
        pythia.readString("ParticleDecays:limitTau0 = on");
        pythia.readString("ParticleDecays:tau0Max = 10.");

        // Initialize the Angantyr model to fit the total and semi-includive cross sections in Pythia within some tolerance.
        pythia.readString("HeavyIon:SigFitErr = 0.02,0.02,0.1,0.05,0.05,0.0,0.1,0.0");
        // These parameters are typicall suitable for sqrt(S_NN)=5TeV
        pythia.readString("HeavyIon:SigFitDefPar = 1.58,16.96,0.19");
        //pythia.readString("HeavyIon:SigFitDefPar = 2.17,17.56,0.30");
        // A simple genetic algorithm is run for 20 generations to fit the parameters.
        pythia.readString("HeavyIon:SigFitNGen = 0");
        //pythia.readString("HeavyIon:SigFitDefAvNDb = 1.00");

        pythia.readString("HeavyIon:AlphaCluster = on"); // Activates Alpha Cluster Geometry
        // pythia.readString("HardQCD:all = on");
        // pythia.readString("HadronLevel:all = on"); // administra a etapa final onde a configuração partônica é hadronizada

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
            nCharged_0p5 = 0, nCharged_1p0 = 0, nCharged_2p0 = 0, nCharged_3p0 = 0;

            // Defining the entries of each event
            int entries = pythia.event.size();

            // Loop over entries
            for(int j = 0; j < entries; j++)
            {
                // Flags for final event and if it is a charged particle
                if(pythia.event[j].isFinal() && pythia.event[j].isCharged()) // && pythia.event[j].pT() > pT_min && pythia.event[j].pT() < pT_max)
                {
                    // Evaluates charged particle for each eta cut
                    //if (abs(pythia.event[j].eta()) < eta4) nCharged_3p0++;
                    //if (abs(pythia.event[j].eta()) < eta3) nCharged_2p0++;
                    if (abs(pythia.event[j].eta()) < eta2) nCharged_1p0++;
                    if (abs(pythia.event[j].eta()) < eta1) nCharged_0p5++;                    
                    
                }
            }

            // Stocking values in the histograms
            //if (nCharged_3p0 > 0) eta3p0 -> Fill(nCharged_3p0);
            //if (nCharged_2p0 > 0) eta2p0 -> Fill(nCharged_2p0);
            if (nCharged_1p0 > 0) eta1p0 -> Fill(nCharged_1p0);
            if (nCharged_0p5 > 0) eta0p5 -> Fill(nCharged_0p5);
        
        }


    // make clone
    TH1D* eta0p5_norm = (TH1D*)eta0p5 -> Clone("eta0p5_norm");
    TH1D* eta1p0_norm = (TH1D*)eta1p0 -> Clone("eta1p0_norm");
    //TH1D* eta2p0_norm = (TH1D*)eta2p0 -> Clone("eta2p0_norm");
    //TH1D* eta3p0_norm = (TH1D*)eta3p0 -> Clone("eta3p0_norm");

    // After all the events, normalize histograms
    //eta3p0_norm -> Scale(1.0/ eta3p0 -> GetEntries(), "width");
    //eta2p0_norm -> Scale(1.0/ eta2p0 -> GetEntries(), "width");
    eta1p0_norm -> Scale(1.0/ eta1p0 -> GetEntries(), "width");
    eta0p5_norm -> Scale(1.0/ eta0p5 -> GetEntries(), "width");

    // Writing information
    //eta3p0 -> Write();
    //eta2p0 -> Write();
    eta1p0 -> Write();
    eta0p5 -> Write();
    //eta3p0_norm -> Write();
    //eta2p0_norm -> Write();
    eta1p0_norm -> Write();
    eta0p5_norm -> Write();

    output -> Close();
    
    return 0;
}