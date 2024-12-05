import pickle

# Empty data for names and faces
empty_names = []
empty_face_data = []

# Save empty data to the pickle files
with open('data/names.pkl', 'wb') as f:
    pickle.dump(empty_names, f)
    print("Names data has been cleared.")

with open('data/face_data.pkl', 'wb') as f:
    pickle.dump(empty_face_data, f)
    print("Face data has been cleared.")
