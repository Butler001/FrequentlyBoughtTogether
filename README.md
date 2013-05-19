FrequentlyBoughtTogether
========================

An implementation of frequently bought together items through the example of a book catalog.

This project was exported directly from Visual Studio. 
If you have Visual Studio installed, unzip the file and click the FBTProject.sln 

The C++ and header files are located within the FBTProject folder.
Within this folder are the text files "FBT.txt" and "iBookCatalog.txt". These are given purely for example purposes.


DESCRIPTION:

This C++ project aims to act exactly like a "frequently bought together" (FBT) interface you would see on a commercial website. At the start of the program, a linked list is built from reading in the "iBookCatalog.txt" file. This file contains a variety of books and are initialized by type. The books are broken down into four types: Fiction, Magazine, Textbook, and Generic. 

Next, The program reads in the "FBT.txt" file that contains information on frequently bought together books. Each book created from reading in the iBookCatalog file will contain its own list of FBT books.

Once of both files are read, a menu is created which allows users to perform a variety of options. The main functionality is seen when adding a book to the cart. Once this happens, the programs looks into the book's FBT list and asks the user if they would like to add the TOP THREE FBT books. If the user agrees, that book is automatically added to the cart. When the program is exiting, the FBT file is updated if there are were any books purchased together. 
