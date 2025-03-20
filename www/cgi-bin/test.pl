#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = CGI->new;
print "Content-Type: text/html\r\n\r\n";
print "<html><body><h1>CGI en Perl funcionando!</h1>";
print "<p>QUERY_STRING: " . $cgi->query_string() . "</p>";
print "</body></html>";
