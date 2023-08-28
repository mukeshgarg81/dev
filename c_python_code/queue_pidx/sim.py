# python_example.py
import ctypes
import threading
import time

# Load the shared library
example_lib = ctypes.CDLL('./example.so')

# Define the argument and return types of the functions
example_lib.create_queue.argtypes = [ctypes.c_char_p]
example_lib.create_queue.restype = ctypes.POINTER(ctypes.c_int)
example_lib.enqueue.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int]
example_lib.dequeue.argtypes = [ctypes.POINTER(ctypes.c_int)]
example_lib.dequeue.restype = ctypes.c_int

# Call the create_queue function from the shared library
filename = b"queue_file.bin"
queue_ptr = example_lib.create_queue(filename)

# Producer function
def producer():
    for i in range(10):
        example_lib.enqueue(queue_ptr, i)
        print(f"Produced: {i}")
        time.sleep(1)


# Consumer function
def consumer():
    for _ in range(10):
        value = example_lib.dequeue(queue_ptr)
        if value != -1:
            print(f"Consumed: {value}")
        time.sleep(2)

# Create threads for producer and consumer
producer_thread = threading.Thread(target=producer)
consumer_thread = threading.Thread(target=consumer)

# Start threads
producer_thread.start()
consumer_thread.start()

# Wait for threads to finish
producer_thread.join()
consumer_thread.join()


