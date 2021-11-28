//
// Created by Rufus on 28/11/21.
//

#ifndef WORDS_TO_NUMBER_WORDS2TEXT_H
#define WORDS_TO_NUMBER_WORDS2TEXT_H

#include <vector>
#include <map>
#include <tuple>
#include <cstdint>
#include <algorithm>
#include <string>

// The main class where the stuff is done

class Words2Text {
private:
    // Converts a simple string with ONLY numbers in English into an int64_t
    int64_t word_to_int(std::string &candidate);

public:
    bool verbose = false;

    // Does the translation text->number by splitting in sub-units separated by the words million and billion
    // I finally decided to make it public because it can be useful to convert standalone text numbers instead of a full sentence

    int64_t convert(std::string &candidate);

    // Converts a full sentence in English into the same sentence with the numbers replaced by its decimal representation

    std::string replace(std::string candidate);
};


// Order of words in the array is important !!
std::vector<std::string> ones = {
        "one", "two", "three", "four", "five", "six",
        "seven", "eight", "nine"
};
std::vector<std::string> teens = {
        "eleven", "twelve", "thirteen", "fourteen", "fifteen",
        "sixteen", "seventeen", "eighteen", "nineteen"
};

std::vector<std::string> tens = {
        "ten", "twenty", "thirty", "forty", "fifty", "sixty",
        "seventy", "eighty", "ninety"
};

std::map<std::string, int64_t> bigscales = {
        {"hundred",  100},
        {"hundreds", 100},
        {"thousand", 1000},
        {"million",  1000000},
        {"billion",  1000000000},
};

std::vector<std::string> minusWords = {"minus", "negative"};
std::vector<char> splitchars = {' ', '-', ','};


// Utility functions, independent from main algorithm


// Gets the index of a value in a vector (shortens code)

inline int64_t getIndex(std::vector<std::string> v, const std::string &K) {
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) {
        int64_t index = it - v.begin();
        return index;
    } else {
        return -1;
    }
}

// Checks if a char is present in an array of chars

inline int64_t containsChar(std::vector<char> v, char K) {
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) {
        int64_t index = it - v.begin();
        return index;
    } else {
        return -1;
    }
}

// Checks if a word is related to the numbers mini-grammar (naive approximation, needs more work to exclude no-number cases)

bool is_number_word(std::string word) {
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
    if (word == "and" || word == "zero" || word == "none" || word == "minus" || word == "negative" ||
        getIndex(ones, word) != -1 || getIndex(teens, word) != -1 || getIndex(tens, word) != -1 ||
        bigscales.find(word) != bigscales.end())
        return true;
    else
        return false;
}


// Extremely simple lexer to split sentences into words

std::vector<std::string> lexer(const std::string &str, const std::vector<char> &chars) {
    std::string next;
    std::vector<std::string> result;

    for (auto it: str) {
        // Hit the terminal character
        if (containsChar(chars, it) != -1) {
            // If characters accumulated
            if (!next.empty()) {
                // Add them to result
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate character
            next += it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}


// Converts a string with ONLY numbers in English into an int64_t

int64_t Words2Text::word_to_int(std::string &candidate) {

    // Convert all words to lowercase, avoids camel-case, capitalization etc...
    std::transform(candidate.begin(), candidate.end(), candidate.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    auto input_words = lexer(candidate, splitchars);

    int64_t result = 0;
    int64_t current_result = 0;
    int64_t big_multiplier_value = 1;
    bool big_multiplier_is_active = false;
    bool minus_flag = false;

    if (verbose)
        std::cout << "Processing number : " + candidate << std::endl;

    for (auto &current_word: input_words) {

        if (verbose)
        {
            std::cout << "Processing word " << current_word << std::endl;
        }

        if (bigscales.find(current_word) != bigscales.end()) {
            big_multiplier_value = bigscales[current_word];
            big_multiplier_is_active = true;
            if (verbose)
                std::cout << "    Is a multiplier word " << std::endl;
        }

        if (big_multiplier_is_active) {  // If a multiplier is active, is applied to the accumulated temporal value
            if (big_multiplier_value > 100) {
                result = (result + current_result) * big_multiplier_value;
                if (verbose)
                    std::cout << "    Multiplying accumulator by " << big_multiplier_value << std::endl;
            }
            else {
                result = result + (current_result * big_multiplier_value);
                if (verbose)
                    std::cout << "    Multiplying accumulator by " << big_multiplier_value << std::endl;
            }

            current_result = 0;
            big_multiplier_value = 1;
            big_multiplier_is_active = false;
        } else {

            int64_t n;
            if ((n = getIndex(ones, current_word) + 1) > 0) {
                current_result += n;
                if (verbose)
                    std::cout << "    Adding " << n << " to accumulator " << std::endl;

            } else if ((n = getIndex(teens, current_word) + 1) > 0) {
                current_result += n + 10;
                if (verbose)
                    std::cout << "    Adding " << n+10 << " to accumulator " << std::endl;
            } else if ((n = getIndex(tens, current_word) + 1) > 0) {
                current_result += n * 10; //n * 10;
                if (verbose)
                    std::cout << "    Adding " << n*10 << " to accumulator " << std::endl;

            }
            // Negative words
            else if (getIndex(minusWords, current_word) != -1) {
                minus_flag = true;
                if (verbose)
                    std::cout << "    Is a negative number" << std::endl;

            } else if (current_word == "zero" || current_word == "none") {
                if (verbose)
                    std::cout << "    Is a zero number" << std::endl;
                continue;

            } else if (current_word != "and") {
                if (verbose)
                    std::cout << "Something unexpected happens " + current_word << std::endl;
                std::cout << "Expected a number: " + current_word << std::endl;
                return -1;
            }
        }
    }

    if (verbose)
        std::cout << "    Final result is " << result << " + " << current_result << " * " << big_multiplier_value << std::endl;
    auto final = result + current_result * big_multiplier_value;

    if (minus_flag)
        final *= -1;
    return final;
}

// Does the translation text->number by splitting in sub-units separated by the words million and billion

int64_t Words2Text::convert(std::string &candidate) {
    auto words = lexer(candidate, splitchars);
    std::string part;
    int64_t total = 0;

    if (verbose)
        std::cout << "Converting number : " + candidate << std::endl;

    for (auto &word: words) {
        part += word + " ";
        if (word == "million" || word == "billion") {  // Separator words
            if (verbose)
                std::cout << "Converting sub-number : " + part << std::endl;

            total += word_to_int(part);
            part = "";
        }
    }
    if (part != "")
        total += word_to_int(part);
    return total;
}

// Converts a free sentence in English into the same sentence with the numbers replaced by its decimal representation

std::string Words2Text::replace(std::string candidate) {
    auto words = lexer(candidate, splitchars);
    std::string number;
    std::string sentence;
    std::string word;

    if (verbose)
        std::cout << "Replacing sentence : " + candidate << std::endl;

    for (int n = 0; n < words.size(); n++) {
        word = words[n];
        number = "";

        // Accumulates numbers in substring, and converts it, replacing its text by the decimal representation
        while (is_number_word(word)) {
            number += word + " ";
            word = words[++n];
        }

        if (number != "") {
            if (verbose)
            {
                std::cout << "Found number in sentence  : " + number << std::endl;
            }
            auto result = convert(number);
            number = std::to_string(result);
            sentence += number + " ";
            --n;
        } else
            sentence += word + " ";

    }
    // Remove last blankspace (residual blankspace left by the accumlation)
    sentence.erase(sentence.size() - 1);
    return sentence;
}


#endif //WORDS_TO_NUMBER_WORDS2TEXT_H
