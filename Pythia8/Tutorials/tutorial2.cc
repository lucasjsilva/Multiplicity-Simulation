#include <iostream>
# include "Pythia8/Pythia.h"

using namespace Pythia8;

int main()
{
    int nevents = 10;

    Pythia pythia;

    // Definicao da composicao dos beams que vao interagir
    // As referencias para a identificacao das particulas definidas pelo PDG podem ser encontradas em 
    // https://pdg.lbl.gov/2024/reviews/rpp2024-rev-monte-carlo-numbering.pdf

    pythia.readString("Beams:idA = 2212"); // identificacao de proton
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 14.e3"); // collision center mass energy
    pythia.readString("SoftQCD:all = on"); 
    pythia.readString("HardQCD:all = on");

    // Criacao do histograma
    // Nome do histograma; numero de bins, limite inferior, limite superior
    Hist hpz("Momentum Distribution", 100, -10, 10); 

    // Inicializacao do Pythia
    pythia.init();

    for(int i = 0; i < nevents; i++)
    {
        if(!pythia.next()) continue;

        int entries = pythia.event.size();

        std::cout << "Event: " << i << std::endl;
        std::cout << "Event size: " << entries << std::endl;

        for(int j = 0; j < entries; j++)
        {
            int id = pythia.event[j].id();

            double m = pythia.event[j].m();

            double px = pythia.event[j].px();
            double py = pythia.event[j].py();
            double pz = pythia.event[j].pz();

            double pabs = sqrt(pow(px,2) + pow(py,2) + pow(pz,2));

            // Preenchendo o histograma
            hpz.fill(pz);

            std::cout << id << " " << m << " " << pabs << std::endl;
        }
    }

    std::cout << hpz << std::endl;

    //Build-in-function para exibir o plot atraves do python, criando um arquivo em PDF
    HistPlot hpl("tutorial2"); // Nome do plot
    // Nome do arquivo, titulo do plot, nome eixo x, nome eixo y
    hpl.frame("output", "Momentum Distribution", "Momentum", "Entries");
    hpl.add(hpz);
    hpl.plot();


    return 0;
}