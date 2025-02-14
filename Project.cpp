// bringing packets into code
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

// thread syncchronization
std::mutex mtx;

// this is to perform the brick sort
void brick_sort(std::vector<char>& data) {
    bool sorted = false;
    int n = data.size();
    
    while (!sorted) {
        sorted = true;

        // for odd elements
        for (int i = 1; i < n - 1; i += 2) {
            if (data[i] > data[i + 1]) {
                std::swap(data[i], data[i + 1]);
                sorted = false;
            }
        }

        // for even elements
        for (int i = 0; i < n - 1; i += 2) {
            if (data[i] > data[i + 1]) {
                std::swap(data[i], data[i + 1]);
                sorted = false;
            }
        }
    }
}

// this executs the brick sort in a seperate thread
void parallel_brick_sort(std::vector<char>& data) {
    std::thread t1(brick_sort, std::ref(data));
    t1.join();
}

int main() {
    // opening the input file
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    // reading characters and sorting themm accordingly 
    std::vector<char> numbers, uppercase, lowercase;
    char c;
    while (inputFile.get(c)) {
        if (c >= '0' && c <= '9') numbers.push_back(c);
        else if (c >= 'A' && c <= 'Z') uppercase.push_back(c);
        else if (c >= 'a' && c <= 'z') lowercase.push_back(c);
    }
    inputFile.close();

    // creating threads to sort each category 
    std::thread t1(parallel_brick_sort, std::ref(numbers));
    std::thread t2(parallel_brick_sort, std::ref(uppercase));
    std::thread t3(parallel_brick_sort, std::ref(lowercase));

    // ensuring all threads are complete
    t1.join();
    t2.join();
    t3.join();

    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // opening output file and writing sorted data
    for (char n : numbers) outputFile << n;
    for (char u : uppercase) outputFile << u;
    for (char l : lowercase) outputFile << l;

    // closing and telling user its done
    outputFile.close();
    std::cout << "Sorting complete. Check output.txt for results." << std::endl;

    return 0;
}
