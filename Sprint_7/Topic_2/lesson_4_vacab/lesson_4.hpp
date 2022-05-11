#include <string_view>
#include <string>
#include <deque>
#include <utility>
#include <map>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        base_.push_back(make_pair(string(source.begin(), source.end()), string(target.begin(), target.end())));
        translate_forward_[base_.back().first] = base_.back().second;
        translate_backward_[base_.back().second] = base_.back().first;
    };

    string_view TranslateForward(string_view source) const {
        if (translate_forward_.count(source)) {
            return translate_forward_.at(source);
        }
        return ""s;
    };

    string_view TranslateBackward(string_view target) const {
        if (translate_backward_.count(target)) {
            return translate_backward_.at(target);
        }
        return ""s;
    };

    Translator() = default;

private:
    std::deque<pair<string, string>> base_;
    std::map<std::string_view, std::string_view> translate_forward_;
    std::map<std::string_view, std::string_view> translate_backward_;
};

void TestSimple() {
    Translator translator;
    
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));
    
    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);

    translator.Add(string("okno"s), string("jopa"s));
}

int lesson_4() {

    std::map<string, int> tmp;
    tmp["a"] = 1;
    tmp["b"] = 2;
    tmp["c"] = 3;

    TestSimple();
    return 0;
}