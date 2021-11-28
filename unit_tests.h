//
// Created by Rufus on 28/11/21.
//

#ifndef WORDS_TO_NUMBER_UNIT_TESTS_H
#define WORDS_TO_NUMBER_UNIT_TESTS_H

#include "words2text.h"

bool pass_utilities_tests() {
    bool result = true;

    if (getIndex(ones, "six") + 1 == 6)
        std::cout << "OK ones[\"six\"] = " << getIndex(ones, "six") + 1 << std::endl;
    else {
        std::cout << "ERROR tens[\"six\"] = " << getIndex(ones, "six") + 1 << std::endl;
        result = false;
    }

    if (getIndex(ones, "one") + 1 == 1)
        std::cout << "OK ones[\"one\"] = " << getIndex(ones, "one") + 1 << std::endl;
    else {
        std::cout << "ERROR tens[\"one\"] = " << getIndex(ones, "one") + 1 << std::endl;
        result = false;
    }
    if (getIndex(teens, "twelve") + 10 + 1 == 12)
        std::cout << "OK teens[\"twelve\"] = " << getIndex(teens, "twelve") + 10 + 1 << std::endl;
    else {
        std::cout << "ERROR teens[\"twelve\"] = " << getIndex(teens, "twelve") + 10 + 1 << std::endl;
        result = false;
    }
    if (getIndex(teens, "nineteen") + 10 + 1 == 19)
        std::cout << "OK teens[\"nineteen\"] = " << getIndex(teens, "nineteen") + 10 + 1 << std::endl;
    else {
        std::cout << "ERROR teens[\"nineteen\"] = " << getIndex(teens, "nineteen") + 10 + 1 << std::endl;
        result = false;
    }

    std::string test_string = "one two three";

    auto words = lexer(test_string, splitchars);

    if (words[0] == "one" && words[1] == "two" && words[2] == "three") {
        std::cout << "OK lexer with " << test_string << std::endl;
    } else {
        std::cout << "ERROR in lexer " << test_string << std::endl;
        result = false;
    }

    test_string = "         one     two three       ";
    words = lexer(test_string, splitchars);

    if (words[0] == "one" && words[1] == "two" && words[2] == "three") {
        std::cout << "OK lexer with " << test_string << std::endl;
    } else {
        std::cout << "ERROR in lexer " << test_string << std::endl;
        result = false;
    }

    return result;
}

bool unit_test_converter(std::string candidate, int64_t expected_value) {
    Words2Text w2t;

    int64_t value;
    value = w2t.convert(candidate);

    if (value != expected_value) {
        std::cout << "Error in " << candidate << " != " << value << std::endl;
        return false;
    } else {
        std::cout << "OK " << candidate << " = " << value << std::endl;
        return true;
    }
}

bool pass_converter_tests() {
    bool result = true;

    // Small numbers
    result = result && unit_test_converter("zero", 0);
    result = result && unit_test_converter("none", 0);
    result = result && unit_test_converter("one", 1);
    result = result && unit_test_converter("twelve", 12);
    result = result && unit_test_converter("twenty-seven", 27);
    result = result && unit_test_converter("seventy six", 76);

    // Negative numbers

    result = result && unit_test_converter("minus seven", -7);
    result = result && unit_test_converter("minus fifty six", -56);
    result = result && unit_test_converter("negative six hundred", -600);


    // Under 1000 numbers
    result = result && unit_test_converter("three hundred and one", 301);
    result = result && unit_test_converter("nine hundred and twenty six", 926);

    // Check lowercase mixing
    result = result && unit_test_converter("Six thousand Eight hundred and Fifty seven", 6857);

    // Mid size numbers
    result = result && unit_test_converter("thirty six thousand four hundred and eighty two", 36482);
    result = result && unit_test_converter("six hundred ninety thousand seven hundred and twelve ", 690712);
    result = result && unit_test_converter("one hundred and six thousand two hundred and eleven", 106211);

    // Big ones

    result = result && unit_test_converter("one hundred million and ten", 100000010);
    result = result && unit_test_converter("seventy six million two thousand nine hundred and five", 76002905);
    result = result && unit_test_converter(
            "nine thousand three hundred and fifteen million six hundred thousand four hundred and sixty nine",
            9315600469);
    result = result && unit_test_converter("one billion and one", 1000000001);

    return result;
}

bool unit_test_replacer(const std::string &candidate, const std::string &expected_value) {
    Words2Text w2t;
    std::string value;
    value = w2t.replace(candidate);

    if (value != expected_value) {
        std::cout << "Error in [" << expected_value << "] != [" << value << "]" << std::endl;
        return false;
    } else {
        std::cout << "OK " << candidate << " = " << value << std::endl;
        return true;
    }
}

bool pass_replacer_tests() {
    bool result = true;

    // No numbers on the sentence
    result = result && unit_test_replacer("There aren't any numbers in this sentence",
                                          "There aren't any numbers in this sentence");

    // At the begin
    result = result && unit_test_replacer("Ninety-nine years old", "99 years old");

    // In the middle of text
    result = result && unit_test_replacer("There are six thousand five hundred and twelve apples on the tree",
                                          "There are 6512 apples on the tree");

    // At the end of text
    result = result && unit_test_replacer("The total amount of the bill is five hundred and twenty",
                                          "The total amount of the bill is 520");

    // The test example
    result = result && unit_test_replacer("I have one hundred apples", "I have 100 apples");

    // Some natural language cases
    result = result && unit_test_replacer("I have zero money", "I have 0 money");
    result = result && unit_test_replacer("It is minus twelve degrees in winter", "It is -12 degrees in winter");
    result = result && unit_test_replacer("The value is negative five in the scale", "The value is -5 in the scale");

    // Many numbers in the sentence

    result = result && unit_test_replacer("The number two is bigger than one but smaller than three",
                                          "The number 2 is bigger than 1 but smaller than 3");


    return result;
}

void pass_tests() {
    if (pass_utilities_tests()) {
        std::cout << std::endl;
        std::cout << "Utilities OK" << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "Utilities FAILED" << std::endl;
    }

    std::cout << std::endl;

    if (pass_converter_tests()) {
        std::cout << std::endl;
        std::cout << "Converter OK" << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "Converter FAILED" << std::endl;
    }

    if (pass_replacer_tests()) {
        std::cout << std::endl;
        std::cout << "Replacer OK" << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "Replacer FAILED" << std::endl;
    }
}

#endif //WORDS_TO_NUMBER_UNIT_TESTS_H
