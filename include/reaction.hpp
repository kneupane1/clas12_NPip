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
  std::unique_ptr<TLorentzVector> _pip;
  std::unique_ptr<TLorentzVector> _prot;

  std::vector<std::unique_ptr<TLorentzVector>> _other;
  std::vector<std::shared_ptr<TLorentzVector>> _photons;

  bool _hasE = false;
  bool _hasOther = false;
  bool _hasPip = false;
  bool _hasProt = false;

  short _numNeg = 0;
  short _numNeutral = 0;
  short _numOther = 0;
  short _numPip = 0;
  short _numProt = 0;

  short _sector = -1;

  float _pi0_mass = NAN;

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

  void SetPip(int i);

  void SetProt(int i);

  void SetOther(int i);

  void CalcMissMass();
  void CalcMassPi0();
  float pi0_mass();

  float MM_NPip();
  float MM2_NPip();

  float theta_beam();
  float theta_elec();
  float E_elec();

  inline float W() { return _W; }
  inline float Q2() { return _Q2; }
  inline short sec() { return _data->dc_sec(0); }
  inline short det() { return abs(_data->status(0) / 1000); }

  inline float phi_e() { return _elec->Phi(); }
  inline bool elastic() {
    return (_hasE && _hasProt && !_hasPip && !_hasOther && _numProt == 1);
  }
  inline bool MM_cut() { return abs(MM2_NPip()) < MM2_cut; }
  inline bool NPip() { return (_hasE && _hasPip && _numPip == 1); }
};

#endif
