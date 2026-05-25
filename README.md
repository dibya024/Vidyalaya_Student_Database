# Vidyalaya Student Database Management System


A C++ based Student Database Management System with authentication, SQLite integration, hashing, caching, and CRUD operations.


## Features

- Multi-role login system
  - Admin
  - Teacher
  - Student

- Student authentication system
- SQLite database integration
- Password hashing
- Fast student search using cache
- Add / Delete / Update / Search students
- JSON-based admin and teacher storage



## Technologies Used

- C++
- SQLite3
- JSON (nlohmann/json)
- STL
- Object-Oriented Programming
- Hash Maps



## Data Structures Used

- unordered_map
- unordered_set
- Hashing
- Cache system



## How to Run

Compile:

```bash
g++ main.cpp student.cpp student_manager.cpp database.cpp auth_manager.cpp -Iinclude -lsqlite3 -o main.exe
```

Run:

```bash
./main.exe
```



## Future Improvements

- Trie-based search
- GPA leaderboard
- GUI version
- CSV/PDF export
- Better password security
- Analytics dashboard
- 



## What I Learned

- SQLite integration with C++
- Authentication system design
- Hash-based caching
- Git and GitHub workflow
- Better OOP-based project structuring



## Author

Dibyaranjan Sahoo
