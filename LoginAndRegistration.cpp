#include <iostream>
#include <cstdlib> // For system() or popen()
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

// Function Prototypes
void NewRegister();
void Login();
void callPythonScript(const string &scriptPath);
void readAttendanceFile(const string &filename);

int main()
{
    int choice;
    do
    {
        cout << "\nMenu:\n";
        cout << "1. Register (Face Detection)\n";
        cout << "2. Login (Face Detection)\n";
        cout << "3. View Attendance\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            NewRegister();
            break;
        case 2:
            Login();
            break;
        case 3:
            readAttendanceFile("/Users/huong/Desktop/FaceDetection_final/Attendance.csv");
            break;
        case 4:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}

void NewRegister()
{
    cout << "Starting face registration..." << endl;
    callPythonScript("/Users/huong/Desktop/FaceDetection_final/Dataset.py");  // Pass only the script path
    cout << "Registration complete!" << endl;
}

void Login()
{
    cout << "Starting face login..." << endl;
    callPythonScript("/Users/huong/Desktop/FaceDetection_final/Attendance.py");  // Pass only the script path
    cout << "Login process completed!" << endl;
}

void callPythonScript(const string &scriptPath)
{
    // Correctly construct the command
    string command = "/Users/huong/Desktop/FaceDetection_final/.venv/bin/python3 " + scriptPath;

    // Execute the command
    int ret = system(command.c_str());
    if (ret != 0)
    {
        cerr << "Error: Failed to execute Python script: " << scriptPath << endl;
    }
}

void readAttendanceFile(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file " << filename << endl;
        return;
    }

    vector<vector<string> > attendanceData;
    string line;

    while (getline(file, line))
    {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        attendanceData.push_back(row);
    }

    file.close();

    // Display the attendance data
    for (size_t i = 0; i < attendanceData.size(); ++i)
    {
        for (size_t j = 0; j < attendanceData[i].size(); ++j)
        {
            cout << attendanceData[i][j] << " ";
        }
        cout << endl;
    }
}
