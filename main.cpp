#include <bits/stdc++.h>

using namespace std;

struct LogData {
    string name;
    int lineNum = 0, infoNum = 0, warningNum = 0, errorNum = 0, criticalNum = 0;
    double totalTimeDiff = 0;
    vector<pair<string, int>> topWords;
};

vector<pair<string, int>> topWords(map<string, int>& wordCount){
    // Function to calculate top words
    priority_queue<pair<int, string>> orderedWords;

    for (auto const& [word, qty] : wordCount) {
        orderedWords.push({qty, word}); 
    }

    vector<pair<string, int>> finalList;

    // Extract top 5
    for(int i = 0; !orderedWords.empty() && i < 5; i++){
        pair<int, string> item = orderedWords.top();
        orderedWords.pop();

        // Store as {Word, Quantity}
        finalList.push_back({item.second, item.first});
    }

    return finalList;
}

void generateReport(ostream& out, LogData logDisplay){
    // Function that can write to CONSOLE (cout) or FILE (ofstream)
    out << "=== Analysis Report ===\n";
    out << "Archive: " << logDisplay.name << "\n";
    out << "Logs total: " << logDisplay.lineNum << "\n";
    out << "Mean time between errors: ";
    if(logDisplay.errorNum >= 2) {
        // If there is at least one interval between two errors, exhibit their mean time
        double meanSeconds = logDisplay.totalTimeDiff / (logDisplay.errorNum - 1); // total time difference / number of intervals
        out << (int)meanSeconds << " seconds (approx. " << (int)(meanSeconds/60) << " minutes)\n\n";
    }
    else {
        out << "Not enough errors to calculate the mean time.\n\n";
    }

    out << "Count by level:\n";
    // Calculate percentages and print result
    int infoPerc = (int)(100.0 * logDisplay.infoNum / logDisplay.lineNum);
    int warningPerc = (int)(100.0 * logDisplay.warningNum / logDisplay.lineNum);
    int errorPerc = (int)(100.0 * logDisplay.errorNum / logDisplay.lineNum);
    int criticalPerc = (int)(100.0 * logDisplay.criticalNum / logDisplay.lineNum);
    out << "- INFO: " << logDisplay.infoNum << " logs (" << infoPerc << "%)\n";
    out << "- WARNING: " << logDisplay.warningNum << " logs (" << warningPerc << "%)\n";
    out << "- ERROR: " << logDisplay.errorNum << " logs (" << errorPerc << "%)\n";
    out << "- CRITICAL: " << logDisplay.criticalNum << " logs (" << criticalPerc << "%)\n\n";

    out << "Most frequent words:\n";
    for(int i = 0; i < 5; i++){
        out << i+1 << ": " << logDisplay.topWords[i].first << " (" << logDisplay.topWords[i].second << " times)\n";
    }

    
}

time_t toSeconds(string dateStr, string timeStr) {
    struct tm tm = {0}; // Initialize standard C time structure
    
    int year, month, day, hour, min, sec;
    
    // Parse format: [YYYY-MM-DD HH:MM:SS]
    sscanf(dateStr.c_str(), "[%d-%d-%d", &year, &month, &day);
    sscanf(timeStr.c_str(), "%d:%d:%d]", &hour, &min, &sec);

    tm.tm_year = year - 1900; // time struct counts years since 1900
    tm.tm_mon = month - 1;    // months 0-11
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = sec;

    return mktime(&tm); // Returns seconds (time_t)
}

int main(int argc, char* argv[]){
    // Error handling
    if(argc < 2){
        cerr << "Error. Usage: " << argv[0] << " <path_to_log>\n";
        return 1;
    }

    string fileName = argv[1];
    if(fileName.length() <= 4 || fileName.substr(fileName.length() - 4) != ".txt"){
        cerr << "Error. Please provide a .txt log file.\n";
        return 1;
    }

    ifstream logFile(fileName);
    if(!logFile.is_open()){
        cerr << "Error. Could not open the file.\n";
        return 1;
    }

    // Standard file variables initialization
    LogData thisLogData;
    thisLogData.name = fileName;

    map<string, int> wordCount;
    set<string> ignoredWords = {"de", "do", "da", "e", "a", "o", "ao", "com", "em", "na", "no", "ja", "nao", "que", "os", "as"};

    time_t lastErrorTime = 0;
    double totalTimeDiff = 0;

    string line;
    while(getline(logFile, line)){
        if(line[0] != '[') continue;
        // If line valid, scan the line
        thisLogData.lineNum++;

        stringstream ss(line);
        string word, dateStr, timeStr;

        // Extract timestamp
        ss >> dateStr;
        ss >> timeStr;
        
        // Level Counter
        ss >> word;
        if(word == "INFO") thisLogData.infoNum++;
        else if(word == "WARNING") thisLogData.warningNum++;
        else if(word == "CRITICAL") thisLogData.criticalNum++;
        else if(word == "ERROR") {
            thisLogData.errorNum++;

            // Convert current string to time in seconds
            time_t currentTime = toSeconds(dateStr, timeStr);

            if(lastErrorTime != 0){
                // If not the first error, calculate the gap
                double diff = difftime(currentTime, lastErrorTime);
                totalTimeDiff += diff;
            }

            // Update last error to the current one
            lastErrorTime = currentTime;
        }

        // Word Counter after the Level
        while(ss >> word) {
            // For each character in the string, make it lower case
            string lowerWord = "";
            for(char c : word) lowerWord += tolower(c); 

            if(ignoredWords.find(lowerWord) == ignoredWords.end()) {
                // If not ignored, count it
                wordCount[lowerWord]++;
            }
        }
        
    }

    // Finalize data
    thisLogData.totalTimeDiff = totalTimeDiff;
    thisLogData.topWords = topWords(wordCount);
    
    // Output
    generateReport(cout, thisLogData);

    cout << "\n------------------------------------------------" << endl;
    cout << "Do you want to export this report to 'AnalysisReport.txt'? (y/n): ";
    char response;
    cin >> response;
    if(response == 'y' || response == 'Y') {
        ofstream outputFile("AnalysisReport.txt");
        
        if(outputFile.is_open()) {
            generateReport(outputFile, thisLogData);
            
            cout << "Success! Report saved to AnalysisReport.txt\n";
            outputFile.close();
        } else {
            cerr << "Error creating output file.\n";
        }
    }

    logFile.close();
    return 0;
}