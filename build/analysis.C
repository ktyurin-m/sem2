
using namespace ROOT;
{
    TFile in_file("B2out.root");
    TTree* mytuple;
    in_file.GetObject("Data", mytuple);
    for (int i = 0; i < mytuple->GetEntries(); i++)
    {
        mytuple->GetEntry(i);
        row_content = mytuple->
        one = row_content;
        cout << one << endl;
    }
    
}