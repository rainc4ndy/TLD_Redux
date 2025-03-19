import os
from PIL import Image

# Get the current working directory
working_directory = os.getcwd()

# Resize all PNG files in the working directory
for filename in os.listdir(working_directory):
    if filename.endswith('.png'):
        img_path = os.path.join(working_directory, filename)
        img = Image.open(img_path)

        # Ensure the image is 512x512 before resizing
        if img.size == (512, 512):
            img_resized = img.resize((128, 128), Image.Resampling.LANCZOS)

            # Save the resized image, overwriting the original file
            img_resized.save(img_path)
