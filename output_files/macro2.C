// Reads the points from a file and produces a simple graph.
int macro2(){
    auto c=new TCanvas();c->SetGrid();

    TGraphErrors graph_expected("./macro2_input_expected.txt",
                                "%lg %lg %lg");
    graph_expected.SetTitle(
       "Resonance region;"//"3rd resonance region;"
       "Sectors;"
       "W (GeV)");
       //graph_expected.SetMaximum(5.);
       //graph_expected.SetMinimum(1.);
       graph_expected.DrawClone("E3AL"); // E3 draws the band
       TGraphErrors graph("./macro2_input.txt","%lg %lg %lg");
     // graph.SetMaximum(5.);
     // graph.SetMinimum(1.0);
    graph.SetMarkerStyle(kCircle);
     graph.SetFillColor(2);
    graph.DrawClone("PESame");
    //
    // Float_t xmin = graph.GetXaxis()->GetXmin();
    // Float_t xmin = graph.GetXaxis()->GetXmax();
    // Float_t ymin = graph.GetMinimum();
    // Float_t ymax = graph.GetMaximum();
     //TAxis* a = graph.GetXaxis();
     TLine *line = new TLine(0.,1.50991e+00,6.5,1.50991e+00);
     line->SetLineColor(kRed);
     line->Draw();
     TLine *line1 = new TLine(0.,1.68587e+00,6.5,1.68587e+00);
     line1->SetLineColor(kRed);
     line1->Draw();
    // Draw the Legend
    // TLegend leg(.1,.7,.3,.9,"Lab. Lesson 2");
    // leg.SetFillColor(0);
    // leg.AddEntry(&graph_expected,"Expected Points");
    // leg.AddEntry(&graph,"Measured Points");
    // leg.DrawClone("Same");

    graph.Print();
    return 0;
}
