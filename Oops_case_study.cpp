#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Abstract base class
class Person {
protected:
    int id;
    string name;

public:
    static int totalPersons; // Static member to keep track of total persons

    Person(int i, string n) : id(i), name(n) {
        totalPersons++;
        cout << "Person Constructor: " << name << " created." << endl;
    }

    virtual ~Person() {
        totalPersons--;
        cout << "Person Destructor: " << name << " destroyed." << endl;
    }

    // Pure virtual function
    virtual void displayDetails() const = 0;
};

// Initialize static member
int Person::totalPersons = 0;

// Derived class Student
class Student : public Person {
private:
    string course;
    int marks;

public:
    Student(int i, string n, string c, int m) : Person(i, n), course(c) {
        // Exception handling for invalid marks
        if (m < 0 || m > 100) {
            throw invalid_argument("Invalid marks. Marks must be between 0 and 100.");
        }
        marks = m;
        cout << "Student Constructor: Details initialized for student " << name << "." << endl;
    }

    ~Student() {
        cout << "Student Destructor: Cleanup done for student " << name << "." << endl;
    }

    void displayDetails() const override {
        cout << "\n--- Student Details ---" << endl;
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Course: " << course << endl;
        cout << "Marks: " << marks << endl;
    }

    // Friend function to compare marks
    friend void compareMarks(const Student& s1, const Student& s2);
};

// Derived class Faculty
class Faculty : public Person {
private:
    string department;
    double salary;

public:
    Faculty(int i, string n, string d, double s) : Person(i, n), department(d), salary(s) {
        cout << "Faculty Constructor: Details initialized for faculty " << name << "." << endl;
    }

    ~Faculty() {
        cout << "Faculty Destructor: Cleanup done for faculty " << name << "." << endl;
    }

    void displayDetails() const override {
        cout << "\n--- Faculty Details ---" << endl;
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Department: " << department << endl;
        cout << "Salary: Rs." << salary << endl;
    }
};

// Friend function implementation
void compareMarks(const Student& s1, const Student& s2) {
    cout << "\n--- Comparing Marks ---" << endl;
    if (s1.marks > s2.marks) {
        cout << s1.name << " scored higher than " << s2.name << " (" << s1.marks << " vs " << s2.marks << ")." << endl;
    } else if (s1.marks < s2.marks) {
        cout << s2.name << " scored higher than " << s1.name << " (" << s2.marks << " vs " << s1.marks << ")." << endl;
    } else {
        cout << s1.name << " and " << s2.name << " scored the same (" << s1.marks << ")." << endl;
    }
}

// Function to read students from CSV
void loadStudentsFromCSV(const string& filename, vector<Person*>& people, vector<Student*>& studentsList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line); // Skip header line
    
    while (getline(file, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        string idStr, name, course, marksStr;
        
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, course, ',');
        getline(ss, marksStr, ',');
        
        try {
            int id = stoi(idStr);
            int marks = stoi(marksStr);
            
            // This implicitly calls exception handling logic inside Student Constructor
            Student* newStudent = new Student(id, name, course, marks);
            people.push_back(newStudent);
            studentsList.push_back(newStudent);
            
        } catch (const invalid_argument& e) {
            cout << "\nException Caught while processing row: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\nData format error on row: " << name << endl;
        }
    }
}

// Function to read faculty from CSV
void loadFacultyFromCSV(const string& filename, vector<Person*>& people) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line); // Skip header line
    
    while (getline(file, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        string idStr, name, dept, salaryStr;
        
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, dept, ',');
        getline(ss, salaryStr, ',');
        
        try {
            int id = stoi(idStr);
            double salary = stod(salaryStr);
            
            people.push_back(new Faculty(id, name, dept, salary));
            
        } catch (const exception& e) {
            cout << "\nData format error on row: " << name << endl;
        }
    }
}

int main() {
    // Standard C++ file output
    ofstream outFile("Oops_Output.doc");
    
    // Professional redirection to capture all console activity
    stringstream ss;
    streambuf* oldCout = cout.rdbuf(ss.rdbuf());

    cout << "==========================================================" << endl;
    cout << "   UNIVERSITY MEMBER MANAGEMENT SYSTEM - CASE STUDY" << endl;
    cout << "==========================================================" << endl << endl;
    cout << "--- EXECUTION OUTPUT ---" << endl << endl;

    cout << "Total persons initially: " << Person::totalPersons << endl << endl;

    vector<Person*> universityMembers;
    vector<Student*> validStudents; 

    cout << "--- Loading Students Data from students.csv ---" << endl;
    loadStudentsFromCSV("students.csv", universityMembers, validStudents);

    cout << "\n--- Loading Faculty Data from faculty.csv ---" << endl;
    loadFacultyFromCSV("faculty.csv", universityMembers);

    cout << "\nTotal persons loaded from files: " << Person::totalPersons << endl;

    for(Person* p : universityMembers) {
        p->displayDetails();
    }

    if (validStudents.size() >= 2) {
        compareMarks(*validStudents[0], *validStudents[1]);
    }

    cout << "\nDeleting objects and cleaning up memory..." << endl;
    for(Person* p : universityMembers) {
        delete p;
    }
    
    cout << "\nTotal persons finally: " << Person::totalPersons << endl;

    // Output results to BOTH console and file
    cout.rdbuf(oldCout);
    cout << ss.str();
    outFile << ss.str();
    outFile.close();

    cout << "\n[DONE] Professional output saved to Oops_Output.doc" << endl;

    return 0;
}
