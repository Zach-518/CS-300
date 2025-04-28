#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


// Structure to hold course information
struct Course {
    string courseID;
    string courseName;
    vector<string>preList;

    //constructor to initialize with a name & ID
    Course(string id, string name){ 
        courseID = id;
        courseName = name;
    }

    //Default constructor
    Course() {
        courseID = "";
        courseName = "";
        preList = {};
    }

    //add a prerequisite to the prerequisite list
    void addPrerequisite(string prereq) {
        preList.push_back(prereq);
    }

    //print the course information
    void printCourseInformation(bool printPrereqs) {
        cout << courseID << ", " << courseName << "\n";
        //check if printPrereqs bool is true
        if (printPrereqs) {
            cout << "Prerequisites: ";
            //loop through each prerequisite in the vector
            for (size_t i = 0; i < preList.size(); i++){
                cout << preList[i];
                //add a comma if there is another prerequisite
                if (i < preList.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "\n";
        }
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor 
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initialize with a node with a specific course
    Node(Course aCourse) :
        Node() {
            course = aCourse;
    }
};

//Class that represents the BST to store the courses
class BST {
private:
    Node* root;
    Node* insertCourse(Node* node, Course course);
    void printSortedCourses(Node* node);
    Node* searchCourse(Node* node, string CourseID);
public:
    //constructor for BST
    BST();
    void insert(Course course);
    void printCourses();
    void printCourseData(string courseID);
};

//default constructor
BST::BST() {
    //set root to equal nullptr
    root = nullptr;
}

//Recursive method to insert a new course into the tree
Node* BST::insertCourse(Node* node, Course course) {
    //Add a new node in the current node equals nullptr
    if (node == nullptr) {
        return new Node(course);
    }
    //compare the course ID to the node
    if (course.courseID < node->course.courseID) {
        //insert it to the left subtree if it is smaller
        node->left = insertCourse(node->left, course);
    }
    else {
        //insert it to the right subtree if it is larger
        node->right = insertCourse(node->right, course);
    }
    return node;
}

//public method to instert a course
void BST::insert(Course course) {
    root = insertCourse(root, course);
}

//Recursive method to search for a course by its ID
Node* BST::searchCourse(Node* node, string courseID) {
    //Return the node if it is found or if nullptr is found
    if (node == nullptr || node->course.courseID == courseID) {
        return node;
    }
    //Search the left subtree if the comparison is met
    if (courseID < node->course.courseID) {
        return searchCourse(node->left, courseID);
    }
    //Search the right subtree otherwise
    else {
        return searchCourse(node->right, courseID);
    }
}

//Public method to print a specific course's information
void BST::printCourseData(string courseID) {
    //Search for the course's node from the course ID
    Node* currNode = searchCourse(root, courseID);
    //Call printCourseInformation if course is found
    if (currNode != nullptr) {
        currNode->course.printCourseInformation(true);
    }
    //Print error if course could not be found
    else {
        cout << "Course could not be found.\n";
    }
}

//Recursive method to print all courses in alphanumerical order
void BST::printSortedCourses(Node* node) {
    if (node != nullptr) {
        //Print the left subtree first
        printSortedCourses(node->left);
        //Print the current node
        node->course.printCourseInformation(false);
        //Print the right subtree
        printSortedCourses(node->right);
    }
}

//Public method to print all the courses
void BST::printCourses() {
    //Print the courses starting from the root
    printSortedCourses(root);
}

//Method to load courses from the file and into the BST
void loadCourses(string filename, BST& bst) {
    //Open the file
    ifstream file(filename);
    string line;
    vector<string> tokens;

    //Print an error if the file is not open
    if (!file.is_open()) {
        cout << "Error in opening file\n";
        return;
    }

    //Read each line from the file
    while (getline(file, line)) {
        //Create a string stream
        istringstream ss(line);
        string token;

        //Get the course ID
        getline(ss, token, ',');
        string courseID = token;

        //Get the course name
        getline(ss, token, ',');
        string courseName = token;

        //Create course object with the name & ID
        Course course(courseID, courseName);

        //Get each of the prerequisites
        while (getline(ss, token, ',')) {
            course.addPrerequisite(token);
        }

        //Insert the course into the BST
        bst.insert(course);

    }

    //close the file after finishing the reading
    file.close();
    cout << "Data Loaded\n";
}

int main()
{
    BST bst;               //Create an empty tree
    bool running = true;   //Initialize the running state to being true
    int selection;
    //set filename to predefined file
    string filename = "CS 300 ABCU_Advising_Program_Input.csv";

    //loop while the running value is true
    while (running) {
        cout << "Menu: \n";
        cout << "1. Load Data\n";
        cout << "2. Print Courses\n";
        cout << "3. Print Specific Course\n";
        cout << "9. Exit\n";
        cout << "What would you like to do?\n";
        //store user input in selection variable
        cin >> selection;
        cin.ignore();

        switch (selection) {
        //Load the filename into the bst if the user selects option 1
        case 1:
            loadCourses(filename, bst);
            break;
        //Print all the courses in the tree if the user selects option 2
        case 2:
            cout << "Here is a sample schedule\n";
            bst.printCourses();
            break;
        //Print a specific course's information
        case 3: {
            string courseID;
            cout << "What course would you like to know about?\n";
            getline(cin, courseID); //Read the course ID
            //Convert the lowercase input to uppercase
            for (char& c : courseID) {
                c = toupper(c);
            }
            bst.printCourseData(courseID);  //Print the course information
            break;
        }
        //Break the loop by setting the running variable to false
        case 9:
            cout << "Thank you for using the course planner!\n";
            running = false;
            break;
        //Default message for any cases not listed
        default:
            cout << selection << " is not a valid option.\n";
            break;
        }
    }
    return 0;
}

