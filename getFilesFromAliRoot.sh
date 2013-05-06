#!/bin/bash

function copy_files {
  cp $ALICE_ROOT/PWGHF/correlationHF/macros/run-single-task.C .
}

if [ -z $ALICE_ROOT ]
then
    echo "error: $ALICE_ROOT not set, you need to set up your aliroot environment and variables!"
else
    copy_files
    echo Don't forget to provide a filelist.txt
fi
