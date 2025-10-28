#include <iostream>
#include <Windows.h>

using namespace std;

int* arr = nullptr;
int array_size = 0;
CRITICAL_SECTION cs;

DWORD WINAPI marker_thread(LPVOID param);

struct ThreadData {
    int thread_id;
    int array_size;
    HANDLE start_event;
    HANDLE stop_event;
    HANDLE terminate_event;
    HANDLE continue_event;
};
int main() {
    cout << "Enter array size: ";
    cin >> array_size;
    arr = new int[array_size]();
    for (int i = 0; i < array_size; i++) {
        cout << arr[i] << " ";
    }
    cout << std::endl;

    int num_threads;
    cout << "Enter the number of marker threads: ";
    cin >> num_threads;

    if (num_threads <= 0 || num_threads > array_size) {
        cout << "Error: The number of threads must be between 1 and" << array_size << endl;
        delete[] arr;
        return 1;
    }
   
    return 0;
}
DWORD WINAPI marker_thread(LPVOID param) {
   
    return 0;
}

