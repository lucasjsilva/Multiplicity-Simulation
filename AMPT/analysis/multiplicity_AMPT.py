import os
import numpy as np
import ROOT
from ROOT import TFile, TH1D, TCanvas, TLegend, TLatex


###############################################################################
# ¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´ FUNCTIONS¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´
###############################################################################

def pseudorapidity(px, py, pz):
    # module of the momentum
    p = np.sqrt(px**2 + py**2 + pz**2)

    # eta = (1/2) * ln((p + pz) / (p - pz))
    if (p - pz) == 0:
        eta = 99999.0
    else:
        eta = 0.5 * np.log((p + pz) / (p - pz))

    return eta

def readChargedParticles(inputFile):

    # PID of charged particles (pions, kaons, protons and its antiparticles)
    particleIDs = [211, -211, 321, -321, 2212, -2212]

    # INEL > 0 condition: at least one charged particle in |eta| < 1
    inelCondition = False
    # Flag to indicate if the event has started
    eventStarted = False

    # Counter of charged particles in |eta| < 0.5 and |eta| < 1.0
    countEta0p5, countEta1p0 = 0, 0

    # Create histograms for multiplicity distributions
    histEta0p5 = TH1D("histEta0p5", "", 500, -0.5, 499.5)
    histEta1p0 = TH1D("histEta1p0", "", 500, -0.5, 499.5)
    histEta = TH1D("histEta", "", 100, -10.0, 10.0)
    # Apply weighted sum of histograms
    histEta0p5.Sumw2()
    histEta1p0.Sumw2()
    
    mult0p5 = []
    mult1p0 = []

    # read input file
    with open(inputFile, 'r') as f:
        #lines = [line.strip().split() for line in f if line.strip()]
        for line in f:
            # First the elements of the line are split
            elements = line.strip().split()

            if len(elements) == 11:
                # Write in the histogram
                if eventStarted and inelCondition:
                    histEta0p5.Fill(countEta0p5)
                    histEta1p0.Fill(countEta1p0)
                    mult0p5.append(countEta0p5)
                    mult1p0.append(countEta1p0)

                # Set counters to 0
                countEta0p5, countEta1p0 = 0, 0
                
                # Reestore INEL > 0 condition and update eventStarted flag
                eventStarted = True
                inelCondition = False
           
            elif len(elements) == 9:
                # Check if the event has started
                if not eventStarted:
                    continue
                # Check particle ID
                if int(elements[0]) not in particleIDs:
                    continue

                # Calculate Eta
                px = float(elements[1])
                py = float(elements[2])
                pz = float(elements[3])
                eta = pseudorapidity(px, py, pz)
                histEta.Fill(eta)
                #print(f"Particle ID: {elements[0]}, eta: {eta}")

                # check if INEL > 0
                if abs(eta) < 1.0:
                    inelCondition = True
                    #print("PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU PASSOU")
                    #print("INEL CONDITION: ", inelCondition)

                # Count charged particles based on INEL > 0 condition
                if inelCondition:
                    if abs(eta) < 0.5:
                        countEta0p5 += 1
                        #print("TEM ETA 0.5------TEM ETA 0.5-------TEM ETA 0.5-------TEM ETA 0.5")
                    if abs(eta) < 1.0:
                        countEta1p0 += 1
                        #print("TEM ETA 1.0------TEM ETA 1.0-------TEM ETA 1.0-------TEM ETA 1.0")

        # After the loop, values are written in the histogram for the last event
        if eventStarted and inelCondition:
            histEta0p5.Fill(countEta0p5)
            histEta1p0.Fill(countEta1p0)
            mult0p5.append(countEta0p5)
            mult1p0.append(countEta1p0)

        # The header of the event is composed of 11 items, while the particle lines have 9 items
        #print(lines)
    
    #print("Multiplicity in |eta| < 0.5:", mult0p5)
    #print("Multiplicity in |eta| < 1.0:", mult1p0)

    return histEta0p5, histEta1p0, histEta

    # After file is 

def readDirectory(inputDir):
    # Create histograms for multiplicity distribution
    countsEta0p5 = TH1D("countsEta0p5", "", 500, -0.5, 499.5)
    countsEta1p0 = TH1D("countsEta1p0", "", 500, -0.5, 499.5)
    countsEta = TH1D("countsEta", "", 100, -10.0, 10.0)

    # Loop over all files in the directory
    for filename in os.listdir(inputDir):
        #print(f"Processing file: {filename}")
        # Read file
        file = os.path.join(inputDir, filename)
        print(f"Processing file: {file}")

        # Apply read charged particles
        histEta0p5, histEta1p0, histEta = readChargedParticles(file)

        # Add histogram to the total count
        countsEta0p5.Add(histEta0p5)
        countsEta1p0.Add(histEta1p0)
        countsEta.Add(histEta)

    # Normalizing histograms
    PnchEta0p5 = countsEta0p5.Clone("PnchEta0p5")
    PnchEta1p0 = countsEta1p0.Clone("PnchEta1p0")
    dNdEta = countsEta.Clone("dNdEta")

    sumEta0p5 = countsEta0p5.Integral()
    sumEta1p0 = countsEta1p0.Integral()

    PnchEta0p5.Scale(1.0 / PnchEta0p5.Integral())
    PnchEta1p0.Scale(1.0 / PnchEta1p0.Integral())
    dNdEta.Scale(1.0 / dNdEta.Integral())

    # Applying statistical errors
    for bin in range(1, countsEta0p5.GetNbinsX() + 1):
        content0p5 = countsEta0p5.GetBinContent(bin)
        content1p0 = countsEta1p0.GetBinContent(bin)
        error0p5 = np.sqrt(content0p5) if content0p5 > 0 else 0
        error1p0 = np.sqrt(content1p0) if content1p0 > 0 else 0

        PnchEta0p5.SetBinError(bin, error0p5 / sumEta0p5)
        PnchEta1p0.SetBinError(bin, error1p0 / sumEta1p0)


    # Save histograms to a ROOT file
    outputFile = TFile("multiplicity_AMPTLOCAL.root", "RECREATE")
    countsEta0p5.Write()
    countsEta1p0.Write()
    countsEta.Write()
    PnchEta0p5.Write()
    PnchEta1p0.Write()
    dNdEta.Write()
    outputFile.Close()

#def normalization(rootFile):
#    # Open ROOT file
#    file = TFile.Open(rootFile)
#    # Create histograms for normalization
#    histEta0p5 = TFile.Open(rootFile).Get("countsEta0p5")
#    histEta1p0 = TFile.Open(rootFile).Get("countsEta1p0")
#    PnchEta0p5 = histEta0p5.Clone("PnchEta0p5") #TH1D("PnchEta0p5", "", 500, -0.5, 499.5)
#    PnchEta1p0 = histEta1p0.Clone("PnchEta1p0") #TH1D("PnchEta1p0", "", 500, -0.5, 499.5)
#
#    # Normalize histograms
#    PnchEta0p5.Scale(1.0 / PnchEta0p5.Integral())
#    PnchEta1p0.Scale(1.0 / PnchEta1p0.Integral())
#
#    # Write new histograms


###############################################################################
# ·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´ PATHS ¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´
###############################################################################

#dirname = os.path.dirname("/home/lucasjfsilva/Documentos/Mestrado/Multiplicity-Simulations/Outputs/AMPT/")
dirname = os.path.dirname("/home/lucasjfsilva/Documentos/Mestrado/Multiplicity-Simulations/Outputs/localOutput/")
dirAna = os.path.join(dirname, "ampt.dat")

###############################################################################
# ·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´ MAIN ¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´¯·.¸¸.·´
###############################################################################
#readChargedParticles(dirAna)
readDirectory(dirname)
