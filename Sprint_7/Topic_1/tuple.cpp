#include <iostream>
#include <tuple>

using namespace std;

auto GetTuple(int x, int y) {
    return tie(x, y);
}

int main() {
    auto [x, y] = GetTuple(10, 15);
    cout << x << ' ' << y << endl;
    return 0;
} 