#include <FL/Fl.H>
#include <FL/fl_message.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

// Function prototypes
void open_login_window(Fl_Widget*, void*);
void open_register_window(Fl_Widget*, void*);
void process_login(Fl_Widget*, void*);
void process_register(Fl_Widget*, void*);
bool check_credentials(const string&, const string&);
bool save_user(const string&, const string&);
bool is_username_taken(const string&);
bool detect_face();

// Main window
void main_window() {
    Fl_Window* window = new Fl_Window(400, 300, "Login or Register");
    Fl_Box* box = new Fl_Box(0, 50, 400, 50, "Login or Register");
    box->labelfont(FL_BOLD);
    box->labelsize(20);

    Fl_Button* login_btn = new Fl_Button(100, 150, 200, 30, "Login");
    login_btn->callback(open_login_window);

    Fl_Button* register_btn = new Fl_Button(100, 200, 200, 30, "Register New Account");
    register_btn->callback(open_register_window);

    window->end();
    window->show();
}

// Login window
void login_window() {
    Fl_Window* window = new Fl_Window(400, 300, "Login");
    Fl_Input* username_input = new Fl_Input(100, 100, 200, 30, "Username:");
    Fl_Input* password_input = new Fl_Input(100, 150, 200, 30, "Password:");
    password_input->type(FL_SECRET_INPUT);

    Fl_Button* login_btn = new Fl_Button(150, 200, 100, 30, "Login");
    login_btn->callback(process_login, new pair<Fl_Input*, Fl_Input*>(username_input, password_input));

    window->end();
    window->show();
}

// Register window
void register_window() {
    Fl_Window* window = new Fl_Window(400, 400, "Register New Account");
    Fl_Input* username_input = new Fl_Input(100, 100, 200, 30, "Username:");
    Fl_Input* password_input = new Fl_Input(100, 150, 200, 30, "Password:");
    password_input->type(FL_SECRET_INPUT);

    Fl_Button* register_btn = new Fl_Button(150, 250, 100, 30, "Register");
    register_btn->callback(process_register, new pair<Fl_Input*, Fl_Input*>(username_input, password_input));

    window->end();
    window->show();
}

// Detect face using OpenCV
bool detect_face() {
    cv::VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened()) {
        fl_message("Unable to access the camera!");
        return false;
    }

    cv::Mat frame;
    cv::CascadeClassifier face_cascade;

    // Load the pre-trained face detection model
    if (!face_cascade.load(cv::samples::findFile("haarcascade_frontalface_default.xml"))) {
        fl_message("Error loading face detection model!");
        return false;
    }

    fl_message("Look at the camera. Detecting face...");
    bool face_detected = false;
    for (int i = 0; i < 100; i++) { // Try for several frames
        cap >> frame;
        if (frame.empty()) break;

        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces);

        if (!faces.empty()) {
            face_detected = true;
            break;
        }
    }
    cap.release();

    if (face_detected) {
        fl_message("Face detected!");
    } else {
        fl_message("No face detected. Please try again.");
    }
    return face_detected;
}

// Process login
void process_login(Fl_Widget*, void* data) {
    auto inputs = static_cast<pair<Fl_Input*, Fl_Input*>*>(data);
    string username = inputs->first->value();
    string password = inputs->second->value();
    delete inputs;

    if (username.empty() || password.empty()) {
        fl_message("Username or password cannot be empty!");
        return;
    }

    if (check_credentials(username, password)) {
        if (detect_face()) {
            fl_message("Login successful!");
        } else {
            fl_message("Face authentication failed!");
        }
    } else {
        fl_message("Invalid username or password!");
    }
}

// Process registration
void process_register(Fl_Widget*, void* data) {
    auto inputs = static_cast<pair<Fl_Input*, Fl_Input*>*>(data);
    string username = inputs->first->value();
    string password = inputs->second->value();
    delete inputs;

    if (username.empty() || password.empty()) {
        fl_message("Username or password cannot be empty!");
        return;
    }

    if (is_username_taken(username)) {
        fl_message("Username is already taken!");
        return;
    }

    if (save_user(username, password)) {
        fl_message("Registration successful!");
    } else {
        fl_message("Failed to save user data!");
    }
}

// Check credentials
bool check_credentials(const string& username, const string& password) {
    ifstream file("users.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        size_t delimiter = line.find(',');
        string stored_username = line.substr(0, delimiter);
        string stored_password = line.substr(delimiter + 1);

        if (stored_username == username && stored_password == password) {
            return true;
        }
    }
    return false;
}

// Save user
bool save_user(const string& username, const string& password) {
    ofstream file("users.txt", ios::app);
    if (!file) return false;

    file << username << "," << password << endl;
    return true;
}

// Check if username is taken
bool is_username_taken(const string& username) {
    ifstream file("users.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        size_t delimiter = line.find(',');
        string stored_username = line.substr(0, delimiter);

        if (stored_username == username) {
            return true;
        }
    }
    return false;
}

// Open login window
void open_login_window(Fl_Widget*, void*) {
    login_window();
}

// Open register window
void open_register_window(Fl_Widget*, void*) {
    register_window();
}

// Main function
int main(int argc, char** argv) {
    main_window();
    return Fl::run();
}
