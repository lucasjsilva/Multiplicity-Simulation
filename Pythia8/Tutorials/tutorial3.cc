#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main()
{
    // Criando um TFile
    TFile *output = new TFile("tutorial3.root", "recreate"); // Recria o arquivo toda vez que for executado
    // Criando uma TTree
    TTree *tree = new TTree("tree", "tree"); // nome da TTree
    // Definindo variaveis
    int id, event, size, no; // define inteiros como: id da particula, quantidade de eventos tamanho do evento e numero da particula
    double m, px, py, pz;// define doubles como: massa, momentos

    // Definindo as branches da TTree
    //nome da Branch; endereço; nome da Brach/Integer (assim o root sabe que queremos o tamanho de integer)
    
    tree -> Branch("event", &event, "event/I");
    tree ->Branch("size", &size, "size/I");
    tree ->Branch("id", &id, "id/I");
    tree ->Branch("m", &m, "m/D"); // Branch/Double para o root saber que é um double 
    tree ->Branch("pz", &pz, "pz/D");

    int nevents = 1e4;

    Pythia pythia;

    // Definicao da composicao dos beams que vao interagir
    // As referencias para a identificacao das particulas definidas pelo PDG podem ser encontradas em 
    // https://pdg.lbl.gov/2024/reviews/rpp2024-rev-monte-carlo-numbering.pdf

    pythia.readString("Beams:idA = 2212"); // identificacao de proton
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 14.e3"); // collision center mass energy
    pythia.readString("SoftQCD:all = on"); 
    pythia.readString("HardQCD:all = on");

    // Inicializacao do Pythia
    pythia.init();

    for(int i = 0; i < nevents; i++)
    {
        if(!pythia.next()) continue;

        int entries = pythia.event.size();

        std::cout << "Event: " << i << std::endl;
        
        // Armazenar o event number
        event = i;
        size = entries;

        for(int j = 0; j < entries; j++)
        {
            id = pythia.event[j].id();
            no = j;

            m = pythia.event[j].m();
            pz = pythia.event[j].pz();

            // Preenchendo a TTree
            tree -> Fill();
        }
    }

    output->Write();
    output->Close();
    return 0;
}