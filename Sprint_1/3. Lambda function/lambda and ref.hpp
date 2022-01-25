#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
    const map<string, int>& max_amount, map<string, int>& shelter) {

    return count_if(new_dogs.begin(), new_dogs.end(), [&max_amount, &shelter](const string& str)
        {
            int& current_amount = shelter[str];

            if (current_amount < max_amount.at(str))
            {
                current_amount++;
                return true;
            }
            return false;
        });
}

int run() {
    
    map<string, int> shelter = { {"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3} };
    const map<string, int> max_amount = { {"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7} };
    const vector<string> new_dogs = { "landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s };

    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter);

    return 0;
}