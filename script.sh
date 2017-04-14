#!/bin/bash

ionArray=(He C O Fe)
eneArray=(20 40 60 80 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380 400 420 440 460 480 500 520 540 560 580 600 620 640 660 680 700 720 740 760 780 800 820 840 860 880 900 920 940 960 980 1000)

for ion in ${ionArray[@]}
do
    echo "/IBT/Primary/ionName $ion" > tmp.mac
    echo "/run/beamOn 1000000" >> tmp.mac
    ./IBT -m tmp.mac
    hadd -f $ion.root result_t*
done

for ion in ${ionArray[@]}
do
    for ene in ${eneArray[@]}
    do
	echo "/IBT/Primary/ionName $ion" > tmp.mac
	echo "/run/beamOn 1000000" >> tmp.mac
	./IBT -e $ene -m tmp.mac
	hadd -f "$ion""$ene"MeV.root result_t*
    done
done
