// library.cpp - Library Management Module (fixed externs)
#include <iostream>
#include <iomanip>
#include <string>
#include "structures.h"

using namespace std;

// extern globals from main.cpp
extern Book *bookRoot;
extern IssueRecord *issueHead;
extern Student *studentHead;
extern HashNode *hashTable[];
extern int issueCounter;
extern const float FINE_PER_DAY;
extern const int MAX_ISSUE_DAYS;
extern const int MAX_BOOKS_PER_STUDENT;

// color externs
extern const std::string COLOR_RESET;
extern const std::string COLOR_RED;
extern const std::string COLOR_GREEN;
extern const std::string COLOR_YELLOW;
extern const std::string COLOR_BLUE;
extern const std::string COLOR_MAGENTA;
extern const std::string COLOR_CYAN;

// Forward declarations implemented in main.cpp
void clearScreen();
void waitForEnter();
void displayHeader(const string &);
Student *searchInHash(int);
string getCurrentDate();
string getFutureDate(int);

// ==================== BINARY SEARCH TREE OPERATIONS ====================

Book *insertBook(Book *root, Book *newBook)
{
    if (root == nullptr)
    {
        return newBook;
    }

    if (newBook->bookID < root->bookID)
    {
        root->left = insertBook(root->left, newBook);
    }
    else if (newBook->bookID > root->bookID)
    {
        root->right = insertBook(root->right, newBook);
    }

    return root;
}

Book *searchBook(Book *root, int bookID)
{
    if (root == nullptr || root->bookID == bookID)
    {
        return root;
    }

    if (bookID < root->bookID)
    {
        return searchBook(root->left, bookID);
    }
    else
    {
        return searchBook(root->right, bookID);
    }
}

void displayBooksInorder(Book *root)
{
    if (root == nullptr)
        return;

    displayBooksInorder(root->left);

    cout << "\t" << left << setw(8) << root->bookID
         << setw(35) << root->title
         << setw(25) << root->author
         << setw(12) << root->isbn
         << setw(10) << root->totalCopies
         << setw(10) << root->availableCopies << "\n";

    displayBooksInorder(root->right);
}

int countBooks(Book *root)
{
    if (root == nullptr)
        return 0;
    return 1 + countBooks(root->left) + countBooks(root->right);
}

Book *findMin(Book *root)
{
    while (root->left != nullptr)
    {
        root = root->left;
    }
    return root;
}

Book *deleteBookNode(Book *root, int bookID)
{
    if (root == nullptr)
        return root;

    if (bookID < root->bookID)
    {
        root->left = deleteBookNode(root->left, bookID);
    }
    else if (bookID > root->bookID)
    {
        root->right = deleteBookNode(root->right, bookID);
    }
    else
    {
        // Node found - delete it
        if (root->left == nullptr)
        {
            Book *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            Book *temp = root->left;
            delete root;
            return temp;
        }

        // Node with two children
        Book *temp = findMin(root->right);

        // Copy data
        root->bookID = temp->bookID;
        root->title = temp->title;
        root->author = temp->author;
        root->isbn = temp->isbn;
        root->totalCopies = temp->totalCopies;
        root->availableCopies = temp->availableCopies;

        root->right = deleteBookNode(root->right, temp->bookID);
    }

    return root;
}

// ==================== LIBRARY MANAGEMENT FUNCTIONS ====================

void addBook()
{
    clearScreen();
    displayHeader("ADD NEW BOOK");

    Book *newBook = new Book();

    cout << "\tEnter Book ID: ";
    cin >> newBook->bookID;

    // Check if book already exists
    if (searchBook(bookRoot, newBook->bookID) != nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Book ID already exists!" << COLOR_RESET << "\n";
        delete newBook;
        waitForEnter();
        return;
    }

    cin.ignore();
    cout << "\tEnter Book Title: ";
    getline(cin, newBook->title);

    cout << "\tEnter Author Name: ";
    getline(cin, newBook->author);

    cout << "\tEnter Publisher: ";
    getline(cin, newBook->publisher);

    cout << "\tEnter ISBN: ";
    getline(cin, newBook->isbn);

    cout << "\tEnter Category (Fiction/Technical/Reference/Other): ";
    getline(cin, newBook->category);

    cout << "\tEnter Edition: ";
    cin >> newBook->edition;

    cout << "\tEnter Publication Year: ";
    cin >> newBook->publicationYear;

    cout << "\tEnter Price: ₹";
    cin >> newBook->price;

    cout << "\tEnter Total Copies: ";
    cin >> newBook->totalCopies;

    newBook->availableCopies = newBook->totalCopies;

    cin.ignore();
    cout << "\tEnter Rack Number: ";
    getline(cin, newBook->rackNumber);

    // Insert into Binary Search Tree
    bookRoot = insertBook(bookRoot, newBook);

    cout << "\n\t" << COLOR_GREEN << "✓ Book added successfully!" << COLOR_RESET << "\n";
    cout << "\tBook ID: " << newBook->bookID << "\n";
    cout << "\tAvailable Copies: " << newBook->availableCopies << "\n";

    waitForEnter();
}

void searchBookByID()
{
    clearScreen();
    displayHeader("SEARCH BOOK");

    int bookID;
    cout << "\tEnter Book ID: ";
    cin >> bookID;

    Book *book = searchBook(bookRoot, bookID);

    if (book == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Book not found!" << COLOR_RESET << "\n";
    }
    else
    {
        cout << "\n\t" << string(88, '=') << "\n";
        cout << "\t" << COLOR_CYAN << "BOOK INFORMATION" << COLOR_RESET << "\n";
        cout << "\t" << string(88, '=') << "\n\n";

        cout << "\tBook ID          : " << book->bookID << "\n";
        cout << "\tTitle            : " << book->title << "\n";
        cout << "\tAuthor           : " << book->author << "\n";
        cout << "\tPublisher        : " << book->publisher << "\n";
        cout << "\tISBN             : " << book->isbn << "\n";
        cout << "\tCategory         : " << book->category << "\n";
        cout << "\tEdition          : " << book->edition << "\n";
        cout << "\tPublication Year : " << book->publicationYear << "\n";
        cout << "\tPrice            : ₹" << fixed << setprecision(2) << book->price << "\n";
        cout << "\tTotal Copies     : " << book->totalCopies << "\n";
        cout << "\tAvailable Copies : " << (book->availableCopies > 0 ? COLOR_GREEN : COLOR_RED) << book->availableCopies << COLOR_RESET << "\n";
        cout << "\tRack Number      : " << book->rackNumber << "\n";

        cout << "\t" << string(88, '=') << "\n";
    }

    waitForEnter();
}

void displayAllBooks()
{
    clearScreen();
    displayHeader("ALL BOOKS IN LIBRARY");

    if (bookRoot == nullptr)
    {
        cout << "\t" << COLOR_YELLOW << "No books in library!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\t" << string(110, '=') << "\n";
    cout << "\t" << left << setw(8) << "ID"
         << setw(35) << "Title"
         << setw(25) << "Author"
         << setw(12) << "ISBN"
         << setw(10) << "Total"
         << setw(10) << "Available" << "\n";
    cout << "\t" << string(110, '=') << "\n";

    displayBooksInorder(bookRoot);

    cout << "\t" << string(110, '=') << "\n";
    cout << "\tTotal Books: " << countBooks(bookRoot) << "\n";

    waitForEnter();
}

void issueBook()
{
    clearScreen();
    displayHeader("ISSUE BOOK");

    int studentID, bookID;

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

    // Count books already issued to this student
    IssueRecord *temp = issueHead;
    int issuedCount = 0;

    while (temp != nullptr)
    {
        if (temp->studentID == studentID && !temp->isReturned)
        {
            issuedCount++;
        }
        temp = temp->next;
    }

    if (issuedCount >= MAX_BOOKS_PER_STUDENT)
    {
        cout << "\t" << COLOR_RED << "✗ Maximum book limit reached! (" << MAX_BOOKS_PER_STUDENT << " books)"
             << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    cout << "\tEnter Book ID: ";
    cin >> bookID;

    Book *book = searchBook(bookRoot, bookID);

    if (book == nullptr)
    {
        cout << "\n\t" << COLOR_RED << "✗ Book not found!" << COLOR_RESET << "\n";
        waitForEnter();
        return;
    }

    if (book->availableCopies <= 0)
    {
        cout << "\n\t" << COLOR_RED << "✗ Book not available!" << COLOR_RESET << "\n";
        cout << "\tAll copies are currently issued.\n";
        waitForEnter();
        return;
    }

    // Create new issue record
    IssueRecord *newIssue = new IssueRecord();

    newIssue->issueID = issueCounter++;
    newIssue->studentID = studentID;
    newIssue->bookID = bookID;
    newIssue->issueDate = getCurrentDate();
    newIssue->dueDate = getFutureDate(MAX_ISSUE_DAYS);
    newIssue->isReturned = false;
    newIssue->isRenewed = false;
    newIssue->renewCount = 0;
    newIssue->fine = 0.0f;

    cin.ignore();
    cout << "\n\tIssued by (Librarian Name): ";
    getline(cin, newIssue->issuedBy);

    // Insert into linked list
    newIssue->next = issueHead;
    issueHead = newIssue;

    // Decrease available copies
    book->availableCopies--;

    cout << "\n\t" << COLOR_GREEN << "✓ Book issued successfully!" << COLOR_RESET << "\n";
    cout << "\n\t" << string(70, '-') << "\n";
    cout << "\tIssue ID    : " << newIssue->issueID << "\n";
    cout << "\tStudent     : " << student->name << "\n";
    cout << "\tBook        : " << book->title << "\n";
    cout << "\tIssue Date  : " << newIssue->issueDate << "\n";
    cout << "\tDue Date    : " << COLOR_YELLOW << newIssue->dueDate << COLOR_RESET << "\n";
    cout << "\t" << string(70, '-') << "\n";
    cout << "\n\t" << COLOR_CYAN << "Books issued: " << (issuedCount + 1) << "/" << MAX_BOOKS_PER_STUDENT
         << COLOR_RESET << "\n";

    waitForEnter();
}

void returnBook()
{
    clearScreen();
    displayHeader("RETURN BOOK");

    int issueID;
    cout << "\tEnter Issue ID: ";
    cin >> issueID;

    IssueRecord *issue = issueHead;
    bool found = false;

    while (issue != nullptr)
    {
        if (issue->issueID == issueID && !issue->isReturned)
        {
            found = true;

            Student *student = searchInHash(issue->studentID);
            Book *book = searchBook(bookRoot, issue->bookID);

            cout << "\n\t" << string(70, '=') << "\n";
            cout << "\tStudent     : " << (student ? student->name : "Unknown") << "\n";
            cout << "\tBook        : " << (book ? book->title : "Unknown") << "\n";
            cout << "\tIssue Date  : " << issue->issueDate << "\n";
            cout << "\tDue Date    : " << issue->dueDate << "\n";
            cout << "\t" << string(70, '=') << "\n\n";

            issue->returnDate = getCurrentDate();
            issue->isReturned = true;

            cin.ignore();
            cout << "\tReturned to (Librarian Name): ";
            getline(cin, issue->returnedTo);

            // Fine calculation placeholder
            issue->fine = 0.0f;

            // Increase available copies
            if (book != nullptr)
            {
                book->availableCopies++;
            }

            cout << "\n\t" << COLOR_GREEN << "✓ Book returned successfully!" << COLOR_RESET << "\n";
            cout << "\tReturn Date: " << issue->returnDate << "\n";
            cout << "\tFine: ₹" << fixed << setprecision(2) << issue->fine << "\n";

            break;
        }
        issue = issue->next;
    }

    if (!found)
    {
        cout << "\n\t" << COLOR_RED << "✗ Issue record not found or book already returned!"
             << COLOR_RESET << "\n";
    }

    waitForEnter();
}

void viewIssuedBooks()
{
    clearScreen();
    displayHeader("CURRENTLY ISSUED BOOKS");

    cout << "\t" << string(125, '=') << "\n";
    cout << "\t" << left << setw(10) << "Issue ID"
         << setw(10) << "Stud ID"
         << setw(25) << "Student Name"
         << setw(35) << "Book Title"
         << setw(15) << "Issue Date"
         << setw(15) << "Due Date" << "\n";
    cout << "\t" << string(125, '=') << "\n";

    IssueRecord *current = issueHead;
    int count = 0;

    while (current != nullptr)
    {
        if (!current->isReturned)
        {
            Student *student = searchInHash(current->studentID);
            Book *book = searchBook(bookRoot, current->bookID);

            cout << "\t" << left << setw(10) << current->issueID
                 << setw(10) << current->studentID
                 << setw(25) << (student ? student->name : "Unknown")
                 << setw(35) << (book ? book->title : "Unknown")
                 << setw(15) << current->issueDate
                 << setw(15) << current->dueDate << "\n";

            count++;
        }
        current = current->next;
    }

    if (count == 0)
    {
        cout << "\n\t" << COLOR_YELLOW << "No books currently issued!" << COLOR_RESET << "\n";
    }
    else
    {
        cout << "\t" << string(125, '=') << "\n";
        cout << "\tTotal Issued Books: " << count << "\n";
    }

    waitForEnter();
}

void libraryManagementMenu()
{
    int choice;

    while (true)
    {
        clearScreen();
        cout << "\n\n\t" << COLOR_CYAN;
        cout << "╔════════════════════════════════════════════════════════════════╗\n";
        cout << "║                 LIBRARY MANAGEMENT MODULE                      ║\n";
        cout << "╠════════════════════════════════════════════════════════════════╣\n";
        cout << "║                                                                ║\n";
        cout << "║    1. Add New Book                                             ║\n";
        cout << "║    2. Search Book                                              ║\n";
        cout << "║    3. Display All Books                                        ║\n";
        cout << "║    4. Issue Book                                               ║\n";
        cout << "║    5. Return Book                                              ║\n";
        cout << "║    6. View Issued Books                                        ║\n";
        cout << "║    7. View Student's Issued Books                              ║\n";
        cout << "║    8. Calculate Fine                                           ║\n";
        cout << "║    9. Delete Book                                              ║\n";
        cout << "║    0. Back to Main Menu                                        ║\n";
        cout << "║                                                                ║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n";
        cout << COLOR_RESET;

        cout << "\n\tEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            searchBookByID();
            break;
        case 3:
            displayAllBooks();
            break;
        case 4:
            issueBook();
            break;
        case 5:
            returnBook();
            break;
        case 6:
            viewIssuedBooks();
            break;
        case 0:
            return;
        default:
            cout << "\n\t" << COLOR_RED << "Invalid choice!" << COLOR_RESET << "\n";
            waitForEnter();
        }
    }
}
