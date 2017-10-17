/// \file
/// \ingroup tutorial_io
/// \notebook -nodraw
/// Example of script showing how to copy all objects (including directories)
/// from a source file.
/// For each input file, a new directory is created in the current directory
/// with the name of the source file.
/// After the execution of:
/// ~~~{.bash}
/// root [0] .x copyFiles.C
/// ~~~
/// the file result.root will contain 4 subdirectories:
/// "tot100.root", "hsimple.root", "hs1.root","hs2.root"
///
/// \macro_code
///
/// \author Rene Brun

#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

void CopyDir(TDirectory *source) {
   //copy all objects and subdirs of directory source as a subdir of the current directory
   source->ls();
   TDirectory *savdir = gDirectory;
   TString dirName = source->GetName();
   dirName.ReplaceAll(".root", "");
   TDirectory *adir = savdir->mkdir(dirName);
   adir->cd();
   //loop on all entries of this directory
   TKey *key;
   //Loop in reverse order to make sure that the order of cycles is
   //preserved.
   TIter nextkey(source->GetListOfKeys(),kIterBackward);
   while ((key = (TKey*)nextkey())) {
      const char *classname = key->GetClassName();
      TClass *cl = gROOT->GetClass(classname);
      if (!cl) continue;
      if (cl->InheritsFrom(TDirectory::Class())) {
         source->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         adir->cd();
         CopyDir(subdir);
         adir->cd();
      } else if (cl->InheritsFrom(TTree::Class())) {
         TTree *T = (TTree*)source->Get(key->GetName());
         // Avoid writing the data of a TTree more than once.
         // Note this assume that older cycles are (as expected) older
         // snapshots of the TTree meta data.
         if (!adir->FindObject(key->GetName())) {
            adir->cd();
            TTree *newT = T->CloneTree(-1,"fast");
            newT->Write();
         }
      } else {
         source->cd();
         TObject *obj = key->ReadObj();
         adir->cd();
         //obj->Write(); // The name of Time (TVectorT<double>) is not copied from TKey to TObject
         obj->Write(key->GetName());
         delete obj;
     }
  }
  adir->SaveSelf(kTRUE);
  savdir->cd();
}

void CopyFile(const char *fname) {
   //Copy all objects and subdirs of file fname as a subdir of the current directory
   TDirectory *target = gDirectory;
   TFile *f = TFile::Open(fname);
   if (!f || f->IsZombie()) {
      printf("Cannot copy file: %s\n",fname);
      target->cd();
      return;
   }
   target->cd();
   CopyDir(f);
   delete f;
   target->cd();
}

void copyFiles() {
   TString outName = "result.root";
   TFile *f = new TFile(outName, "recreate");
   ifstream fin("fileList");

   TSystemDirectory dir("./", "./");
   TList *files = dir.GetListOfFiles();
   if (files) {
      files->Sort();
      TSystemFile *file;
      TString fileName;
      TIter next(files);
      while ((file = (TSystemFile*)next())) {
         fileName = file->GetName();
         if (!file->IsDirectory() && fileName != outName && fileName.EndsWith(".root")) {
            cout << fileName << endl;
            CopyFile(fileName);
         }
      }
   }
      
   delete f;
}
