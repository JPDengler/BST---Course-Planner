/* Joseph Dengler
CS-300 Southern New Hampshire University */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// Define the Course struct to hold course information
struct Course {
    std::string courseNumber;
    std::string title;
    std::vector<std::string> prerequisites;
};

// Define the Node struct for the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;
};

// Define the CourseBST class for the binary search tree
class CourseBST {
private:
    Node* root;

public:
    CourseBST() : root(nullptr) {}

    // Recursive insertion into the binary search tree
    Node* insert(Node* root, Course course) {
        if (root == nullptr) {
            return new Node{ course, nullptr, nullptr };
        }

        if (course.courseNumber < root->course.courseNumber) {
            root->left = insert(root->left, course);
        }
        else {
            root->right = insert(root->right, course);
        }

        return root;
    }

    // Wrapper function for inserting into the tree
    void insert(Course course) {
        root = insert(root, course);
    }

    Node* getRoot() {
        return root;
    }
};

CourseBST courseBST;

// Function to convert a string to uppercase
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Function to load course data from a file into the binary search tree
void loadCourseDataFromFile(const std::string& filePath) {
    try {
        // Inform the user about loading data from the given file
        std::cout << "Loading course data from file: " << filePath << std::endl;

        // Open the file using the provided file path
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            std::cerr << "File path: " << filePath << std::endl;
            return;
        }

        // Read each line from the file
        std::string line;
        while (std::getline(file, line)) {
            // Use a string stream to extract course information from the line
            std::stringstream ss(line);
            Course course;

            // Extract the course number and title from the line separated by commas
            std::getline(ss, course.courseNumber, ',');
            std::getline(ss, course.title, ',');

            // Extract prerequisites from the remaining part of the line
            std::string prerequisites;
            if (std::getline(ss, prerequisites)) {
                // Use another string stream to split prerequisites separated by commas
                std::stringstream prereqStream(prerequisites);
                std::string prereq;
                while (std::getline(prereqStream, prereq, ',')) {
                    // Add each prerequisite to the prerequisites vector
                    course.prerequisites.push_back(prereq);
                }
            }
            courseBST.insert(course);
        }
        file.close();
        std::cout << "Course data loaded successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error while reading file: " << e.what() << std::endl;
    }
}

// Function to print course prerequisites
void printPrerequisites(const std::vector<std::string>& prerequisites) {
    std::cout << "Prerequisites: ";
    if (prerequisites.empty()) {
        std::cout << "None";
    }
    else {
        for (size_t i = 0; i < prerequisites.size() && i < 2; ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << prerequisites[i];
        }
    }
    std::cout << std::endl;
}

// Function to print the list of courses in alphanumeric order
void printAlphanumericCourseList(Node* root) {
    if (root == nullptr) {
        return;
    }

    printAlphanumericCourseList(root->left);
    std::cout << root->course.courseNumber << ", " << root->course.title << std::endl;
    printAlphanumericCourseList(root->right);
}

// Function to search for a course in the binary search tree
Node* searchCourse(Node* root, const std::string& courseNumber) {
    if (root == nullptr || toUpper(root->course.courseNumber) == toUpper(courseNumber)) {
        return root;
    }

    std::string upperTreeCourseNumber = toUpper(root->course.courseNumber);
    std::string upperInputCourseNumber = toUpper(courseNumber);

    if (upperInputCourseNumber < upperTreeCourseNumber) {
        return searchCourse(root->left, courseNumber);
    }
    else {
        return searchCourse(root->right, courseNumber);
    }
}

// Function to print detailed course information including prerequisites
void printCourseInformation(Node* root, const std::string& courseNumber) {
    std::string upperCourseNumber = courseNumber;
    std::transform(upperCourseNumber.begin(), upperCourseNumber.end(), upperCourseNumber.begin(), ::toupper);
    Node* courseNode = searchCourse(root, upperCourseNumber);

    if (courseNode) {
        std::cout << courseNode->course.courseNumber << ", " << courseNode->course.title << std::endl;
        printPrerequisites(courseNode->course.prerequisites);
    }
    else {
        std::cout << "Course not found." << std::endl;
    }
}

int main() {
    int choice;
    std::string filePath;

    std::cout << "Welcome to the course planner." << std::endl;

    do {
        //Displays user menu
        std::cout << "\nMenu:\n";
        std::cout << "1. Load Data Structure.\n";
        std::cout << "2. Print Course List.\n";
        std::cout << "3. Print Course.\n";
        std::cout << "9. Exit\n";
        std::cout << "What would you like to do? ";
        std::cin >> choice;

        std::string courseNumber;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //Switch case dependent on user selection; user is notified if choice is not valid
        switch (choice) {
        case 1:
            std::cout << "Enter the file path to load data: ";
            std::getline(std::cin, filePath);
            loadCourseDataFromFile(filePath);
            break;
        case 2:
            std::cout << "Here is a sample schedule:\n";
            printAlphanumericCourseList(courseBST.getRoot());
            break;
        case 3:
            std::cout << "What course do you want to know about? (Enter course number) :";
            std::cin >> courseNumber;
            printCourseInformation(courseBST.getRoot(), courseNumber);
            break;
        case 9:
            std::cout << "Thank you for using the course planner! Goodbye!" << std::endl;
            break;
        default:
            std::cout << choice << " is not a valid option." << std::endl;
            break;
        }
    } while (choice != 9);

    return 0;
}
