import os
import hashlib

# Constants
hostname = "jimmypc"
time = "2024-11-23"
pdf_header = bytes([0x25, 0x50, 0x44, 0x46])  # "%PDF"
mp4_header = bytes([0x00, 0x00, 0x00, 0x20, 0x66, 0x74, 0x79, 0x70])  # "mp4"
mp4_non_garbage = bytes([0x66, 0x74, 0x79, 0x70])
encrypted_file_path = os.path.expanduser("ctf-writeups/osusec-league-2024/yikes/ctf-files/chal/jimjams-video/super_important_video_final1.mp4.ohio")

# Function to generate the key
def generate_key(random_thing):
    seed = f"{hostname}{random_thing}{time}".encode()
    return hashlib.md5(seed).digest()

# Function to decrypt the file
def xor_decrypt(data, key):
    return bytes(data[i] ^ key[i % len(key)] for i in range(len(data)))

# Function to brute force the key
def brute_force_decryption(file_path, target_header):
    with open(file_path, "rb") as f:
        encrypted_data = f.read()

    # Iterate over possible randomThing values
    for random_thing in range(0, 13333338):  # Adjust range based on expected randomThing range
        key = generate_key(random_thing)
        decrypted_data = xor_decrypt(encrypted_data[:len(target_header)], key)

        # Check if the decrypted header matches the target header
        if decrypted_data[4:] == mp4_non_garbage:
            print(f"Key found! randomThing: {random_thing}")
            full_decrypted_data = xor_decrypt(encrypted_data, key)
            # Save the decrypted file
            output_file_path = file_path.replace(".ohio", ".mp4")
            with open(output_file_path, "wb") as out_file:
                out_file.write(full_decrypted_data)

            print(f"Decrypted file saved to: {output_file_path}")
            return

    print("Key not found within the specified range.")

# Run the brute force
brute_force_decryption(encrypted_file_path, mp4_header)