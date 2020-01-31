/**************************************/
/*																		*/
/*  Created by Nick Tyler             */
/*	University Of South Carolina      */
/**************************************/
#include "histogram.hpp"

Histogram::Histogram(const std::string &output_file) {
  RootOutputFile = std::make_shared<TFile>(output_file.c_str(), "RECREATE");
  def = std::make_shared<TCanvas>("def");

  makeHists();
  Nsparce = std::make_shared<THnSparseD>("nsparce", "nsparce", 3, sparce_bins,
                                         sparce_xmin, sparce_xmax);
  makeHists_electron_cuts();
}

Histogram::~Histogram() { this->Write(); }

void Histogram::Write() {
  std::cout << GREEN << "Writting" << DEF << std::endl;
  Nsparce->Write();
  std::cout << BOLDBLUE << "WvsQ2()" << DEF << std::endl;
  Write_WvsQ2();

  deltaT_proton[before_cut]->Write();
  deltaT_proton[after_cut]->Write();

  std::cerr << BOLDBLUE << "Write_Electron_cuts()" << DEF << std::endl;
  TDirectory *Electron_Cuts = RootOutputFile->mkdir("Electron_Cuts");
  Electron_Cuts->cd();
  Write_Electron_cuts();

  std::cout << BOLDBLUE << "Done Writing!!!" << DEF << std::endl;
}

void Histogram::makeHists() {
  deltaT_proton[before_cut] = std::make_shared<TH2D>(
      "DeltaTProton", "DeltaTProton", bins, zero, 10, bins, -5, 5);
  deltaT_proton[after_cut] = std::make_shared<TH2D>(
      "DeltaTProton_cut", "DeltaTProton_cut", bins, zero, 10, bins, -5, 5);

  for (short sec = 0; sec < NUM_SECTORS; sec++) {
    E_vs_theta_e_all_events[sec] =
        std::make_shared<TH2D>(Form("E_vs_theta_e_all_events_%d", sec),
                               Form("E_vs_theta_e_all_events_%d", sec), bins,
                               zero, 40.0, bins, zero, p_max);
    E_vs_theta_e_elastic_events[sec] =
        std::make_shared<TH2D>(Form("E_vs_theta_e_elastic_events_%d", sec),
                               Form("E_vs_theta_e_elastic_events_%d", sec),
                               bins, zero, 40.0, bins, zero, p_max);
    E_vs_theta_e_2nd_reso_events[sec] = std::make_shared<TH2D>(
        Form("E_vs_theta_e_2nd_resonance_region_%d", sec),
        Form("E_vs_theta_e_2nd_resonance_region_%d", sec), bins, zero, 40.0,
        bins, zero, p_max);
    E_vs_theta_e_3rd_reso_events[sec] = std::make_shared<TH2D>(
        Form("E_vs_theta_e_3rd_resonance_region_%d", sec),
        Form("E_vs_theta_e_3rd_resonance_region_%d", sec), bins, zero, 40.0,
        bins, zero, p_max);

    W_hist_NPip_before_mmsq_cut_events[sec] = std::make_shared<TH1D>(
        Form("W_hist_NPip_events_sec_%d", sec),
        Form("W_hist_NPip_events_sec_%d", sec), bins, zero, w_max);

    W_hist_NPip_events[sec] = std::make_shared<TH1D>(
        Form("W_hist_NPip_sec_%d", sec), Form("W_hist_NPip_sec_%d", sec), bins,
        0.0, w_max);
    MM2_hist_NPip_events[sec] = std::make_shared<TH1D>(
        Form("MM2_hist_NPip_sec_%d", sec), Form("MM2_hist_NPip_sec_%d", sec),
        bins, -1.0, 16.0);

    MissingMass[sec] = std::make_shared<TH1D>(Form("MM2_hist_sec_%d", sec),
                                              Form("MM2_hist_sec_%d", sec),
                                              bins, -w_max, w_max);

    W_hist_all_events[sec] =
        std::make_shared<TH1D>(Form("W_hist_sec_%d", sec),
                               Form("W_hist_sec_%d", sec), bins, zero, w_max);

    W_vs_q2_all_events[sec] =
        std::make_shared<TH2D>(Form("WQ2_sec_%d", sec), Form("WQ2_sec_%d", sec),
                               bins, zero, w_max, bins, zero, q2_max);
  }
}

void Histogram::makeHists_electron_cuts() {
  for (auto &&cut : before_after_cut) {
    int c = cut.first;
    auto type = cut.second.c_str();
    EC_sampling_fraction[c] =
        std::make_shared<TH2D>(Form("EC_sampling_fraction%s", type),
                               Form("EC_sampling_fraction%s", type), bins,
                               p_min, p_max, bins, zero, 0.50);
    vz_position[c] =
        std::make_shared<TH1D>(Form("vz_position%s", type),
                               Form("vz_position%s", type), bins, -40, 40);
    pcal_sec[c] = std::make_shared<TH2D>(Form("pcal_sec%s", type),
                                         Form("pcal_sec%s", type), bins, -420,
                                         420, bins, -420, 420);
    dcr1_sec[c] = std::make_shared<TH2D>(Form("dcr1_sec%s", type),
                                         Form("dcr1_sec%s", type), bins, -180,
                                         180, bins, -180, 180);
    dcr2_sec[c] = std::make_shared<TH2D>(Form("dcr2_sec%s", type),
                                         Form("dcr2_sec%s", type), bins, -270,
                                         270, bins, -270, 270);
    dcr3_sec[c] = std::make_shared<TH2D>(Form("dcr3_sec%s", type),
                                         Form("dcr3_sec%s", type), bins, -320,
                                         320, bins, -320, 320);
  }
}

void Histogram::FillHists_electron_cuts(const std::shared_ptr<Branches12> &_d) {
  vz_position[before_cut]->Fill(_d->vz(0));
  pcal_sec[before_cut]->Fill(_d->ec_pcal_x(0), _d->ec_pcal_y(0));
  dcr1_sec[before_cut]->Fill(_d->dc_r1_x(0), _d->dc_r1_y(0));
  dcr2_sec[before_cut]->Fill(_d->dc_r2_x(0), _d->dc_r2_y(0));
  dcr3_sec[before_cut]->Fill(_d->dc_r3_x(0), _d->dc_r3_y(0));
  EC_sampling_fraction[before_cut]->Fill(_d->p(0),
                                         _d->ec_tot_energy(0) / _d->p(0));
}

void Histogram::FillHists_electron_with_cuts(
    const std::shared_ptr<Branches12> &_d) {
  vz_position[after_cut]->Fill(_d->vz(0));
  pcal_sec[after_cut]->Fill(_d->ec_pcal_x(0), _d->ec_pcal_y(0));
  dcr1_sec[after_cut]->Fill(_d->dc_r1_x(0), _d->dc_r1_y(0));
  dcr2_sec[after_cut]->Fill(_d->dc_r2_x(0), _d->dc_r2_y(0));
  dcr3_sec[after_cut]->Fill(_d->dc_r3_x(0), _d->dc_r3_y(0));
  EC_sampling_fraction[after_cut]->Fill(_d->p(0),
                                        _d->ec_tot_energy(0) / _d->p(0));
}
void Histogram::Write_Electron_cuts() {
  for (auto &&cut : before_after_cut) {
    int c = cut.first;
    vz_position[c]->SetXTitle("vz (GeV)");

    if (vz_position[c]->GetEntries())
      vz_position[c]->Write();
    pcal_sec[c]->SetXTitle("x (cm)");
    pcal_sec[c]->SetYTitle("y (cm)");
    pcal_sec[c]->SetOption("COLZ1");
    if (pcal_sec[c]->GetEntries())
      pcal_sec[c]->Write();

    dcr1_sec[c]->SetXTitle("x (cm)");
    dcr1_sec[c]->SetYTitle("y (cm)");
    dcr1_sec[c]->SetOption("COLZ1");
    if (dcr1_sec[c]->GetEntries())
      dcr1_sec[c]->Write();

    dcr2_sec[c]->SetXTitle("x (cm)");
    dcr2_sec[c]->SetYTitle("y (cm)");
    dcr2_sec[c]->SetOption("COLZ1");
    if (dcr2_sec[c]->GetEntries())
      dcr2_sec[c]->Write();

    dcr3_sec[c]->SetXTitle("x (cm)");
    dcr3_sec[c]->SetYTitle("y (cm)");
    dcr3_sec[c]->SetOption("COLZ1");
    if (dcr3_sec[c]->GetEntries())
      dcr3_sec[c]->Write();

    EC_sampling_fraction[c]->SetXTitle("Momentum (GeV)");
    EC_sampling_fraction[c]->SetYTitle("Sampling Fraction");
    EC_sampling_fraction[c]->SetOption("COLZ1");
    EC_sampling_fraction[c]->Write();
  }
}
void Histogram::Fill_Sparce(const std::shared_ptr<Reaction> &_e) {
  std::lock_guard<std::mutex> lk(mutex);
  double ret[NUM_DIM] = {_e->W(), _e->Q2(), static_cast<double>(_e->sec())};
  Nsparce->Fill(ret);
}

void Histogram::Fill_WvsQ2(const std::shared_ptr<Reaction> &_e) {
  short sec = _e->sec();
  if ((sec > 0 && sec < NUM_SECTORS)) {
    W_hist_all_events[all_sectors]->Fill(_e->W());
    W_vs_q2_all_events[all_sectors]->Fill(_e->W(), _e->Q2());
    W_hist_all_events[sec]->Fill(_e->W());
    W_vs_q2_all_events[sec]->Fill(_e->W(), _e->Q2());
    E_vs_theta_e_all_events[all_sectors]->Fill(_e->theta_elec(), _e->E_elec());
    E_vs_theta_e_all_events[sec]->Fill(_e->theta_elec(), _e->E_elec());

    if (_e->elastic()) {

      E_vs_theta_e_elastic_events[all_sectors]->Fill(_e->theta_elec(),
                                                     _e->E_elec());
      E_vs_theta_e_elastic_events[sec]->Fill(_e->theta_elec(), _e->E_elec());
    }

    if (_e->NPip()) {
      W_hist_NPip_before_mmsq_cut_events[all_sectors]->Fill(_e->W());
      W_hist_NPip_before_mmsq_cut_events[sec]->Fill(_e->W());
      MM2_hist_NPip_events[all_sectors]->Fill(_e->MM2_NPip());
      MM2_hist_NPip_events[sec]->Fill(_e->MM2_NPip());
      MM_hist_NPip_before_cut->Fill(_e->MM_NPip());
      MM2_hist_NPip_before_cut->Fill(_e->MM2_NPip());
      if (0.8 < _e->MM2_NPip() && _e->MM2_NPip() < 1.0) {
        W_hist_NPip_events[all_sectors]->Fill(_e->W());
        W_hist_NPip_events[sec]->Fill(_e->W());
        MM_hist_NPip->Fill(_e->MM_NPip());
        MM2_hist_NPip->Fill(_e->MM2_NPip());

        if (_e->W() > 1.44 && _e->W() < 1.58) {
          E_vs_theta_e_2nd_reso_events[all_sectors]->Fill(_e->theta_elec(),
                                                          _e->E_elec());
          E_vs_theta_e_2nd_reso_events[sec]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
        } else if (_e->W() > 1.64 && _e->W() < 1.73) {
          E_vs_theta_e_3rd_reso_events[all_sectors]->Fill(_e->theta_elec(),
                                                          _e->E_elec());
          E_vs_theta_e_3rd_reso_events[sec]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
        }
      }
    }
  }
}

void Histogram::Write_WvsQ2() {
  MM_hist_NPip_before_cut->SetXTitle("MM (GeV)");
  MM_hist_NPip_before_cut->Write();
  MM2_hist_NPip_before_cut->SetXTitle("MM2 (GeV)");
  MM2_hist_NPip_before_cut->Write();
  MM_hist_NPip->SetXTitle("MM (GeV)");
  MM_hist_NPip->Write();
  MM2_hist_NPip->SetXTitle("MM2 (GeV)");
  MM2_hist_NPip->Write();

  TDirectory *W_vs_Q2_folder = RootOutputFile->mkdir("W_vs_Q2");
  W_vs_Q2_folder->cd();
  for (int i = 0; i < NUM_SECTORS; i++) {
    MissingMass[i]->SetXTitle("MM^2 (GeV)");
    MissingMass[i]->Write();

    // W_hist_all_events[i] -> Fit("gaus", "QMR+", "QMR+", 0.8, 1.1);
    // gStyle->SetOptFit(1111);
    W_hist_all_events[i]->SetXTitle("W (GeV)");
    W_hist_all_events[i]->Write();

    W_vs_q2_all_events[i]->SetXTitle("W (GeV)");
    W_vs_q2_all_events[i]->SetYTitle("Q^2 (GeV^2)");
    W_vs_q2_all_events[i]->SetOption("COLZ");
    W_vs_q2_all_events[i]->Write();
  }
  W_vs_Q2_folder->Write();
  delete W_vs_Q2_folder;

  // NPip I added
  TDirectory *NPip_folder = RootOutputFile->mkdir("NPip");
  NPip_folder->cd();
  // auto W_NPip_can = std::make_unique<TCanvas>("W NPip_can", "W NPip
  // sectors", 1920, 1080); W_NPip_can->Divide(4, 2);
  for (short i = 0; i < NUM_SECTORS; i++) {
    W_hist_NPip_events[i]->SetXTitle("W (GeV)");
    W_hist_NPip_events[i]->Write();
    // W_NPip_can->cd(i + 1);
    // W_hist_NPip_events[i]->Draw("same");
    //}
    // W_NPip_can->Write();
    // for (short i = 0; i < NUM_SECTORS; i++) {
    W_hist_NPip_before_mmsq_cut_events[i]->SetXTitle("W (GeV)");
    W_hist_NPip_before_mmsq_cut_events[i]->Write();

    E_vs_theta_e_all_events[i]->SetXTitle("theta (deg)");
    E_vs_theta_e_all_events[i]->SetYTitle("E' (GeV)");
    E_vs_theta_e_all_events[i]->SetOption("COLZ");
    E_vs_theta_e_all_events[i]->Write();

    E_vs_theta_e_elastic_events[i]->SetXTitle("theta (deg)");
    E_vs_theta_e_elastic_events[i]->SetYTitle("E' (GeV)");
    E_vs_theta_e_elastic_events[i]->SetOption("COLZ");
    E_vs_theta_e_elastic_events[i]->Write();

    E_vs_theta_e_2nd_reso_events[i]->SetXTitle("theta (deg)");
    E_vs_theta_e_2nd_reso_events[i]->SetYTitle("E' (GeV)");
    E_vs_theta_e_2nd_reso_events[i]->SetOption("COLZ");
    E_vs_theta_e_2nd_reso_events[i]->Write();

    E_vs_theta_e_3rd_reso_events[i]->SetXTitle("theta_ (deg)");
    E_vs_theta_e_3rd_reso_events[i]->SetYTitle("E' (GeV)");
    E_vs_theta_e_3rd_reso_events[i]->SetOption("COLZ");
    E_vs_theta_e_3rd_reso_events[i]->Write();
    //}

    // auto MM2_NPip_can = std::make_unique<TCanvas>("MM2 NPip_can", "MM2 NPip
    // sectors", 1920, 1080); MM2_NPip_can->Divide(4, 2); for (short i = 0; i
    // < NUM_SECTORS; i++) {
    MM2_hist_NPip_events[i]->SetXTitle("MM2 (GeV)");
    MM2_hist_NPip_events[i]->Write();
    //  MM2_NPip_can->cd(i + 1);
    //  MM2_hist_NPip_events[i]->Draw("same");
  }
  // MM2_NPip_can->Write();
  NPip_folder->Write();
  delete NPip_folder;
}

void Histogram::Fill_Dt(const std::shared_ptr<Delta_T> &dt) {
  for (int i = 0; i < dt->gpart(); i++) {
    if (dt->charge(i) == 1)
      deltaT_proton[before_cut]->Fill(dt->mom(i), dt->dt_P(i));
  }
}

void Histogram::Fill_Dt(const std::shared_ptr<Delta_T> &dt, int part) {
  deltaT_proton[after_cut]->Fill(dt->mom(part), dt->dt_P(part));
}
