DOMAINS="example.com example.org google.com code.google.com elpescado.net przemyslawsitek.pl"
PASSWORDS="1234 pwd passw0rd t0p-s3cret foo sH4l1-1-c0mP4r3-Th33-t0-4-sUmm3r5-dAy"
LENGTHS="10 12 16 20 28"
PWMONKEY=../pwmonkey

for DOMAIN in $DOMAINS
do
	for PASSWORD in $PASSWORDS
	do
		for LENGTH in $LENGTHS
		do
			PWD=`$PWMONKEY -d "$DOMAIN" -p "$PASSWORD" -l $LENGTH`
			echo "{\"$PASSWORD\", \"$DOMAIN\", $LENGTH, \"$PWD\"},"
		done
	done
done
