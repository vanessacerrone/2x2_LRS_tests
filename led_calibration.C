#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TText.h"
#include "TPaveText.h"
#include "TColor.h"
#include "TSpectrum.h"
#include "TCanvas.h"
#include "TF1.h"

#include "set_style.C"

using namespace std;

#define blue TColor::GetColor("#004C97")
#define red TColor::GetColor("#AF272F")
#define green TColor::GetColor("#4C8C2B")


vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


Double_t LinearFit(Double_t *x, Double_t *par) 
{
   Double_t fitval = par[0] + par[1]*x[0];
  return fitval;
}



vector<Double_t> single_channel(const string infilename, short chan, int maxpeaks, int verbose = 0)
{
   /*
    * Extracts gain/calibration factor by analyzing single p.e. spectrum
    * Opens data file, loads TTree and reads integral branch
    * Performs single channel analysis 
    * Returns vector with # fitted peaks and calibration curve parameters
    
    * Input:
    *   1. infilename: rlog data file (.root) 
    *   2. chan: channel to analyze
    *   3. maxpeaks: max # of p.e. peaks to look for
    *   4. verbose: == 0 default
    *               == 1 prints found peaks 
    *               == 2 saves plots  
    */

    // --- Set the style --- //
    style(gStyle); 

    // example of input filename: rlog_0cd913fb_20220207_020111_aaa.data.root
    // 0cd913fb -> ACL ADC
    // 0cd93db0 -> LCM ADC

    vector<string> v = split (infilename, '_');

    string lrs;         // lrs: ArCLight (ACL) or Light Collection Module (LCM)
    string SR = v[1];   // serial number identifies ADC


    if(SR == "0cd913fb") {
        lrs = "ACL";
    }

    else if(SR == "0cd93db0") {
        lrs = "LCM";
    }

    TFile *infile = new TFile(infilename.c_str());
    TTree *intree = (TTree*) infile->Get("rlog");

    Float_t   integral[64];
    
    // Get integral branch 
    intree->SetBranchAddress("integral", &integral);
    ULong64_t n_events = intree->GetEntries();
    ULong64_t counter = 0;

    Int_t nbins;
    Double_t xmin = 10000;
    Double_t xmax;


    if (lrs == "ACL") {
        xmax = 45000;
        nbins = 400;
    }
    else if (lrs == "LCM"){
        xmax = 45000; 
        nbins = 400;
    }
    else {
        cout << "Invalid LRS option.\nChoose either LCM or ACL. \n";
        exit(1);
    }

    // Create histogram
    TH1D *spectrum = new TH1D(Form("ch%i",chan), Form("ch%i",chan), nbins, xmin, xmax);

    // Fill the histogram
    while( counter < n_events)
    {
	intree->GetEntry(counter);
	spectrum->Fill(integral[chan]);
	counter++;
	    
    }


    // --- Search for peaks --- //
    TH1D *h_peaks = (TH1D*)spectrum->Clone();
    TSpectrum *s = new TSpectrum(maxpeaks);
    int nPeaks;
    double *xPeaks;
    int foundPeaks;

    int sigma = 3;
    double minratio = 0.05;	// minimum ratio between a peak and the main peak

    nPeaks = s->Search(h_peaks, sigma, "goff", minratio);
    xPeaks = s->GetPositionX();

    vector<double> vect_peaks;

    for (int p = 0; p < nPeaks; p++){
        
        vect_peaks.push_back(xPeaks[p]);

    }
   
    
    sort(vect_peaks.begin(), vect_peaks.end()); // sort vector
    foundPeaks = vect_peaks.size(); // # of peaks found


    if(verbose == 1) {

        for (int p = 0; p < nPeaks; p++) {

            cout << "Peak #" << p << " @ ADC " << vect_peaks[p] << endl;	

        }
        cout << "# of peaks found" << foundPeaks << endl;

    }


    TCanvas* c1 = new TCanvas(Form("ch%i",chan),"",900,900);
    c1->cd();

    histo_style(spectrum);

    float w;    // bin width
    w = spectrum->GetXaxis()->GetBinWidth(0);

    spectrum->GetXaxis()->SetTitle("ADC counts");
	spectrum->GetYaxis()->SetTitle(Form("Counts / %0.1f ADC",w));

    // define range for gaussian fit 
    Double_t min[foundPeaks];
    Double_t max[foundPeaks];
    Double_t min_resolution = 0.025;
    Double_t max_resolution = 0.027;

    for (int i = 0; i<foundPeaks; i++){

        min[i] = vect_peaks[i] * (1 - min_resolution);
        max[i] = vect_peaks[i] * (1 + max_resolution);
	    
    }

    // --- Fit peaks --- //
    TF1** fit = new TF1*[foundPeaks]; 

    spectrum->Draw();

    vector<double> mu;
    vector<double> err_mu;

    for (int i=0; i < foundPeaks; i++) { 
        
        fit[i] = new TF1(Form("f%d",i), "gaus", min[i], max[i]);
        fit[i]->SetParameter(1, vect_peaks[i]);
        fit[i]->SetLineColorAlpha(red,0.6);
        fit[i]->SetLineStyle(1);
	fit[i]->SetLineWidth(2);
        
        //TFitResultPtr r = spectrum->Fit(fit[i],"SR+");
        //r->Print();
        spectrum->Fit(fit[i],"R+");
        fit[i]->Draw("same");

        mu.push_back(fit[i]->GetParameter(1));
        err_mu.push_back(fit[i]->GetParError(1));

   } 



    // --- Linear fit to retrieve SiPM gain --- //
    
    TF1 *f1 = new TF1("f1", LinearFit, 0, 40000, 2);

    Double_t n_pe[foundPeaks]; // number of PEs

    for (int i=0;i <= foundPeaks;i++) {

	n_pe[i] = i;

	}

    TCanvas* c2 = new TCanvas("c2","Linear Fit", 900, 900);

    c2->cd();

    TGraphErrors *g = new TGraphErrors(foundPeaks, &n_pe[0], &mu[0], 0, &err_mu[0]);

    graph_style(g);

    g->GetXaxis()->SetTitle("# p.e.");
    g->GetYaxis()->SetTitle("ADC counts");
    g->SetMarkerStyle(20); 
    g->SetMarkerSize(1.1);

    f1->SetLineColorAlpha(blue,0.6);
    f1->SetLineStyle(1);
    f1->SetLineWidth(2);

    f1->SetParNames("a", "b");

    g->Fit(f1,"R+");
    g->Draw("ap SAME");
    
	
    double a = f1->GetParameter(0);
    double b = f1->GetParameter(1);
    double err_a = f1->GetParError(0);
    double err_b = f1->GetParError(1);



    // ---  Graphical stuff --- //

    c1->cd();

    TPaveText *title = new TPaveText(0.7,0.83,0.8,0.89, "NB NDC");
    title->SetTextSize(0.02);
    title->SetFillColor(0); 
    title->SetTextAlign(12);
    title->AddText((Form("Channel %i",chan))); 
    title->Draw();

    TText *t1 = title->GetLineWith("Channel");
    t1->SetTextColor(blue);

    c2->cd();

    // write fit parameters on canvas 
    TPaveText *label = new TPaveText(0.2,0.75,0.4,0.85, "NB NDC");
    label->SetTextSize(0.02);
    label->SetFillColor(0); 
    label->SetTextAlign(12);
    label->AddText((Form("Channel %i",chan))); 
    label->AddText(Form("Offset = ( %1.0f #pm %1.0f ) ADC ", a, err_a)); 
    label->AddText(Form("Gain = ( %1.0f #pm  %1.0f ) ADC / p.e. ", b, err_b)); 
    label->Draw();

    TText *t2 = label->GetLineWith("Channel");
    t2->SetTextColor(blue);

    string plot_file = v[2] + "_" + v[3];

    if(verbose == 2) {
        // do not save plot for inactive channels (empty branches -> gain=0)
        if(b != 0) {
            c1->SaveAs(Form("plots/%s/%s_ch%d_spectrum.pdf",lrs.c_str(),plot_file.c_str(),chan));
            c2->SaveAs(Form("plots/%s/%s_ch%d_fit.pdf",lrs.c_str(),plot_file.c_str(),chan));
        }
    }
    

    // vector to store the results
    vector<Double_t> results;

    results.push_back(foundPeaks);
    results.push_back(a);
    results.push_back(b);
    results.push_back(err_a);
    results.push_back(err_b);

    return(results);
	
}



void all_channels(string infilename, int n_channels, int maxpeaks, int verbose = 0)
{
   /*
    * Performs all channels analysis 
    * Saves csv file with the results: channel, # peaks, gain ± err, offset ± err

    * Input:
    *   1. infilename: rlog data file (.root) 
    *   2. n_channel: # channels to analyze
    *   3. maxpeaks: max # of p.e. peaks to look for
    *   4. verbose: == 0 default
    *               == 1 prints found peaks 
    *               == 2 saves plots 
    */

    // example of input filename: rlog_0cd913fb_20220207_020111_aaa.data.root
    // output filename -> results_0cd913fb_20220207_020111.csv

    vector<string> v = split (infilename, '_');
    
    string lrs;
    string SR = v[1]; // serial number identifies ADC

    if(SR == "0cd913fb") {
        lrs = "ACL";
    }

    else if(SR == "0cd93db0") {
        lrs = "LCM";
    }

    string csv_path = "csv_files/";
    string results_file = csv_path + lrs + "_" + v[1] + "_" + v[2] + "_" + v[3] + ".csv";

    FILE *fo;

    fo = fopen(const_cast<char*>(results_file.c_str()), "w+");

    fprintf(fo,"Channel,Peaks,Gain,Error_g, Offset,Error_o\n");
    
    vector<Double_t> temp(5);
    
    for (int i = 0; i < n_channels; i++) {    

        temp = single_channel(infilename, i, maxpeaks, verbose);

        // force gain (and error) to -1000 (to 0) for channels with pedestal only
        if(temp.at(2) < 0) { 
            temp.at(2) = -1000;
            temp.at(4) = 0;
            }

        fprintf(fo,"%d,%1.0f,%1.2f,%1.2f,%1.2f,%1.2f\n", i, temp.at(0), temp.at(2), temp.at(4), temp.at(1), temp.at(3));    

    }
   
    return;

}
