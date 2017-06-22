#!/bin/bash

events="1000000"

name=$1
Z=$2
A=$3
charge=$4

echo "/gun/particle ion" > tmp.mac
echo "/gun/ion $Z $A $charge" >> tmp.mac
echo "/run/beamOn $events" >> tmp.mac

for baseEne in 1 10 20 30 40 50
do
    ene=$((baseEne * A))
    ./IBT -e $ene -m tmp.mac -mono

    fileName="$name$baseEne"GeV_Z"$Z"_A"$A.root"
    hadd -f $fileName result_t*
done

