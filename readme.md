# Code Metrics

**Code Metrics** is a comprehensive tool designed to analyze and provide detailed metrics on C source code files. The tool evaluates various aspects of the code, including function usage, variable types, bracket matching, and potential errors. Additionally, it can integrate with the Gemini API to evaluate the time complexity and space complexity of the code provided by the user.

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

## Installation

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

## Contact

For questions or feedback, please contact [niharikakapoor864@gmail.com](mailto:your.email@example.com).


