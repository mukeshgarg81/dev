# python_example.py
import ctypes

# Load the shared library
example_lib = ctypes.CDLL('./example.so')

# Define the argument and return types of the functions
example_lib.allocate_memory.argtypes = [ctypes.c_int]
example_lib.allocate_memory.restype = ctypes.POINTER(ctypes.c_int)
example_lib.print_array.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]

# Call the allocate_memory function from the shared library
size = 5
arr_ptr = example_lib.allocate_memory(size)

# Access and modify the array values through the pointer
for i in range(size):
    arr_ptr[i] *= 2

# Call the print_array function from the shared library to print the modified data
example_lib.print_array(arr_ptr, size)

