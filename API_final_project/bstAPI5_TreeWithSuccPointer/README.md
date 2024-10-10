# Word Buffer, BST Tree, and Word Matching System

## 1. Overview

This program implements a system that manages word buffers, constructs a binary search tree (BST) for efficient word insertion and searching, and performs word comparison based on a reference word. It supports the ability to compare a guessed word with a reference word, track letter constraints, and provide feedback on how close the guessed word is to the reference.

## 2. Core Components

The main components are:

- **Word Buffer (wordsBuffer)**: A dynamic buffer that stores words in a large continuous memory block.
- **Binary Search Tree (bstTree and bstTreeNode)**: A tree structure for organizing and searching words efficiently.
- **Reference Letter Group (refLetterGroup)**: A structure for comparing guessed words with a reference word, tracking letter occurrences and positions.

## 3. Data Structures

### 3.1. wordsBuffer
Manages memory allocation for storing words.

- **Fields**:
  - `char *words`: Pointer to the word buffer.
  - `int offset`: Tracks the next free position in the buffer.
  - `int bufferSize`: The current size of the buffer.
  - `int increaseSize`: Size by which the buffer grows when resized.
  - `int wordSize`: Length of a single word.

- **Functions**:
  - `new_wordsBuffer(int wordSize)`: Allocates a new wordsBuffer.
  - `resizeBuffer(wordsBuffer *buffer)`: Resizes the buffer when it's full.
  - `free_buffer(wordsBuffer *buffer)`: Frees the buffer memory.
  - `new_word(wordsBuffer *buffer, char *word)`: Inserts a word into the buffer.

### 3.2. bstTreeNode
Node of a binary search tree.

- **Fields**:
  - `int offset`: Offset of the word in the buffer.
  - `bstTreeNode *left, *right, *parent, *suc, *next_comp`: Pointers to manage tree structure and linked lists.

- **Functions**:
  - `new_bstTreeNode(int offset)`: Creates a new tree node.
  - `bst_Min` and `bst_Max`: Finds the minimum/maximum node in the subtree.
  - `bst_successor`, `bst_predecessor`: Finds the successor or predecessor of a node.
  - `bst_insert(bstTree *t, bstTreeNode *z)`: Inserts a new word node into the tree.
  - `bstTreeDeleteALl_recursive`: Recursively deletes all nodes from the tree.

### 3.3. bstTree
Binary search tree for storing and searching words.

- **Fields**:
  - `wordsBuffer *buffer`: The word buffer where words are stored.
  - `bstTreeNode *root`: The root node of the tree.
  - `int num_comp`: The number of compatible words.

- **Functions**:
  - `new_bstTree(int wordSize)`: Initializes a new binary search tree.
  - `comp_initialization(bstTree *t)`: Initializes the compatibility check for all words.
  - `comp_print_comp_set(bstTree *t)`: Prints the set of compatible words.
  - `comp_fix(bstTreeNode *node, bstTreeNode *root)`: Updates the compatibility status.
  - `treeContainsNode`: Checks if a word exists in the tree.
  - `comp_check(bstTree *t, refLetterGroup *letterGroup, char *letterArchive)`: Checks if the current word is compatible with the reference.

### 3.4. letterCheck
Tracks details of letter occurrences for comparison with a reference word.

- **Fields**:
  - `char letter`: The letter being tracked.
  - `int n_least, n_exact, exact_ok`: Constraints on how many times the letter appears.
  - `char *check`: Array indicating valid positions for the letter.

- **Functions**:
  - `new_letterCheck(char letter)`: Initializes a letterCheck for a specific letter.

### 3.5. refLetterGroup
Tracks letter statistics for the reference word and manages compatibility checks.

- **Fields**:
  - `char *ref`: The reference word.
  - `letterCheck **letterCheckList`: An array of letter check pointers, one for each character in the word.
  - `letterCheck *firstCheck`: The first letter check in a linked list.

- **Functions**:
  - `new_refLetterGroup(char *ref)`: Initializes a refLetterGroup for the reference word.
  - `freeLetterGroup(refLetterGroup *letterGroup)`: Frees the memory used by the refLetterGroup.

## 4. Word Matching and Compatibility Logic

### 4.1. Word Comparison

- `stringCompare(char *s1, char *s2, int len)`: Compares two strings lexicographically. Used in the BST to position nodes and in word comparison logic.

### 4.2. Compatibility Check

The main function `isCompatible` checks if a guessed word is compatible with the reference word based on letter positions and occurrences.

- **Steps**:
  - Checks if all letters in the guessed word exist in the reference word.
  - Validates letter positions using letterCheck constraints.
  - Updates the compatibility tree to remove incompatible words.

## 5. Game Flow

- **Initialization**: The tree and word buffer are set up, and words are inserted into the tree.
- **New Game**: A reference word is set, and a new game begins.
- **Word Guessing**: The player inputs a guessed word, which is compared with the reference word.
- **Feedback**: The program generates output indicating how close the guessed word is to the reference, and adjusts the set of compatible words accordingly.
- **End of Game**: If the correct word is guessed or the number of attempts is exhausted, the game ends.

## 6. Utility Functions

- `new_string(int size)`, `reset_letterArchive`, `free_string`: Helper functions to manage strings.
- `indexOfChar(char c)`: Maps a character to an index in the letterCheckList.
- `next_line`: Reads the next line of input.

## 7. Memory Management

The program uses dynamic memory allocation throughout (`malloc`, `calloc`, `realloc`), and ensures proper cleanup using functions like `free_buffer`, `free_bstTree`, and `freeLetterGroup`.

## 8. Input/Output

The program reads input lines representing words and commands, and prints output related to word compatibility and game state.
