def hash_function(input_str: str) -> str:

    var_10 = 0x539  # this hex is 1337 in decimal, of course
    output_chars = [''] * 16  
    for i in range(8):
        c_val = ord(input_str[i]) - ord('a')
        
        ecx_2 = ((2 * c_val) + var_10) % 26  
        ecx_4 = (3 * c_val) % 26

        var_10 = ecx_2 - ecx_4
        
        output_chars[i]     = chr(ecx_2 + ord('a'))
        output_chars[i + 8] = chr(ecx_4 + ord('a'))

    return ''.join(output_chars)

if __name__ == "__main__":
    test_input = "allaaaaa"
    hashed = hash_function(test_input)
    print(f"Input:  {test_input}")
    print(f"Output: {hashed}")