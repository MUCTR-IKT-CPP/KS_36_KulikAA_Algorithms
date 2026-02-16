// var 2 - cocktail sort (shaker sort)
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <thread>

void fillVectorWithFloats(std::vector<float>& vec);
void printVector(std::vector<float>& vec);

/*
* Превращает вектор в строку формата "[el1, el2, el3, ...]"
* 
* @param vec Шаблонный вектор
* 
* @result Строка нужного формата
*/
template <typename T>
std::string vectorToString(std::vector<T>& vec);

/*
* Отсортировать массив шейкерной сортировкой
* 
* @param vec Вектор чисел
* 
* @return std::pair<unsigned long long, unsigned long long> (Num_swaps, Num_passes)
*/
std::pair<unsigned long long, unsigned long long> shakerSort(std::vector<float>& vec);
std::string sortData(std::vector<float>& vec);


std::mt19937 engine(time(0));
std::uniform_real_distribution<float> generator(-1.0f, 1.0f);

int main()
{
    std::vector<unsigned int> M_series = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 };
    //std::vector<unsigned int> M_series = { 1000, 2000, 4000, 8000 };

    std::ofstream file("results.txt");

    for (auto& el : M_series)
    {
        std::vector<float> test_vector(el);
        fillVectorWithFloats(test_vector);
        std::cout << "M: " << el << '\n';
        std::string result = sortData(test_vector);
        file << result << '\n';
        std::cout << result << '\n';
    }

    file.close();
}


void fillVectorWithFloats(std::vector<float>& vec)
{
    for (auto& el : vec)
    {
        el = generator(engine);
    }
}

void printVector(std::vector<float>& vec)
{
    std::cout << std::setprecision(2);
    for (auto& el : vec)
    {
        std::cout << el << ' ';
    }
    std::cout << '\n';
}

std::pair<unsigned long long, unsigned long long> shakerSort(std::vector<float>& vec)
{
    unsigned long long swaps = 0;
    unsigned long long passes = 0;

    bool swapped = false;
    do
    {
        swapped = false;

        for (int i = 0; i < vec.size() - 1; ++i)
        {
            if (vec.at(i) > vec.at(i + 1))
            {
                std::swap(vec.at(i), vec.at(i + 1));
                swapped = true;
                swaps++;
            }
        }

        if (!swapped) { break; }

        swapped = false;

        for (int i = vec.size() - 2; i >= 0; --i)
        {
            if (vec.at(i) > vec.at(i + 1))
            {
                std::swap(vec.at(i), vec.at(i + 1));
                swapped = true;
                swaps++;
            }
        }
        passes++;
    } while (swapped);

    return std::make_pair(swaps, passes);
}

std::string sortData(std::vector<float>& vec)
{
    // Выводим размер массива, количество обменов, количество проходов по массиву, время сортировки
    // string: [M_size, [N_swaps1..N_swaps20], [N_passes1..N_passes20], [T1..T20]]
    const int num_iter = 20;
    std::string result;

    std::vector<unsigned long long> swaps;
    swaps.reserve(num_iter);
    std::vector<unsigned long long> passes;
    passes.reserve(num_iter);

    std::vector<double> times;
    times.reserve(num_iter);

    for (int i = 1; i <= num_iter; ++i)
    {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::pair<unsigned long long, unsigned long long> cur = shakerSort(vec);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration<float, std::milli>(end - start);
        
        swaps.push_back(cur.first);
        passes.push_back(cur.second);
        times.push_back(ms.count());

        //std::cout << ms.count() << '\n';
        fillVectorWithFloats(vec);
    }
    //std::cout << '\n';

    result = '[' + std::to_string(vec.size()) + ", " + vectorToString(swaps) + ", " + vectorToString(passes) + ", " + vectorToString(times) + ']';

    return result;
}


template <typename T>
std::string vectorToString(std::vector<T>& vec)
{
    std::string result;
    result += '[';
    
    for (int i = 0; i < vec.size(); ++i)
    {
        result += std::to_string(vec.at(i));

        if (i != vec.size() - 1) // Если не последняя итерация
        {
            result += ", ";
        }
    }

    result += ']';

    return result;
}