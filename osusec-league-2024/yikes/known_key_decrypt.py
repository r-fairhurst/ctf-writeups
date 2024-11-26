import os
import hashlib

# Constants
hostname = "osu{loose_lips_sink--oh_wait_wrong_metaphor}"
time = "2024-11-22"
encrypted_file_path = os.path.expanduser("17/super_important_video_final1.mp4.ohio")

# Function to generate the key
def generate_key():
    seed = f"{hostname}1234994{time}".encode()
    return hashlib.md5(seed).digest()

# Function to decrypt the file
def xor_decrypt(data, key):
    return bytes(data[i] ^ key[i % len(key)] for i in range(len(data)))

# Function to brute force the key
def brute_force_decryption(file_path, target_header):
    with open(file_path, "rb") as f:
        encrypted_data = f.read()

    key = generate_key()
    decrypted_data = xor_decrypt(encrypted_data[:len(target_header)], key)

    # Check if the decrypted header matches the target header
    full_decrypted_data = xor_decrypt(encrypted_data, key)

    # Save the decrypted file
    output_file_path = file_path.replace(".ohio", ".mp4")
    with open(output_file_path, "wb") as out_file:
        out_file.write(full_decrypted_data)

    print(f"Decrypted file saved to: {output_file_path}")
    return


# Define the target header
target_header = b"\xff\xd8\xff"

# Run the brute force
brute_force_decryption(encrypted_file_path, target_header)