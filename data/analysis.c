
#include <iomanip>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"


void analysis(){
    TFile *f = new TFile("out.root");
    TTree *t = (TTree*)f->Get("Data");
    Double_t z;
    Double_t y;
    Double_t x;
    Double_t Energy;
    t->SetBranchAddress("y",&y);
    t->SetBranchAddress("z",&z);
    t->SetBranchAddress("x",&x);
    t->SetBranchAddress("Energy",&Energy);

    TH2F *h1 = new TH2F("h1","h1",100,-250,250,100,-250,250);
    TH2F *h2 = new TH2F("h2","h2",100,-250,250,100,1900,2000);
    TH1F *h3  = new TH1F("h3","h3",300, 1900,2000) ; //energy
    TH1F *h4  = new TH1F("h3","h3",300, -250,250) ;
    Int_t ent = t->GetEntries();
    // TF1 *g1 = new TF1("g1", "gaus", 1960, 2000);
    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);
	//if (Energy > 2000){
	//cout << "Energy: " << Energy << ", x" << x << endl;}
        h3->Fill(Energy);
        h4->Fill(x);
        h2->Fill(x,Energy);
        h1->Fill(x,y);
    }

    t->ResetBranchAddresses();
    TCanvas* c = new TCanvas;
    c->Divide(2,2);
    c->cd(1);
    //y(x)
    h1->GetYaxis()->SetTitle("y");
    h1->GetXaxis()->SetTitle("x");
    h1->Draw("colz");

    //event(x)
    c->cd(3);
    h4->GetYaxis()->SetTitle("Event");
    h4->GetXaxis()->SetTitle("x");
    h4->Draw();
    //event(Energy)
    c->cd(2);
    h3->GetYaxis()->SetTitle("Event");
    h3->GetXaxis()->SetTitle("Energy");
	h3->SetTitle("");
    h3->Draw();
	TF1 *f_cb= new TF1("MyCrystalBall", "crystalball", 1900, 2000);
	f_cb->SetParameters(h3->GetBinContent(h3->GetMaximumBin()), h3->GetMean(), h3->GetRMS(), 1, 1.5);
	f_cb->SetLineColor(kRed);
	f_cb->SetLineWidth(5);
	h3->Fit(f_cb, "R");
	cout << "E_mean: " << f_cb->GetParameter(1) << " +/- " << f_cb->GetParError(1) << endl;
	cout << "E_sigma: " << f_cb->GetParameter(2) << " +/- " << f_cb->GetParError(2) << endl;


    c->cd(4);
    h2->GetYaxis()->SetTitle("Energy");
    h2->GetXaxis()->SetTitle("x");
    h2->Draw("colz");

    c->Update();
}
