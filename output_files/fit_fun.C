//#include "TCanvas.h"

// W_hist_NPip_events[i]->Fit("gaus", "QMR+", "QMR+", 1.44, 1.58);
// W_hist_NPip_events[i]->Fit("gaus", "QMR+", "QMR+", 1.64, 1.73);
void fit_fun() {
  auto f = TFile::Open("elastic_clas12_alldata_total_01_30_2020.root", "READ");
//Npip
TCanvas *W_hist_NPip = new TCanvas("W_hist_NPip_all_sec", "W_hist_NPip_all_sec", 900, 900);
TH1D *W_hist_NPip_events = (TH1D *)f->Get("NPip/W_hist_NPip_sec_0");
Double_t par[8];
TF1* g1 = new TF1("g1", "gaus", 1.44, 1.58);
TF1* g2 = new TF1("g2", "gaus", 1.64, 1.73);
TF1* g3 = new TF1("g3", "pol2", 1.4, 1.8);
TF1* total = new TF1("gtotal", "gaus(0)+gaus(3)+pol2(6)", 1.4, 1.8);
W_hist_NPip_events->Fit(g1, "NR");
W_hist_NPip_events->Fit(g2, "NR+");
W_hist_NPip_events->Fit(g3, "NR+");

g1->GetParameters(&par[0]);
g2->GetParameters(&par[3]);
g3->GetParameters(&par[6]);
total->SetParameters(par);

W_hist_NPip_events->Fit(total, "NR+");

//fit_can->cd(1);
//gStyle->SetOptFit(1111);
W_hist_NPip->Divide(2, 2);

W_hist_NPip->cd(1);
W_hist_NPip_events->Draw("");
g1->Draw("same");

W_hist_NPip->cd(2);
W_hist_NPip_events->Draw("");
g2->Draw("same");

W_hist_NPip->cd(3);
W_hist_NPip_events->Draw("");
g3->Draw("same");
W_hist_NPip->cd(4);
W_hist_NPip_events->Draw("");
total->Draw("same");

//c1->Divide(2, 2);  //(column, row)
//c1->cd(1);
//  e1->SetLineColor(2);
//e->SetLineColor(4);
//gStyle->SetOptStat(00);
//W_hist_NPip_events->SetStats(0);


// vz
// TCanvas *vz = new TCanvas("v_z", "v_z", 900, 900);
//TH1D *vz_position = (TH1D *)f->Get("Electron_Cuts/vz_position_before_cut");

// gStyle->SetOptFit(1111);
// vz_position->Fit("gaus", "QMR+", "QMR+",-7.089, 2.0);
//vz_position->Draw("");
// //x_mu
//   E_x_mu_hist[i]->Fit("gaus", "QMR+", "QMR+", -0.45, 0.38);
//   //  gROOT->SetStyle("Plain");
//   gStyle->SetOptFit(1111);
//
// TH2D *E_vs_theta_e_elastic_events = (TH2D *)f->Get("NPip/E_vs_theta_e_elastic_events_0");
// TH2D *E_vs_theta_e_2nd_rsonance_region = (TH2D *)f->Get("NPip/E_vs_theta_e_2nd_resonance_region_0");
// TH2D *E_vs_theta_e_3rd_rsonance_region = (TH2D *)f->Get("NPip/E_vs_theta_e_3rd_resonance_region_0");

// TCanvas *c1 = new TCanvas("canvas", "canvas", 900, 900);
// //c1->Divide(2, 2);  //(column, row)
// //c1->cd(1);
// //  e1->SetLineColor(2);
// //e->SetLineColor(4);
// gStyle->SetOptStat(00);
// E_vs_theta_e_elastic_events->SetStats(0);
// E_vs_theta_e_elastic_events->Draw("");
// //  E_vs_theta_e_2nd_rsonance_region ->SetOption("COLZ");
// E_vs_theta_e_2nd_rsonance_region ->Draw("SAME");
// //  E_vs_theta_e_2nd_rsonance_region ->SetOption("COLZ");
// E_vs_theta_e_3rd_rsonance_region ->Draw("SAME");
}
