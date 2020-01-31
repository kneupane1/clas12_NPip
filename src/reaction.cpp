/**************************************/
/*																		*/
/*  Created by Nick Tyler             */
/*	University Of South Carolina      */
/**************************************/
#include "reaction.hpp"

Reaction::Reaction(const std::shared_ptr<Branches12> &data, float beam_energy) {
  _data = data;
  _beam = std::make_unique<TLorentzVector>();
  _beam_energy = beam_energy;

  _beam->SetPxPyPzE(0.0, 0.0, 10.587,
                    10.6); // sqrt(_beam_energy * _beam_energy - MASS_E *
                           // MASS_E), _beam_energy);
  _gamma = std::make_unique<TLorentzVector>();
  _target = std::make_unique<TLorentzVector>(0.0, 0.0, 0.0, MASS_P);
  _elec = std::make_unique<TLorentzVector>();
  _pip = std::make_unique<TLorentzVector>();

  this->SetElec();
}

Reaction::~Reaction() {}

void Reaction::SetElec() {
  _hasE = true;
  _elec->SetXYZM(_data->px(0), _data->py(0), _data->pz(0), MASS_E);

  *_gamma += *_beam - *_elec;
  // Can calculate W and Q2 here
  _W = physics::W_calc(*_beam, *_elec);
  _Q2 = physics::Q2_calc(*_beam, *_elec);
  _beam_theta = _beam->Theta() * RAD2DEG;
  _elec_theta = _elec->Theta() * RAD2DEG;
  _E_elec = _elec->E();
}

void Reaction::SetPip(int i) {
  _numPip++;
  _hasPip = true;
  _pip->SetXYZM(_data->px(i), _data->py(i), _data->pz(i), MASS_PIP);
}

void Reaction::SetOther(int i) {
  _numOther++;
  _hasOther = true;
  if (_data->charge(i) != 0) {
    _other.push_back(std::make_unique<TLorentzVector>());
    _other.back()->SetXYZM(_data->px(i), _data->py(i), _data->pz(i),
                           mass[_data->pid(i)]);
  }
  if (_data->pid(i) == PHOTON) {
    _photons.push_back(std::make_unique<TLorentzVector>());
    _photons.back()->SetXYZM(_data->px(i), _data->py(i), _data->pz(i), 0);
  }
}

void Reaction::CalcMissMass() {
  if (NPip()) {
    auto mm = std::make_unique<TLorentzVector>();
    *mm += (*_gamma + *_target);
    // for (auto& _p : _photons) *mm -= *_p;
    *mm -= *_pip;
    _MM_NPip = mm->M();
    _MM2_NPip = mm->M2();
  }
}

float Reaction::MM_NPip() {
  if (_MM_NPip != _MM_NPip)
    CalcMissMass();
  return _MM_NPip;
}
float Reaction::MM2_NPip() {
  if (_MM2_NPip != _MM2_NPip)
    CalcMissMass();

  return _MM2_NPip;
}

float Reaction::theta_beam() { return _beam_theta; }
float Reaction::theta_elec() { return _elec_theta; }
float Reaction::E_elec() { return _E_elec; }

void Reaction::CalcMassPi0() {
  _pi0_mass = 0;
  if (_photons.size() == 2) {
    auto mass = std::make_unique<TLorentzVector>();
    for (auto &_p : _photons)
      *mass -= *_p;
    _pi0_mass = mass->M();
  }
}

float Reaction::pi0_mass() {
  if (_pi0_mass != _pi0_mass)
    CalcMassPi0();
  return _pi0_mass;
}
