📊 C++ Log Analyzer

A robust Command Line Interface (CLI) tool written in C++ to parse, analyze, and generate statistical reports from server log files.

🚀 Features

This tool processes raw .txt log files and performs the following analyses:

Level Distribution: Calculates the percentage and count of logs by type (INFO, WARNING, ERROR, CRITICAL).

Temporal Analysis (MTBE): Calculates the Mean Time Between Errors by parsing timestamps to measure system stability.

Keyword Frequency: Identifies the top 5 most frequent words in log messages (excluding stop-words) using a Priority Queue algorithm.

Report Export: Offers the option to export the analysis results to a text file (AnalysisReport.txt).

🛠️ Tech Stack

Language: C++ (STL)

Data Structures: std::map, std::vector, std::priority_queue, std::set.

Concepts: Stream Polymorphism (ostream), File I/O (fstream), String Manipulation (stringstream).

📋 Prerequisite: Input Format

The analyzer expects log files in the following format:

[YYYY-MM-DD HH:MM:SS] LEVEL Message content...


Example:

[2025-12-04 14:00:00] INFO System started successfully
[2025-12-04 14:05:23] ERROR Connection lost to database
[2025-12-04 14:10:00] WARNING High memory usage detected


💻 How to Run

1. Compile the code

You need a C++ compiler (like g++). Run the following command in your terminal:

g++ main.cpp -o analyzer


2. Run the executable

Pass the log file path as an argument:

Windows:

analyzer.exe sample_logs.txt


Linux/Mac:

./analyzer sample_logs.txt


📊 Sample Output

=== Analysis Report ===
Archive: sample_logs.txt
Logs total: 107
Mean time between errors: 350 seconds (approx. 5 minutes)

Count by level:
- INFO: 69 logs (65%)
- WARNING: 22 logs (20%)
- ERROR: 12 logs (11%)
- CRITICAL: 3 logs (2%)

Most frequent words:
1: timeout (25 times)
2: connection (22 times)
3: database (13 times)
4: retry (8 times)
5: failed (7 times)


Developed by @pedrojosuegr
