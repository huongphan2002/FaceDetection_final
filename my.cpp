#include <FL/Fl.H>
#include <FL/fl_message.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // For system()
#include <sstream>  // For stringstream

using namespace std;

// Prototypes
void open_login_window(Fl_Widget*, void*);
void open_register_window(Fl_Widget*, void*);
void process_login(Fl_Widget*, void*);
void process_register(Fl_Widget*, void*);
bool check_credentials(const string&, const string&);
bool save_user(const string&, const string&);
bool is_username_taken(const string&);
string recognize_face(const string&);

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

// Login processing
void process_login(Fl_Widget*, void* data) {
    auto inputs = static_cast<pair<Fl_Input*, Fl_Input*>*>(data);
    string username = inputs->first->value();
    string password = inputs->second->value();
    delete inputs;

    // Assuming the user has taken a photo that we save as "user_photo.jpg"
    string face_recognition_result = recognize_face("user_photo.jpg");

    if (face_recognition_result == "No face detected") {
        fl_message("No face detected. Please try again.");
    } else if (face_recognition_result == "No face recognized") {
        fl_message("Face not recognized. Try again.");
    } else if (check_credentials(username, password)) {
        fl_message("Login successful! Welcome, " + face_recognition_result);
    } else {
        fl_message("Invalid username or password!");
    }
}

// Call the Python script for face recognition
string recognize_face(const string& image_path) {
    stringstream ss;
    ss << "python3 face_recognition.py " << image_path;
    FILE* pipe = popen(ss.str().c_str(), "r");
    if (!pipe) return "Error executing Python script";

    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}

// Register processing
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

// Other functions...
