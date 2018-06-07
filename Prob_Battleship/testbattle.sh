# Simon Zeng szeng7@jhu.edu, Bryan Ki, nki1@jhu.edu
for IN in *.in
do
	OUT="`basename $IN .in`.out"
#	echo "1Checking $IN against $OUT"
	./battle <$IN >result
#	echo "2Checking $IN against $OUT"
	if ! diff $OUT result
	then
		echo "^^^ test $OUT failed"
	fi
done
for IN in *.fin	
do
	OUT="`basename $IN .fin`.fout"
	./battle -f <$IN >result
	if ! diff $OUT result
	then
		echo "^^^ test $OUT failed"
	fi

done
rm -f result
