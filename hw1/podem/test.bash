#!/bin/bash

if [ "$1" == "c17" ]
then
	./atpg -path -start G3 -end PO_G16 ../circuits/iscas85/c17.bench
elif [ "$1" == "c880" ]
then
	./atpg -path -start 126GAT_30 -end PO_863GAT_424 ../circuits/iscas85/c880.bench

elif [ "$1" == "c6288" ]
then
	./atpg -path -start 307GAT_18 -end PO_2548GAT_840 ../circuits/iscas85/c6288.bench
else
	echo ""OAO
fi

