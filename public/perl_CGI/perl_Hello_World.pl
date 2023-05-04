#!/usr/bin/perl
use strict; 
use CGI ':standard'; 
print header; 
print start_html('Hello World'); 
print h1('Hello World'); 
print end_html(); 
exit; 