////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_CULTUREINFO_H__
#define __CORE_CULTUREINFO_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

enum class DigitShapes
{
    // The digit shape is not changed. Full Unicode compatibility is maintained
    None,

    // The digit shape is the native equivalent of the digits from 0 through 9. ASCII digits
    // from 0 through 9 are replaced by equivalent native national digits
    NativeNational,

    // The digit shape depends on the previous text in the same output. European digits follow
    // Latin scripts; Arabic-Indic digits follow Arabic text; and Thai digits follow Thai text
    Context
};

// Provides culture-specific information for formatting and parsing numeric values.
struct NumberFormatInfo
{
    // String that denotes that the associated number is positive
    const char* positiveSign = "+";

    // String that denotes that the associated number is negative
    const char* negativeSign = "-";

    // Format pattern for negative numeric values
    // (n) -n - n n- n -
    int numberNegativePattern = 1;

    // Number of decimal places to use in numeric values
    int numberDecimalDigits = 2;

    // String to use as the decimal separator in numeric values
    const char* numberDecimalSeparator = ".";

    // String that separates groups of digits to the left of the decimal in numeric values
    const char* numberGroupSeparator = ",";

    // Number of decimal places to use in currency values
    int currencyDecimalDigits = 2;

    // Format pattern for positive currency values
    // '$n' 'n$' '$ n' 'n $'
    int currencyPositivePattern = 0;
    
    // Format pattern for negative currency values
    // '($n)' '-$n' '$-n' '$n-' '(n$)' '-n$' 'n-$' 'n$-' '-n $' '-$ n' 'n $-' '$ n-' '$ -n' 'n- $' '($ n)' '(n $)'
    int currencyNegativePattern = 0;
    
    // String to use as the currency symbol
    const char* currencySymbol = "$";

    // String to use as the decimal separator in currency values
    const char* currencyDecimalSeparator = ".";

    // String that separates groups of digits to the left of the decimal in currency values
    const char* currencyGroupSeparator = ",";

    // Number of decimal places to use in percent values
    int percentDecimalDigits = 2;

    // Format pattern for positive percent values
    // 'n %' 'n%' '%n' '% n'
    int percentPositivePattern = 0;

    // Format pattern for negative percent values
    // '-n %' '-n%' '-%n' '%-n' '%n-' 'n-%' 'n%-' '-% n' 'n %-' '% n-' '% -n' 'n- %'
    int percentNegativePattern = 0;

    // String to use as the percent symbol
    const char* percentSymbol = "%";

    // String to use as the decimal separator in percent values
    const char* percentDecimalSeparator = ".";

    // String that separates groups of digits to the left of the decimal in percent values
    const char* percentGroupSeparator = ",";

    // String that represents negative infinity
    const char* negativeInfinitySymbol = "-\xE2\x88\x9E";

    // String that represents positive infinity
    const char* positiveInfinitySymbol = "\xE2\x88\x9E";

    // String that represents the IEEE NaN (not a number) value
    const char* nanSymbol = "NaN";

    // Specifies the culture-specific digit shape
    DigitShapes digitSubstitution = DigitShapes::Context;
};


// Provides information about a specific culture
struct CultureInfo
{
    // The culture name in BCP-47 language tag format
    const char* name = "en-US";

    // Defines the culturally appropriate format of displaying numbers, currency, and percentage
    NumberFormatInfo numberFormat;
};

}

#endif
