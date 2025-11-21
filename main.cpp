#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Book {
public:
    int bookID;
    string title;
    string author;
    string publisher;
    int totalCopies;
    int availableCopies;

    Book() {
        bookID = 0;
        totalCopies = 0;
        availableCopies = 0;
    }

    void inputBook() {
        cout << "Enter Book ID: ";
        cin >> bookID;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        cout << "Enter Publisher: ";
        getline(cin, publisher);

        cout << "Enter Total Copies: ";
        cin >> totalCopies;

        availableCopies = totalCopies;
    }

    bool isAvailable() {
        return availableCopies > 0;
    }

    void issueCopy() {
        if (availableCopies > 0)
            availableCopies--;
    }

    void returnCopy() {
        availableCopies++;
    }
};

class Member {
public:
    int memberID;
    string name;
    string membershipType;
    int issuedCount;

    Member() {
        memberID = 0;
        issuedCount = 0;
    }

    void inputMember() {
        cout << "Enter Member ID: ";
        cin >> memberID;
        cin.ignore();

        cout << "Enter Member Name: ";
        getline(cin, name);

        cout << "Enter Membership Type: ";
        getline(cin, membershipType);

        issuedCount = 0;
    }

    bool canIssue() {
        return issuedCount < 5;
    }

    void increaseIssued() {
        issuedCount++;
    }

    void decreaseIssued() {
        issuedCount--;
    }
};

class Transaction {
public:
    int recordID;
    int bookID;
    int memberID;
    string date;

    Transaction() {
        recordID = 0;
        bookID = 0;
        memberID = 0;
    }

    void setDate(string d) {
        date = d;
    }
};

class IssueRecord : public Transaction {
public:
    string dueDate;

    void createRecord(int bID, int mID) {
        static int counter = 1000;

        recordID = counter++;
        bookID = bID;
        memberID = mID;

        cout << "Enter Issue Date: ";
        cin >> date;

        cout << "Enter Due Date: ";
        cin >> dueDate;
    }
};

class ReturnRecord : public Transaction {
public:
    string returnDate;
    float fine;

    void createReturn(int bID, int mID) {
        static int counter = 5000;

        recordID = counter++;
        bookID = bID;
        memberID = mID;

        cout << "Enter Return Date: ";
        cin >> returnDate;
    }
};

class Library {
public:
    Book books[100];
    Member members[100];
    IssueRecord issues[100];
    ReturnRecord returnsList[100];

    int bookCount = 0;
    int memberCount = 0;
    int issueCount = 0;
    int returnCount = 0;

    void addBook() {
        Book b;
        b.inputBook();
        books[bookCount++] = b;

        ofstream fout("books.txt", ios::app);
        fout << b.bookID << "|" << b.title << "|" << b.author << "|"
             << b.publisher << "|" << b.totalCopies << "|" << b.availableCopies << "\n";
        fout.close();

        cout << "Book saved\n";
    }

    void addMember() {
        Member m;
        m.inputMember();
        members[memberCount++] = m;

        ofstream fout("members.txt", ios::app);
        fout << m.memberID << "|" << m.name << "|" << m.membershipType
             << "|" << m.issuedCount << "\n";
        fout.close();

        cout << "Member saved\n";
    }

    void issueBook() {
        int bID, mID;
        cout << "Enter Book ID to Issue: ";
        cin >> bID;
        cout << "Enter Member ID: ";
        cin >> mID;

        int bi = -1, mi = -1;

        for (int i = 0; i < bookCount; i++)
            if (books[i].bookID == bID)
                bi = i;

        for (int i = 0; i < memberCount; i++)
            if (members[i].memberID == mID)
                mi = i;

        if (bi == -1 || mi == -1) {
            cout << "Book or Member not found.\n";
            return;
        }

        if (!books[bi].isAvailable()) {
            cout << "Book not available.\n";
            return;
        }

        if (!members[mi].canIssue()) {
            cout << "Member reached issue limit.\n";
            return;
        }

        books[bi].issueCopy();
        members[mi].increaseIssued();

        IssueRecord ir;
        ir.createRecord(bID, mID);
        issues[issueCount++] = ir;

        ofstream fout("issue.txt", ios::app);
        fout << ir.recordID << "|" << ir.bookID << "|" << ir.memberID
             << "|" << ir.date << "|" << ir.dueDate << "\n";
        fout.close();

        cout << "Book issued\n";
    }

    void returnBook() {
        int bID, mID;
        cout << "Enter Book ID to Return: ";
        cin >> bID;
        cout << "Enter Member ID: ";
        cin >> mID;

        int bi = -1, mi = -1;

        for (int i = 0; i < bookCount; i++)
            if (books[i].bookID == bID)
                bi = i;

        for (int i = 0; i < memberCount; i++)
            if (members[i].memberID == mID)
                mi = i;

        if (bi == -1 || mi == -1) {
            cout << "Book or Member not found.\n";
            return;
        }

        books[bi].returnCopy();
        members[mi].decreaseIssued();

        ReturnRecord rr;
        rr.createReturn(bID, mID);
        returnsList[returnCount++] = rr;

        ofstream fout("return.txt", ios::app);
        fout << rr.recordID << "|" << rr.bookID << "|"
             << rr.memberID << "|" << rr.returnDate << "|\n";
        fout.close();

        cout << "Book returned & saved!\n";
    }

    void showBooks() {
        cout << "\n=== BOOK LIST ===\n";
        for (int i = 0; i < bookCount; i++) {
            cout << books[i].bookID << " - " << books[i].title
                 << " (" << books[i].availableCopies << "/"
                 << books[i].totalCopies << " available)\n";
        }
    }

    void showMembers() {
        cout << "\n=== MEMBER LIST ===\n";
        for (int i = 0; i < memberCount; i++) {
            cout << members[i].memberID << " - " << members[i].name
                 << " | Issued: " << members[i].issuedCount << "\n";
        }
    }

    void showTransactions() {
        cout << "\n=== ISSUE RECORDS ===\n";
        for (int i = 0; i < issueCount; i++) {
            cout << issues[i].recordID << " | Book: " << issues[i].bookID
                 << " | Member: " << issues[i].memberID
                 << " | Date: " << issues[i].date
                 << " | Due: " << issues[i].dueDate << "\n";
        }

        cout << "\n=== RETURN RECORDS ===\n";
        for (int i = 0; i < returnCount; i++) {
            cout << returnsList[i].recordID << " | Book: " << returnsList[i].bookID
                 << " | Member: " << returnsList[i].memberID
                 << " | Return: " << returnsList[i].returnDate << "\n";
        }
    }
};

int main() {
    Library lib;
    int choice;

    do {
        cout << "\n==== LIBRARY MANAGEMENT SYSTEM ====\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Show All Books\n";
        cout << "6. Show All Members\n";
        cout << "7. Show All Transactions\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addMember(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.showBooks(); break;
            case 6: lib.showMembers(); break;
            case 7: lib.showTransactions(); break;
            case 0: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
