// bringing packets into code
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

std::mutex mtx; //thread synchronization

// function to get max value in a vector
char getMax(const std::vector<char>& data) {
    return *std::max_element(data.begin(), data.end());
}

// counting sort for algo
void countingSort(std::vector<char>& data, int exp) {
    std::vector<char> output(data.size());
    int count[256] = {0};

    //count occurrences
    for (char c : data) {
        count[(c / exp) % 256]++;
    }

    // function to update and store the position of the next occurrence
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    //output array
    for (int i = data.size() - 1; i >= 0; i--) {
        output[count[(data[i] / exp) % 256] - 1] = data[i];
        count[(data[i] / exp) % 256]--;
    }

    // copy sorted data back
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = output[i];
    }
}

// this is the radix sort 
void radixSort(std::vector<char>& data) {
    char maxVal = getMax(data);

    for (int exp = 1; maxVal / exp > 0; exp *= 256) {
        countingSort(data, exp);
    }
}

//parallel radix sort
void parallel_radix_sort(std::vector<char>& data) {
    std::thread t1(radixSort, std::ref(data)); //radix sort in a separate thread
    t1.join(); // waiting thread to finish
}

int main() {
    // input file
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    //vector to store categorized characters
    std::vector<char> numbers, uppercase, lowercase;
    
    // varibale for each character read from input
    char c;

    //read characters from the file and sort them
    while (inputFile.get(c)) {
        if (c >= '0' && c <= '9') numbers.push_back(c);
        else if (c >= 'A' && c <= 'Z') uppercase.push_back(c);
        else if (c >= 'a' && c <= 'z') lowercase.push_back(c);
    }
    inputFile.close();

    // threads for parallel sorting
    std::thread t1(parallel_radix_sort, std::ref(numbers));
    std::thread t2(parallel_radix_sort, std::ref(uppercase));
    std::thread t3(parallel_radix_sort, std::ref(lowercase));

    t1.join();
    t2.join();
    t3.join();

    // output file
    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // writing sorted characters to output
    for (char n : numbers) outputFile << n;
    for (char u : uppercase) outputFile << u;
    for (char l : lowercase) outputFile << l;

    // closing file and telling user all done
    outputFile.close();
    std::cout << "Sorting complete. Check output.txt for results." << std::endl;

    return 0;
}
