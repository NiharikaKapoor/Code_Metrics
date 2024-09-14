
# Code Metrics

**Code Metrics** is a comprehensive tool designed to analyze and provide detailed metrics on C source code files. The tool analyzes C code to count functions, variables, and specific keywords, as well as identify bracket errors. Additionally, it can integrate with the Gemini API to evaluate the time complexity and space complexity of the code provided by the user.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
  - [Clone the Repository](#step-1-clone-the-repository)
  - [Install Dependencies](#step-2-install-dependencies)
  - [Compile the Project](#step-3-compile-the-project)
  - [Set Up API Key](#step-4-set-up-api-key)
  - [Run the Program](#step-5-run-the-program)
  - [Analyze the Output](#step-6-analyze-the-output)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

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
gcc -c ./implementation/complexity.c -o complexity -lcurl
gcc -c ./implementation/metrics.c -o metrics
ar rcs lib.a ./implementation/complexity ./implementation/metrics
gcc -o Code_Metrics main.c -L. lib.a -lcurl
./Code_Metrics your_code_file.c
```


### Step 6: Analyze the Output

The program provides detailed metrics about the input code, including:

- Total number of lines, built-in functions, and variables.
- Information about different types of functions and variable usage.
- Bracket mismatches and their locations.
- Time and space complexity of the analyzed code snippet (using the Gemini API).

## Sample Output

![Results](https://github.com/user-attachments/assets/f716daa9-bb04-40a5-b17a-e471a9ac8900)

## Troubleshooting

- **API Key Not Set**: Make sure your `GEMINI_API_KEY` environment variable is correctly set. Run `echo $GEMINI_API_KEY` to verify.
- **libcurl Not Installed**: Ensure that `libcurl4-openssl-dev` is installed.

## Contributing

If you'd like to contribute, please fork the repository and use a feature branch. Pull requests are welcome!

## License

This project is licensed under the MIT License.

## Contact

For questions or feedback, please contact [niharikakapoor864@gmail.com](mailto:your.email@example.com).

