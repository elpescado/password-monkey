#!/usr/bin/env perl

use PasswordMonkey;

my $hash = generate_pass ("dupa.com", "aaa", 44);
print "$hash\n";
