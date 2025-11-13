// EduTrack Pro - Student Management System

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
extern const std::string COLOR_RESET;
extern const std::string COLOR_RED;
extern const std::string COLOR_GREEN;
extern const std::string COLOR_YELLOW;
extern const std::string COLOR_BLUE;
extern const std::string COLOR_MAGENTA;
extern const std::string COLOR_CYAN;
#include <ctime>
using namespace std;

// STUDENT MODULE

struct Student
{
    // Basic Information
    int studentID;
    string name;
    string fatherName;
    string motherName;
    string email;
    string phone;
    string address;
    string dateOfBirth;
    string bloodGroup;
    string gender;

    // Academic Information
    string enrollmentDate;
    string branch;
    string section;
    int semester;
    float cgpa;
    int totalCredits;

    // Status Information
    bool isActive;
    string status; // Active, Graduated, Suspended, Dropout

    // Linked List Pointer
    Student *next;

    Student()
    {
        studentID = 0;
        semester = 1;
        cgpa = 0.0;
        totalCredits = 0;
        isActive = true;
        status = "Active";
        next = nullptr;
    }
};

//  ATTENDANCE MODULE

struct AttendanceRecord
{
    int studentID;
    string date;
    string subject;
    string subjectCode;
    int period;
    bool isPresent;
    string markedBy; // Faculty name
    string remarks;

    AttendanceRecord *next;

    AttendanceRecord()
    {
        studentID = 0;
        period = 0;
        isPresent = false;
        next = nullptr;
    }
};

struct AttendanceSummary
{
    int studentID;
    string subject;
    int totalClasses;
    int classesAttended;
    float percentage;

    AttendanceSummary *next;

    AttendanceSummary()
    {
        studentID = 0;
        totalClasses = 0;
        classesAttended = 0;
        percentage = 0.0;
        next = nullptr;
    }
};

//  GRADE MODULE

struct Grade
{
    int studentID;
    string subject;
    string subjectCode;
    int semester;

    // Internal Assessment
    int midterm1;
    int midterm2;
    int assignment;
    int quiz;
    int internal;

    // External Assessment
    int external;
    int total;

    // Grade Info
    string letterGrade;
    float gradePoint;
    int credits;

    Grade *next;

    Grade()
    {
        studentID = 0;
        semester = 0;
        midterm1 = midterm2 = assignment = quiz = 0;
        internal = external = total = 0;
        gradePoint = 0.0;
        credits = 0;
        next = nullptr;
    }
};

struct SemesterResult
{
    int studentID;
    int semester;
    float sgpa;
    float cgpa;
    int totalCredits;
    int creditsEarned;
    string result; // Pass/Fail/Backlog
    int backlogs;

    SemesterResult *next;

    SemesterResult()
    {
        studentID = semester = 0;
        sgpa = cgpa = 0.0;
        totalCredits = creditsEarned = backlogs = 0;
        result = "Pass";
        next = nullptr;
    }
};

// ==================== FEE MODULE ====================

struct FeeStructure
{
    string branch;
    int semester;
    float tuitionFee;
    float examFee;
    float libraryFee;
    float labFee;
    float sportsFee;
    float totalFee;

    FeeStructure *next;

    FeeStructure()
    {
        semester = 0;
        tuitionFee = examFee = libraryFee = labFee = sportsFee = totalFee = 0.0;
        next = nullptr;
    }
};

struct FeePayment
{
    int paymentID;
    int studentID;
    int semester;
    float amount;
    string paymentDate;
    string paymentMode; // Cash, Card, UPI, Net Banking
    string receiptNumber;
    string remarks;

    FeePayment *next;

    FeePayment()
    {
        paymentID = studentID = semester = 0;
        amount = 0.0;
        next = nullptr;
    }
};

struct FeeRecord
{
    int studentID;
    int semester;
    float totalFee;
    float paidAmount;
    float pendingAmount;
    float discountAmount;
    string lastPaymentDate;
    string paymentStatus; // PAID, PARTIAL, PENDING, OVERDUE
    string dueDate;

    FeeRecord *next;

    FeeRecord()
    {
        studentID = semester = 0;
        totalFee = paidAmount = pendingAmount = discountAmount = 0.0;
        paymentStatus = "PENDING";
        next = nullptr;
    }
};

// ==================== LIBRARY MODULE ====================

struct Book
{
    int bookID;
    string title;
    string author;
    string publisher;
    string isbn;
    string category;
    int edition;
    int publicationYear;
    int totalCopies;
    int availableCopies;
    float price;
    string rackNumber;

    // Binary Search Tree Pointers
    Book *left;
    Book *right;

    Book()
    {
        bookID = edition = publicationYear = 0;
        totalCopies = availableCopies = 0;
        price = 0.0;
        left = right = nullptr;
    }
};

struct IssueRecord
{
    int issueID;
    int studentID;
    int bookID;
    string issueDate;
    string dueDate;
    string returnDate;
    float fine;
    bool isReturned;
    bool isRenewed;
    int renewCount;
    string issuedBy;
    string returnedTo;

    IssueRecord *next;

    IssueRecord()
    {
        issueID = studentID = bookID = renewCount = 0;
        fine = 0.0;
        isReturned = isRenewed = false;
        next = nullptr;
    }
};

// ==================== COURSE MODULE ====================

struct Course
{
    string courseCode;
    string courseName;
    string branch;
    int semester;
    int credits;
    string courseType; // Theory, Practical, Elective
    string faculty;
    int maxStrength;
    int enrolled;

    Course *next;

    Course()
    {
        semester = credits = maxStrength = enrolled = 0;
        next = nullptr;
    }
};

struct CourseRegistration
{
    int studentID;
    string courseCode;
    int semester;
    string registrationDate;
    string status; // Registered, Dropped, Completed

    CourseRegistration *next;

    CourseRegistration()
    {
        studentID = semester = 0;
        status = "Registered";
        next = nullptr;
    }
};

// ==================== TIMETABLE MODULE ====================

struct TimeSlot
{
    string day;
    string startTime;
    string endTime;
    string courseCode;
    string faculty;
    string room;
    string branch;
    int semester;
    string section;

    TimeSlot *next;

    TimeSlot()
    {
        semester = 0;
        next = nullptr;
    }
};

// ==================== FACULTY MODULE ====================

struct Faculty
{
    int facultyID;
    string name;
    string email;
    string phone;
    string department;
    string designation;
    string qualification;
    int experience;
    string joiningDate;

    Faculty *next;

    Faculty()
    {
        facultyID = experience = 0;
        next = nullptr;
    }
};

// ==================== NOTIFICATION MODULE ====================

struct Notification
{
    int notificationID;
    string title;
    string message;
    string date;
    string priority;       // High, Medium, Low
    string category;       // Academic, Fee, Library, General
    string targetAudience; // All, Semester-wise, Branch-wise
    bool isActive;

    Notification *next;

    Notification()
    {
        notificationID = 0;
        isActive = true;
        next = nullptr;
    }
};

// ==================== EXAM MODULE ====================

struct ExamSchedule
{
    int examID;
    string courseCode;
    string examType; // Midterm, Final, Practical
    string date;
    string startTime;
    string endTime;
    string room;
    int semester;
    string branch;

    ExamSchedule *next;

    ExamSchedule()
    {
        examID = semester = 0;
        next = nullptr;
    }
};

// ==================== HASH TABLE NODE ====================
// For O(1) student lookup

struct HashNode
{
    Student *student;
    HashNode *next;

    HashNode()
    {
        student = nullptr;
        next = nullptr;
    }
};

#endif // STRUCTURES_H