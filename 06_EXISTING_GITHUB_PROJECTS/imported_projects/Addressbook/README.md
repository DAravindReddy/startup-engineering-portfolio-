# Modular Address Book Application in C

[![Language: C11](https://img.shields.io/badge/Language-C11-orange.svg)]()
[![Status: Completed](https://img.shields.io/badge/Status-Completed-brightgreen.svg)](PROJECT_STATUS.md)
[![Original Source](https://img.shields.io/badge/Original%20Repo-My__projects-lightgrey.svg)](https://github.com/DAravindReddy/My_projects)

A console-based, persistent contact management system developed using structured C programming. Demonstrates pointers, dynamic struct arrays, buffer management, input validation, and file-based CSV persistence.

---

## 1. Core Functionality
- **Add Contact:** Validates name, 10-digit mobile phone number, and RFC email format with duplicate detection.
- **Search Contacts:** Multi-criteria search by Name, Phone Number, or Email.
- **Edit Contact:** In-place record update with validation.
- **Delete Contact:** Re-indexes internal struct array to preserve contiguous memory.
- **List Contacts:** Formatted tabular display of all stored records.
- **Persistent Storage:** Reads and commits contacts to a CSV file across sessions.

---

## 2. Compilation and Execution

### Build with GCC:
```bash
make
./addressbook
```

*Or via direct GCC command:*
```bash
gcc -Wall -Wextra -std=c11 source_code/src/*.c -Isource_code/include -o addressbook
./addressbook
```

---

## 3. Attribution
- **Author:** **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy))
- **Original Repository:** [`https://github.com/DAravindReddy/My_projects`](https://github.com/DAravindReddy/My_projects)
- **License:** [MIT License](LICENSE)
