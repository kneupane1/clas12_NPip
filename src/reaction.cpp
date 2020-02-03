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
  //_prot = std::make_unique<TLorentzVector>();

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
void Reaction::SetPositive(int i) {
  _pos_beta.push_back(_data->beta(i));
  _numPos++;
  _hasPos = true;
  _pos_det.push_back(abs(_data->status(i) / 1000));
  _pos.push_back(std::make_unique<TLorentzVector>());
  _pos.back()->SetXYZM(_data->px(i), _data->py(i), _data->pz(i), MASS_P);
}
void Reaction::SetPip(int i) {
  _numPip++;
  _numPos++;
  _hasPip = true;
  _pip->SetXYZM(_data->px(i), _data->py(i), _data->pz(i), MASS_PIP);
}

bool Reaction::PosStats() {
  if (_pos.size() != 2)
    return false;
  if (abs(_pos.front()->Phi() - _pos.back()->Phi()) > 0.5)
    return false;
  if (abs(_pos.front()->Theta() - _pos.back()->Theta()) > 0.5)
    return false;
  if (_pos_det.front() == _pos_det.back())
    return false;
  /*
     std::cout << "num_pos: " << _pos.size() << std::endl;
     std::cout << "num_phi: ";
     for (auto& _d : _pos_det) std::cout << detector_name[_d] << "\t";
     std::cout << std::endl;
     std::cout << "pos_phi: ";
     for (auto& _p : _pos) std::cout << _p->Phi() << "\t";
     std::cout << std::endl;
     std::cout << "pos_theta: ";
     for (auto& _p : _pos) std::cout << _p->Theta() << "\t";
     std::cout << std::endl;
   */

  return true;
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
  if (_pos.size() > 0) {
    auto mm = std::make_unique<TLorentzVector>();
    *mm += (*_gamma + *_target);
    // if (elastic()) {
    for (auto &_p : _pos)
      *mm -= *_p;
    _MM = mm->M();
    _MM2 = mm->M2();
  }
}

void Reaction::CalcMissMass_NPip() {

  if (NPip()) {
    auto mm_MPip = std::make_unique<TLorentzVector>();
    *mm_MPip += (*_gamma + *_target);
    // for (auto& _p : _photons) *mm -= *_p;
    *mm_MPip -= *_pip;
    _MM_NPip = mm_MPip->M();
    _MM2_NPip = mm_MPip->M2();
  }
}

float Reaction::MM() {
  if (_MM != _MM)
    CalcMissMass();
  return _MM;
}
float Reaction::MM2() {
  if (_MM2 != _MM2)
    CalcMissMass();
  return _MM2;
}
float Reaction::MM_NPip() {
  if (_MM_NPip != _MM_NPip)
    CalcMissMass_NPip();
  return _MM_NPip;
}
float Reaction::MM2_NPip() {
  if (_MM2_NPip != _MM2_NPip)
    CalcMissMass_NPip();

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
