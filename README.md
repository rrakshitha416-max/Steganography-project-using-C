README
📌 Description
This project implements a Steganography system that hides secret data (such as text messages) inside a BMP image file using the Least Significant Bit (LSB) encoding technique.
It enables secure transmission of confidential information by embedding it into an image in a way that is imperceptible to the human eye.

The system supports both:

Encoding – hiding secret data into an image.
Decoding – extracting the hidden data from the image.
🎯 Objectives
Securely hide secret data (text or files) inside a BMP image using the LSB technique.
Ensure data confidentiality and integrity during transmission.
Keep the hidden data invisible to human eyes while maintaining image quality.
Design both encoding and decoding modules for data hiding and retrieval.
Provide file validation and error checking for reliable operation.
Allow easy extension to support other file formats or encryption methods in the future.


🔐 Encoding Module – Functionalities
Accepts a source image (.bmp) and a secret file (e.g., .txt) as inputs.
Validates the presence and format of both input files.
Checks whether the image has sufficient capacity to store the secret data.
Encodes the following into the image’s LSB bits:
A magic string for authentication.
The size and extension of the secret file.
The actual secret data.
Generates a stego image (.bmp) that looks visually identical to the original image.
Displays appropriate success or error messages at each step.


🔓 Decoding Module – Functionalities
Accepts the stego image as input.
Extracts and verifies the magic string to confirm valid encoding.
Retrieves the secret file’s extension, size, and hidden data.
Recreates the original secret file from the extracted information.
Ensures accurate and lossless recovery of the hidden information.

Footer
©
