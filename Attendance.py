import cv2
import numpy as np
import os
import csv
import time
import pickle
from sklearn.neighbors import KNeighborsClassifier
from datetime import datetime

# Initialize video capture and face detection
video = cv2.VideoCapture(0)
facedetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Load pre-saved dataimport cv2
import pickle
import numpy as np
from sklearn.neighbors import KNeighborsClassifier
import time
from datetime import datetime

# Initialize video capture and face detection
video = cv2.VideoCapture(0)
facedetect = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Load pre-saved data for faces and labels
with open('data/names.pkl', 'rb') as w:
    LABELS = pickle.load(w)
with open('data/face_data.pkl', 'rb') as f:
    FACES = pickle.load(f)

# Train the KNN model
knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(FACES, LABELS)

def face_login():
    while True:
        ret, frame = video.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = facedetect.detectMultiScale(gray, 1.3, 5)

        for (x, y, w, h) in faces:
            crop_img = frame[y:y + h, x:x + w, :]
            resized_img = cv2.resize(crop_img, (50, 50)).flatten().reshape(1, -1)
            output = knn.predict(resized_img)

            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(frame, str(output[0]), (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 255, 255), 2)

            # Perform attendance logging based on face recognition
            ts = time.time()
            date = datetime.fromtimestamp(ts).strftime("%d-%m-%Y")
            timestamp = datetime.fromtimestamp(ts).strftime("%H:%M:%S")
            attendance_file = f"Attendance/Attendance_{date}.csv"
            with open(attendance_file, "a") as csvfile:
                writer = csv.writer(csvfile)
                writer.writerow([output[0], timestamp])

        cv2.imshow("Face Recognition Login", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    video.release()
    cv2.destroyAllWindows()

# Call the face_login function when needed
face_login()
for face, label in faces_and_labels:
    print("Face:", face, "Label:", label)
with open('data/names.pkl', 'rb') as w:
    LABELS = pickle.load(w)
with open('data/face_data.pkl', 'rb') as f:
    FACES = pickle.load(f)

print("Number of faces:", len(FACES))
print("Number of labels:", len(LABELS))

# Ensure FACES and LABELS have the same length
if len(FACES) != len(LABELS):
    raise ValueError("FACES and LABELS must have the same number of samples.")

# Train the KNN model
knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(FACES, LABELS)

# Load background image
imBackground = cv2.imread("bg.png")
if imBackground is None:
    print("Error loading background image.")
    exit()

# Create Attendance directory if it doesn't exist
attendance_dir = "Attendance"
os.makedirs(attendance_dir, exist_ok=True)

while True:
    ret, frame = video.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = facedetect.detectMultiScale(gray, 1.3, 5)

    # Detect and process faces
    for (x, y, w, h) in faces:
        crop_img = frame[y:y + h, x:x + w, :]
        resized_img = cv2.resize(crop_img, (50, 50)).flatten().reshape(1, -1)
        output = knn.predict(resized_img)

        # Generate timestamp for the attendance record
        ts = time.time()
        date = datetime.fromtimestamp(ts).strftime("%d-%m-%Y")
        timestamp = datetime.fromtimestamp(ts).strftime("%H:%M:%S")
        attendance_file = os.path.join(attendance_dir, f"Attendance_{date}.csv")

        # Write attendance data directly
        with open(attendance_file, "a", newline="") as csvfile:
            writer = csv.writer(csvfile)
            # Write column names only if the file is empty
            if csvfile.tell() == 0:
                writer.writerow(['NAME', 'TIME'])
            writer.writerow([output[0], timestamp])

        # Draw face rectangle and text
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.putText(frame, str(output[0]), (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 255, 255), 2)

    # Display the video frame
    cv2.imshow("Attendance System", frame)

    # Quit the program when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release video capture and close windows
video.release()
cv2.destroyAllWindows()

# Load face data
with open("data/face_data.pkl", "rb") as f:
    faces_data = pickle.load(f)
