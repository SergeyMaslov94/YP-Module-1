#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) 
            {
                return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
//template <typename Token>
//vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
//    auto start = tokens.begin();
//    vector<Sentence<Token>> split_sentence;
//    while (start < tokens.end()) {
//        auto before_sentence_end = FindSentenceEnd(start, tokens.end());
//        Sentence<Token> sentence(start, before_sentence_end);
//        split_sentence.push_back(move(sentence));
//        start = before_sentence_end;
//    }
//   
//    return split_sentence;
//}

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token>&& tokens) {
    auto start = tokens.begin();
    vector<Sentence<Token>> split_sentence;
    while (start < tokens.end()) {
        auto before_sentence_end = FindSentenceEnd(start, tokens.end());
        Sentence<Token> sentence;
        for (auto it = start; it < before_sentence_end; it++) {
            Token token({ move(*it) });
            sentence.push_back(move(token));
        }
        split_sentence.push_back(move(sentence));
        start = before_sentence_end;
    }

    return split_sentence;
}
//==============================================================================
struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}
// ==============================================================================
// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем
// об этом далее в нашем курсе

struct TestTokenNotCopy {
    string data;
    bool is_end_sentence_punctuation = false;

    TestTokenNotCopy(const TestTokenNotCopy&) = delete;
    TestTokenNotCopy& operator=(const TestTokenNotCopy&) = delete;

    TestTokenNotCopy(TestTokenNotCopy&&) = default;
    TestTokenNotCopy& operator=(TestTokenNotCopy&&) = default;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
};

bool operator==(const TestTokenNotCopy& lhs, const TestTokenNotCopy& rhs) {
    return lhs.data == rhs.data && lhs.is_end_sentence_punctuation == rhs.is_end_sentence_punctuation;
}

ostream& operator<<(ostream& stream, const TestTokenNotCopy& token) {
    return stream << token.data;
}
//============================================================================
// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    {
        assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s} }))
            == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}} }));
    }
    {
        assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true} }))
            == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}} }));
    }
    {
        assert(SplitIntoSentences(vector<TestToken>(
            { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} }))
            == vector<Sentence<TestToken>>({
                {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
                {{"Without"s}, {"copies"s}, {"."s, true}},
                }));
    }
    std::cout << "TestSplitting - OK!" << std::endl;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplittingNotCopy() {
    {
       vector<TestTokenNotCopy> tmp;
       tmp.push_back({ "Split"s });
       tmp.push_back({ "into"s });
       tmp.push_back({ "sentences"s });
       tmp.push_back({ "!"s });

       vector<Sentence<TestTokenNotCopy>> res(1);
       res.back().push_back({ "Split"s });
       res.back().push_back({ "into"s });
       res.back().push_back({ "sentences"s });
       res.back().push_back({ "!"s });

       assert(SplitIntoSentences(move(tmp)) == res);
    }
    {
        vector<TestTokenNotCopy> tmp;
        tmp.push_back({ "Split"s });
        tmp.push_back({ "into"s });
        tmp.push_back({ "sentences"s });
        tmp.push_back({ "!"s, true });

        vector<Sentence<TestTokenNotCopy>> res(1);
        res.back().push_back({ "Split"s });
        res.back().push_back({ "into"s });
        res.back().push_back({ "sentences"s });
        res.back().push_back({ "!"s, true });

        assert(SplitIntoSentences(move(tmp)) == res);
    }
    {
        vector<TestTokenNotCopy> tmp;
        tmp.push_back({ "Split"s });
        tmp.push_back({ "into"s });
        tmp.push_back({ "sentences"s });
        tmp.push_back({ "!"s, true });
        tmp.push_back({ "!"s, true });
        tmp.push_back({ "Without"s});
        tmp.push_back({ "copies"s });
        tmp.push_back({ "."s, true });

        vector<Sentence<TestTokenNotCopy>> res(1);
        res.back().push_back({ "Split"s });
        res.back().push_back({ "into"s });
        res.back().push_back({ "sentences"s });
        res.back().push_back({ "!"s, true });
        res.back().push_back({ "!"s, true });
        res.push_back({});
        res.back().push_back({ "Without"s });
        res.back().push_back({ "copies"s });
        res.back().push_back({ "."s, true });

        assert(SplitIntoSentences(move(tmp)) == res);
    }
    std::cout << "TestSplittingNotCopy - OK!" << std::endl;
}

int lesson_4() {
    TestSplitting();
    TestSplittingNotCopy();
    return 0;
}