#ifndef Task_cxx
#define Task_cxx

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Analysis task to fill histograms with PHOS ESD or AOD clusters and cells
// Authors : Dmitri Peressounko
// Date    : 28.05.2011
// Modified: 03.08.2012 Henrik Qvigstad
/* $Id$ */

class TObjArray;
class TH1F;
class TH2I;
class TH2F;
class TH3F;
class TF1;
class AliStack;
class AliESDtrackCuts;
class AliPHOSGeometry;
class AliESDEvent;
class AliPHOSCalibData;
class AliESDtrack;
class AliESDCaloCluster;
class AliEPFlattener;


#include "AliAnalysisTaskSE.h"

class ClusterSelectionTask:public AliAnalysisTaskSE {
public:
  ClusterSelectionTask(const char *name = "Task");
   virtual ~ ClusterSelectionTask();

  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t * option);


protected:
   ClusterSelectionTask(const ClusterSelectionTask &);	// not implemented
   ClusterSelectionTask & operator=(const ClusterSelectionTask &);	// not implemented

  AliVEvent *GetEvent();
  AliESDEvent *GetESDEvent(Bool_t silent = false);
  AliAODEvent *GetAODEvent(Bool_t silent = false);

protected:
  static const Double_t kMinClusterEnergy = 0.3;
  static const Int_t kMinNCells = 3;

  TList *fHistogramContainer;	//final histogram container
  TList *fClusterContainer;
  

  TH1 *fh1NClusters;
  TH1 *fh1CluEnergy;

   ClassDef(ClusterSelectionTask, 2);	// PHOS analysis task
};

#endif
