#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

class MoneyBox {
public:
    explicit MoneyBox(vector<int64_t> nominals)
        : nominals_(move(nominals))
        , counts_(nominals_.size()) {
    }

    const vector<int>& GetCounts() const {
        return counts_;
    }

    void PushCoin(int64_t value) {
        // реализуйте метод добавления купюры или монеты
        auto it = std::find(nominals_.begin(), nominals_.end(), value);
        auto i = std::distance(nominals_.begin(), it);
        ++counts_.at(i);
    }

    void PrintCoins(ostream& out) const {
        // реализуйте метод печати доступных средств
        for (auto i = 0; i < nominals_.size(); i++)
        {
            if (counts_.at(i))
            {
                out << nominals_.at(i) << ": " << counts_.at(i) << std::endl;
            }
        }
    }

private:
    const vector<int64_t> nominals_;
    vector<int> counts_;
};

ostream& operator<<(ostream& out, const MoneyBox& cash) {
    cash.PrintCoins(out);
    return out;
}

int lesson_2() {
    MoneyBox cash({ 1, 500, 10000 });
    cash.PushCoin(500);
    cash.PushCoin(500);
    cash.PushCoin(10000);
    assert((cash.GetCounts() == vector<int>{0, 2, 1}));
    cout << cash << endl;

    return 0;
}