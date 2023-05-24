import os


def split_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
        file_length = len(content)
        split_index = file_length // 2

        # Extracting the first half of the content
        first_half = content[:split_index]

        if 'neuronas' in file_path:
            # Check if there are double line breaks in the first half content
            double_break_index = first_half.rfind('\n\n')
            if double_break_index != -1:
                # Extend the first half until the next double line break
                next_double_break_index = content.find('\n\n', double_break_index + 2)
                if next_double_break_index != -1:
                    first_half = content[:next_double_break_index]
                else:
                    first_half = content

        elif 'solapamiento' in file_path:
            # Check if there is a single line break in the first half content
            single_break_index = first_half.rfind('\n')
            if single_break_index != -1:
                # Extend the first half until the next single line break
                next_single_break_index = content.find('\n', single_break_index + 1)
                if next_single_break_index != -1:
                    first_half = content[:next_single_break_index]
                else:
                    first_half = content

        # Remove any trailing line breaks at the end of the first half
        first_half = first_half.rstrip('\n')

        # Overwriting the original file with the modified first half content
        with open(file_path, 'w') as new_file:
            new_file.write(first_half)

        print(f"Split file: {file_path}")
        print(f"Overwritten with modified first half content.")


def iterate_files(folder_path):
    for root, dirs, files in os.walk(folder_path):
        for file_name in files:
            if file_name.endswith('.dat'):  # Process only text files
                file_path = os.path.join(root, file_name)
                split_file(file_path)


# Provide the folder path where you want to perform the splitting
folder_path = 'Datos generados copia'

# Call the function to iterate over files and split them
iterate_files(folder_path)
