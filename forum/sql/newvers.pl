#! /usr/bin/perl
use strict;
use warnings;
use File::Copy;

foreach (@ARGV) {
	if (/(.*)0\.5(.*)/) {
		copy($_, sprintf("$1%s$2", 0.6));
	}
}
	
