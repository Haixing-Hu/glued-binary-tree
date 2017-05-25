#!/bin/bash

if (( $# < 1 )); then
  echo "ERROR: must specify the maximum levels.";
  exit -1;
fi

n=$1;

for (( i = 1; i <= n; ++i )); do
  echo "Random walk on the glued binary tree of level $i ...";
  time ./random_walk_hidden_graph.out $i;
  echo "=================================";
  echo "";
done