#include <bits/stdc++.h>


using namespace std;

struct ProcessInfo {
    int pid;
    string user;
};

void runTopCommandAndSaveOutput(const string& outputFilePath) {
    string command = "top -n 1 -b > " + outputFilePath;
    int status = system(command.c_str());

    if (status == 0) {
        cout << "Output of top command saved to " << outputFilePath << endl;
    } else {
        cerr << "Failed to run top command or save output." << endl;
    }
}

vector<ProcessInfo> parseTopOutput(const string& filePath) {
    vector<ProcessInfo> pidUserList;
    ifstream inputFile(filePath);

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            if (regex_search(line, regex("^\\s*\\d+"))) {
                istringstream iss(line);
                ProcessInfo processInfo;
                iss >> processInfo.pid >> processInfo.user;
                pidUserList.push_back(processInfo);
            }
        }
        inputFile.close();
    }

    return pidUserList;
}

int main() {
    string outputFilePath = "Task3 _top_cmd_output.txt";

    runTopCommandAndSaveOutput(outputFilePath);

    vector<ProcessInfo> pidsAndUsers = parseTopOutput(outputFilePath);

    cout << "PID\tUSER\n";
    for (const ProcessInfo& info : pidsAndUsers) {
        cout << info.pid << "\t" << info.user << "\n";
    }

    return 0;
}
