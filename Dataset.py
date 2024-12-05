import os
import cv2
import pickle
import numpy as np

# Initialize face detection
facedetect = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")
video = cv2.VideoCapture(0)  # Use webcam

if not os.path.exists("data"):
    os.makedirs("data")

print("Enter your name (or type 'done' to finish): ")
while True:
    name = input("Name: ")
    if name.lower() == "done":
        print("Exiting registration...")
        break

    print(f"Collecting faces for {name}...")

    faces_data = []
    collected_count = 0

    while collected_count < 50:  # Collect 50 frames per user
        ret, frame = video.read()
        if not ret:
            print("Warning: Failed to capture frame from the webcam.")
            continue

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = facedetect.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5)

        for (x, y, w, h) in faces:
            crop_img = frame[y:y+h, x:x+w]
            resized_img = cv2.resize(crop_img, (50, 50))
            faces_data.append(resized_img)
            collected_count += 1
            cv2.putText(frame, f"Collecting {collected_count}/50", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

        cv2.imshow("Collecting Faces", frame)
        if cv2.waitKey(1) == ord("q"):  # Press 'q' to stop early
            break

    cv2.destroyAllWindows()

    # Check if any faces were collected
    if len(faces_data) == 0:
        print(f"No faces collected for {name}. Try again.")
        continue

    # Convert and save face data
    faces_data = np.array(faces_data)
    faces_data = faces_data.reshape(faces_data.shape[0], -1)  # Flatten images

    # Save to pickle file
    try:
        with open("data/face_data.pkl", "ab") as f:
            pickle.dump({"name": name, "data": faces_data}, f)
        print(f"Data for {name} saved successfully.")
    except Exception as e:
        print(f"Error saving data for {name}: {e}")

video.release()
cv2.destroyAllWindows()
