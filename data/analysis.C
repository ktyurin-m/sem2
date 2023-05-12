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
    Double_t Edep;
    t->SetBranchAddress("y",&y);
    t->SetBranchAddress("z",&z);
    t->SetBranchAddress("x",&x);
    t->SetBranchAddress("Edep",&Edep);
    TH2F *h1 = new TH2F("h1","h1",100,-500,500,100,0,500);
    TH2F *h2 = new TH2F("h2","h2",100,-250,250,100,1900,2100);
    TH1F *h3  = new TH1F("h3","h3",100, 1900,2000) ; //energy
    TH1F *h4  = new TH1F("h3","h3",300, -2000,2000) ;
    Int_t ent = t->GetEntries();
    TF1 *g1 = new TF1("g1", "gaus", 1960, 2000);
    for (Int_t i = 0; i < ent; i++)
    {
        t->GetEntry(i);
        h3->Fill(Edep);
        // h3->Fill(Edep);
        // h4->Fill(x);
        //h2->Fill(x,Edep);
    }
    
    t->ResetBranchAddresses();
    // h1->Draw("colz");
    //h3->Draw();
    // h3->Fit(g1,"R");
    // h3->Draw();
    //h2->Draw("colz");
    h3->Draw();
}

// {
//     // auto file = new TFile("B2out.root","update");
//     // auto tree = file->Get<TTree>("Data");
//     // Float_t new_v;

//     // auto newBranch = tree->Branch("new_v", &new_v);
 
//     // Long64_t nentries = tree->GetEntries(); // read the number of entries in the t3
 
//     // for (Long64_t i = 0; i < nentries; i++) {
//     //     new_v = gRandom->Gaus(0, 1);
//     //     newBranch->Fill();
//     // };
//     // tree->Write("", TObject::kOverwrite); 



// }
