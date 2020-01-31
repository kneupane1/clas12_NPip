#include "TCanvas.h"
#include "TFile.h"
#include "TFrame.h"
#include "TH2.h"
#include "TInterpreter.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TSystem.h"

// color our pid 2 red, their pid 4 purple

void to_overlap() {
  auto f = TFile::Open("alldata_total_fun.root", "READ");
//  auto f1 = TFile::Open("elastic_sim_with_our_pid_cuts_7542MeV_july_21.root", "READ");
  // auto f2 = TFile::Open("a_elastic_sim_using_their_pid.root", "READ");2  // auto f3 = TFile::Open("a_elastic_sim_no_cuts_their_pid.root", "READ");
  //TH1D *E_vs_theta_e_elastic_events = (TH1D *)f->Get("NPip/W_hist_NPip_sec_0");
  TH2D *E_vs_theta_e_elastic_events = (TH2D *)f->Get("NPip/E_vs_theta_e_elastic_events_0");
  TH2D *E_vs_theta_e_2nd_rsonance_region = (TH2D *)f->Get("NPip/E_vs_theta_e_2nd_resonance_region_0");
  TH2D *E_vs_theta_e_3rd_rsonance_region = (TH2D *)f->Get("NPip/E_vs_theta_e_3rd_resonance_region_0");

  TCanvas *c1 = new TCanvas("canvas", "canvas", 900, 900);
  //c1->Divide(2, 2);  //(column, row)
  //c1->cd(1);
//  e1->SetLineColor(2);
  //e->SetLineColor(4);
  gStyle->SetOptStat(00);
  E_vs_theta_e_elastic_events->SetStats(0);
  E_vs_theta_e_elastic_events->Draw("");
  //  E_vs_theta_e_2nd_rsonance_region ->SetOption("COLZ");
  E_vs_theta_e_2nd_rsonance_region ->Draw("SAME");
//  E_vs_theta_e_2nd_rsonance_region ->SetOption("COLZ");
  E_vs_theta_e_3rd_rsonance_region ->Draw("SAME");

  // c1->cd(2);
  // p1->SetLineColor(2);
  // p->SetLineColor(4);
  // p->Draw("");
  // p1->Draw("SAME");
  //
  // c1->cd(3);
  // pip1->SetLineColor(2);
  // pip->SetLineColor(4);
  // pip->Draw("");
  // pip1->Draw("SAME");
  //
  // c1->cd(4);
  // pim1->SetLineColor(2);
  // pim->SetLineColor(4);
  // pim->Draw("");
  // pim1->Draw("SAME");

}
