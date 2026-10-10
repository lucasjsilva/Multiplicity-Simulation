#include <iostream>
#include "Pythia8/Pythia.h" 
#include "Pythia8/HeavyIons.h"
#include "Pythia8/AlphaClusterModel.h"

using namespace Pythia8; 

int main(){

  Pythia pythia; 

  pythia.readString("Beams:idA = 2212"); 
  pythia.readString("Beams:idB = 1000080160"); 
  pythia.readString("Beams:eCM = 5020.0");
	
  pythia.readString("SoftQCD:all = on");
  
  pythia.readString("HeavyIon:mode = 1");  
  pythia.readString("HeavyIon:AlphaCluster = on");
        	
  if (!pythia.init()) {
    cout << "Erro: falha na inicialização do Pythia." << endl;
  return 1; 
  }
  cout << "Pythia inicializado com sucesso." << endl;
    
  Hist HistCharged0(" ", 600, -0.5, 599.5);
  Hist HistCharged1(" ", 600, -0.5, 599.5);
  Hist HistCharged2(" ", 600, -0.5, 599.5);
  Hist HistCharged3(" ", 600, -0.5, 599.5);
  Hist HistCharged4(" ", 600, -0.5, 599.5);
  Hist HistCharged5(" ", 600, -0.5, 599.5);
  Hist HistCharged6(" ", 600, -0.5, 599.5);
  Hist HistCharged7(" ", 600, -0.5, 599.5);
       
  int nEvent = 1e4;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent){
  
    if (!pythia.next()) continue;

    int nCharged0 = 0;
    int nCharged1 = 0;
    int nCharged2 = 0;
    int nCharged3 = 0;
    int nCharged4 = 0;
    int nCharged5 = 0;   
    int nCharged6 = 0;
    int nCharged7 = 0;
       
  for (int i = 0; i < pythia.event.size(); ++i){

    if (pythia.event[i].isFinal() && pythia.event[i].isCharged()){  
      double eta = pythia.event[i].eta();
      if (eta > -0.5 && eta < 0.5) ++nCharged0;
      if (eta > -1.0 && eta < 1.0) ++nCharged1;
      if (eta > -2.0 && eta < 2.0) ++nCharged2;
      if (eta > -3.0 && eta < 3.0) ++nCharged3;
      if (eta > -4.0 && eta < 4.0) ++nCharged4;
      if (eta > -5.0 && eta < 5.0) ++nCharged5;
      if (eta > -6.0 && eta < 6.0) ++nCharged6;
      if (eta > -7.0 && eta < 7.0) ++nCharged7;
    } 
  }
  
  HistCharged0.fill(nCharged0);
  HistCharged1.fill(nCharged1);
  HistCharged2.fill(nCharged2); 
  HistCharged3.fill(nCharged3); 
  HistCharged4.fill(nCharged4); 
  HistCharged5.fill(nCharged5); 
  HistCharged6.fill(nCharged6); 
  HistCharged7.fill(nCharged7);
  }

  HistPlot hpl("charged");
  hpl.frame("charged5020GeV");
  hpl.add(HistCharged0);
  hpl.add(HistCharged1);
  hpl.add(HistCharged2);
  hpl.add(HistCharged3);
  hpl.add(HistCharged4);
  hpl.add(HistCharged5);
  hpl.add(HistCharged6);
  hpl.add(HistCharged7);
  hpl.plot();
     
  return 0;
}
