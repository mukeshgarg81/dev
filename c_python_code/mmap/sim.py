# python_example.py
import ctypes

# Load the shared library
example_lib = ctypes.CDLL('./example.so')

# Define the argument and return types of the functions
example_lib.create_and_map_file.argtypes = [ctypes.c_char_p, ctypes.c_int]
example_lib.create_and_map_file.restype = ctypes.POINTER(ctypes.c_int)
example_lib.unmap_and_close.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]

# Call the create_and_map_file function from the shared library
filename = b"mapped_file.bin"
size = 5
arr_ptr = example_lib.create_and_map_file(filename, size)

# Access and modify the array values through the pointer in Python
for i in range(size):
    arr_ptr[i] *= 2

# Call the unmap_and_close function from the shared library to unmap the file
example_lib.unmap_and_close(arr_ptr, size)
# Now, let's access the modified data using the C code
# Note: You can create another C function similar to print_array to access the data
# and call it from the shared library in a similar manner as shown earlier.


