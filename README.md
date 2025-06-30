# ft_nm

This project is a reimplementation of the Unix `nm` command, specifically designed to work with ELF binaries.  
It allows displaying the symbol table of an executable, object file (.o), or shared library (.so), reproducing the behavior of the system nm (ordering, padding, offset, etc.).

---

## 🚀 Objective

The goal of this project is to understand and manipulate the structure of ELF files (Executable and Linkable Format), by:

- Mapping files into memory using mmap.
- Parsing ELF headers (both 32-bit and 64-bit).
- Reading section headers and symbol tables.
- Displaying the symbol table with formatting similar to the system nm.

This project also requires strict error handling: no segmentation faults, bus errors, or double frees are tolerated.

---

## ⚙️ Compilation

To build the project, simply run:

```bash
make
```

This will generate the ft_nm binary.

## 🧠 Usage

To use the ft_nm command, you can run it with the following syntax:

```bash
./ft_nm [options] file1 [file2 ...]
```

Where `file1`, `file2`, etc. are the ELF files you want to analyze.

If no file is provided, ft_nm will attempt to analyze a.out by default.

---

## ✨ Bonus Options

These options are part of the bonus section of the project.  
All are implemented except for -a.

- -g : Display only external (global) symbols.
- -u : Display only undefined symbols.
- -r : Reverse the alphabetical sort order.
- -p : Do not sort; display symbols in the order they appear in the file.

The -a option (which includes debug symbols) is not implemented.

---

## 🏗️ Project Structure

- files_handler/ → File handling, memory mapping, ELF verification.
- symbols_handler/ → Symbol extraction and analysis.
- output_ft_nm/ → Sorting, formatting, and displaying symbols.
- libft/ → Personal utility library.

---

## 🔍 Example

./ft_nm /bin/ls

This will display the list of symbols along with their types and addresses if available.

---

## 📜 Important Notes

- The subject PDF is included in this repository.  
It is crucial to read it to fully understand the constraints and the reasoning behind certain design decisions.

- This project is an educational implementation. It does not aim to be a full or production-ready replacement for the real nm.

- There are many valid ways to design an ELF parser and symbol analyzer.  
This project reflects the design I chose, based on how I mentally visualized the problem and how I wanted to structure the solution.

---

## 📚 Useful Documentation

- [man nm](https://man7.org/linux/man-pages/man1/nm.1.html)  
- [ELF format (Wikipedia)](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)  
- [man elf (ELF file format manual)](https://man7.org/linux/man-pages/man5/elf.5.html)
- [GNU binutils nm implementation (source code)](https://sourceware.org/git/?p=binutils-gdb.git;a=tree;f=binutils;hb=HEAD)
