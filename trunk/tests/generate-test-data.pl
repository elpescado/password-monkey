#!/usr/bin/perl

use strict;
use warnings;
use Digest::SHA qw(sha256_hex);
use MIME::Base64;

# declarations

my $PWMON_ID = "5dfabc489612601f0e253781fbc6f009";
my @DOMAINS  = qw/example.com example.org google.com code.google.com elpescado.net przemyslawsitek.pl/;
my @PASSWORDS= qw/1234 pwd passw0rd t0p-s3cret foo sH4l1-1-c0mP4r3-Th33-t0-4-sUmm3r5-dAy/;
my @LENGTHS  = qw/10 12 16 20 28/;

my $DATA_DIR = "data";

# Generate test data for IDENT function

open F, ">$DATA_DIR/ident.csv" or die $!;
for my $dom (@DOMAINS) {
	for my $pwd (@PASSWORDS) {
		print F "$dom,$pwd,pwmon($dom;$pwd;$PWMON_ID)\n";
	}
}
close F;


# Generate data for HASH function
my @hashes = ();
sub write_hash {
	my $str = shift;
	my $hash = sha256_hex ($str);

	# verifu results using sha256sum
	my $out = `echo -n '$str' | sha256sum`;
	if ($out =~ /^([0-9a-f]+)  -/ && $hash eq $1) {
		print F "$str,$hash\n";
		push @hashes, $hash;
	} else {
		print STDERR "Digest::SHA '$hash' sha256sum '$1'\n";
		die;
	}
}
open F, ">$DATA_DIR/hash.csv" or die$!;
for my $dom (@DOMAINS) {
	write_hash ($dom);
}
for my $pwd (@PASSWORDS) {
	write_hash ($pwd);
}
for my $dom (@DOMAINS) {
	for my $pwd (@PASSWORDS) {
		my $id = "pwmon($dom;$pwd;$PWMON_ID)";
		write_hash ($id);
	}
}
close F;


# Generate data for ENCODE function
open F, ">$DATA_DIR/encode.csv" or die $!;
for my $hash (@hashes) {
	my $bin = "";
	my $xstr = "";
#	print "$hash\n";
	for (my $i = 0; $i < length($hash)/2; $i++) {
		my $bytestr = substr ($hash, 2*$i, 2);
		my $val = hex $bytestr;
#		print " $bytestr($val)";
		$bin .= chr($val);
		$xstr .= "\\x$bytestr";

		if ($bytestr eq "00") {
			# very good, very good
			#print ("$hash: hell yeah\n");
		}
	}
	if (length($hash) != 2*length($bin)) {
		die ("hash != bin\n");
	}

	my $b64 = encode_base64 ($bin, "");
	if (decode_base64 ($b64) ne $bin) {
		die ("$hash: decode failed\n");
	}

	# Verify using base64 utility
	my $b64v = `bash -c 'echo -ne "$xstr" | base64 -w 0'`;
	if ($b64 ne $b64v) {
		die ("MIME:Base64 '$b64' base64 '$b64v'\n");
	}

	if (length ($b64) != 44) {
		die ("Wrong length\n");
	}

	print F "$hash,$b64\n";
}
close F;


# Generate data for TRUNCATE operation
my $str = 'VYCNtMLVb/uciCgXmLTDobNPhv1x21CZUDYTsen/364=';
open F, ">$DATA_DIR/truncate.csv" or die $!;
for (my $i = 44; $i > 0; $i--) {
	my $s = substr ($str, 0, $i);
	print F "$str,$i,$s\n";
}
close F;
