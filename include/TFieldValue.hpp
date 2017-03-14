#ifndef TFIELDVALUE
#define TFIELDVALUE 1

/*
    Data class for field values.
    Don't fix the histogram name and Generating each name by sdf file is better.
    It is impossible in C++.  Setting variable name his** is one solution.  
    But, I don't like such a way.  
*/

#include <vector>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

class TFieldValue
{
public:
   TFieldValue(){};
   ~TFieldValue(){};
   
   TH1 *Ex{nullptr}; 
   TH1 *Ey{nullptr}; 
   TH1 *Ez{nullptr}; 
   
   TH1 *Bx{nullptr}; 
   TH1 *By{nullptr}; 
   TH1 *Bz{nullptr}; 

   TH1 *Jx{nullptr}; 
   TH1 *Jy{nullptr}; 
   TH1 *Jz{nullptr}; 
};

#ifdef __MAKECINT__
#pragma link C++ class TFieldValue+;
#pragma link C++ class std::vector<TFieldValue>+;
#endif

#endif

