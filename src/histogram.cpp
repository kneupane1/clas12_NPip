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

  deltaT_proton[before_cut]->SetOption("COLZ");
  deltaT_proton[before_cut]->Write();
  deltaT_proton[after_cut]->SetOption("COLZ");
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

    mass_pi0_hist[before_cut][sec] =
        std::make_shared<TH1D>(Form("mass_pi0_hist_%d", sec),
                               Form("mass_pi0_hist_%d", sec), bins, 0, 2.5);
    mass_pi0_hist[after_cut][sec] = std::make_shared<TH1D>(
        Form("mass_pi0_hist_aferPcuts_%d", sec),
        Form("mass_pi0_hist_aferPcuts_%d", sec), bins, 0, 2.5);

    for (short q2 = 0; q2 < NUM_Q2_BINS; q2++) {
      E_vs_theta_e_all_events[sec][q2] =
          std::make_shared<TH2D>(Form("E_vs_theta_e_all_events_%d_%6.12s", sec,
                                      Q2_BIN_NAME[q2].c_str()),
                                 Form("E_vs_theta_e_all_events_%d_%6.12s", sec,
                                      Q2_BIN_NAME[q2].c_str()),
                                 bins, zero, 40.0, bins, zero, p_max);
      E_vs_theta_e_elastic_events[sec][q2] =
          std::make_shared<TH2D>(Form("E_vs_theta_e_elastic_events_%d_%6.12s",
                                      sec, Q2_BIN_NAME[q2].c_str()),
                                 Form("E_vs_theta_e_elastic_events_%d_%6.12s",
                                      sec, Q2_BIN_NAME[q2].c_str()),
                                 bins, zero, 40.0, bins, zero, p_max);
      E_vs_theta_e_2nd_reso_events[sec][q2] = std::make_shared<TH2D>(
          Form("E_vs_theta_e_2nd_resonance_region_%d_%6.12s", sec,
               Q2_BIN_NAME[q2].c_str()),
          Form("E_vs_theta_e_2nd_resonance_region_%d_%6.12s", sec,
               Q2_BIN_NAME[q2].c_str()),
          bins, zero, 40.0, bins, zero, p_max);
      E_vs_theta_e_3rd_reso_events[sec][q2] = std::make_shared<TH2D>(
          Form("E_vs_theta_e_3rd_resonance_region_%d_%6.12s", sec,
               Q2_BIN_NAME[q2].c_str()),
          Form("E_vs_theta_e_3rd_resonance_region_%d_%6.12s", sec,
               Q2_BIN_NAME[q2].c_str()),
          bins, zero, 40.0, bins, zero, p_max);

      W_hist_NPip_before_mmsq_cut_events[sec][q2] =
          std::make_shared<TH1D>(Form("W_hist_NPip_events_sec_%d_%6.12s", sec,
                                      Q2_BIN_NAME[q2].c_str()),
                                 Form("W_hist_NPip_events_sec_%d_%6.12s", sec,
                                      Q2_BIN_NAME[q2].c_str()),
                                 bins, zero, w_max);

      W_hist_NPip_events[sec][q2] = std::make_shared<TH1D>(
          Form("W_hist_NPip_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          Form("W_hist_NPip_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()), bins,
          0.0, w_max);
      MM2_hist_NPip_events[sec][q2] = std::make_shared<TH1D>(
          Form("MM2_hist_NPip_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          Form("MM2_hist_NPip_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          bins, -1.0, 16.0);

      W_hist_all_events[sec][q2] = std::make_shared<TH1D>(
          Form("W_hist_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          Form("W_hist_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()), bins,
          zero, w_max);

      W_vs_q2_all_events[sec][q2] = std::make_shared<TH2D>(
          Form("WQ2_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          Form("WQ2_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()), bins, zero,
          w_max, bins, zero, q2_max);

      MissingMass[sec][q2] = std::make_shared<TH1D>(
          Form("MM2_hist_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()),
          Form("MM2_hist_sec_%d_%6.12s", sec, Q2_BIN_NAME[q2].c_str()), bins,
          -w_max, w_max);
    }
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
  short pos_det = _e->pos_det();
  short sec = _e->sec();

  if ((sec > 0 && sec < NUM_SECTORS) || pos_det != -1) {

    W_hist_all_events[all_sectors][0]->Fill(_e->W());
    W_vs_q2_all_events[all_sectors][0]->Fill(_e->W(), _e->Q2());
    W_hist_all_events[sec][0]->Fill(_e->W());
    W_vs_q2_all_events[sec][0]->Fill(_e->W(), _e->Q2());
    E_vs_theta_e_all_events[all_sectors][0]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
    E_vs_theta_e_all_events[sec][0]->Fill(_e->theta_elec(), _e->E_elec());
    if (_e->elastic()) {
      E_vs_theta_e_elastic_events[all_sectors][0]->Fill(_e->theta_elec(),
                                                        _e->E_elec());
      E_vs_theta_e_elastic_events[sec][0]->Fill(_e->theta_elec(), _e->E_elec());
      MissingMass[all_sectors][0]->Fill(_e->MM2());
      MissingMass[sec][0]->Fill(_e->MM2());
    }

    if (_e->NPip()) {
      W_hist_NPip_before_mmsq_cut_events[all_sectors][0]->Fill(_e->W());
      W_hist_NPip_before_mmsq_cut_events[sec][0]->Fill(_e->W());
      MM2_hist_NPip_events[all_sectors][0]->Fill(_e->MM2_NPip());
      MM2_hist_NPip_events[sec][0]->Fill(_e->MM2_NPip());
      MM_hist_NPip_before_cut->Fill(_e->MM_NPip());
      MM2_hist_NPip_before_cut->Fill(_e->MM2_NPip());
      if (_e->MM_NPip_cut()) {
        W_hist_NPip_events[all_sectors][0]->Fill(_e->W());
        W_hist_NPip_events[sec][0]->Fill(_e->W());
        MM_hist_NPip->Fill(_e->MM_NPip());
        MM2_hist_NPip->Fill(_e->MM2_NPip());

        Q2_hist_NPip->Fill(_e->Q2());

        if (_e->W() > 1.44 && _e->W() < 1.58) {
          E_vs_theta_e_2nd_reso_events[all_sectors][0]->Fill(_e->theta_elec(),
                                                             _e->E_elec());
          E_vs_theta_e_2nd_reso_events[sec][0]->Fill(_e->theta_elec(),
                                                     _e->E_elec());
        } else if (_e->W() > 1.64 && _e->W() < 1.73) {
          E_vs_theta_e_3rd_reso_events[all_sectors][0]->Fill(_e->theta_elec(),
                                                             _e->E_elec());
          E_vs_theta_e_3rd_reso_events[sec][0]->Fill(_e->theta_elec(),
                                                     _e->E_elec());
        }
      }
    }

    if (_e->Q2() < 2.0) {

      W_hist_all_events[all_sectors][1]->Fill(_e->W());
      W_vs_q2_all_events[all_sectors][1]->Fill(_e->W(), _e->Q2());
      W_hist_all_events[sec][1]->Fill(_e->W());
      W_vs_q2_all_events[sec][1]->Fill(_e->W(), _e->Q2());
      E_vs_theta_e_all_events[all_sectors][1]->Fill(_e->theta_elec(),
                                                    _e->E_elec());
      E_vs_theta_e_all_events[sec][1]->Fill(_e->theta_elec(), _e->E_elec());
      if (_e->elastic()) {
        E_vs_theta_e_elastic_events[all_sectors][1]->Fill(_e->theta_elec(),
                                                          _e->E_elec());
        E_vs_theta_e_elastic_events[sec][1]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
        MissingMass[all_sectors][1]->Fill(_e->MM2());
        MissingMass[sec][1]->Fill(_e->MM2());
      }

      if (_e->NPip()) {
        W_hist_NPip_before_mmsq_cut_events[all_sectors][1]->Fill(_e->W());
        W_hist_NPip_before_mmsq_cut_events[sec][1]->Fill(_e->W());
        MM2_hist_NPip_events[all_sectors][1]->Fill(_e->MM2_NPip());
        MM2_hist_NPip_events[sec][1]->Fill(_e->MM2_NPip());

        if (_e->MM_NPip_cut()) {
          W_hist_NPip_events[all_sectors][1]->Fill(_e->W());
          W_hist_NPip_events[sec][1]->Fill(_e->W());

          Q2_hist_NPip->Fill(_e->Q2());

          if (_e->W() > 1.44 && _e->W() < 1.58) {
            E_vs_theta_e_2nd_reso_events[all_sectors][1]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_2nd_reso_events[sec][1]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          } else if (_e->W() > 1.64 && _e->W() < 1.73) {
            E_vs_theta_e_3rd_reso_events[all_sectors][1]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_3rd_reso_events[sec][1]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          }
        }
      }
    } else if (_e->Q2() > 2. && _e->Q2() < 3.5) {

      W_hist_all_events[all_sectors][2]->Fill(_e->W());
      W_vs_q2_all_events[all_sectors][2]->Fill(_e->W(), _e->Q2());
      W_hist_all_events[sec][2]->Fill(_e->W());
      W_vs_q2_all_events[sec][2]->Fill(_e->W(), _e->Q2());
      E_vs_theta_e_all_events[all_sectors][2]->Fill(_e->theta_elec(),
                                                    _e->E_elec());
      E_vs_theta_e_all_events[sec][2]->Fill(_e->theta_elec(), _e->E_elec());
      if (_e->elastic()) {
        E_vs_theta_e_elastic_events[all_sectors][2]->Fill(_e->theta_elec(),
                                                          _e->E_elec());
        E_vs_theta_e_elastic_events[sec][2]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
        MissingMass[all_sectors][2]->Fill(_e->MM2());
        MissingMass[sec][2]->Fill(_e->MM2());
      }

      if (_e->NPip()) {
        W_hist_NPip_before_mmsq_cut_events[all_sectors][2]->Fill(_e->W());
        W_hist_NPip_before_mmsq_cut_events[sec][2]->Fill(_e->W());
        MM2_hist_NPip_events[all_sectors][2]->Fill(_e->MM2_NPip());
        MM2_hist_NPip_events[sec][2]->Fill(_e->MM2_NPip());

        if (_e->MM_NPip_cut()) {
          W_hist_NPip_events[all_sectors][2]->Fill(_e->W());
          W_hist_NPip_events[sec][2]->Fill(_e->W());

          Q2_hist_NPip->Fill(_e->Q2());

          if (_e->W() > 1.44 && _e->W() < 1.58) {
            E_vs_theta_e_2nd_reso_events[all_sectors][2]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_2nd_reso_events[sec][2]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          } else if (_e->W() > 1.64 && _e->W() < 1.73) {
            E_vs_theta_e_3rd_reso_events[all_sectors][2]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_3rd_reso_events[sec][2]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          }
        }
      }
    } else if (_e->Q2() > 3.5 && _e->Q2() < 12.0) {
      W_hist_all_events[all_sectors][3]->Fill(_e->W());
      W_vs_q2_all_events[all_sectors][3]->Fill(_e->W(), _e->Q2());
      W_hist_all_events[sec][3]->Fill(_e->W());
      W_vs_q2_all_events[sec][3]->Fill(_e->W(), _e->Q2());
      E_vs_theta_e_all_events[all_sectors][3]->Fill(_e->theta_elec(),
                                                    _e->E_elec());
      E_vs_theta_e_all_events[sec][3]->Fill(_e->theta_elec(), _e->E_elec());
      if (_e->elastic()) {
        E_vs_theta_e_elastic_events[all_sectors][3]->Fill(_e->theta_elec(),
                                                          _e->E_elec());
        E_vs_theta_e_elastic_events[sec][3]->Fill(_e->theta_elec(),
                                                  _e->E_elec());
        MissingMass[all_sectors][3]->Fill(_e->MM2());
        MissingMass[sec][3]->Fill(_e->MM2());
      }

      if (_e->NPip()) {
        W_hist_NPip_before_mmsq_cut_events[all_sectors][3]->Fill(_e->W());
        W_hist_NPip_before_mmsq_cut_events[sec][3]->Fill(_e->W());
        MM2_hist_NPip_events[all_sectors][3]->Fill(_e->MM2_NPip());
        MM2_hist_NPip_events[sec][3]->Fill(_e->MM2_NPip());

        if (_e->MM_NPip_cut()) {
          W_hist_NPip_events[all_sectors][3]->Fill(_e->W());
          W_hist_NPip_events[sec][3]->Fill(_e->W());

          Q2_hist_NPip->Fill(_e->Q2());

          if (_e->W() > 1.44 && _e->W() < 1.58) {
            E_vs_theta_e_2nd_reso_events[all_sectors][3]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_2nd_reso_events[sec][3]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          } else if (_e->W() > 1.64 && _e->W() < 1.73) {
            E_vs_theta_e_3rd_reso_events[all_sectors][3]->Fill(_e->theta_elec(),
                                                               _e->E_elec());
            E_vs_theta_e_3rd_reso_events[sec][3]->Fill(_e->theta_elec(),
                                                       _e->E_elec());
          }
        }
      }
    }
  }
}
void Histogram::Fill_pi0(const std::shared_ptr<Reaction> &_e) {
  if (_e->pi0_mass() < 0.0001)
    return;
  short sec = _e->sec();
  short pos_det = _e->pos_det();
  mass_pi0_hist[before_cut][all_sectors]->Fill(_e->pi0_mass());
  if ((sec > 0 && sec < NUM_SECTORS) || pos_det != -1) {
    mass_pi0_hist[before_cut][sec]->Fill(_e->pi0_mass());
    if (_e->elastic()) {
      mass_pi0_hist[after_cut][all_sectors]->Fill(_e->pi0_mass());
      mass_pi0_hist[after_cut][sec]->Fill(_e->pi0_mass());
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
  Q2_hist_NPip->SetTitle("Q2_NPip");
  Q2_hist_NPip->Write();

  TDirectory *W_vs_Q2_folder = RootOutputFile->mkdir("W_vs_Q2");
  W_vs_Q2_folder->cd();
  for (int i = 0; i < NUM_SECTORS; i++) {
    mass_pi0_hist[before_cut][i]->SetXTitle("MM(GeV)");
    mass_pi0_hist[before_cut][i]->Write();

    mass_pi0_hist[after_cut][i]->SetXTitle("MM(GeV)");
    mass_pi0_hist[after_cut][i]->Write();
    for (int j = 0; j < NUM_Q2_BINS; j++) {

      MissingMass[i][j]->SetXTitle("MM^2 (GeV)");
      MissingMass[i][j]->Write();

      // W_hist_all_events[i][j] -> Fit("gaus", "QMR+", "QMR+", 0.8, 1.1);
      // gStyle->SetOptFit(1111);
      W_hist_all_events[i][j]->SetXTitle("W (GeV)");
      W_hist_all_events[i][j]->Write();

      W_vs_q2_all_events[i][j]->SetXTitle("W (GeV)");
      W_vs_q2_all_events[i][j]->SetYTitle("Q^2 (GeV^2)");
      W_vs_q2_all_events[i][j]->SetOption("COLZ");
      W_vs_q2_all_events[i][j]->Write();
    }
  }
  W_vs_Q2_folder->Write();
  delete W_vs_Q2_folder;

  // NPip I added
  TDirectory *NPip_folder = RootOutputFile->mkdir("NPip");
  NPip_folder->cd();
  // auto W_NPip_can = std::make_unique<TCanvas>("W NPip_can", "W NPip
  // sectors", 1920, 1080); W_NPip_can->Divide(4, 2);
  for (short i = 0; i < NUM_SECTORS; i++) {
    for (int j = 0; j < NUM_Q2_BINS; j++) {

      W_hist_NPip_events[i][j]->SetXTitle("W (GeV)");
      W_hist_NPip_events[i][j]->Write();
      // W_NPip_can->cd(i + 1);
      // W_hist_NPip_events[i][j]->Draw("same");
      //}
      // W_NPip_can->Write();
      // for (short i = 0; i < NUM_SECTORS; i++) {
      W_hist_NPip_before_mmsq_cut_events[i][j]->SetXTitle("W (GeV)");
      W_hist_NPip_before_mmsq_cut_events[i][j]->Write();

      E_vs_theta_e_all_events[i][j]->SetXTitle("theta (deg)");
      E_vs_theta_e_all_events[i][j]->SetYTitle("E' (GeV)");
      E_vs_theta_e_all_events[i][j]->SetOption("COLZ");
      E_vs_theta_e_all_events[i][j]->Write();

      E_vs_theta_e_elastic_events[i][j]->SetXTitle("theta (deg)");
      E_vs_theta_e_elastic_events[i][j]->SetYTitle("E' (GeV)");
      E_vs_theta_e_elastic_events[i][j]->SetOption("COLZ");
      E_vs_theta_e_elastic_events[i][j]->Write();

      E_vs_theta_e_2nd_reso_events[i][j]->SetXTitle("theta (deg)");
      E_vs_theta_e_2nd_reso_events[i][j]->SetYTitle("E' (GeV)");
      E_vs_theta_e_2nd_reso_events[i][j]->SetOption("COLZ");
      E_vs_theta_e_2nd_reso_events[i][j]->Write();

      E_vs_theta_e_3rd_reso_events[i][j]->SetXTitle("theta_ (deg)");
      E_vs_theta_e_3rd_reso_events[i][j]->SetYTitle("E' (GeV)");
      E_vs_theta_e_3rd_reso_events[i][j]->SetOption("COLZ");
      E_vs_theta_e_3rd_reso_events[i][j]->Write();
      //}

      // auto MM2_NPip_can = std::make_unique<TCanvas>("MM2 NPip_can", "MM2 NPip
      // sectors", 1920, 1080); MM2_NPip_can->Divide(4, 2); for (short i = 0; i
      // < NUM_SECTORS; i++) {
      MM2_hist_NPip_events[i][j]->SetXTitle("MM2 (GeV)");
      MM2_hist_NPip_events[i][j]->Write();
      //  MM2_NPip_can->cd(i + 1);
      //  MM2_hist_NPip_events[i][j]->Draw("same");
    }
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
