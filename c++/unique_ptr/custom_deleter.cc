#include <iostream>
#include <memory>
#include <functional>
using namespace std;

void func1() {
    struct _data {
        int a;
    };
    auto teardown = [](_data *d) {
        cout << "Delete 1" << endl;
        delete d;
    };
    unique_ptr<_data, decltype(teardown)> data(new _data(), teardown);
    data->a = 1;
}

void func2() {
    int x = 1;
    int y = 2;
    auto teardown = [&](int *_x) {
        cout << "Delete 2: x: " << *_x << ", y: " << y << endl;
    };
    unique_ptr<int, decltype(teardown)> data(&x, teardown);
}

void func3() {
    auto teardown = [](void *) {
        cout << "Delete 3" << endl;
    };
    unique_ptr<void, decltype(teardown)> data((void *)1, teardown);
}

void func4() {
    auto teardown = [](void *) {
        cout << "Delete 4 (Probably not called)" << endl;
    };
    unique_ptr<void, decltype(teardown)> data(nullptr, teardown);
}

int main(int argc, char *argv[]) {
    cout << "point 1" << endl;
    func1();
    cout << "point 2" << endl;
    func2();
    cout << "point 3" << endl;
    func3();
    cout << "point 4" << endl;
    func4();
    cout << "point 5" << endl;
    return EXIT_SUCCESS;
}
