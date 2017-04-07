#ifndef TMACROPARTICLE
#define TMACROPARTICLE 1
/*
    Macroparticle tree class.  
*/

#include <vector>

#include <TVector3.h>


class TMacroParticle: public TObject
{
public:
   TMacroParticle(){};
   ~TMacroParticle(){};

   void SetWeight(Double_t weight) {fWeight = weight;};
   Double_t GetWeight() {return fWeight;};
   
   void SetPosition(TVector3 position) {fPosition = position;};
   void SetPositionx(Double_t x) {fPosition.SetX(x);};
   void SetPositiony(Double_t y) {fPosition.SetY(y);};
   void SetPositionz(Double_t z) {fPosition.SetZ(z);};
   TVector3 GetPosition() {return fPosition;};
   
   void SetP(TVector3 p) {fP = p;};
   void SetPx(Double_t x) {fP.SetX(x);};
   void SetPy(Double_t y) {fP.SetY(y);};
   void SetPz(Double_t z) {fP.SetZ(z);};
   TVector3 GetP() {return fP;};

   //void SetID(Int_t id) {fID = id;};
   //Int_t GetID() {return fID;};
   
   void SetV(TVector3 v) {fV = v;};
   void SetVx(Double_t x) {fV.SetX(x);};
   void SetVy(Double_t y) {fV.SetY(y);};
   void SetVz(Double_t z) {fV.SetZ(z);};
   TVector3 GetV() {return fV;};

private:
   Double_t fWeight;
   TVector3 fPosition;
   TVector3 fP;
   Double_t fKineticEnergy;
   TVector3 fV;
   // After those, not so important? NYI
   /*
   Int_t fID;
   Int_t fCharge;
   Double_t fMass;
   Double_t fRelMass;
   Double_t fGamma;
   Double_t fOpticalDepth;
   Double_t fTridentOpticalDepth;
   Double_t fQEDEnergy;
   */
};

#ifdef __MAKECINT__
#pragma link C++ class TEventData+;
#pragma link C++ class std::vector<TEventData>+;
#endif

#endif
