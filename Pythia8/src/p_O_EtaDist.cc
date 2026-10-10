// This script simulates p-O collision at the following energies: 
// 2.36 TeV, 5.02 TeV, 7 TeV and 13 TeV.
// It will be evaluated pseudorapidity distribution

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
    // ROOT File
    //TFile *output = new TFile("../results/pp_Collisions/EtaDist/pp_Eta_distribution.root", "RECREATE");
    TFile *output = new TFile("../results/p_O_Collisions/EtaDist/pO_Eta_distribution.root", "RECREATE");
    //TFile *output = new TFile("../results/p_Pb_Collisions/EtaDist/pPb_Eta_distribution.root", "RECREATE");

    // Histograms
    TH1D* energy2p36TeV = new TH1D("energy2p36TeV", "Pseudorapidity Distribution; #eta; dN_{ch}/d#eta", 200, -10.0, 10.0);
    TH1D* energy5p02TeV = new TH1D("energy5p02TeV", "Pseudorapidity Distribution; #eta; dN_{ch}/d#eta", 200, -10.5, 9.5);
    //TH1D* energy8p16TeV = new TH1D("energy8p16TeV", "Pseudorapidity Distribution; #eta; dN_{ch}/d#eta", 200, -10.0, 10.0);
    TH1D* energy9p62TeV = new TH1D("energy9p62TeV", "Pseudorapidity Distribution; #eta; dN_{ch}/d#eta", 200, -10.0, 10.0);
    TH1D* energy13TeV = new TH1D("energy13TeV", "Pseudorapidity Distribution; #eta; dN_{ch}/d#eta", 200, -10.0, 10.0);
    TH1D* Ncoll2p36 = new TH1D("Ncoll 2.36 TeV", "Ncoll 2.36 TeV; N_{coll}; P(N_{coll})", 500, -0.5, 499.5);
    TH1D* Ncoll5p02 = new TH1D("Ncoll 5.02 TeV", "Ncoll 5.02 TeV; N_{coll}; P(N_{coll})", 500, -0.5, 499.5);
    TH1D* Ncoll9p62 = new TH1D("Ncoll 9.62 TeV", "Ncoll 9.62 TeV; N_{coll}; P(N_{coll})", 500, -0.5, 499.5);
    TH1D* Ncoll13 = new TH1D("Ncoll 13 TeV", "Ncoll 13 TeV; N_{coll}; P(N_{coll})", 500, -0.5, 499.5);
    TH1D* Npart2p36 = new TH1D("Npart 2.36 TeV", "Npart 2.36 TeV; N_{part}; P(N_{part})", 500, -0.5, 499.5);
    TH1D* Npart5p02 = new TH1D("Npart 5.02 TeV", "Npart 5.02 TeV; N_{part}; P(N_{part})", 500, -0.5, 499.5);
    TH1D* Npart9p62 = new TH1D("Npart 9.62 TeV", "Npart 9.62 TeV; N_{part}; P(N_{part})", 500, -0.5, 499.5);
    TH1D* Npart13 = new TH1D("Npart 13 TeV", "Npart 13 TeV; N_{part}; P(N_{part})", 500, -0.5, 499.5);
    
    // Relevant variables
    //int nEvents = 1e7;
    int nEvents = 1e4;
    vector<double> energies = {2.36e3, 5.02e3, 9.62e3, 13e3}; // GeV
    //vector<double> energies = {8.16e3}; // GeV
    // double pT_min = 0.15, pT_max = 10; // GeV ############## A SER DISCUTIDO
    double N_ch2p36, mean_Nch2p36 = 0;

    //double eta;

    // Loop over energies
    for (double energy : energies)
    {
        Pythia pythia;

        // Setup Simulation
        pythia.readString("Beams:idA = 2212"); // proton
        //pythia.readString("Beams:idB = 2212"); // proton
        //pythia.readString("Beams:idB = 1000822080");    // Pb Nucleus   
        pythia.readString("Beams:idB = 1000080160");// O nucleus
        pythia.readString("Beams:eCM = " + std::to_string(energy)); // GeV
        pythia.readString("SoftQCD:all = on");
        pythia.readString("HeavyIon:mode = 1");   
        // monash tune
        pythia.readString("Tune:ee = 7");
        pythia.readString("Tune:pp = 14");  
        //pythia.readString("HeavyIon:mode = 1");          // Activates Angantyr
        //pythia.readString("HeavyIon:AlphaCluster = on");   // Activates Alpha Cluster Geometry
        // Defining Woods-Saxon parameters
        //pythia.readString("Angantyr:NucleusModelB = 2");
        pythia.readString("HeavyIonB:ACR = 2.608");      // R parameter for Woods-Saxon Distribution
        pythia.readString("HeavyIonB:ACa = 0.513");      // a parameter for Woods-Saxon Distribution
        //pythia.readString("HeavyIonB:ACw = 0.0");        // w parameter for Woods-Saxon Distribution

        pythia.init();

        // Loop over events
        for (int i = 0; i < nEvents; i++)
        {
            if(!pythia.next()) continue;

            // Defining entries
            int entries = pythia.event.size();

            // Show event and energy
            if (i % 1000 == 0) std::cout << "Energy: " << energy << "GeV | Event: " << i << "/" << nEvents << std::endl;

            // Obtain number of participants and collisions on each event
            double nPartProj = pythia.info.hiInfo -> nPartProj();
            double nPartTarg = pythia.info.hiInfo -> nPartTarg();
            double nPart = nPartProj + nPartTarg;
            double nColl = pythia.info.hiInfo -> nCollTot();

            // Fill both histograms nPart and nColl for each energy
            if (energy == 2.36e3) {
                Ncoll2p36 -> Fill(nColl);
                Npart2p36 -> Fill(nPart);
            } 
            if (energy == 5.02e3){
                Ncoll5p02 -> Fill(nColl);
                Npart5p02 -> Fill(nPart);
            }
            if (energy == 9.62e3) {
                Ncoll9p62 -> Fill(nColl);
                Npart9p62 -> Fill(nPart);
            }
            if (energy == 13e3) {
                Ncoll13 -> Fill(nColl);
                Npart13 -> Fill(nPart);
            }

            // Loop over entries
            for (int j = 0; j < entries; j++)
            {
                if(pythia.event[j].isFinal() && pythia.event[j].isCharged()) // && pythia.event[j].pT() > pT_min) && pythia.event[j].pT() < pT_max)
                {
                    double eta = pythia.event[j].eta();

                    // Evaluates which histogram will be filled
                    if (energy == 2.36e3) energy2p36TeV -> Fill(eta); 
                    if (energy == 5.02e3) energy5p02TeV -> Fill(eta);
                    if (energy == 9.62e3) energy9p62TeV -> Fill(eta);
                    if (energy == 13e3) energy13TeV -> Fill(eta);
                }
            }
        }
    }

    // Normalizing histograms
    //energy2p36TeV -> Scale(1.0 / energy2p36TeV->GetEntries(), "width");
    energy2p36TeV -> Scale(1.0 / nEvents, "width");
    energy5p02TeV -> Scale(1.0 / nEvents, "width");
    energy9p62TeV -> Scale(1.0 / nEvents, "width");
    energy13TeV -> Scale(1.0 / nEvents, "width");

    //std::cout << "GET ENTRIES TEST: \n" << std::endl;
    //std::cout << energy2p36TeV->GetEntries() << std::endl;

    // Writing results
    energy2p36TeV -> Write();
    energy5p02TeV -> Write();
    energy9p62TeV -> Write();
    energy13TeV -> Write();
    Ncoll2p36 -> Write();
    Ncoll5p02 -> Write();
    Ncoll9p62 -> Write();
    Ncoll13 -> Write();
    Npart2p36 -> Write();
    Npart5p02 -> Write();
    Npart9p62 -> Write();
    Npart13 -> Write();
    output -> Close();

    return 0;
}