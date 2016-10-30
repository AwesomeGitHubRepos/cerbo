#!/usr/bin/env bash

source mcacc-env.sh


(cd $root/arc/gaap && grep "Net Assets" *.txt) | sed -e 's/.txt:/   /' | sort -r >$s3/nass-2.rep


