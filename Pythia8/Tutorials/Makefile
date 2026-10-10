tutorial%: tutorial%.cc
	g++ $@.cc -I/home/lucasjfsilva/pythia/pythia8315/include -L/home/lucasjfsilva/pythia/pythia8315/lib -lpythia8 -o $@

tutorial3: tutorial3.cc
	g++ $@.cc -I/home/lucasjfsilva/pythia/pythia8315/include `root-config --cflags` -L/home/lucasjfsilva/pythia/pythia8315/lib `root-config --glibs` -lpythia8 -o $@
