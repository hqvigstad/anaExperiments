void AddClusterSelectionTask(TString name = "ClusterSelectionTask")
{
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskPHOSPi0Flow", "No analysis manager to connect to");
    return NULL;
  }
  
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskPHOSPi0Flow", "This task requires an input event handler");
    return NULL;
  }

  gROOT->LoadMacro("ClusterSelectionTask.cxx+g");
  ClusterSelectionTask * task = new ClusterSelectionTask();
  
  mgr->AddTask(task);
  mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer() );
  
  TString cname(Form("%sCoutput1", name));
  TString pname(Form("%s:%s", AliAnalysisManager::GetCommonFileName(), name));
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(cname.Data(), TList::Class(), AliAnalysisManager::kOutputContainer, pname.Data());
  mgr->ConnectOutput(task, 1, coutput1);
  
  return task;
}
