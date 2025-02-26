#!/bin/bash

# Array of argument values for the second argument (before -size)
arg_values=(1 2 3 4 5 8 16 24)

# Folder to delete before each run
folder_to_delete="./2024-Datasets/Dataset-A/old_photo_PAR_B"

# Function to delete the folder
delete_folder() {
    if [ -d "$folder_to_delete" ]; then
        echo "Deleting folder: $folder_to_delete"
        rm -rf "$folder_to_delete"
    fi
}

 Run for DatasetA
echo "Running for DatasetA:"
for value in "${arg_values[@]}"; do
    echo "Deleting folder: $folder_to_delete"
    rm -rf "$folder_to_delete"
    echo "Running: ./old-photo-parallel-B ./2024-Datasets/Dataset-A $value -name"
    ./old-photo-parallel-B ./2024-Datasets/Dataset-A $value -name
done
folder_to_delete="./2024-Datasets/Dataset-B/old_photo_PAR_B"
# Run for DatasetB
echo "Running for DatasetB:"
for value in "${arg_values[@]}"; do
    echo "Deleting folder: $folder_to_delete"
    rm -rf "$folder_to_delete"
    echo "Running: ./old-photo-parallel-B ./2024-Datasets/Dataset-B $value -name"
    ./old-photo-parallel-B ./2024-Datasets/Dataset-B $value -name
done
