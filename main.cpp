#include <iostream>


#include "words2text.h"
#include "unit_tests.h"

int main(int argc, char* argv[]) {
    bool pass_test = false;
    bool verbose=false;
    std::string sentence;

    if(argc==1) {
        std::cout << "Usage: " << std::endl;
        std::cout << "     words_to_number [-verbose] [-test] sentence " << std::endl;
        std::cout << "     Please pay attemption to pass your sentence double quoted" << std::endl;
        std::cout << "     if it includes non alfabetical characters" << std::endl;
        exit(0);
    }

    std::string argument;

    for (int n=1;n<argc;n++)
    {
        argument=argv[n];

        if (argument=="-verbose")
            verbose=true;
        else if (argument=="-test")
            pass_test=true;
        else
            sentence+=argument+" ";
    }

    if (pass_test) {
        pass_tests();
    }

    if (sentence!="") {

        Words2Text w2t;
        w2t.verbose = verbose;
        auto result = w2t.replace(sentence);
        std::cout << std::endl << "RESULT: " << std::endl << result << std::endl;
    }

    return 0;
}
