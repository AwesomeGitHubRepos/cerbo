#!/usr/bin/env bash

root=`mcacc --root`
w=$root/work
s3=$w/s3


getrc () {
	sed "$1q;d" $s3/portfolios.rep | awk -v col=$2 '{print $col}'
}



mine=$(getrc 8 6)
#echo "mine = $mine"
ftas=$(getrc 14 5)
#echo $mine $ftas

SCOUT=$s3/returns.sc

cp $root/inputs/rettmpl.sc $SCOUT
echo "let minepc = $mine" >> $SCOUT
echo "let asxpc  = $ftas" >> $SCOUT

sc -W % $SCOUT > $s3/returns.rep 2>/dev/null

