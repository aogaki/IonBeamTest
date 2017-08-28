#!/bin/bash

for ((i=1;i<=100;i++))
do
    echo $i
    ./IBT -m tmp.mac
    hadd -f Fe$i.root result_t*
done
