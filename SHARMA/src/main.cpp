#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

// == DATA STRUCTURES ==

struct Student
{
    int studentID;
    string name, fatherName, motherName, email, phone, address;
    string dateOfBirth, bloodGroup, gender, enrollmentDate;
    string branch, section, status;
    int semester, totalCredits;
    float cgpa;
    bool isActive;
    Student *next;

    Student() : studentID(0), semester(1), totalCredits(0), cgpa(0.0),
                isActive(true), status("Active"), next(nullptr) {}
};

struct AttendanceRecord
{
    int studentID, period;
    string date, subject, subjectCode, markedBy, remarks;
    bool isPresent;
    AttendanceRecord *next;

    AttendanceRecord() : studentID(0), period(0), isPresent(false), next(nullptr) {}
};

struct Grade
{
    int studentID, semester, midterm1, midterm2, assignment, quiz;
    int internal, external, total, credits;
    string subject, subjectCode, letterGrade;
    float gradePoint;
    Grade *next;

    Grade() : studentID(0), semester(0), midterm1(0), midterm2(0),
              assignment(0), quiz(0), internal(0), external(0), total(0),
              credits(0), gradePoint(0.0), next(nullptr) {}
};

struct FeeRecord
{
    int studentID, semester;
    float totalFee, paidAmount, pendingAmount, discountAmount;
    string lastPaymentDate, paymentStatus, dueDate;
    FeeRecord *next;

    FeeRecord() : studentID(0), semester(0), totalFee(0.0), paidAmount(0.0),
                  pendingAmount(0.0), discountAmount(0.0),
                  paymentStatus("PENDING"), next(nullptr) {}
};

struct Book
{
    int bookID, edition, publicationYear, totalCopies, availableCopies;
    string title, author, publisher, isbn, category, rackNumber;
    float price;
    Book *left, *right;

    Book() : bookID(0), edition(0), publicationYear(0), totalCopies(0),
             availableCopies(0), price(0.0), left(nullptr), right(nullptr) {}
};

struct IssueRecord
{
    int issueID, studentID, bookID, renewCount;
    string issueDate, dueDate, returnDate, issuedBy, returnedTo;
    float fine;
    bool isReturned, isRenewed;
    IssueRecord *next;

    IssueRecord() : issueID(0), studentID(0), bookID(0), renewCount(0),
                    fine(0.0), isReturned(false), isRenewed(false), next(nullptr) {}
};

struct HashNode
{
    Student *student;
    HashNode *next;
    HashNode() : student(nullptr), next(nullptr) {}
};

// == GLOBALS ==

const int HASH_TABLE_SIZE = 1000;
const string COLOR_RESET = "\033[0m", COLOR_RED = "\033[31m";
const string COLOR_GREEN = "\033[32m", COLOR_YELLOW = "\033[33m";
const string COLOR_BLUE = "\033[34m", COLOR_MAGENTA = "\033[35m";
const string COLOR_CYAN = "\033[36m";

Student *studentHead = nullptr;
HashNode *hashTable[HASH_TABLE_SIZE] = {nullptr};
AttendanceRecord *attendanceHead = nullptr;
Grade *gradeHead = nullptr;
FeeRecord *feeRecordHead = nullptr;
Book *bookRoot = nullptr;
IssueRecord *issueHead = nullptr;

int studentCounter = 10001;
int issueCounter = 5001;

// == UTILITY FUNCTIONS ==

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter()
{
    cout << "\n\t" << COLOR_CYAN << "Press Enter to continue..." << COLOR_RESET;
    cin.ignore();
    cin.get();
}

string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return string(buffer);
}

string getFutureDate(int days)
{
    time_t now = time(0);
    now += days * 24 * 60 * 60;
    tm *ltm = localtime(&now);
    char buffer[11];
    sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return string(buffer);
}

void displayHeader(const string &title)
{
    cout << "\n\n\t" << COLOR_CYAN;
    cout << "╔" << string(88, '═') << "╗\n";
    int padding = (88 - title.length()) / 2;
    cout << "\t║" << string(padding, ' ') << title
         << string(88 - padding - title.length(), ' ') << "║\n";
    cout << "\t╚" << string(88, '═') << "╝" << COLOR_RESET << "\n\n";
}

void displayLogo()
{
    clearScreen();
    cout << COLOR_CYAN << R"(
        ███████╗██████╗ ██╗   ██╗████████╗██████╗  █████╗  ██████╗██╗  ██╗
        ██╔════╝██╔══██╗██║   ██║╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██║ ██╔╝
        █████╗  ██║  ██║██║   ██║   ██║   ██████╔╝███████║██║     █████╔╝ 
        ██╔══╝  ██║  ██║██║   ██║   ██║   ██╔══██╗██╔══██║██║     ██╔═██╗ 
        ███████╗██████╔╝╚██████╔╝   ██║   ██║  ██║██║  ██║╚██████╗██║  ██╗
        ╚══════╝╚═════╝  ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝
    )" << COLOR_RESET
         << "\n";
    cout << "\t\t" << COLOR_YELLOW << "Pro Student Management System" << COLOR_RESET << "\n";
    cout << "\t\t" << COLOR_GREEN << "Made by ADITYA SHARMA" << COLOR_RESET << "\n\n";
    cout << "\t\t" << COLOR_CYAN << "═══════════════════════════════════" << COLOR_RESET << "\n\n";
}

// == HASH TABLE FUNCTIONS ==

int hashFunction(int studentID)
{
    return studentID % HASH_TABLE_SIZE;
}

void insertIntoHash(Student *student)
{
    int index = hashFunction(student->studentID);
    HashNode *newNode = new HashNode();
    newNode->student = student;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

Student *searchInHash(int studentID)
{
    int index = hashFunction(studentID);
    HashNode *current = hashTable[index];
    while (current != nullptr)
    {
        if (current->student->studentID == studentID)
        {
            return current->student;
        }
        current = current->next;
    }
    return nullptr;
}

// == STUDENT MODULE ==

void addStudent()
{
    clearScreen();
    displayHeader("ADD NEW STUDENT");

    Student *newStudent = new Student();
    newStudent->studentID = studentCounter++;

    cout << "\t" << COLOR_GREEN << "Student ID (Auto): " << newStudent->studentID << COLOR_RESET << "\n\n";

    cin.ignore();
    cout << "\tFull Name: ";
    getline(cin, newStudent->name);
    cout << "\tFather's Name: ";
    getline(cin, newStudent->fatherName);
    cout << "\tMother's Name: ";
    getline(cin, newStudent->motherName);
    cout << "\tEmail: ";
    getline(cin, newStudent->email);
    cout << "\tPhone: ";
    getline(cin, newStudent->phone);
    cout << "\tAddress: ";
    getline(cin, newStudent->address);
    cout << "\tDate of Birth (DD/MM/YYYY): ";
    getline(cin, newStudent->dateOfBirth);
    cout << "\tBlood Group: ";
    getline(cin, newStudent->bloodGroup);
    cout << "\tGender (M/F/Other): ";
    getline(cin, newStudent->gender);

    cout << "\n\t" << COLOR_YELLOW << "Academic Details:" << COLOR_RESET << "\n";
    cout << "\tBranch (CSE/ECE/ME/CE): ";
    getline(cin, newStudent->branch);
    cout << "\tSection (A/B/C): ";
    getline(cin, newStudent->section);
    cout << "\tSemester (1-8): ";
    cin >> newStudent->semester;

    newStudent->enrollmentDate = getCurrentDate();
    newStudent->next = studentHead;
    studentHead = newStudent;
    insertIntoHash(newStudent);

    cout << "\n\t" << COLOR_GREEN << "✓ Student added successfully!" << COLOR_RESET << "\n";
    waitForEnter();
}

void searchStudent()
{
    clearScreen();
    displayHeader("SEARCH STUDENT");

    int id;
    cout << "\tStudent ID: ";
    cin >> id;

    Student *s = searchInHash(id);
    if (s == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
    }
    else
    {
        cout << "\n\t" << string(88, '=') << "\n";
        cout << "\tID: " << s->studentID << " | Name: " << s->name << "\n";
        cout << "\tBranch: " << s->branch << " | Semester: " << s->semester << "\n";
        cout << "\tEmail: " << s->email << " | Phone: " << s->phone << "\n";
        cout << "\tCGPA: " << fixed << setprecision(2) << s->cgpa << "\n";
        cout << "\t" << string(88, '=') << "\n";
    }
    waitForEnter();
}

void displayAllStudents()
{
    clearScreen();
    displayHeader("ALL STUDENTS");

    cout << "\t" << string(120, '=') << "\n";
    cout << "\t" << left << setw(10) << "ID" << setw(25) << "Name"
         << setw(12) << "Branch" << setw(6) << "Sem" << setw(25) << "Email"
         << setw(15) << "Phone" << setw(8) << "CGPA" << "\n";
    cout << "\t" << string(120, '=') << "\n";

    Student *current = studentHead;
    int count = 0;

    while (current != nullptr)
    {
        cout << "\t" << left << setw(10) << current->studentID
             << setw(25) << current->name << setw(12) << current->branch
             << setw(6) << current->semester << setw(25) << current->email
             << setw(15) << current->phone << setw(8) << fixed << setprecision(2)
             << current->cgpa << "\n";
        current = current->next;
        count++;
    }

    cout << "\t" << string(120, '=') << "\n";
    cout << "\t" << COLOR_CYAN << "Total: " << count << COLOR_RESET << "\n";
    waitForEnter();
}

// == ATTENDANCE MODULE ==

void markAttendance()
{
    clearScreen();
    displayHeader("MARK ATTENDANCE");

    int id;
    cout << "\tStudent ID: ";
    cin >> id;

    Student *s = searchInHash(id);
    if (s == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\n\tStudent: " << COLOR_CYAN << s->name << COLOR_RESET << "\n\n";

    AttendanceRecord *rec = new AttendanceRecord();
    rec->studentID = id;
    rec->date = getCurrentDate();

    cin.ignore();
    cout << "\tSubject: ";
    getline(cin, rec->subject);
    cout << "\tSubject Code: ";
    getline(cin, rec->subjectCode);
    cout << "\tPeriod (1-8): ";
    cin >> rec->period;

    char p;
    cout << "\tPresent (Y/N): ";
    cin >> p;
    rec->isPresent = (toupper(p) == 'Y');

    cin.ignore();
    cout << "\tMarked By: ";
    getline(cin, rec->markedBy);

    rec->next = attendanceHead;
    attendanceHead = rec;

    cout << "\n\t" << COLOR_GREEN << "✓ Attendance marked!" << COLOR_RESET << "\n";
    waitForEnter();
}

void viewAttendance()
{
    clearScreen();
    displayHeader("VIEW ATTENDANCE");

    int id;
    cout << "\tStudent ID: ";
    cin >> id;

    Student *s = searchInHash(id);
    if (s == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\n\tStudent: " << COLOR_CYAN << s->name << COLOR_RESET << "\n\n";

    cout << "\t" << string(110, '=') << "\n";
    cout << "\t" << left << setw(12) << "Date" << setw(30) << "Subject"
         << setw(12) << "Code" << setw(8) << "Period" << setw(12) << "Status" << "\n";
    cout << "\t" << string(110, '=') << "\n";

    AttendanceRecord *curr = attendanceHead;
    int total = 0, present = 0;

    while (curr != nullptr)
    {
        if (curr->studentID == id)
        {
            cout << "\t" << left << setw(12) << curr->date
                 << setw(30) << curr->subject << setw(12) << curr->subjectCode
                 << setw(8) << curr->period
                 << (curr->isPresent ? COLOR_GREEN + "Present" : COLOR_RED + "Absent")
                 << COLOR_RESET << "\n";
            total++;
            if (curr->isPresent)
                present++;
        }
        curr = curr->next;
    }

    if (total > 0)
    {
        float perc = (float)present / total * 100;
        cout << "\t" << string(110, '=') << "\n";
        cout << "\n\tTotal: " << total << " | Present: " << present
             << " | Percentage: " << fixed << setprecision(2) << perc << "%\n";
    }

    waitForEnter();
}

// == LIBRARY MODULE (BST) ==

Book *insertBook(Book *root, Book *newBook)
{
    if (root == nullptr)
        return newBook;
    if (newBook->bookID < root->bookID)
        root->left = insertBook(root->left, newBook);
    else if (newBook->bookID > root->bookID)
        root->right = insertBook(root->right, newBook);
    return root;
}

Book *searchBook(Book *root, int bookID)
{
    if (root == nullptr || root->bookID == bookID)
        return root;
    return (bookID < root->bookID) ? searchBook(root->left, bookID)
                                   : searchBook(root->right, bookID);
}

void displayBooksInorder(Book *root)
{
    if (root == nullptr)
        return;
    displayBooksInorder(root->left);
    cout << "\t" << left << setw(8) << root->bookID << setw(40) << root->title
         << setw(25) << root->author << setw(8) << root->availableCopies << "\n";
    displayBooksInorder(root->right);
}

void addBook()
{
    clearScreen();
    displayHeader("ADD NEW BOOK");

    Book *b = new Book();
    cout << "\tBook ID: ";
    cin >> b->bookID;

    if (searchBook(bookRoot, b->bookID))
    {
        cout << "\n\t" << COLOR_RED << "✗ Book ID exists!" << COLOR_RESET << "\n";
        delete b;
        waitForEnter();
        return;
    }

    cin.ignore();
    cout << "\tTitle: ";
    getline(cin, b->title);
    cout << "\tAuthor: ";
    getline(cin, b->author);
    cout << "\tPublisher: ";
    getline(cin, b->publisher);
    cout << "\tISBN: ";
    getline(cin, b->isbn);
    cout << "\tCategory: ";
    getline(cin, b->category);
    cout << "\tEdition: ";
    cin >> b->edition;
    cout << "\tYear: ";
    cin >> b->publicationYear;
    cout << "\tPrice: ₹";
    cin >> b->price;
    cout << "\tCopies: ";
    cin >> b->totalCopies;
    b->availableCopies = b->totalCopies;

    bookRoot = insertBook(bookRoot, b);

    cout << "\n\t" << COLOR_GREEN << "✓ Book added!" << COLOR_RESET << "\n";
    waitForEnter();
}

void issueBook()
{
    clearScreen();
    displayHeader("ISSUE BOOK");

    int sid, bid;
    cout << "\tStudent ID: ";
    cin >> sid;

    Student *s = searchInHash(sid);
    if (!s)
    {
        cout << "\n\t" << COLOR_RED << "✗ Student not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\n\tStudent: " << s->name << "\n\n";
    cout << "\tBook ID: ";
    cin >> bid;

    Book *b = searchBook(bookRoot, bid);
    if (!b)
    {
        cout << "\n\t" << COLOR_RED << "✗ Book not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    if (b->availableCopies <= 0)
    {
        cout << "\n\t" << COLOR_RED << "✗ No copies available!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    IssueRecord *issue = new IssueRecord();
    issue->issueID = issueCounter++;
    issue->studentID = sid;
    issue->bookID = bid;
    issue->issueDate = getCurrentDate();
    issue->dueDate = getFutureDate(14);
    issue->next = issueHead;
    issueHead = issue;

    b->availableCopies--;

    cout << "\n\t" << COLOR_GREEN << "✓ Book issued!" << COLOR_RESET << "\n";
    cout << "\tIssue ID: " << issue->issueID << " | Due: " << issue->dueDate << "\n";
    waitForEnter();
}

void returnBook()
{
    clearScreen();
    displayHeader("RETURN BOOK");

    int iid;
    cout << "\tIssue ID: ";
    cin >> iid;

    IssueRecord *curr = issueHead;
    while (curr)
    {
        if (curr->issueID == iid && !curr->isReturned)
        {
            curr->returnDate = getCurrentDate();
            curr->isReturned = true;

            Book *b = searchBook(bookRoot, curr->bookID);
            if (b)
                b->availableCopies++;

            cout << "\n\t" << COLOR_GREEN << "✓ Book returned!" << COLOR_RESET << "\n";
            waitForEnter();
            return;
        }
        curr = curr->next;
    }

    cout << "\n\t" << COLOR_RED << "✗ Invalid Issue ID!" << COLOR_RESET << "\n";
    waitForEnter();
}

// == MENUS ==

void studentMenu()
{
    while (true)
    {
        clearScreen();
        cout << "\n\n\t" << COLOR_CYAN;
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║            STUDENT MANAGEMENT SYSTEM                     ║\n";
        cout << "╠══════════════════════════════════════════════════════════╣\n";
        cout << "║  1. Add New Student                                      ║\n";
        cout << "║  2. Search Student                                       ║\n";
        cout << "║  3. Display All Students                                 ║\n";
        cout << "║  4. Update Student                                       ║\n";
        cout << "║  5. Generate Student Report                              ║\n";
        cout << "║  0. Back                                                 ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        int ch;
        cout << "\n\tChoice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            addStudent();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            displayAllStudents();
            break;
        case 0:
            return;
        }
    }
}

void attendanceMenu()
{
    while (true)
    {
        clearScreen();
        cout << "\n\n\t" << COLOR_CYAN;
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║           ATTENDANCE MANAGEMENT SYSTEM                   ║\n";
        cout << "╠══════════════════════════════════════════════════════════╣\n";
        cout << "║  1. Mark Attendance                                      ║\n";
        cout << "║  2. View Student Attendance                              ║\n";
        cout << "║  3. Attendance Report                                    ║\n";
        cout << "║  0. Back                                                 ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        int ch;
        cout << "\n\tChoice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            markAttendance();
            break;
        case 2:
            viewAttendance();
            break;
        case 0:
            return;
        }
    }
}

void libraryMenu()
{
    while (true)
    {
        clearScreen();
        cout << "\n\n\t" << COLOR_CYAN;
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║            LIBRARY MANAGEMENT SYSTEM                     ║\n";
        cout << "╠══════════════════════════════════════════════════════════╣\n";
        cout << "║  1. Add New Book                                         ║\n";
        cout << "║  2. Issue Book                                           ║\n";
        cout << "║  3. Return Book                                          ║\n";
        cout << "║  4. View All Books                                       ║\n";
        cout << "║  0. Back                                                 ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        int ch;
        cout << "\n\tChoice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            addBook();
            break;
        case 2:
            issueBook();
            break;
        case 3:
            returnBook();
            break;
        case 0:
            return;
        }
    }
}

void mainMenu()
{
    while (true)
    {
        displayLogo();

        cout << "\t\t" << COLOR_CYAN;
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "\t\t║                   MAIN MENU                          ║\n";
        cout << "\t\t╠══════════════════════════════════════════════════════╣\n";
        cout << "\t\t║                                                      ║\n";
        cout << "\t\t║  1. Student Management                               ║\n";
        cout << "\t\t║  2. Attendance Management                            ║\n";
        cout << "\t\t║  3. Grade Management                                 ║\n";
        cout << "\t\t║  4. Fee Management                                   ║\n";
        cout << "\t\t║  5. Library Management                               ║\n";
        cout << "\t\t║  6. Reports & Analytics                              ║\n";
        cout << "\t\t║  7. System Settings                                  ║\n";
        cout << "\t\t║  0. Exit                                             ║\n";
        cout << "\t\t║                                                      ║\n";
        cout << "\t\t╚══════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        int choice;
        cout << "\n\t\tEnter Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            studentMenu();
            break;
        case 2:
            attendanceMenu();
            break;
        case 5:
            libraryMenu();
            break;
        case 0:
            cout << "\n\t\t" << COLOR_GREEN << "Thank you for using EduTrack Pro!"
                 << COLOR_RESET << "\n\n";
            return;
        default:
            cout << "\n\t\t" << COLOR_RED << "Invalid choice!" << COLOR_RESET << "\n";
            waitForEnter();
        }
    }
}

// == MAIN FUNCTION ==

int main()
{
    // Initialize hash table
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        hashTable[i] = nullptr;
    }

    mainMenu();

    return 0;
}