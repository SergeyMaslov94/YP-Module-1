#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <future>
#include <deque>
#include <string_view>
#include "../Topic_1/log_duration.hpp"

using namespace std;

// ����� ������ ���� ������� ���������� ������ ��������� string_view
std::vector<std::string_view> SplitIntoWordsView_(std::string_view&& str) {
    /*// 1
    ������� ������ �� str �� ������� ������������� �������, ���������������� ������� remove_prefix.
    �� ����� �� string_view ��������� ���������� ��������.*/
    int64_t pos = str.find_first_not_of(" ");

    const int64_t pos_end = str.npos;
    if (pos == pos_end)
        return {};

    str.remove_prefix(pos);
    /*// 2
    ����� ���������� �����, ����� ������ ��������� �� �����. � string � string_view ����� ����
    ��������� npos � ����������� ��������� ������. ������ ��� ������ ������� �����, �������
    ���� �� �����-������ ������ ��������� �������� �������� � ������.

    ���������� ����. ������� �� ����, ���� �� ������� ����� ������������ ������.
    */
    std::vector<std::string_view> result;
    while (!str.empty()) {
        /*// 4
        � ����� ����������� ����� find � ����� ����������, ����� ����� ����� ������� ������� �������.
        */
        int64_t space = str.find(' ');
        /*// 5
        �������� � �������������� ������ ������� string_view, ���������� ������� ������ substr,
        ��� ��������� ������� ����� 0, � �������� � ��������� ������� ������� ��� npos.
        */
        result.push_back(str.substr(0, space));

        /*// 7
        �������� ������ str ���, ����� ��� ��������� �� ������� �� ��������. ��� ����� �������
        ������� remove_prefix, ���������� ������ str �� ��������� � ��������� ���������� �������.
        */
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(space)));
        pos = str.find_first_not_of(" ");
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(pos)));
    }
    return result;
}

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats& other) {
        for (auto& [key, value] : other.word_frequences) {
            word_frequences[key] += value;
        }
    }
};

using KeyWords = set<string, less<>>;

Stats CalcFrequences(const KeyWords& key_words, std::vector<std::string_view>&& data) {
    Stats statistics;
    for (auto& lines : data) {
        for(auto& word : SplitIntoWordsView_(std::move(lines))) {
            auto num = key_words.count(word);
            if (num) {
                statistics.word_frequences[std::string(word.begin(), word.end())] += num;
            }
        }
    }
    return statistics;
}

Stats ExploreKeyWords(const KeyWords& key_words, istream& input) {
    std::vector<std::string_view> data;
    std::deque<string> base;
    std::string c;
    std::vector<std::future<Stats>> futures;
    Stats statistics;
    
    int num_line = 10'000;

    while (!input.eof()) {
        std::getline(input, c);
        base.push_back(c);
        data.push_back(base.back());

        if (!(data.size() % num_line)) {
            futures.push_back(std::move(std::async(CalcFrequences, std::cref(key_words), std::move(data))));
            data.resize(0);
        }
    }

    if (!data.empty()) {
        futures.push_back(std::move(std::async(CalcFrequences, std::cref(key_words), std::move(data))));
    }

    for (std::future<Stats>& future : futures) {
        statistics += future.get();
    }

    return statistics;
}

Stats ExploreKeyWordsSeq(const KeyWords& key_words, istream& input) {
    std::vector<std::string_view> data;
    std::deque<string> base;
    std::string c;
    Stats statistics;

    while (!input.eof()) {
        std::getline(input, c);
        base.push_back(c);
        data.push_back(base.back());
    }

    if (!data.empty()) {
        statistics = CalcFrequences(key_words, std::move(data));
    }

    return statistics;
}


void benchmark() {
    
        stringstream ss;
        {
            auto k = 0;
            while (k < 50000) {
                ss << "this new yangle service really rocks\n";
                k++;
            }
        }

        stringstream ss2;
        {
            auto k = 0;
            while (k < 50000) {
                ss2 << "this new yangle service really rocks\n";
                k++;
            }
        }

        const KeyWords key_words = { "yangle", "rocks", "sucks", "all" };
        
        Stats result_1;
        {
            LOG_DURATION("sequence");
            result_1 = ExploreKeyWordsSeq(key_words, ss);
        }

        Stats result_2;
        {
            LOG_DURATION("parallel");
            result_2 = ExploreKeyWords(key_words, ss2);
        }

        for (const auto& [word, frequency] : result_1.word_frequences) {
            cout << word << " " << frequency << endl;
        }

        for (const auto& [word, frequency] : result_2.word_frequences) {
            cout << word << " " << frequency << endl;
        }
}

int lesson_2() {

    benchmark();

    const KeyWords key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto& [word, frequency] : ExploreKeyWords(key_words, ss).word_frequences) {
        cout << word << " " << frequency << endl;
    }

    return 0;
}