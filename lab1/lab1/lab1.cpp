#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <list>

using namespace std;

void FormArray(vector<int>& array, list<thread>& threads, int arrayN, int threadN)
{
    int* check = new int[arrayN];
    
    int threadStart = 0;

    int spanSize = arrayN / threadN;

    int divisionRemainder = arrayN % threadN;

    for (int i = 0; i < arrayN; i++) 
    {
        check[i] = -1;
    }

    for (int i = 0; i < threadN; i++) 
    {
        threads.push_back(thread([&spanSize, &array, &check, &divisionRemainder, &threadN, j = i]() {
            
            for (int i = spanSize * j; i < spanSize * (j + 1); i++)
            {
                array[i] = rand() % 100;
                check[i]++;
            }

            if (divisionRemainder != 0) 
            {
                int it = spanSize * threadN + j;
                if (it < array.size()) 
                {
                    array[it] = rand() % 100;
                    check[it]++;
                }
            }
        }));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    for (int i = 0; i < arrayN; i++)
    {
        if (check[i] == -1)
        {
            cout << "Error: Element not generated" << endl;
        }

        if (check[i] > 0)
        {
            cout << "Error: Element generated more than once" << endl;
        }
    }
}

int main()
{
    int arrayN;
    int threadN;
    
    cout << "Enter array size: ";
    cin >> arrayN;

    cout << "Enter thread amount: ";
    cin >> threadN;

    vector<int> vectorInt(arrayN);
    list<thread> threads;

    FormArray(vectorInt, threads, arrayN, threadN);

    cout << "Array: "; 
    for (auto& num : vectorInt) 
    {
        cout << num << ' ';
    }
    
    return 0;
}