# File system in C
This project focuses on the data structure part of a file system model implemented in C using linked lists. The Linux file manipulation commands are being used as aliases due to their similar functionality. 

## 1. touch *file_name*
`void touch(Dir* parent, char* name)`

Creates a new file named *file_name* in the current working directory.

Files work as nodes in a singly linked list, therefore only requiring carefully treating special cases such as:
+ Empty list => The new file becomes the head
+ Already existing file => Do nothing and show an error message

## 2. mkdir *dir_name*
`void mkdir(Dir* parent, char* name)`

Creates a new directory named *dir_name* in the current working directory.

Directories also work as nodes in singly linked lists, but they also hold a reference to a list of subdirectories and files. They present the same special cases as files.

## 3. ls
`void ls(Dir* parent)`

Displays the subdirectories first, then the files present in the current working directory.

Works by iterating throught the list of subdirectories first, then through the list of files and displaying their name, one on each line.

## 4. rm *file_name* 
`void rm(Dir* parent, char* name)`

Deletes file under the name *file_name* from current working directory. If the searched file does not exist, the command will return an error.

The function checks the head node of the files to see if it's the sought one after, then it iterates and checks the rest, if it's not. A few special cases appear:

+ The node is the first one => Move the head to it's successor and delete the old one
+ We don't find the node => Display an error message

## 5. rmdir *dir_name*
`void rmdir(Dir* parent, char* name)`

Is different from the original rmdir, acts as `rm -r dir_name`. Functionally identical to **rm**.

Because directories act like a tree, the program simulates a depth first search, purging each directory and it's contents as it returns from it's traversal. Same special cases as **rm**.

## 6. cd *dir_name*
`void cd(Dir** target, char* name)`

A more limited version of the original **cd**, can change only 1 directory at a time.

Searches for *dir_name* or the string `".."`. If found, the reference to the current working directory is changed to the specified one. If the `".."` string is introduced instead, the reference is changed to the parent of the current working directory.

## 7. tree
`void tree(Dir* target, int level)`

Displays the content of the current working directory, but each time it encounters a subdirectory, it displays it's contents as well, indented for each level of depth.

Performs a depth first search on the subdirectories, displaying their name, before proceeding with the traversal. After each function call, it displays the contained files as well.

## 8. pwd
`char* pwd(Dir* target)`

Returns a string with the path to the current working directory.

Works by recursively digging back to the root directory and concatenating each directory name after the function call, resulting in the full path to the current working directory.

## 9. stop
`void stop(Dir* target)`

Stops the program and frees all the memory used by the root directory. Utilizes the recusive function from **rmdir**.

## 10. mv *old_name new name*
`void mv(Dir* parent, char* oldname, char* newname)`

Changes the name of a directory/file from *old_name* to *new_name* if *old_name* exists and *new_name* isn't taken. Moves the directory/file to the end of the list they exist in.

First checks for the existance of *old_name*, if it doesn't exist it shows and error message. Then it checks for the existance of *new_name*, if it already exists then it shows an error message.

It proceeds onto checking the head directory to see if it matches, if not, it searches in the rest of the list. Once it's found, the name is changed and the directory is pushed to the end of the list.

If a directory is not found at all, it proceeds onto doing the same, but this time in the files list.

A few special cases appear:
+ Only 1 directory/file present => No moving is done
+ The head of the list matches => The head is moved to it's succesor and the old head is moved to the end of the list
