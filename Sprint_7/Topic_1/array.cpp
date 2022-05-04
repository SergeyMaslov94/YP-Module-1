class IntArray {
public:
    IntArray(int n)
        : ptr(new int[n]) {
    }
    ~IntArray() {
        delete[] ptr;
    }

private:
    int* ptr;
};

int main() {
    IntArray a1(5);
    IntArray a2(a1);
} 