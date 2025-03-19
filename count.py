import os
import fnmatch

def count_lines_of_code(directory):
    total_lines = 0
    file_types = ('*.cpp', '*.h')  # Add other C++ file types if needed

    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(('.cpp', '.h')):  # Only consider .cpp and .h files
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                    lines = f.readlines()
                    # Count non-empty lines, ignore comments
                    non_empty_lines = [line for line in lines if line.strip() and not line.strip().startswith('//')]
                    total_lines += len(non_empty_lines)

    return total_lines

if __name__ == "__main__":
    folder_path = input("Enter the folder path to count lines of C++ code: ")
    total_lines = count_lines_of_code(folder_path)
    print(f"Total lines of C++ code in '{folder_path}': {total_lines}")
