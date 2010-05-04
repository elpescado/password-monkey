#!/usr/bin/env perl

use strict;
use warnings;

use Digest::SHA qw(sha256_hex);
use MIME::Base64;

my $PWMON_ID = "5dfabc489612601f0e253781fbc6f009";

sub ident
{
	my $domain = shift;
	my $password = shift;

	return "pwmon($domain;$password;$PWMON_ID)\n";
}


sub hash
{
	my $data = shift;

	my $hash = sha256_hex ($data);
	my $bin = "";
	for (my $i = 0; $i < length($hash)/2; $i++) {
		my $bytestr = substr ($hash, 2*$i, 2);
		my $val = hex $bytestr;
		$bin .= chr($val);
	}
	return $bin;
}


sub encode
{
	my $bin = shift;

	my $b64 = encode_base64 ($bin, "");
	return $b64;
}


sub _truncate
{
	my $str = shift;
	my $len = shift;

	return substr ($str, 0, $len);
}


sub generate_pass
{
	my $domain = shift;
	my $password = shift;
	my $len = shift;

	my $id = ident ($domain, $password);
	my $hash = hash ($id);
	my $encoded = encode ($hash);
	return _truncate ($encoded, $len);
}


1;
