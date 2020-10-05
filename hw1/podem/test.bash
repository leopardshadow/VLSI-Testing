#!/bin/bash

if [ "$1" == "1" ]
then
	./atpg -path -start G3 -end PO_G16 ../circuits/iscas85/c17.bench
elif [ "$1" == "2" ]
then
	./atpg -path -start 126GAT_30 -end PO_863GAT_424 ../circuits/iscas85/c880.bench

elif [ "$1" == "3" ]
then
	./atpg -path -start 307GAT_18 -end PO_2548GAT_840 ../circuits/iscas85/c6288.bench
elif [ "$1" == "4" ]
then
	./atpg -path -start 1GAT_0 -end PO_3895GAT_1423 ../circuits/iscas85/c6288.bench
elif [ "$1" == "5" ]
then
	./atpg -path -start 1GAT_0 -end PO_4241GAT_1572 ../circuits/iscas85/c6288.bench
elif [ "$1" == "6" ]
then
	./atpg -path -start 1GAT_0 -end PO_6150GAT_2378 ../circuits/iscas85/c6288.bench
else
	echo ""OAO
fi

