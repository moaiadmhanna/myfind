# myfind

A command-line utility to search for files within a given directory, supporting recursive search and case-insensitive matching. The program forks child processes for each file search to run in parallel and outputs the results in a specific format.

## Usage

### Arguments:

- `searchpath`:  
  The directory to search in. Can be an absolute or relative path.
  
- `filename`:  
  The name(s) of the file(s) to search for. You can specify multiple filenames as arguments. No support for subpaths, wildcards, or complex expressions—only simple filenames.

### Options:

- `-R`:  
  Enables **recursive mode**. This will search not only in the `searchpath` directory but also in all its subdirectories.

- `-i`:  
  Enables **case-insensitive search**, making filename matching case-insensitive.

### Example:

In this example, `myfind` will search for the files `test.txt`, `test.doc`, and `test` in the current working directory (`./`) without recursion, as the `-R` option is not provided.

## Output Format:

For each found file, `myfind` will print the following information:


Where:
- `<pid>`: is the process ID of the child process that found the file.
- `<filename>`: is the filename that was passed as an argument to the program.
- `<complete-path-to-found-file>`: is the absolute path to the found file.

## Implementation Details:

- The program spawns a **child process** for each filename passed as an argument using `fork()`. Each child process searches for its respective file.
  
- When a file is found, the **process ID (PID)** of the child process, along with the **filename** and the **complete absolute path**, are printed to `stdout`.

- The program uses **`getopt()`** to handle argument parsing. Both the `-R` and `-i` options can be placed anywhere in the argument list.

- **Parent process** must handle the termination of child processes to avoid zombie processes. Make sure to use proper signal handling or wait for the child processes to exit.

## Notes:

- **Parallelism**: The search for each file is handled in parallel by forking child processes. This approach allows multiple files to be searched simultaneously.

- **Synchronization**: Take care of synchronization when printing to `stdout` from different child processes to avoid mixed-up outputs.

- **Code Quality**: Ensure the code is properly indented, structured, and commented. Proper C-programming practices should be followed.

## Hints:

- Use **file system functions** available in C or the **C++17 File System library** for interacting with directories and files. For more information, check resources like this [filesystem tutorial](https://www.geeksforgeeks.org/filesystem-library-in-cpp-17/).

- Ensure proper **output synchronization** when child processes print results to `stdout`.

- Make sure that the parent process correctly waits for the termination of all child processes to avoid creating **zombie processes**.
