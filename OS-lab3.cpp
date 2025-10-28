#include <iostream>
#include <Windows.h>
#include <vector>

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

    InitializeCriticalSection(&cs);

    HANDLE start_event = CreateEvent(
        NULL,               // Атрибуты безопасности по умолчанию
        TRUE,               // Ручной сброс (manual-reset)
        FALSE,              // Изначально несигнальное состояние
        NULL                // Имя не нужно
    );

    vector<HANDLE> stop_events(num_threads);
    vector<HANDLE> terminate_events(num_threads);
    vector<HANDLE> continue_events(num_threads);

    for (int i = 0; i < num_threads; i++) {
        stop_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);      
        terminate_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL); 
        continue_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);  
    }

    vector<HANDLE> threads(num_threads);
    vector<ThreadData> thread_data(num_threads);

    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = {
            i + 1,
            array_size,
            start_event,
            stop_events[i],
            terminate_events[i],
            continue_events[i]
        };

        threads[i] = CreateThread(
            NULL,
            0,
            marker_thread,
            &thread_data[i],
            0,
            NULL
        );

        if (threads[i] == NULL) {
            cout << "Error creating thread " << i + 1 << endl;
        }
    }

    cout << "Launching all streams..." << endl;
    SetEvent(start_event);

   return 0;
}
DWORD WINAPI marker_thread(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    int marks_count = 0;
    int last_index = -1;

    WaitForSingleObject(data->start_event, INFINITE);

    srand(data->thread_id);

    while (true) {
        int index = rand() % data->array_size;
        last_index = index;

        EnterCriticalSection(&cs);

        if (arr[index] == 0) {
            Sleep(5);
            arr[index] = data->thread_id;
            marks_count++;
            Sleep(5);
            LeaveCriticalSection(&cs);
        }
        else {
            LeaveCriticalSection(&cs);
            break;
        }
    }

    cout << "Thread " << data->thread_id << " stopped. "
        << "Number of marks:: " << marks_count << ", "
        << "Lock index: " << last_index << endl;

    SetEvent(data->stop_event);

    return 0;
}

