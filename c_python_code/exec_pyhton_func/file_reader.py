def read_file(file_path):
    with open(file_path, 'r') as file:
        code = file.read()
    return code

if __name__ == "__main__":
    code = read_file("script_to_execute.py")
    
    # Function to execute
    function_name = "calculate_sum"
    
    # Create a dictionary for local and global variables
    global_vars = {}
    local_vars = {}
    
    # Execute the code
    exec(code, global_vars, local_vars)
    
    # Get the function from local_vars and execute it
    if function_name in local_vars:
        function = local_vars[function_name]
        result = function(10, 20)
        print("Result:", result)
    else:
        print("Function '{}' not found in the file.".format(function_name))






