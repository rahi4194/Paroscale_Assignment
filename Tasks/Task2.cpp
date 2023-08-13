//Note by Altamash
// This will only run on C++ new version
// To run this code write command "g++ -std=c++17 Task2.cpp" on terminal

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <filesystem>
using namespace std;

namespace fs = filesystem;

mutex mtx;
atomic<int> totalFiles(0);
atomic<int> totalDirs(0);

void countFilesAndDirs(const fs::path &dirPath)
{
    int localFiles = 0;
    int localDirs = 0;

    for (const auto &entry : fs::directory_iterator(dirPath))
    {
        if (entry.is_regular_file())
        {
            localFiles++;
        }
        else if (entry.is_directory())
        {
            localDirs++;
            countFilesAndDirs(entry.path()); // Recurse into subdirectories
        }
    }

    {
        lock_guard<mutex> lock(mtx);
        totalFiles += localFiles;
        totalDirs += localDirs;
        cout << "Directory: " << dirPath << " | Files: " << localFiles << " | Directories: " << localDirs << endl;
    }
}

int main()
{
    fs::path rootPath = "D:\\jdk_18"; // Replace with the actual directory path

    thread workerThread(countFilesAndDirs, rootPath);
    workerThread.join();

    cout << "Total Files: " << totalFiles << " | Total Directories: " << totalDirs << endl;

    return 0;
}
