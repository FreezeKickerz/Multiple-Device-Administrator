# mdadm Device — Simulated RAID and Caching System  

## Overview  
This project was developed for **CMPSC 311: Introduction to Systems Programming**, a course focused on Unix system programming in C. The project demonstrates low-level system programming concepts by simulating a **software RAID management tool** (similar to `mdadm` in Linux) that combines multiple physical disks into one logical storage device.  

The implementation focuses on memory management, data caching, and network communication between client and server storage systems. It showcases skills in modular C programming, data organization, and performance optimization through caching and networked I/O.  

---

## Course Context  
**Course:** CMPSC 311 — Introduction to Systems Programming  
**Language:** C  
**System:** Unix/Linux  

### Course Topics  
- Unix system programming in C  
- Program organization and data structure design  
- Software standards and tools  
- System calls and file operations  
- Program analysis and debugging  
- Networking and inter-process communication  
- Performance optimization and system-level reasoning  

---

## Features  

### Linear Device Management  
The project simulates a linear RAID setup that treats multiple disks as a single logical unit.  

- **`mdadm_mount()`**: Mounts the linear device for read and write operations.  
- **`mdadm_unmount()`**: Unmounts the device, preventing further access until remounted.  
- **`mdadm_read()`**: Reads data from a specific memory address into a buffer.  
- **`mdadm_write()`**: Writes data from a buffer to a specific address on the storage system.  

Each operation enforces address validation and byte length limits to maintain data integrity and prevent out-of-bounds memory access.  

---

### Caching System  
The caching layer improves performance by reducing the need to repeatedly access slower storage.  

- **`cache_create(int num_entries)`**: Dynamically allocates memory for the cache.  
- **`cache_destroy()`**: Frees cache memory.  
- **`cache_lookup()`**: Checks if a requested block is already stored in cache.  
- **`cache_insert()`**: Inserts or overwrites cache entries using an **LRU (Least Recently Used)** policy.  
- **`cache_update()`**: Updates existing cache entries when new data is written.  
- **`cache_enabled()`**: Returns whether caching is currently active.  

Caching performance is tracked through `num_queries`, `num_hits`, and `clock` metrics, enabling hit ratio calculations and usage analysis.  

---

### Networking Integration  
To simulate distributed storage reliability, the project introduces network functionality that allows communication between client and server JBOD systems.  

- **`jbod_connect()`**: Establishes a connection to a JBOD server.  
- **`jbod_disconnect()`**: Closes the connection.  

Through these network calls, the system can route storage operations over the internet, ensuring redundancy and fault tolerance.  

---

## Technical Highlights  
- Written entirely in **C**, emphasizing pointer management and memory safety.  
- Implements **dynamic memory allocation**, **buffer manipulation**, and **bitwise operations**.  
- Uses **modular programming** principles for scalability and readability.  
- Includes **error handling** for invalid operations, multiple mounts/unmounts, and invalid cache states.  

---

## Learning Outcomes  
- Gained hands-on experience with **Unix-based system programming**.  
- Applied **C memory management** and **modular design** techniques.  
- Explored how **caching**, **networking**, and **RAID concepts** integrate in storage systems.  
- Strengthened debugging, performance profiling, and documentation skills.  

---

## Author  
Developed by a student of **Computer Engineering** and **Computer Science** at **Penn State University** for **CMPSC 311: Introduction to Systems Programming**.  
