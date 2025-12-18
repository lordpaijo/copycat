# CopyCat

A rewriting of:

```sh
$ cat file1.txt > file2.txt
```

written in C, for learning purposes.

`copycat` copies the contents of one or more input files into one or more output files using low-level POSIX system calls (`open`, `read`, `write`, `close`).

---

## ✨ Features

* Copy one or multiple input files
* Write to one or multiple output files
* Concatenates inputs in order (like `cat`)
* Duplicates output to all output files (like `tee`)
* Uses POSIX I/O (no stdio buffering)
* Built with CMake

---

## 📦 Project Structure

```
.
├── CMakeLists.txt
├── src/
│   └── main.c
├── build/
└── project.txt
```

---

## 🔧 Building

```sh
$ mkdir -p build
$ cd build
$ cmake ..
$ make
```

The resulting binary will be located at:

```sh
$ build/copycat
```

## ⬇️ Installing

From your build/ directory:
```sh
$ make install
```

If installing system-wide:
```sh
$ sudo make install
```

---

## ▶️ Usage

```sh
$ copycat if=<input> [if=<input> ...] of=<output> [of=<output> ...]
```

### Examples

Copy one file to one file:

```sh
$ copycat if=file1.txt of=file2.txt
```

Copy multiple input files into one output file:

```sh
$ copycat if=file1.txt if=file2.txt of=project.txt
```

Copy multiple input files into multiple output files:

```sh
$ copycat if=file1.txt if=file2.txt of=out1.txt of=out2.txt
```

All output files receive the same concatenated content.

---

## 🧠 Design Notes

* Input files are processed sequentially.
* Output files are opened once and written to repeatedly.
* Each read writes exactly the number of bytes returned by `read()`.
* The program intentionally mirrors Unix tool behavior.

---

## ⚠️ Limitations

* Does not support stdin (`if=-`) or stdout (`of=-`) yet
* Does not handle partial writes explicitly
* No append mode (`O_APPEND`)

These limitations are intentional to keep the code simple and educational.

---

## 🛠️ Requirements

* POSIX-compatible system
* C11-compatible compiler
* CMake ≥ 3.10

---

## 📚 Motivation

This project exists to:

* learn POSIX file I/O
* understand Unix tool design
* practice argument parsing
* explore systems programming in C

---

## 📜 License

See [MIT License](https://github.com/lordpaijo/copycat/blob/main/LICENSE).

---

