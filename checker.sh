#!/bin/bash
# Script pentru tema 2 la AA.
# Rulare din root.
# Cristian Pavel
#

TEST_DIR=test
INPUT_DIR=$TEST_DIR/in
REF_DIR=$TEST_DIR/ref
INPUT="graph.in"
OUTPUT="bexpr.out"

ls Makefile > /dev/null 2>&1

if ! [ $? -eq 0 ]; then
	echo "Trebuie sa aveti un Makefile."
	exit -1
fi

echo "Compilare"
make build
nr=0

echo ""
echo "Rulare checker"
for i in $(seq 1 10);
do
	rm -rf $INPUT
	rm -rf $OUTPUT
	cp $INPUT_DIR/graph$i.in $INPUT
	make run &> /dev/null
	./main $OUTPUT $REF_DIR/bexpr$i.ref > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		ok="PASSED"
		nr=$(($nr+10))
	else
		ok="FAILED"
	fi
	echo "Test $i...............$ok"
done

rm -rf $INPUT
rm -rf $OUTPUT
echo "Punctaj: $nr"
