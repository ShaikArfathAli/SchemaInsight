# SchemaInsight


# 📊 SchemaInsight - DBMS Normalization Analyzer

SchemaInsight is a C++ command-line tool designed to **analyze database schemas** and determine their **highest normal form** (1NF, 2NF, 3NF, or BCNF) based on the input attributes, primary key, and functional dependencies (FDs).

---

## 🔧 Features

- ✅ **Supports 1NF, 2NF, 3NF, and BCNF detection**
- ✅ Input flexible sets of attributes and functional dependencies
- ✅ Analyzes **partial**, **transitive**, and **superkey** dependencies
- ✅ Provides clear CLI prompts and results
- ✅ Can be extended with candidate key and closure logic

---

## 🖥️ How It Works

The tool checks:
- **1NF** is assumed true (atomic values).
- **2NF**: No non-prime attribute should be partially dependent on any subset of a composite primary key.
- **3NF**: Every non-prime attribute must be fully functionally dependent on the primary key and not transitively dependent.
- **BCNF**: For every FD `X → A`, X must be a superkey.

---

## 📥 Input Format

When you run the program, it will prompt you step-by-step:

