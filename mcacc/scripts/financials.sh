#!/usr/bin/env bash

root=`mcacc --root`
s3=$root/work/s3
SCTMP=$s3/financials.sc


cp $root/inputs/fitmpl.sc $SCTMP


awk '
{
printf "let %s = %12.2f\n", $1, $2* $3;}
' $s3/etb.dsv >> $SCTMP

sc -W % $SCTMP > $s3/financials.rep 2>/dev/null

