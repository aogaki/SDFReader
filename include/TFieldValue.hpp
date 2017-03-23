#ifndef TFIELDVALUE
#define TFIELDVALUE 1

/*
    Data class for field values.
*/

#include <vector>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

class TFieldValue: public TObject
{
public:
   TFieldValue();
   ~TFieldValue();

   ClassDef(TFieldValue, 1);
   
   void SetEx(TH1 *his) {fEx = his;};
   TH1 *GetEx() {return fEx;};
   void SetEy(TH1 *his) {fEy = his;};
   TH1 *GetEy() {return fEy;};
   void SetEz(TH1 *his) {fEz = his;};
   TH1 *GetEz() {return fEz;};
   
   void SetBx(TH1 *his) {fBx = his;};
   TH1 *GetBx() {return fBx;};
   void SetBy(TH1 *his) {fBy = his;};
   TH1 *GetBy() {return fBy;};
   void SetBz(TH1 *his) {fBz = his;};
   TH1 *GetBz() {return fBz;};
   
   void SetJx(TH1 *his) {fJx = his;};
   TH1 *GetJx() {return fJx;};
   void SetJy(TH1 *his) {fJy = his;};
   TH1 *GetJy() {return fJy;};
   void SetJz(TH1 *his) {fJz = his;};
   TH1 *GetJz() {return fJz;};
   
private:
   // TH1D, TH2D, or TH3D
   TH1 *fEx;
   TH1 *fEy;
   TH1 *fEz;
   
   TH1 *fBx;
   TH1 *fBy;
   TH1 *fBz;

   TH1 *fJx;
   TH1 *fJy;
   TH1 *fJz;

};

#ifdef __MAKECINT__
#pragma link C++ class TFieldValue+;
#pragma link C++ class std::vector<TFieldValue>+;
#endif

#endif
