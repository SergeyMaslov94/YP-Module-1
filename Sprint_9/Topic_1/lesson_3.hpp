#include <array>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VehiclePlate {
public:
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{ l0, l1, l2 }
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        // чтобы дополнить цифровую часть номера слева нулями
        // до трёх цифр, используем подобные манипуляторы:
        // setfill задаёт символ для заполнения,
        // right задаёт выравнивание по правому краю,
        // setw задаёт минимальное желаемое количество знаков
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

//======================================================

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

inline bool operator==(const VehiclePlate& lhs, const VehiclePlate& rhs) 
{
    if (lhs.Hash() == rhs.Hash())
    {
        auto str1 = lhs.ToString();
        auto str2 = rhs.ToString();
        return str1 == str2;
    }
    else
    {
        return false;
    }
}
//======================================================

template <typename T>
class HashableContainer {
public:
    void Insert(T elem) {
        int index = elem.Hash();

        // если вектор недостаточно велик для этого индекса,
        // то увеличим его, выделив место с запасом
        if (index >= int(elements_.size())) {
            elements_.resize(index * 2 + 1);
        }

        auto& element = elements_.at(index);
        auto it = std::find(element.begin(), element.end(), elem);
        
        if (it == element.end())
        {
            element.push_back(move(elem));
        }
    }

    void PrintAll(ostream& out) const {
        for (auto& element : elements_) {
            if (element.empty()) {
                continue;
            }
            for (auto& value : element)
            {
                out << value << endl;
            }
        }
    }

    const auto& GetVector() const {
        return elements_;
    }

private:
    vector<std::vector<T>> elements_;
};

int lesson_3() {
    HashableContainer<VehiclePlate> plate_base;

    plate_base.Insert({ 'B','H', 840, 'E', 99 });
    plate_base.Insert({ 'O','K', 942, 'K', 78 });
    plate_base.Insert({ 'O','K', 942, 'K', 78 });
    plate_base.Insert({ 'O','K', 942, 'K', 78 });
    plate_base.Insert({ 'O','K', 942, 'K', 78 });
    plate_base.Insert({ 'H','E', 968, 'C', 79 });
    plate_base.Insert({ 'T','A', 326, 'X', 83 });
    plate_base.Insert({ 'H','H', 831, 'P', 116 });
    plate_base.Insert({ 'A','P', 831, 'Y', 99 });
    plate_base.Insert({ 'P','M', 884, 'K', 23 });
    plate_base.Insert({ 'O','C', 34, 'P', 24 });
    plate_base.Insert({ 'M','Y', 831, 'M', 43 });
    plate_base.Insert({ 'B','P', 831, 'M', 79 });
    plate_base.Insert({ 'K','T', 478, 'P', 49 });
    plate_base.Insert({ 'X','P', 850, 'A', 50 });

    plate_base.PrintAll(cout);

    return 0;
}