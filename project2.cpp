// Gianna Screen - Project 2
// Resources used: https://stackoverflow.com/questions/20444612/binary-tree-implementation-c
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Course Struct
struct Course {
    string C_ID;
    string C_Name;
    vector<string> C_List;
};

void displayCourse(const Course& curr_Course);

// Binary Search Tree Class
class BinaryTree {
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;

        Node() : course(), right(nullptr), left(nullptr) {}
        explicit Node(const Course& course) : course(course), right(nullptr), left(nullptr) {}
    };

    Node* root;

public:
    BinaryTree() : root(nullptr) {}

    void InOrder(Node* node) {
        if (node == nullptr)
            return;

        InOrder(node->left);
        displayCourse(node->course);
        InOrder(node->right);
    }

    void Insert(const Course& course) {
        Node* currentNode = root;
        if (root == nullptr) {
            root = new Node(course);
        } else {
            while (currentNode != nullptr) {
                if (course.C_ID < currentNode->course.C_ID) {
                    if (currentNode->left == nullptr) {
                        currentNode->left = new Node(course);
                        currentNode = nullptr;
                    } else {
                        currentNode = currentNode->left;
                    }
                } else {
                    if (currentNode->right == nullptr) {
                        currentNode->right = new Node(course);
                        currentNode = nullptr;
                    } else {
                        currentNode = currentNode->right;
                    }
                }
            }
        }
    }

    void Remove(const string& C_ID) {
        Node* first = nullptr;
        Node* current = root;

        while (current != nullptr) {
            if (current->course.C_ID == C_ID) {
                if (current->left == nullptr && current->right == nullptr) {
                    if (first == nullptr) {
                        root = nullptr;
                    } else if (first->left == current) {
                        first->left = nullptr;
                    } else {
                        first->right = nullptr;
                    }
                } else if (current->right == nullptr) {
                    if (first == nullptr) {
                        root = current->left;
                    } else if (first->left == current) {
                        first->left = current->left;
                    } else {
                        first->right = current->left;
                    }
                } else if (current->left == nullptr) {
                    if (first == nullptr) {
                        root = current->right;
                    } else if (first->left == current) {
                        first->left = current->right;
                    } else {
                        first->right = current->right;
                    }
                } else {
                    Node* next_S = current->right;
                    while (next_S->left != nullptr) {
                        next_S = next_S->left;
                    }
                    Node* next_D = new Node(next_S->course);
                    Remove(next_S->course.C_ID);
                    current->course = next_D->course;
                }
                return;
            } else if (C_ID < current->course.C_ID) {
                first = current;
                current = current->left;
            } else {
                first = current;
                current = current->right;
            }
        }

        cout << "Invalid Value" << endl;
    }

    Course Search(const string& C_ID) {
        Node* currentNode = root;
        while (currentNode != nullptr) {
            if (currentNode->course.C_ID == C_ID) {
                return currentNode->course;
            } else if (C_ID < currentNode->course.C_ID) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }
        return Course{};
    }
};

vector<string> Split(const string& fwd) {
    const char delimiter = ',';
    vector<string> tokens;
    string temp;

    for (char c : fwd) {
        if (c == delimiter) {
            tokens.push_back(temp);
            temp.clear();
            continue;
        }
        temp += c;
    }

    return tokens;
}

void loadCourse(const string& csvPath, BinaryTree* curr_CList) {
    ifstream readCSV;
    string string_line;
    vector<string> token_String;

    readCSV.open(csvPath);
    if (!readCSV.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }

    while (getline(readCSV, string_line)) {
        Course curr_Course;
        token_String = Split(string_line);

        if (token_String.size() < 2) {
            cout << "Error" << endl;
        } else {
            curr_Course.C_ID = token_String[0];
            curr_Course.C_Name = token_String[1];

            for (size_t i = 2; i < token_String.size(); ++i) {
                curr_Course.C_List.push_back(token_String[i]);
            }

            curr_CList->Insert(curr_Course);
        }
    }

    readCSV.close();
}

void displayCourse(const Course& curr_Course) {
    cout << "Course ID: " << curr_Course.C_ID << endl;
    cout << "Course Name: " << curr_Course.C_Name << endl;
    cout << "Prerequisites: ";
    if (curr_Course.C_List.empty()) {
        cout << "none";
    } else {
        for (size_t i = 0; i < curr_Course.C_List.size(); ++i) {
            cout << curr_Course.C_List[i];
            if (i != curr_Course.C_List.size() - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}

int main(int argument_1, char* argument_2[]) {
    string csvPath, curr_CourseKey;

    // Process command line arguments
    if (argument_1 == 2) {
        csvPath = argument_2[1];
    } else if (argument_1 == 3) {
        csvPath = argument_2[1];
        curr_CourseKey = argument_2[2];
    } else {
        csvPath = "Project2.txt";
    }

    BinaryTree* curr_CList = new BinaryTree();
    Course course;
    bool valid = false;
    int choice = 0;

    while (choice != 9) {
        cout << "Welcome to the Course Planner." << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;

        cout << "What is your choice: ";
        cin >> choice;

        if (cin.fail()) {
            valid = false;
            cout << "Invalid input: Please try again" << endl;
            cin.clear();
        } else {
            valid = true;
        }

        if (valid) {
            switch (choice) {
                case 1:
                    loadCourse(csvPath, curr_CList);
                    break;
                case 2:
                    curr_CList->InOrder(curr_CList->root);
                    break;
                case 3:
                    cout << "What course would you like to search: ";
                    cin >> curr_CourseKey;
                    course = curr_CList->Search(curr_CourseKey);
                    if (course.C_ID.empty()) {
                        cout << "Course not found." << endl;
                    } else {
                        displayCourse(course);
                    }
                    break;
                case 9:
                    cout << "Goodbye" << endl;
                    break;
                default:
                    cout << "Invalid input: Please try again" << endl;
                    break;
            }
        }
        cout << endl;
    }

    return 0;
}
