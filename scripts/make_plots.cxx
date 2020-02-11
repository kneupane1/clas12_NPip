#include "../include/constants.hpp"
#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <THn.h>
#include <THnSparse.h>
#include <TLine.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <string>

int make_plots(std::string inFileName = "/home/krishna/Downloads/clas12_NPip/"
                                        "output_files/NPip_02_04_2020.root") {
  gStyle->SetHistMinimumZero();
  TFile *root_data = new TFile(inFileName.c_str());

  {
    TCanvas *can =
        new TCanvas("MissingMass2_elastic", "MissingMass2_elastic", 1920, 1080);
    can->Divide(3, 2);
    for (int sec = 1; sec < 7; sec++) {
      can->cd(sec);
      TH1D *missingMass2 =
          (TH1D *)root_data->Get(Form("W_vs_Q2/MM2_hist_sec_%d", sec));
      missingMass2->GetXaxis()->SetRangeUser(-1, 1);
      missingMass2->Draw("");
      TLine *l =
          new TLine(-MM2_cut, 0, -MM2_cut, missingMass2->GetMaximum() * 1.05);
      l->Draw("SAME");
      TLine *r =
          new TLine(MM2_cut, 0, MM2_cut, missingMass2->GetMaximum() * 1.05);
      r->Draw("SAME");
    }
    can->SaveAs(Form("%s.png", can->GetName()));
  }
  // {
  //   TCanvas *can =
  //       new TCanvas("MissingMass_NPip", "MissingMass_NPip", 1920, 1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH1D *missingMass2_NPip =
  //         (TH1D *)root_data->Get(Form("NPip/MM2_hist_NPip_sec_%d", sec));
  //     //  missingMass2_NPip->GetXaxis()->SetRangeUser(0.5, 3.1);
  //     missingMass2_NPip->Draw("");
  //     TLine *l = new TLine(MM2_NPip_cut_l, 0, MM2_NPip_cut_l,
  //                          missingMass2_NPip->GetMaximum() * 1.05);
  //     l->Draw("SAME");
  //     TLine *r = new TLine(MM2_NPip_cut_u, 0, MM2_NPip_cut_u,
  //                          missingMass2_NPip->GetMaximum() * 1.05);
  //     r->Draw("SAME");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  // {
  //   TCanvas *can = new TCanvas("W_hist_sec", "W_hist_sec", 1920, 1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH1D *W_hist = (TH1D *)root_data->Get(Form("W_vs_Q2/W_hist_sec_%d",
  //     sec)); W_hist->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  // {
  //   TCanvas *can =
  //       new TCanvas("W_hist_NPip_before", "W_hist_NPip_before", 1920,
  //       1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH1D *W_hist_NPip_before =
  //         (TH1D *)root_data->Get(Form("NPip/W_hist_NPip_events_sec_%d",
  //         sec));
  //     W_hist_NPip_before->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  {
    TCanvas *can =
        new TCanvas("W_hist_NPip_sec", "W_hist_NPip_sec", 1920, 1080);
    can->Divide(3, 2);
    for (int sec = 1; sec < 7; sec++) {
      can->cd(sec);
      TH1D *W_hist_NPip_events =
          (TH1D *)root_data->Get(Form("NPip/W_hist_NPip_sec_%d", sec));
      for (int i = 0; i < 20; i++) {
        W_hist_NPip_events->SetBinContent(i + 1,
                                          W_hist_NPip_events->GetBinCenter(i));
      }
      Double_t par[9];
      TF1 *g1 = new TF1("g1", "gaus", 1.44, 1.58);
      TF1 *g2 = new TF1("g2", "gaus", 1.64, 1.77);
      TF1 *g3 = new TF1("g3", "pol2", 1.44, 1.77);
      TF1 *total = new TF1("gtotal", "gaus(0)+gaus(3)+ pol2(6)", 1.3, 1.9);
      W_hist_NPip_events->Fit(g1, "NR");
      W_hist_NPip_events->Fit(g2, "NR+");
      W_hist_NPip_events->Fit(g3, "NR+");
      g1->GetParameters(&par[0]);
      g2->GetParameters(&par[3]);
      g3->GetParameters(&par[6]);
      total->SetParameters(par);

      W_hist_NPip_events->Fit(total, "R+");

      W_hist_NPip_events->Draw("");
      // g1->Draw("same");
      // g2->Draw("same");
      // g3->Draw("same");

      TLine *l = new TLine(g1->GetParameter(1), 0, g1->GetParameter(1),
                           W_hist_NPip_events->GetMaximum() * 1.05);
      l->Draw("SAME");
      TLine *r = new TLine(g2->GetParameter(1), 0, g2->GetParameter(1),
                           W_hist_NPip_events->GetMaximum() * 1.05);
      r->Draw("SAME");
    }
    can->SaveAs(Form("%s.png", can->GetName()));
  }
  // {
  //   auto c = new TCanvas();
  //   c->SetGrid();
  //   auto mg = new TMultiGraph("Resonance Region", "Resonance Region");
  //   // c->Divide(2, 1);
  //   Double_t x_R1[7], y_R1[7], ErrX_R1[7], ErrY_R1[7];
  //   Double_t x_R2[7], y_R2[7], ErrX_R2[7], ErrY_R2[7];
  //
  //   for (int sec = 0; sec < 7; sec++) {
  //
  //     TH1D *W_hist_NPip_events_ =
  //         (TH1D *)root_data->Get(Form("NPip/W_hist_NPip_sec_%d", sec));
  //     W_hist_NPip_events_->Draw();
  //     for (int i = 0; i < 7; i++) {
  //       W_hist_NPip_events_->SetBinContent(
  //           i + 1, W_hist_NPip_events_->GetBinCenter(i));
  //     }
  //     Double_t par[9];
  //     TF1 *g1 = new TF1("g1", "gaus", 1.44, 1.58);
  //     TF1 *g2 = new TF1("g2", "gaus", 1.64, 1.77);
  //     TF1 *g3 = new TF1("g3", "pol2", 1.44, 1.77);
  //     TF1 *total = new TF1("gtotal", "gaus(0)+gaus(3)- pol2(6)", 1.44, 1.77);
  //     W_hist_NPip_events_->Fit(g1, "NR");
  //     W_hist_NPip_events_->Fit(g2, "NR+");
  //     W_hist_NPip_events_->Fit(g3, "NR+");
  //     W_hist_NPip_events_->Fit(total, "NR+");
  //
  //     //  g1->GetParameters(&par[0]);
  //     // g2->GetParameters(&par[3]);
  //     // g3->GetParameters(&par[6]);
  //     total->SetParameters(par);
  //
  //     x_R1[sec] = static_cast<double>(sec);
  //     y_R1[sec] = g1->GetParameter(1);
  //     ErrX_R1[sec] = 0;
  //     ErrY_R1[sec] = g1->GetParError(1);
  //
  //     x_R2[sec] = static_cast<double>(sec);
  //     y_R2[sec] = g2->GetParameter(1);
  //     ErrX_R2[sec] = 0;
  //     ErrY_R2[sec] = g2->GetParError(1);
  //   }
  //   //  if(sec !=0){
  //   TGraphErrors *graph = new TGraphErrors(7, x_R1, y_R1, ErrX_R1, ErrY_R1);
  //   graph->SetTitle("2nd Resonence Region");
  //   graph->SetMarkerColor(4);
  //   // graph->SetDrawOption("AP");
  //   graph->SetMarkerStyle(21);
  //
  //   TGraphErrors *graph2 = new TGraphErrors(7, x_R2, y_R2, ErrX_R2, ErrY_R2);
  //   graph2->SetTitle("3rd Resonence Region");
  //
  //   graph2->SetMarkerColor(2);
  //   graph2->SetMarkerStyle(22);
  //   mg->Add(graph);
  //   mg->Add(graph2);
  //   mg->Draw("AP");
  //   mg->GetXaxis()->SetTitle("Sectors");
  //   mg->GetXaxis()->SetLimits(-0.5, 6.5);
  //   mg->GetYaxis()->SetTitle("W (Gev)");
  //   mg->SetMinimum(1.4);
  //   mg->SetMaximum(1.8);
  //   c->BuildLegend();
  //
  //   TLine *l = new TLine(-0.25, y_R1[0], 6.25, y_R1[0]);
  //   l->SetLineColorAlpha(kRed, 0.35);
  //   l->Draw("");
  //   TLine *r = new TLine(-0.25, y_R2[0], 6.25, y_R2[0]);
  //   r->SetLineColorAlpha(kBlue, 0.35);
  //   r->Draw("");
  //   c->SaveAs(Form("%s.png", c->GetName()));
  //   c->Modified();
  //   c->Update();
  // }
  //
  // // {
  // //   TCanvas *can = new TCanvas("E_vs_theta_e_all_events",
  // //                              "E_vs_theta_e_all_events", 1920, 1080);
  // //   can->Divide(3, 2);
  // //   for (int sec = 1; sec < 7; sec++) {
  // //     can->cd(sec);
  // //     TH2D *E_vs_theta_e_all_events =
  // //         (TH2D *)root_data->Get(Form("NPip/E_vs_theta_e_all_events_%d",
  // //         sec));
  //     E_vs_theta_e_all_events->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  //
  // {
  //   TCanvas *can = new TCanvas("E_vs_theta_e_elastic_events",
  //                              "E_vs_theta_e_elastic_events", 1920, 1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH2D *E_vs_theta_e_elastic_events = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_elastic_events_%d", sec));
  //     E_vs_theta_e_elastic_events->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  // {
  //   TCanvas *can = new TCanvas("E_vs_theta_e_2nd_resonance_region",
  //                              "E_vs_theta_e_2nd_resonance_region", 1920,
  //                              1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH2D *E_vs_theta_e_2nd_resonance_region = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_2nd_resonance_region_%d", sec));
  //     E_vs_theta_e_2nd_resonance_region->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  // {
  //   TCanvas *can = new TCanvas("E_vs_theta_e_3rd_resonance_region",
  //                              "E_vs_theta_e_3rd_resonance_region", 1920,
  //                              1080);
  //   can->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     can->cd(sec);
  //     TH2D *E_vs_theta_e_3rd_resonance_region = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_3rd_resonance_region_%d", sec));
  //     E_vs_theta_e_3rd_resonance_region->Draw("");
  //   }
  //   can->SaveAs(Form("%s.png", can->GetName()));
  // }
  // {
  //   TCanvas *c1 =
  //       new TCanvas("E'_vs_theta_e'_sec", "E'_vs_theta_e'_sec", 1920,
  //       1080);
  //   c1->SetGrid();
  //   c1->Divide(3, 2);
  //   for (int sec = 1; sec < 7; sec++) {
  //     c1->cd(sec);
  //     TH2D *E_vs_theta_e_elastic_events = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_elastic_events_%d", sec));
  //     TH2D *E_vs_theta_e_2nd_rsonance_region = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_2nd_resonance_region_%d", sec));
  //     TH2D *E_vs_theta_e_3rd_rsonance_region = (TH2D *)root_data->Get(
  //         Form("NPip/E_vs_theta_e_3rd_resonance_region_%d", sec));
  //
  //     //  E_vs_theta_e_elastic_events->SetStats(0);
  //     E_vs_theta_e_elastic_events->SetTitle("E'_vs_theta_e'_sec");
  //     E_vs_theta_e_elastic_events->Draw("Col");
  //     E_vs_theta_e_2nd_rsonance_region->Draw("same ");
  //     E_vs_theta_e_3rd_rsonance_region->Draw("same ");
  //   }
  //   c1->SaveAs(Form("%s.png", c1->GetName()));
  // }

  /*
    {
      TCanvas *can = new TCanvas("W in Q^2 bins", "W in Q^2 bins", 1920,
    1080);
      // can->DivideSquare(10);
      THnSparseD *nsparce = (THnSparseD *)root_data->Get("nsparce");
      can->cd(0);
      nsparce->Projection(1, 0)->Draw("COLZ");
      nsparce->Projection(1, 0)->SetShowProjectionX(true);
    }
  */
  return 0;
}
