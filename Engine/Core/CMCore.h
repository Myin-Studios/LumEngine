#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class ScriptCore {
public:
    static vector<T>* gameScripts;
};

template <typename T>
vector<T>* ScriptCore<T>::gameScripts = nullptr;