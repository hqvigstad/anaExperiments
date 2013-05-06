/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "TChain.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TH3F.h"
#include "TParticle.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TRandom.h"
#include "THashList.h"

#include "AliAnalysisManager.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliStack.h"
#include "AliAnalysisTaskSE.h"
#include "ClusterSelectionTask.h"
#include "AliCaloPhoton.h"
#include "AliPHOSGeometry.h"
#include "TGeoManager.h"
#include "AliPHOSEsdCluster.h"
#include "AliPHOSCalibData.h"
#include "AliESDEvent.h"
#include "AliESDCaloCells.h"
#include "AliESDVertex.h"
#include "AliESDtrackCuts.h"
#include "AliAODEvent.h"
#include "AliLog.h"
#include "AliPID.h"
#include "AliCDBManager.h"
#include <AliAODCaloCluster.h>
#include "AliCentrality.h"
#include "AliESDtrackCuts.h"
#include "AliEventplane.h"
#include "TProfile.h"
#include "AliOADBContainer.h"
#include "AliEPFlattener.h"

// Analysis task to fill histograms with PHOS ESD or AOD clusters and cells
// Authors : Dmitri Peressounko
// Date    : 28.05.2011
// Modified: 03.08.2012 Henrik Qvigstad
/* $Id$ */

ClassImp(ClusterSelectionTask);

//________________________________________________________________________
Double_t rnlin(Double_t *x, Double_t * /*par*/)
{
  //a = par[0], b = par[1].
  //1+a*exp(-e/b)

// return 0.0241+1.0504*x[0]+0.000249*x[0]*x[0] ;
 return 1.015*(0.0241+1.0504*x[0]+0.000249*x[0]*x[0]) ;

}

//________________________________________________________________________
ClusterSelectionTask::ClusterSelectionTask(const char *name)
: AliAnalysisTaskSE(name),
  fHistogramContainer(0x0),
  fClusterContainer(0x0),
  fh1NClusters(0x0),
  fh1CluEnergy(0x0)
{

  // Output slots #1 write into a TH1 container
  DefineOutput(1,TList::Class());
  DefineOutput(2,TList::Class());
}
//___________________________________________________________________________
ClusterSelectionTask::~ClusterSelectionTask()
{
}
//________________________________________________________________________
void ClusterSelectionTask::UserCreateOutputObjects()
{
  // histograms
  fHistogramContainer = new THashList();
  fHistogramContainer->SetOwner(kTRUE);
  // clusters
  fClusterContainer = new THashList();
  fClusterContainer->SetOwner(kTRUE);

  fHistogramContainer->Add(fh1NClusters = new TH1F("h1NClusters","Number of Clusters", 100, 0., 100.)) ;
  fHistogramContainer->Add(fh1CluEnergy = new TH1F("h1CluEnergy","Event selection", 300,0.,30.)) ;
  
  PostData(1, fHistogramContainer);
  PostData(2, fClusterContainer);
}

//________________________________________________________________________
void ClusterSelectionTask::UserExec(Option_t *)
{
  // Main loop, called for each event
  // Analyze ESD/AOD
  
  fClusterContainer->Clear();

  AliVEvent* event = GetEvent();
  int nClusters =0;
  for (Int_t i=0; i<event->GetNumberOfCaloClusters(); i++) {
    AliVCluster *clu = event->GetCaloCluster(i);

    if ( !clu->IsPHOS() || clu->E()< kMinClusterEnergy) 
      continue; // reject cluster
    else
      nClusters++;
    
    fh1CluEnergy->Fill(clu->E());
    
    TLorentzVector lorentzMomentum;
    Double_t origo[3] = {0,0,0};
    clu->GetMomentum(lorentzMomentum, origo);
    
    //fCaloPhotonsPHOS->Add(new  AliCaloPhoton(lorentzMomentum.X(),lorentzMomentum.Py(),lorentzMomentum.Z(),lorentzMomentum.E()) );
    AliCaloPhoton * ph = new  AliCaloPhoton(lorentzMomentum.X(),lorentzMomentum.Py(),lorentzMomentum.Z(),lorentzMomentum.E());
    fClusterContainer->Add(ph);
  }
  fh1NClusters->Fill(nClusters);

  // Post output data.
  PostData(1, fHistogramContainer);
  PostData(2, fClusterContainer);
}

AliVEvent* ClusterSelectionTask::GetEvent()
{
  AliVEvent* event = InputEvent();
  if( ! event ) {
    AliError("Event could not be retrieved");
    PostData(1, fHistogramContainer);
  }
  return event;
}

//________________________________________________________________________
// void Task::Terminate(Option_t *)
// {
//   // Draw result to the screen
//   // Called once at the end of the query
//   // new TCanvas;
//   // TH1 * hTotSelEvents = dynamic_cast<TH1*>(fOutputContainer->FindObject("hTotSelEvents"));
//   // hTotSelEvents->Draw();
// }
//________________________________________________________________________
