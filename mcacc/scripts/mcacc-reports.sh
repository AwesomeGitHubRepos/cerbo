#!/usr/bin/env bash
# We do these at the end. It is automatically executed by mcacc

source mcacc-env.sh

#financials.sh
#returns.sh


#archive snap
mkdir -p $arc/snap
DS=`grep DSTAMP $s3/snap.rep | awk '{print $2}' `
cp $s3/snap.rep $arc/snap/$DS.txt


#create net assets
paste $s3/gaap-0.rep $s1/gaap-1.rep >$s3/gaap.rep
mkdir -p $arc/gaap
DS=`grep END $s3/gaap-0.rep | awk '{print $2}' `
cp $s3/gaap-0.rep $arc/gaap/$DS.txt
nass.sh
