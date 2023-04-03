/*
* 4. Частотный анализатор текстов. k “читающих” потоков считывают 
     данные из файлов (формат файлов произвольный). 
     Поток-интерфейс отвечает за взаимодействие с пользователем 
     (командная строка или иной формат). Пользователю доступны следующие команды:
     - вывести на экран 5 самых распространённых на данный момент букв; 
     - вывести на экран вероятность появление буквы, введённой пользователем; 
     - выдать три самые редкие буквы.
*/

#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include <utility>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

vector<pair<char, int>> sortedVector;

bool cmp(pair<char, int>& a, pair<char, int>& b)
{
	return a.second > b.second;
}

void sort(map<char, int>& M, function<void(vector<pair<char, int>>& vector)> callback)
{
	if (sortedVector.size() > 0) 
	{
		callback(sortedVector);
		return;
	}

	for (auto& it : M) 
	{
		sortedVector.push_back(it);
	}

	sort(sortedVector.begin(), sortedVector.end(), cmp);

	callback(sortedVector);
}

int main()
{
	map<char, int> mainMap; 
	string texts[2] = { "test1.txt", "test2.txt" };
	thread threadArr[2];
	mutex mapMutex;

	for (int i = 0; i < 2; i++) 
	{
		threadArr[i] = thread([text = texts[i], &mapMutex, &mainMap]() {
			map<char, int> threadMap; 
			ifstream file(text);
			char curr;
			while (file.get(curr)) {
				threadMap[curr]++;
			}
			unique_lock<mutex> lock(mapMutex); 
			for (const auto& pair : threadMap) { 
				mainMap[pair.first] += pair.second; 
			}
		});
	}

	for (int i = 0; i < 2; i++) 
	{
		threadArr[i].join();
	}

	int total = 0;
	for (auto pair : mainMap) {
		total += pair.second;
	}

	cout << "press 'h' - print this list of commands" << endl;
	bool flag = true;
	while (flag) 
	{
		char command;	
		cout << "> ";
		cin >> command;

		switch (command)
		{
		case 'h':
			cout << "c - find probability of character" << endl;
			cout << "f - find 5 most frequent" << endl;
			cout << "r - find 3 most rare" << endl;
			cout << "x - exit program" << endl;
			break;
		case 'x':
			flag = false;
			break;
		case 'c':
			cout << "enter character: ";
			char toFind;
			cin >> toFind;
			cout << endl << "probability: " << toFind << " = " << double(mainMap[toFind]) / total << endl;
			break;
		case 'f':
			sort(mainMap, [](vector<pair<char, int>>& vector) {
				for (int i = 0; i < 5; i++) {
					cout << vector[i].first << " - " << vector[i].second << endl;
				}
				});
			break;
		case 'r':
			sort(mainMap, [](vector<pair<char, int>>& vector) {
				//cout << vector.size() << ' ' << vector.size() - 3 << endl;
				for (int i = vector.size() - 3; i < vector.size(); i++) {
					cout << vector[i].first << " - " << vector[i].second << endl;
				}
				});
			break;
		default:
			break;
		}
	}
}