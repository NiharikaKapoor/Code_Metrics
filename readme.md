# Code Metrics

**Code Metrics** is a comprehensive tool designed to analyze and provide detailed metrics on C source code files.The tool analyzes C code to count functions, variables, and specific keywords, as well as identify bracket errors.Additionally, it can integrate with the Gemini API to evaluate the time complexity and space complexity of the code provided by the user.

## Features

- **Function Analysis**:
  - Counts the number of `printf`, `scanf`, and other functions used.
  - Differentiates between various function types, including integer, float, and void functions.

- **Variable Analysis**:
  - Identifies and counts the use of different variable types such as `int`, `float`, `char`, `double`, `bool`, and `long`.

- **Bracket Matching**:
  - Detects mismatched or missing brackets in the code to help identify potential syntax errors.

- **Comment Handling**:
  - Properly processes single-line (`//`) and multi-line (`/* ... */`) comments to avoid false positives in analysis.

- **Time Complexity Estimation**:
  - Integrates with the Gemini API to estimate the time complexity of the provided code.

- **Space Complexity Estimation**:
- Integrates with the Gemini API to estimate the space complexity of the provided code.


## Installation

Follow these steps to install and run the project:

### Step 1: Clone the Repository

Clone the repository to your local machine:

```sh
git clone https://github.com/yourusername/Code-Metrics.git
cd Code-Metrics
```

### Step 2: Install Dependencies

Install the necessary dependencies:

```sh
sudo apt-get update
sudo apt-get install gcc libcurl4-openssl-dev
```

### Step 3: Compile the Project

Compile the source code using `gcc`:

```sh
gcc -o code_metrics code_metrics.c -lcurl
```

### Step 4: Set Up API Key

Make sure you have your Gemini API key and set it as an environment variable:

```sh
export GEMINI_API_KEY="your_api_key_here"
```

### Step 5: Run the Program

Run the program by providing the C file you want to analyze:

```sh
./code_metrics your_code_file.c
```

Alternatively, you can execute the program without specifying the file name initially, and it will prompt you to enter it:

```sh
./code_metrics
```

### Step 6: Analyze the Output

The program provides detailed metrics about the input code, including:

- Total number of lines, built-in functions, and variables.
- Information about different types of functions and variable usage.
- Bracket mismatches and their locations.
- Time and space complexity of the analyzed code snippet (using the Gemini API).

## Troubleshooting

- **API Key Not Set**: Make sure your `GEMINI_API_KEY` environment variable is correctly set. Run `echo $GEMINI_API_KEY` to verify.
- **libcurl Not Installed**: Ensure that `libcurl4-openssl-dev` is installed.

## Contributing

If you'd like to contribute, please fork the repository and use a feature branch. Pull requests are welcome!

## License

This project is licensed under the MIT License.


<!-- ## Installation

### Prerequisites

- **libcurl**: Ensure that `libcurl` is installed and configured correctly on your system. The library is required for HTTP requests to the Gemini API.

### Windows

1. **Download and Install libcurl**:
   - Download `libcurl` from the [official website](https://curl.se/download.html) or another reliable source.
   - Extract the contents to a known directory (e.g., `C:\libcurl`).

2. **Set Up Environment Variables**:
   - Add the path to `libcurl.dll` (e.g., `C:\libcurl\bin`) to your system's PATH environment variable.

3. **Clone the Repository**:
   ```sh
   git clone https://github.com/yourusername/CodeMetrics.git
   cd CodeMetrics
   ```

4. **Build the Project**:
   - Open the project in Visual Studio or use a makefile if applicable.
   - Ensure that `libcurl` is linked correctly during the build process.

## Usage

1. **Run the Program**:
   - Execute the compiled binary and provide the C source file you wish to analyze.

   ```sh
   CodeMetrics.exe <source_file.c>
   ```

2. **View Metrics**:
   - The tool will output detailed metrics on the console, including the number of functions, variables, and any bracket mismatches.

3. **Time Complexity Analysis**:
   - The tool will optionally connect to the Gemini API to analyze and provide the estimated time complexity of the code.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your enhancements. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
 -->
## Contact

For questions or feedback, please contact [niharikakapoor864@gmail.com](mailto:your.email@example.com).


