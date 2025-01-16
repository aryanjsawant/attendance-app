# Attendance Management App  

A simple and user-friendly attendance management application built using C++ and Qt. This app allows administrators to manage students and track attendance efficiently.  

---

## 🎯 Features
- **Admin Panel**: Add, delete, and manage student records.
- **Student Panel**: Mark attendance and view attendance percentage.
- **Dynamic Subject Handling**: Add or delete subjects seamlessly.
- **Database-Backed**: All data stored in `users.db` (SQLite database).

---

## 📁 File Structure  

```
Attendance Management App/
├── database/
│   └── users.db              # SQLite database file
├── forms/                    # UI design files (.ui)
├── headers/                  # Header files (.h)
├── media/                    # Media assets (icons, images)
├── sources/                  # Source code files (.cpp)
├── attendance_app.zip        # Prebuilt app for end-users
├── QtCode.zip                # Source code for developers
├── uni.pro                   # Qt project configuration file
└── README.md                 # Project documentation
```

---

## 🚀 Quick Start  

### For End Users  
1. Download `attendance_app.zip` from the repository.
2. Extract the ZIP file to any folder.  
3. Open `uni.exe` from the extracted folder.  
4. Log in as Admin:  
   - **User ID**: `admin`  
   - **Password**: `213`  
5. Add students, manage attendance, and enjoy!

---

### For Developers  
1. Download `QtCode.zip` from the repository.  
2. Extract the ZIP file.  
3. Open `uni.pro` in Qt Creator.  
4. Build and run the project to explore or customize the app.  

---

## 🛠️ Tech Stack  
- **Programming Language**: C++  
- **Framework**: Qt 6.8  
- **Database**: SQLite  

---

## 💡 Usage Tips  
- Admin credentials are preconfigured as `admin` and `213`.  
- For student login, add a student in the Admin panel and use the generated ID for login.  
- Always ensure the `users.db` file is in the same directory as `uni.exe`.

---

## 📜 License  
This project is open-source and free to use. Contributions are welcome!

---

**Thank you for checking out the Attendance Management App! Happy tracking! 🎉**
```  
