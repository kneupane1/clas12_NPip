/************************************************************************/
/*  Created by Nick Tyler*/
/*	University Of South Carolina*/
/************************************************************************/

#ifndef REACTION_H_GUARD
#define REACTION_H_GUARD

#include "TLorentzVector.h"
#include "TVector3.h"
#include "branches.hpp"
#include "constants.hpp"
#include "physics.hpp"
#include <iostream>

class Reaction {
protected:
  std::shared_ptr<Branches12> _data;

  double _beam_energy = 10.6;
  std::unique_ptr<TLorentzVector> _beam;
  std::unique_ptr<TLorentzVector> _elec;
  std::unique_ptr<TLorentzVector> _gamma;
  std::unique_ptr<TLorentzVector> _target;
  std::unique_ptr<TLorentzVector> _prot = nullptr;
  std::unique_ptr<TLorentzVector> _pip;

  std::vector<std::unique_ptr<TLorentzVector>> _pos;
  std::vector<std::unique_ptr<TLorentzVector>> _other;
  std::vector<std::shared_ptr<TLorentzVector>> _photons;

  bool _hasE = false;
  bool _hasPos = false;
  bool _hasOther = false;
  bool _hasPip = false;

  short _numPos = 0;
  short _numNeutral = 0;
  short _numOther = 0;
  short _numPip = 0;
  short _numProt = 0;

  std::vector<float> _pos_beta;
  std::vector<short> _pos_det;
  short _sector = -1;

  float _pi0_mass = NAN;

  float _MM = NAN;
  float _MM2 = NAN;
  float _MM_NPip = NAN;
  float _MM2_NPip = NAN;

  float _W = NAN;
  float _Q2 = NAN;

  float _beam_theta = NAN;
  float _elec_theta = NAN;
  float _E_elec = NAN;

  void SetElec();

public:
  Reaction(){};
  Reaction(const std::shared_ptr<Branches12> &data, float beam_energy);
  ~Reaction();

  bool PosStats();
  void SetPositive(int i);
  void SetPip(int i);
  void SetOther(int i);

  void CalcMissMass();
  void CalcMissMass_NPip();
  void CalcMassPi0();
  float pi0_mass();

  float MM();
  float MM2();
  float MM_NPip();
  float MM2_NPip();

  float theta_beam();
  float theta_elec();
  float E_elec();

  inline float W() { return _W; }
  inline float Q2() { return _Q2; }
  inline short sec() { return _data->dc_sec(0); }
  inline short det() { return abs(_data->status(0) / 1000); }
  inline float pos_beta() {
    if (_pos_beta.size() == 0)
      return NAN;
    return _pos_beta.front();
  }
  inline float pos_P() {
    if (_pos.size() == 0)
      return NAN;

    return _pos.front()->P();
  }
  inline short pos_det() {
    if (_pos_det.size() == 0)
      return -1;
    return detector_fill[_pos_det.front()];
  }
  inline float pos_theta() {
    if (_pos.size() == 0)
      return NAN;
    return _pos.front()->Theta() * RAD2DEG;
  }

  inline float pos_theta_calc() {
    if (_pos.size() == 0)
      return NAN;
    double v = _elec->P() / _elec->M();
    double num = v * sin(_elec->Theta());
    double den = (v * cos(_elec->Theta()) - v);

    // std::cout << -((atan(num / den) * RAD2DEG) / 2.0) -
    // (_pos.front()->Theta() * RAD2DEG) << std::endl;

    return -(atan(num / den) * RAD2DEG);
  }

  inline float phi_e() { return _elec->Phi(); }
  inline float phi_p() {
    if (_pos.size() == 0)
      return NAN;
    return _pos.front()->Phi();
  }
  inline float phi_diff() { return abs(_elec->Phi() - phi_p()); }
  inline bool phi_diff_180() {
    // Cut around 10% of peak
    if (phi_diff() > phi_min_cut && phi_diff() < phi_max_cut)
      return true;
    return false;
  }
  inline bool MM_cut() { return abs(MM2()) < MM2_cut; }

  inline bool elastic() { // onePositive_at180_MM0() {
    return (_hasE && _hasPos && _pos.size() == 1 && _other.size() == 0 &&
            phi_diff_180() /* && MM_cut()*/);
  }

  inline bool MM_NPip_cut() { return abs(MM2_NPip()) < MM2_cut; }
  inline bool NPip() { return (_hasE && _hasPip && _numPip == 1); }
};

#endif
