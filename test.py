import subprocess
import shutil
import os

def compile_cpp(filename):
    """Compile the C++ file to an executable."""
    exe_file = filename.replace('.cpp', '.exe')
    print(f"Compiling {filename}...")
    subprocess.run(["g++", filename, "-o", exe_file])
    print(f"Compiled {filename} to {exe_file}")
    return exe_file

# Compile all three C++ files
generator_exe = compile_cpp("generator.cpp")
correct_exe = compile_cpp("correct.cpp")
wrong_exe = compile_cpp("wrong.cpp")

def run_program(exe, input_file=None, output_file="output.txt"):
    """Run the compiled executable with optional input and output redirection."""
    print(f"Running {exe}...")
    with open(output_file, "w") as outfile:
        if input_file:
            with open(input_file, "r") as infile:
                subprocess.run([exe], stdin=infile, stdout=outfile)
        else:
            subprocess.run([exe], stdout=outfile)
    print(f"Completed running {exe}, output saved to {output_file}")

test_case_num = 0
while True:
    print(f"\nStarting test case #{test_case_num}")
    
    # Generate test case by running generator executable directly to output `input.txt`
    run_program(generator_exe, output_file="input.txt")
    print("Generated input.txt for test case.")
    
    # Run both programs on the generated input
    run_program(correct_exe, input_file="input.txt", output_file="correct_output.txt")
    run_program(wrong_exe, input_file="input.txt", output_file="wrong_output.txt")
    
    # Compare outputs
    with open("correct_output.txt", "r") as correct_file, open("wrong_output.txt", "r") as wrong_file:
        correct_output = correct_file.read()
        wrong_output = wrong_file.read()
        
        if correct_output != wrong_output:
            print(f"Discrepancy found in test case #{test_case_num}")
            # Save the test case and outputs for debugging
            shutil.copy("input.txt", f"failing_input_{test_case_num}.txt")
            shutil.copy("correct_output.txt", f"correct_output_{test_case_num}.txt")
            shutil.copy("wrong_output.txt", f"wrong_output_{test_case_num}.txt")
            print(f"Saved failing test case #{test_case_num} data for review.")
            test_case_num += 1

            # Optionally break here or continue to find more cases
            break
        else:
            print(f"Test case #{test_case_num} passed.")
            test_case_num += 1
