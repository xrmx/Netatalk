#!/usr/bin/perl

# usage: make-precompose.h.pl UnicodeData.txt > precompose.h

# See
# http://www.unicode.org/Public/UNIDATA/UCD.html
# http://www.unicode.org/reports/tr15/
# http://www.unicode.org/Public/*/ucd/UnicodeData*.txt
# http://www.unicode.org/Public/UNIDATA/UnicodeData.txt


# table for binary search --------------------------------------------------

open(UNICODEDATA, "<$ARGV[0]");
open(PRECOMPOSETEMP, ">precompose.TEMP");
open( DECOMPOSETEMP, ">decompose.TEMP");

while (<UNICODEDATA>){
    chop;
    (
     $code0,
     $Name1,
     $General_Category2,
     $Canonical_Combining_Class3,
     $Bidi_Class4,
     $Decomposition_Mapping5,
     $Numeric_Value6,
     $Numeric_Value7,
     $Numeric_Value8,
     $Bidi_Mirrored9,
     $Unicode_1_Name10,
     $ISO_Comment11,
     $Simple_Uppercase_Mapping12,
     $Simple_Lowercase_Mapping13,
     $Simple_Titlecase_Mapping14
     ) = split(/\;/);

    if (($Decomposition_Mapping5 ne "") && ($Decomposition_Mapping5 !~ /\</) && ($Decomposition_Mapping5 =~ / /)) {
	($base, $comb) = split(/ /,$Decomposition_Mapping5);
	
	$leftbracket  = "  { ";
	$rightbracket =" },     ";

	if (hex($code0) > 0xFFFF) {           # DELETE THIS LINE  IF INTERNAL CODE IS UCS4
	    $leftbracket  = "\/\*{ ";         # DELETE THIS LINE  IF INTERNAL CODE IS UCS4
	    $rightbracket =" },\*\/   ";      # DELETE THIS LINE  IF INTERNAL CODE IS UCS4
	}                                     # DELETE THIS LINE  IF INTERNAL CODE IS UCS4
	
	printf(PRECOMPOSETEMP "%s0x%08X, 0x%08X, 0x%08X%s\/\* %s \*\/\n", $leftbracket, hex($code0), hex($base), hex($comb), $rightbracket, $Name1);
	
	# AFP 3.x Spec
	if ( ((0x2000  <= hex($code0)) && (hex($code0) <=  0x2FFF))
	  || ((0xFE30  <= hex($code0)) && (hex($code0) <=  0xFE4F))
          || ((0x2F800 <= hex($code0)) && (hex($code0) <= 0x2FA1F))) {
	    $leftbracket  = "\/\*{ ";
	    $rightbracket =" },\*\/   ";
	}
	
	printf( DECOMPOSETEMP "%s0x%08X, 0x%08X, 0x%08X%s\/\* %s \*\/\n", $leftbracket, hex($code0), hex($base), hex($comb), $rightbracket, $Name1);
	
    }
}

# sort ---------------------------------------------------------------------

system("sort -k 3 precompose.TEMP \> precompose.SORT");
system("sort -k 2  decompose.TEMP \>  decompose.SORT");

# print  -------------------------------------------------------------------

printf ("\/\* This file is generated from contrib/misc/make-precompose.h.pl %s \*\/\n", $ARGV[0]);
print ("\/\* DO NOT EDIT BY HAND\!\!\!                                           \*\/\n");
print ("\n");
printf ("\/\* %s is got from                                      \*\/\n", $ARGV[0]);
print ("\/\* http\:\/\/www.unicode.org\/Public\/UNIDATA\/UnicodeData.txt            \*\/\n");
print ("\n");

print ("static const struct \{\n");
print ("  unsigned int replacement\;\n");
print ("  unsigned int base\;\n");
print ("  unsigned int comb\;\n");
print ("\} precompositions\[\] \= \{\n");

system("cat precompose.SORT");

print ("\}\;\n");
print ("\n");

print ("static const struct \{\n");
print ("  unsigned int replacement\;\n");
print ("  unsigned int base\;\n");
print ("  unsigned int comb\;\n");
print ("\} decompositions\[\] \= \{\n");

system("cat decompose.SORT");

print ("\}\;\n");
print ("\n");

print ("\/\* EOF \*\/\n");

# EOF