#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <iomanip>

using namespace std;

class Student {
private:
    int rollNumber;
    string name;
    vector<int> marks;

public:
    Student() : rollNumber(0) {}

    Student(int r, string n, vector<int> m) : rollNumber(r), name(n), marks(m) {}

    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    vector<int> getMarks() const { return marks; }

    int calculateTotal() const {
        return accumulate(marks.begin(), marks.end(), 0);
    }

    double calculatePercentage() const {
        return calculateTotal() / 5.0;
    }

    string toCSV() const {
        stringstream ss;
        ss << rollNumber << "," << name;
        for (int m : marks) {
            ss << "," << m;
        }
        return ss.str();
    }
};

void saveToCSV(const vector<Student>& students) {
    ofstream file("students.csv");
    if (!file.is_open()) {
        cout << "Error opening file for saving!" << endl;
        return;
    }
    for (const auto& s : students) {
        file << s.toCSV() << endl;
    }
    file.close();
    cout << "Data saved successfully to students.csv" << endl;
}

void loadFromCSV(vector<Student>& students) {
    ifstream file("students.csv");
    if (!file.is_open()) return;

    students.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;
        
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 7) {
            int roll = stoi(tokens[0]);
            string name = tokens[1];
            vector<int> marks;
            for (int i = 2; i < 7; ++i) {
                marks.push_back(stoi(tokens[i]));
            }
            students.emplace_back(roll, name, marks);
        }
    }
    file.close();
}

void addStudent(vector<Student>& students) {
    int roll;
    string name;
    vector<int> marks(5);

    cout << "Enter Roll Number: ";
    cin >> roll;
    cin.ignore(); 
    cout << "Enter Name: ";
    getline(cin, name);
    
    cout << "Enter marks for 5 subjects (English Maths Physics Chemistry CS) separated by space: ";
    for (int i = 0; i < 5; ++i) {
        cin >> marks[i];
    }

    students.emplace_back(roll, name, marks);
    saveToCSV(students);
}

void displayStudent(const Student& s) {
    const string subjects[] = {"English", "Maths", "Physics", "Chemistry", "CS"};

    cout << "Roll: " << s.getRollNumber() << " | Name: " << s.getName() << endl;
    cout << "Marks: ";
    
    vector<int> m = s.getMarks();
    for (size_t i = 0; i < m.size(); ++i) {
        cout << subjects[i] << ": " << m[i];
        if (i < m.size() - 1) cout << " | ";
    }
    
    cout << "\nTotal: " << s.calculateTotal() 
         << " | Percentage: " << fixed << setprecision(2) << s.calculatePercentage() << "%" << endl;
    cout << "------------------------------------------------" << endl;
}

void viewAll(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No records found." << endl;
        return;
    }
    cout << "\n--- Student Records ---" << endl;
    for (const auto& s : students) {
        displayStudent(s);
    }
}

void searchStudent(const vector<Student>& students) {
    int roll;
    cout << "Enter Roll Number to search: ";
    cin >> roll;

    bool found = false;
    for (const auto& s : students) {
        if (s.getRollNumber() == roll) {
            cout << "\nRecord Found:" << endl;
            displayStudent(s);
            found = true;
            break;
        }
    }
    if (!found) cout << "Student not found." << endl;
}

int main() {
    vector<Student> students;
    loadFromCSV(students);

    int choice;
    while (true) {
        cout << "\n1. Add Student" << endl;
        cout << "2. View All Students" << endl;
        cout << "3. Search Student" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(students); break;
            case 2: viewAll(students); break;
            case 3: searchStudent(students); break;
            case 4: return 0;
            default: cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}