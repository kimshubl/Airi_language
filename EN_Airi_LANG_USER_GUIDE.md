# Airi Lang (Chisaka Irish Language) Key Use Guide 🐺

Airi Lang is an Esoteric C++ transfile language based on Chisaka Airi's speech habits and memes from Phase Connect. For programmers who are ready to gaslight their brains in the Irish language, it explains how to dig through the main grammar and conditions of the compiler ecosystem.

---

## 1. Tokenizer segmentation and disregard rules
When the compiler Lexer reads the code, unlike the usual C++, it reads the **"continuity of the word"** and the **special meme symbol** first.

1. **Continuous characters are treated as identifiers:**: 
   The symbols 'A', 'i', 'r', and 'I' are operational symbols that mean addition, subtraction, multiplication, and division. However, if they are not separated by spacing (```), they are grouped into a single English word (e.g., 'add', 'Airi') and become identifiers.
   - ⭕ **correct operation**: 'x Ay' (x plus y)
   - ❌ **Wrong operation**: 'xAy' (Error emitted as identifier 'xAy' in C++)

2. **Special symbol priority reading ('match' system):**:
   'Ai...' (open brackets), '...iriA' (closed brackets) Special symbols are first divided into unique tokens without spacing.
   - For example, when trying to use 'add(x)' as an area, it is safely split into at least 'add', 'Ai...', 'x', and '...ri' as 'addAi...x'. 

---

## 2. Functions and Returns (Functions & PON)
When writing a function, you must embody the mental state of Ireland, which you don't know when to commit PON.

- **'Ai'**: Initiation of function declaration (location of 'function' / return type declaration in C++)
- **`PON'**: Return statement ('return') implies dishonorably being kicked out at any price.
- **`Airi>`**(`->`): specify the return type arrow.

```cpp
// Example: A function that adds 10 and 20 and returns the u32 datatype
Ai add Ai... x .. Ai_32 . y .. Ai_32 ...ri Airi> Ai_32 Airi...
    PON x A y ,
...iriA
```
*Interpretation: Function (Ai) add (x: u32, y: u32) -> u32 { return x + y; }*

---

## 3. Memory and Variables (Volatile & Initialization)
Memory control rules are included to enable OS development. 
- Use the **`ri`** (`let`) keyword when declaring a variable.
- When you give a value to a variable, you use **'...'** (subject, '=') to avoid responsibility by blurring the end of the word.

```cpp
// C++: let my_var = 100;
ri my_var ... 100 ,
```

---

## 4. Advanced Grammar: Gaslight and Colonial Parsing
Used to trick the C++ Compiler by force to match the pointer or type. Very appropriate when specifying a type cast or hardware pointer.

- **'Gaslight' (Type Casting) **:
  When the compiler rebels because it is of a different type ('PON! Type Mismatch', it is forced to cover it with another type such as 'AiriType' (pointer).
- **`Gacha_` (memory address gambling) **: 
  Instead of hexadecimal ('0x') indicating a dangerous hardware address, such as a VGA buffer, use 'Gacha_'.

```cpp
// C++: ptr<u16> vga_buffer = (ptr<u16>) 0xB8000;
ri vga_buffer .. AiriType Ai_16 ... Gaslight( Gacha_B8000 into AiriType Ai_16 ) ,
```

---

## 5. Type and logical operations
- Wolf: 'True' - "I'm a wolf!"
- `woof`: 거짓 (`false`) - "Wolf not woof!" 
- 'PON?': Comparison of the same ('==') - "Are you sure you made a mistake?"
- '!PON?': Negative comparison ('!=') - "It's not a mistake, is it?"

Now you can handle the compiler in the spirit of the wolf more confidently than anyone else and cause errors!

---

## 6. Console Output (Print)
Use the 'phase_connects_family_azooma' keyword if you want to display letters or calculation results on the screen.

```cpp
// String Output
phase_connects_famous_azooma "Hello, World!" ,

// Output Equation Results
phase_connects_famous_azooma add Ai... 10 . 20 ...ri ,
```
*해석: print("Hello, World!") / print(add(10, 20))*

The string is enclosed in double quotation marks ('..."').

---

## 7. How to execute the script (Transfile & Run)

The Airi Lang compiler reads and automatically translates the created area script into C++ code, immediately compiling and executing it in the background. In a terminal (CLI) environment, code execution is resolved with one command.

```bash
# Default usage: ./main_integrated [File to run.airi]
./main_integrated my_code.airi
```

The above command leads to the following automated process:
1. Examine and parse the 'my_code.airi' grammar.
2. Generates a standard C++ source code called 'my_code.cpp'.
3. The '[Builder]' module turns on and compiles it into an 'output_bin' executable file through 'g++'.
4. The '[Runner]' module lights up and outputs the result './output_bin' immediately.

If you build a function in the code in the form of 'Ai main Ai......ri Airi> Ai_32 Airi...', which means the main entry point for C++, all of this will be connected seamlessly!

