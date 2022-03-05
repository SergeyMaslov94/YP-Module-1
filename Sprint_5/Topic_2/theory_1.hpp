#include <fstream>
#include <string>
#include "../Topic_1/log_duration.h"

using namespace std;

int theory_1() {
    {
        LOG_DURATION("#1");
        ofstream out_file("ballad.txt"s);
        for (int i = 0; i < 50000; ++i) {
            out_file << "С любимыми не расставайтесь"s << endl;
        }
    }

    {
        LOG_DURATION("#2");
        ofstream out_file("ballad.txt"s);
        for (int i = 0; i < 50000; ++i) {
            out_file << "С любимыми не расставайтесь/n"s << std::flush;
        }
    }

    {
        LOG_DURATION("#3");
        ofstream out_file("ballad.txt"s);
        for (int i = 0; i < 50000; ++i) {
            out_file << "С любимыми не расставайтесь/n"s;
        }
    }

    throw;
}