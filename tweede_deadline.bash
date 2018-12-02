#!/bin/bash
# Name: Testcode voor de eerste deadline.
#
# Voer dit bestand uit in de root van je project.
# De map waarin het bestand sources en de map src zit
#
# By Robbert Gurdeep Singh
################################################################################
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT


function showTripples {
    #xxd -g 4 -c 9 -e | sed 's/.*: //;s/     \.\./ /;s/\.\{4\}/=/'
    xxd -g 4 -c 9 \
        | sed 's/ \([0-9a-f]\{2\}\)\([0-9a-f]\{2\}\)\([0-9a-f]\{2\}\)\([0-9a-f]\{2\}\)/ \4\3\2\1/g' \
        | sed 's/^.*: //;s/  \.\{8\}\(.\)$/=\1/'
}

set +x
# Get sources
if [ -f "sources" ];
then
    sources=$(cat sources | tr "\t" " " | grep "^ *lz77")
    sources=$(echo ${sources#*:} | tr " " "\n" | tr "\n" " ")

    # Get Makefile and execute it
    sed -n '/#MAKEFILE\x2DSTART/,/#EXPECTED\x2DSTART/p' $0 \
        | grep -v '^#' \
        | make -B lz77 OUTDIR="$TMPDIR" SOURCES="${sources#*:}" -f - -C src
    makeExit=$?
else
   echo "Het sources bestand kon niet gevonden worden  "
   makeExit=4
fi


if [ $makeExit -ne 0 ]; then
    echo "TEXT:1:Compilatie gefaald"
else
    echo -e "\n\nEncoderen testen"
    echo 'echo -n "https://bit.ly/LZ77-LZ77-LZ77-LZ77" | ./lz77 -c > output'
    echo -n "https://bit.ly/LZ77-LZ77-LZ77-LZ77" | "$TMPDIR/lz77" -c > $TMPDIR/output.txt
    sed -n '/#EXPECTED\x2DSTART/,/#REPEAT\x2DSTART/p' $0 \
        | grep -v '^#' \
        | grep -o '^[^ ]*' \
        | base64 -d > $TMPDIR/expected.txt

    echo -e "\n\n"
    echo "diff -w -a output expected  "
    echo "(trough xxd -g 4 -c 9 -e | sed 's/.*: //;s/     \.\./ /;s/\.\{4\}/=/') "
    echo ""
    echo "   INDEX    LENGTH  CHAR"
    diff -w -s -a \
        <(cat $TMPDIR/output.txt | showTripples) \
        <(cat $TMPDIR/expected.txt | showTripples)
    diffExitc=$?




    echo -e "\n\nDecoderen testen"
    echo "cat data | ./lz77 -d > output"
    cat $TMPDIR/expected.txt | "$TMPDIR/lz77" -d > $TMPDIR/output.txt

    echo -e "\n\n"
    echo "diff -s -a output expected"
    echo "(spaties worden vergeleken)"
    echo ""
    diff -s -a \
        <(cat $TMPDIR/output.txt) \
        <(echo -n "https://bit.ly/LZ77-LZ77-LZ77-LZ77")
    diffExitd=$?





    echo -e "\n\nEncoderen en decoderen met -o testen"
    echo "cat data | ./lz77 -o -c | ./lz77 -o -d > output"
    sed -n '/#REPEAT\x2DSTART/,$p' $0 | grep -v '#' > $TMPDIR/input.txt
    cat $TMPDIR/input.txt | "$TMPDIR/lz77" -o -c |  "$TMPDIR/lz77" -o -d > $TMPDIR/output.txt

    echo -e "\n\n"
    echo "diff -w -a output data"
    echo ""
    diff -s -a \
        $TMPDIR/output.txt \
        $TMPDIR/input.txt
    diffExito=$?

    if [ $diffExitc -ne 0 ] || [ $diffExitd -ne 0 ]|| [ $diffExito -ne 0 ]; then
        echo -n "TEXT:1:Foute output voor "
        [ $diffExitc -ne 0 ] && echo "-c "
        [ $diffExitd -ne 0 ] && echo "-d "
        [ $diffExito -ne 0 ] && echo "-o "
    else
        echo "";
        echo "TEXT:1:Compilatie en minimale test OK"
        echo "OK"
        exit 0
    fi
fi
echo ""
echo "Bronbestanden voor lz77: $sources"
echo ""
echo "Jouw code kan niet ontvangen worden!"
echo "Zorg ervoor dat volgende exitcodes 0 zijn."
echo "   COMPILATIE exitcode    = $makeExit"
echo "MINIMALE TEST exitcode -c = ${diffExitc:-?}"
echo "MINIMALE TEST exitcode -d = ${diffExitd:-?}"
echo "MINIMALE TEST exitcode -o = ${diffExito:-?}"
echo "FOUT"
exit 1
#MAKEFILE-START
flags = -g -std=c11 -Wall -lm

all: lz77

lz77: $(SOURCES)
	gcc $(flags) -o $(OUTDIR)/$@ $^

#EXPECTED-START
# Output voor https://bit.ly/LZ77-LZ77-LZ77-LZ77
AAAAAAAAAABo # (0,0, h)
AAAAAAAAAAB0 # (0,0, t)
AQAAAAEAAABw # (1,1, p)
AAAAAAAAAABz # (0,0, s)
AAAAAAAAAAA6 # (0,0, :)
AAAAAAAAAAAv # (0,0, /)
BgAAAAEAAABi # (6,1, b)
AAAAAAAAAABp # (0,0, i)
AQAAAAEAAAAu # (1,1, .)
AAAAAAAAAABs # (0,0, l)
AAAAAAAAAAB5 # (0,0, y)
BgAAAAEAAABM # (6,1, L)
AAAAAAAAAABa # (0,0, Z)
AAAAAAAAAAA3 # (0,0, 7)
EQAAAAEAAAAt # (17,1, -)
DwAAAA4AAAAA # (15,14,\0)
#REPEAT-START
frans tegen frans in het frans:
  is frans in het frans ook frans?

frans tegen frans in het frans:
  neen, frans in het frans is francais!
