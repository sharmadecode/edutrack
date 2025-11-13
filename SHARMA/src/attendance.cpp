// attendance.cpp - Attendance Management Module (fixed externs)
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "structures.h"

using namespace std;

// extern globals defined in main.cpp
extern Student *studentHead;
extern AttendanceRecord *attendanceHead;
extern HashNode *hashTable[];

// color externs (must match main.cpp definitions)
extern const std::string COLOR_RESET;
extern const std::string COLOR_RED;
extern const std::string COLOR_GREEN;
extern const std::string COLOR_YELLOW;
extern const std::string COLOR_BLUE;
extern const std::string COLOR_MAGENTA;
extern const std::string COLOR_CYAN;

// Forward declarations for functions provided by main.cpp
void clearScreen();
void waitForEnter();
void displayHeader(const string &);
Student *searchInHash(int);
string getCurrentDate();
string getFutureDate(int);

// ==================== ATTENDANCE FUNCTIONS ====================

void markAttendance()
{
    clearScreen();
    displayHeader("MARK ATTENDANCE");

    int studentID;
    cout << "\tEnter Student ID: ";
    cin >> studentID;

    Student *student = searchInHash(studentID);

    if (student == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\n\tStudent: " << COLOR_CYAN << student->name << COLOR_RESET << "\n";
    cout << "\tBranch: " << student->branch << " | Semester: " << student->semester << "\n\n";

    // Create new attendance record
    AttendanceRecord *newRecord = new AttendanceRecord();

    newRecord->studentID = studentID;
    newRecord->date = getCurrentDate();

    cin.ignore();
    cout << "\tEnter Subject: ";
    getline(cin, newRecord->subject);

    cout << "\tEnter Subject Code: ";
    getline(cin, newRecord->subjectCode);

    cout << "\tEnter Period (1-8): ";
    cin >> newRecord->period;

    cout << "\tMark Present? (Y/N): ";
    char present;
    cin >> present;
    newRecord->isPresent = (toupper(present) == 'Y');

    cin.ignore();
    cout << "\tMarked by (Faculty Name): ";
    getline(cin, newRecord->markedBy);

    cout << "\tRemarks (optional): ";
    getline(cin, newRecord->remarks);

    // Insert at beginning of linked list
    newRecord->next = attendanceHead;
    attendanceHead = newRecord;

    cout << "\n\t" << COLOR_GREEN << "✓ Attendance marked successfully!" << COLOR_RESET << "\n";
    cout << "\tStatus: " << (newRecord->isPresent ? COLOR_GREEN + "PRESENT" : COLOR_RED + "ABSENT") << COLOR_RESET << "\n";

    waitForEnter();
}

void viewAttendanceByStudent()
{
    clearScreen();
    displayHeader("VIEW STUDENT ATTENDANCE");

    int studentID;
    cout << "\tEnter Student ID: ";
    cin >> studentID;

    Student *student = searchInHash(studentID);

    if (student == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\n\tStudent: " << COLOR_CYAN << student->name << COLOR_RESET << "\n";
    cout << "\tBranch: " << student->branch << " | Semester: " << student->semester << "\n\n";

    cout << "\t" << string(120, '=') << "\n";
    cout << "\t" << left << setw(12) << "Date"
         << setw(30) << "Subject"
         << setw(15) << "Code"
         << setw(10) << "Period"
         << setw(15) << "Status"
         << setw(25) << "Marked By"
         << "Remarks" << "\n";
    cout << "\t" << string(120, '=') << "\n";

    AttendanceRecord *current = attendanceHead;
    int totalClasses = 0;
    int attendedClasses = 0;

    while (current != nullptr)
    {
        if (current->studentID == studentID)
        {
            cout << "\t" << left << setw(12) << current->date
                 << setw(30) << current->subject
                 << setw(15) << current->subjectCode
                 << setw(10) << current->period
                 << setw(15) << (current->isPresent ? COLOR_GREEN + string("Present") : COLOR_RED + string("Absent"))
                 << COLOR_RESET << setw(25) << current->markedBy
                 << current->remarks << "\n";

            totalClasses++;
            if (current->isPresent)
                attendedClasses++;
        }
        current = current->next;
    }

    if (totalClasses == 0)
    {
        cout << "\n\t" << COLOR_YELLOW << "No attendance records found!" << COLOR_RESET << "\n";
    }
    else
    {
        float percentage = (float)attendedClasses / totalClasses * 100;

        cout << "\t" << string(120, '=') << "\n";
        cout << "\n\t" << COLOR_CYAN << "Attendance Summary:" << COLOR_RESET << "\n";
        cout << "\tTotal Classes: " << totalClasses << "\n";
        cout << "\tClasses Attended: " << COLOR_GREEN << attendedClasses << COLOR_RESET << "\n";
        cout << "\tClasses Missed: " << COLOR_RED << (totalClasses - attendedClasses)
             << COLOR_RESET << "\n";
        cout << "\tAttendance Percentage: " << fixed << setprecision(2);

        if (percentage >= 75)
        {
            cout << COLOR_GREEN << percentage << "%" << COLOR_RESET;
        }
        else if (percentage >= 65)
        {
            cout << COLOR_YELLOW << percentage << "%" << COLOR_RESET;
        }
        else
        {
            cout << COLOR_RED << percentage << "%" << COLOR_RESET;
        }
        cout << "\n\n";

        if (percentage < 75)
        {
            cout << "\t" << COLOR_RED << "⚠ Warning: Attendance below 75%!" << COLOR_RESET << "\n";
        }
    }

    waitForEnter();
}

void viewAttendanceBySubject()
{
    clearScreen();
    displayHeader("VIEW SUBJECT-WISE ATTENDANCE");

    string subjectCode;
    cin.ignore();
    cout << "\tEnter Subject Code: ";
    getline(cin, subjectCode);

    cout << "\n\t" << string(110, '=') << "\n";
    cout << "\t" << left << setw(12) << "Date"
         << setw(10) << "Stud ID"
         << setw(25) << "Student Name"
         << setw(10) << "Period"
         << setw(15) << "Status"
         << setw(25) << "Marked By" << "\n";
    cout << "\t" << string(110, '=') << "\n";

    AttendanceRecord *current = attendanceHead;
    int count = 0;
    int presentCount = 0;

    while (current != nullptr)
    {
        if (current->subjectCode == subjectCode)
        {
            Student *student = searchInHash(current->studentID);
            string studentName = (student != nullptr) ? student->name : "Unknown";

            cout << "\t" << left << setw(12) << current->date
                 << setw(10) << current->studentID
                 << setw(25) << studentName
                 << setw(10) << current->period
                 << setw(15) << (current->isPresent ? COLOR_GREEN + "Present" : COLOR_RED + "Absent")
                 << COLOR_RESET << setw(25) << current->markedBy << "\n";

            count++;
            if (current->isPresent)
                presentCount++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        cout << "\n\t" << COLOR_YELLOW << "No records found for this subject!"
             << COLOR_RESET << "\n";
    }
    else
    {
        float avgAttendance = (float)presentCount / count * 100;

        cout << "\t" << string(110, '=') << "\n";
        cout << "\n\tTotal Records: " << count << "\n";
        cout << "\tPresent Count: " << COLOR_GREEN << presentCount << COLOR_RESET << "\n";
        cout << "\tAbsent Count: " << COLOR_RED << (count - presentCount) << COLOR_RESET << "\n";
        cout << "\tAverage Attendance: " << fixed << setprecision(2) << avgAttendance << "%\n";
    }

    waitForEnter();
}

void generateAttendanceReport()
{
    clearScreen();
    displayHeader("ATTENDANCE REPORT - ALL STUDENTS");

    // Use map to store student-wise attendance summary
    map<int, pair<int, int>> attendanceMap; // studentID -> (total, present)

    AttendanceRecord *current = attendanceHead;

    while (current != nullptr)
    {
        if (attendanceMap.find(current->studentID) == attendanceMap.end())
        {
            attendanceMap[current->studentID] = make_pair(0, 0);
        }

        attendanceMap[current->studentID].first++;
        if (current->isPresent)
        {
            attendanceMap[current->studentID].second++;
        }

        current = current->next;
    }

    cout << "\t" << string(100, '=') << "\n";
    cout << "\t" << left << setw(10) << "Stud ID"
         << setw(30) << "Name"
         << setw(15) << "Total Classes"
         << setw(15) << "Attended"
         << setw(15) << "Percentage"
         << "Status" << "\n";
    cout << "\t" << string(100, '=') << "\n";

    int belowThreshold = 0;

    for (const auto &entry : attendanceMap)
    {
        Student *student = searchInHash(entry.first);

        if (student != nullptr)
        {
            int total = entry.second.first;
            int present = entry.second.second;
            float percentage = (float)present / total * 100;

            string status;
            if (percentage >= 75)
            {
                status = COLOR_GREEN + "Good" + COLOR_RESET;
            }
            else if (percentage >= 65)
            {
                status = COLOR_YELLOW + "Warning" + COLOR_RESET;
                belowThreshold++;
            }
            else
            {
                status = COLOR_RED + "Critical" + COLOR_RESET;
                belowThreshold++;
            }

            cout << "\t" << left << setw(10) << student->studentID
                 << setw(30) << student->name
                 << setw(15) << total
                 << setw(15) << present
                 << setw(15) << fixed << setprecision(2) << percentage << "%"
                 << status << "\n";
        }
    }

    cout << "\t" << string(100, '=') << "\n";
    cout << "\n\tTotal Students: " << attendanceMap.size() << "\n";
    cout << "\tStudents below 75%: " << COLOR_RED << belowThreshold << COLOR_RESET << "\n";

    waitForEnter();
}

void attendanceManagementMenu()
{
    int choice;

    while (true)
    {
        clearScreen();
        cout << "\n\n\t" << COLOR_CYAN;
        cout << "╔════════════════════════════════════════════════════════════════╗\n";
        cout << "║                 ATTENDANCE MANAGEMENT MODULE                   ║\n";
        cout << "╠════════════════════════════════════════════════════════════════╣\n";
        cout << "║                                                                ║\n";
        cout << "║    1. Mark Attendance                                          ║\n";
        cout << "║    2. View Student Attendance                                  ║\n";
        cout << "║    3. View Subject-wise Attendance                             ║\n";
        cout << "║    4. Generate Attendance Report                               ║\n";
        cout << "║    5. Mark Bulk Attendance (Entire Class)                      ║\n";
        cout << "║    6. Edit Attendance Record                                   ║\n";
        cout << "║    7. Attendance Defaulters List                               ║\n";
        cout << "║    0. Back to Main Menu                                        ║\n";
        cout << "║                                                                ║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        cout << "\n\tEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            markAttendance();
            break;
        case 2:
            viewAttendanceByStudent();
            break;
        case 3:
            viewAttendanceBySubject();
            break;
        case 4:
            generateAttendanceReport();
            break;
        case 0:
            return;
        default:
            cout << "\n\t" << COLOR_RED << "Invalid choice!" << COLOR_RESET << "\n";
            waitForEnter();
        }
    }
}
