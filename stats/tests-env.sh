#!/usr/bin/env bash

LOG=tests-env.sh.log
TRS=tests-env.sh.trs

./testcxxstats > $LOG
cp $LOG $TRS
