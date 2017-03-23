#ifndef TMACROPARTICLE
#define TMACROPARTICLE 1
/*
    Macroparticle data class.  
*/

#include <vector>

#include <TVector3.h>


class TMacroParticle
{
public:
   TMacroParticle();
   ~TMacroParticle();

   void SetID(Int_t id) {fID = id;};
   Int_t GetID() {return fID;};
   
   void SetWeight(Double_t weight) {fWeight = weight;};
   Double_t GetWeight() {return fWeight;};
   
   void SetP(TVector3 p) {fP = p;};
   TVector3 GetP() {return fP;};
   
   void SetV(TVector3 v) {fV = v;};
   TVector3 GetV() {return fV;};
   
private:
   Int_t fID;
   Double_t fWeight;
   TVector3 fP;
   TVector3 fV;
   // After those, not so important? NYI
   Int_t fCharge;
   Double_t fMass;
   Double_t fRelMass;
   Double_t fKineticEnergy;
   Double_t fGamma;
   Double_t fOpticalDepth;
   Double_t fTridentOpticalDepth;
   Double_t fQEDEnergy;
};

#ifdef __MAKECINT__
#pragma link C++ class TEventData+;
#pragma link C++ class std::vector<TEventData>+;
#endif

#endif
