# Project B - CSV Mini Database and Query Engine

## Overview

This project is a course assignment for CS351. It implements a small CSV-backed database and query engine that can load data from CSV files and execute SQL-like queries.

The goal is to demonstrate understanding of data storage, parsing, query processing, and result generation in a simplified database system.

## Features

- Load and parse CSV files as tables
- Execute SQL-like queries such as `SELECT`, `WHERE`, `JOIN`, `GROUP BY`, and aggregation functions
- Support for basic filtering and projection
- Return query results in a readable table or CSV output format

> If your implementation differs, update this section with the exact features you completed.

## Project Structure

Currently, this repository only contains the README. Add your source files under the project root, for example:

- `main.py` or `app.py` – application entry point
- `database/` – query engine and CSV reader modules
- `tests/` – unit and integration tests
- `data/` – sample CSV files used for testing and demonstration

## Getting Started

### Prerequisites

- Python 3.x (or the required language/runtime for your project)
- Any additional libraries listed in `requirements.txt` if used

### Installation

1. Clone the repository:

   ```bash
   git clone <repository-url>
   cd 11402_CS351_ProjectB
   ```

2. Install dependencies (if applicable):

   ```bash
   pip install -r requirements.txt
   ```

## Usage

Add instructions for running your program here. Example:

```bash
python main.py --query "SELECT * FROM students WHERE grade > 80" --data data/students.csv
```

Or, if your project uses a different command:

```bash
python app.py <options>
```

## Examples

Include one or two example commands and outputs once your engine is implemented.

## Notes

- Add details about the CSV format your engine expects
- Mention any limitations or assumptions (for example, supported SQL subset, quoting rules, data types)
- Describe how to add new CSV files or query patterns

## Author

- Course: CS351
- Project: Project B
- Student: [Your Name]
- Semester: [Spring/Fall 2026]

## License

Add license information here if needed.
