#include <iostream>
#include "BookNode.h"
#include "FBTNode.h"

using namespace std;


//-- Forward Declaration --// 
void menu(); 
void branching(char option);
void fileRead();
void addToCart();
BookNode* search(int, BookNode*);
void display(BookNode*);
void checkOut();
void removeFromCart();
void displayFBT(int id);
void updateRank(BookNode*);
void insert(int, FBTNode*);
void addToFbt(int);
void deleteBookList(BookNode*);
void saveFbt();



int main()
{
	fileRead();

	char ch; 
	cout << "\n\nWelcome to CSE240: Bookstore\n";

	do {
		 menu();
	
		 ch = tolower(getchar()); // read a char, convert to lower case
		 branching(ch);
	} while (ch != 'q');
	//when the user decides to quit, these functions are called
	saveFbt();
	deleteBookList(Catalog);
	deleteBookList(cart);
	
	return 0; 
}

void menu()
{
	cout << "\nMenu Options\n";
	cout << "------------------------------------------------------\n";
	cout << "a: Add a book to the Cart\n";
	cout << "c: Go to Checkout\n";
	cout << "d: Delete a book from Cart\n";
	cout << "f: Display FBT of a book\n";
	cout << "p: Display Catalog\n"; 
	cout << "r: Display Cart\n"; 
	cout << "s: Search for a book in Catalog\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (a, c, d, f, s, p, r, or q) ---> "; 
}

void branching(char option)
{
	switch(option)
	{
		BookNode* result;
		case'a':
			addToCart();
		break;

		case 'c':
			checkOut();
		break;

		case 'd':
			removeFromCart();
		break;


		case 'f':
			int fbtId;
			cout<<"please enter the book id for the corresponding fbt list:"<<endl;
			cin >> fbtId;
			displayFBT(fbtId);
		break;

		case 's':
			int searchId;
			cout << "please enter the book id to be searched for: " << endl;
			cin >> searchId;

			result = search(searchId, Catalog);
			 
			if (result == NULL){
				cout<<"The book does not exist.";
			}else{
				cout.precision(2);
				result->next->display();
			}
			
		break;

		case 'p':
			display(Catalog);
		break;

		case 'r':
			display(cart);
		break;

		default:
			cout << "\nError: Invalid Input.  Please try again..."; 
		break;
	}
}

//reads both the catalog and fbt file into linked lists
void fileRead(){
	FILE *CatalogFile;
	FILE *FBTfile;
	BookNode *node;
	FBTNode *fbt;

	int type;
	char sTitle[1024];
	int parentId;
	int bookId;
	int Value;
	float price;


	CatalogFile = fopen("iBookCatalog.txt", "r");
	if (CatalogFile != NULL){
		while(fread(&type, sizeof(int), 1, CatalogFile) == 1){
			
			fread(sTitle, sizeof(sTitle), 1, CatalogFile);
			fread(&bookId, sizeof(int), 1, CatalogFile);
			fread(&price, sizeof(float), 1, CatalogFile);

			node = new BookNode(sTitle,bookId,price);

			//additional initializations if the book is more than just generic
			if(type == 0){
				char author[1024];
				fread(author, 1024, 1, CatalogFile);
				node->type = new FictionNode(author);
			}else if (type == 1){
				int issue;
				fread(&issue, sizeof(int), 1, CatalogFile);
				node->type = new MagazineNode(issue);
			}else if (type == 2){
				char author[1024];
				int edition;
				char isbn[1024];
				fread(author, 1024, 1, CatalogFile);
				fread(&edition, sizeof(int), 1, CatalogFile);
				fread(isbn, 1024, 1, CatalogFile);
				node->type = new TextbookNode(author, edition, isbn);
			}else{
				node->type = NULL;
			}
			//adds the initialized node to the catalog
			if(Catalog != NULL){
				node->next = Catalog;
				Catalog = node;
			}else{
				Catalog = node;
				node->next = NULL;
			}
		}
	}
	
	fclose(CatalogFile);

	FBTfile = fopen("FBT.txt", "r");
	if(FBTfile !=NULL){
		while(fread(&parentId, sizeof(int), 1, FBTfile) == 1){
			fread(&bookId, sizeof(int), 1, FBTfile);
			fread(&Value, sizeof(int), 1, FBTfile);

			fbt = (struct FBTNode *) malloc(sizeof(struct FBTNode));
			fbt->BookId = bookId;
			fbt->fbtValue = Value;
			fbt->next = NULL;
			

			//inserts the newly created fbt node into a sorted position determined by its value
			insert(parentId,fbt);
		}
	}
	fclose(FBTfile);
	return;
}



//adds the book to the linked list "cart" each time.
void addToCart(){
	int addId;
	char tempTitle[1024];
	int tempId;
	float tempPrice;

	cout<<"please enter the book id to be added:"<<endl;
	cin >> addId;

	BookNode* prevBook = search(addId, Catalog);
	BookNode* currentBook = prevBook->next;

	//copies the node of the desired book from the catalog
	strcpy_s(tempTitle, currentBook->getTitle());
	tempId = currentBook->getBookId();
	tempPrice = currentBook->getPrice();

	BookNode* copyNode;
	copyNode = new BookNode(tempTitle,tempId,tempPrice);
	copyNode->type = currentBook->type;

	//adds the node to the begginning of the cart.
	copyNode->next = cart;
	cart = copyNode;

	//now it displays the top 3 frequently bought together books and asks the user if they want to add it as well
	FBTNode* suggestionList = currentBook->getFbtList();
	int i = 1;
	while(suggestionList != NULL || i == 3){
		BookNode* topNode = search(suggestionList->BookId, Catalog);

		cout << "Would you like to add " << topNode->next->getTitle() << " to your cart?(Y/N)"<<endl;
		char answer;
		cin >>answer;
		if(answer == 'Y'){

			//adds the suggested book to the cart
			strcpy_s(tempTitle, topNode->next->getTitle());
			tempId = topNode->next->getBookId();
			tempPrice = topNode->next->getPrice();

			BookNode* copyNode;
			copyNode = new BookNode(tempTitle,tempId,tempPrice);
			copyNode->type = topNode->next->type;

			copyNode->next = cart;
			cart = copyNode;

		}
		suggestionList = suggestionList->next;
		i++;
		
	}
	cout << "Book(s) have been added to the cart!" << endl;
	return;
}



//searches for a specific bookId inside catalog or cart list
//returns the previous node
BookNode* search(int BookId, BookNode*head){    
	
	BookNode *listPtr, *prevNode;
	listPtr = head;
	prevNode = listPtr;
	
	while (listPtr != 0){ 
		if (listPtr->getBookId() == BookId) {
			return prevNode;								
		}
		else {prevNode = listPtr; listPtr = listPtr->next;}
	}					
	return NULL;
}



//displays the list of either the cart or catalog
void display(BookNode*head){
	BookNode *listPtr;
	listPtr = head;
	while(listPtr != 0){
		listPtr->display();
		listPtr = listPtr->next;
	}
	return;
}


//displays all the books added to cart.
//calculates the total price with tax.
void checkOut(){
	float total = 0;
	BookNode *pricePtr = cart;

	cout<<"Book(s) in your cart:"<<endl;
	cout<<"------------------------------------"<<endl;
	display(cart);
	
	//calculates total price by iterating through the list
	while(pricePtr !=NULL){
		total += pricePtr->getPrice();
		pricePtr = pricePtr->next;
	}

	cout<<"\nTotal price (w/ tax):"<<endl;
	cout<<"------------------------------------"<<endl;
	total = total*1.09;
	cout.precision(2);
	cout<< fixed <<"$"<<total<<endl;

	cout << "Are you ready to purchase?(Y/N)" << endl;
	char answer;
	cin >> answer;
	if(answer == 'Y'){

		//updates the rank of each book in the cart
		BookNode* cartPtr2 = cart;
		while(cartPtr2 != NULL){
			updateRank(cartPtr2);
			cartPtr2 = cartPtr2->next;
		}
	cout << "Book(s) have been purchased!" << endl;
	}
	
	return;
}


//removes a node from the cart
void removeFromCart(){
	
	int removeId;
	cout<<"please enter the book id to be deleted from cart:"<< endl;
	cin >> removeId;
	BookNode* cartPtr = search(removeId, cart);

	if (cartPtr==NULL){  //if no id match is found
		return;
	}
	if (cartPtr == cart) {		// case 1
		cart = cart->next;
		cout << "Book was successfully removed!" << endl;
		return;
	}
	if (cartPtr->next->next == NULL)	{	// case 2
		 cartPtr->next = NULL;
		 cout << "Book was successfully removed!" << endl;
		return;
	}
	 cartPtr->next =  cartPtr->next->next;	//case 3
	 cout << "Book was successfully removed!" << endl;
	 return;
}


//displays all the Frequently Bought Together items of a certain book node.
void displayFBT(int id){
	BookNode *fbtPtr;
	fbtPtr = search(id, Catalog);
	FBTNode* node = fbtPtr->next->getFbtList();

	if(node == NULL){
		cout<<"There isn't any FBT here"<<endl;
		return;
	}

	while(node != NULL){
		cout<<"ID: "<< node->BookId<< " Value: "<< node->fbtValue<<endl;
		node = node->next;
	}
	return;

	 
}

//inserts a node into the SORTED fbtlist
void insert(int parentId, FBTNode* node){
	
	BookNode *prevParent = search(parentId, Catalog);
	FBTNode* fbtHead = prevParent->next->getFbtList();
	FBTNode* temp = NULL;
	temp = fbtHead;
	int thisValue = node->fbtValue;
	
	if(fbtHead == NULL){
		prevParent->next->setFbtList(node);
		return;
	}
	
	//case 1: insertion at the beginning

	if(thisValue > fbtHead->fbtValue){
		
		node->next = prevParent->next->getFbtList();
		prevParent->next->setFbtList(node);
		return;
	}

	//case 2: Arbitrary position
	while(temp->next != NULL){
		
		if(thisValue > temp->next->fbtValue){

			node->next = temp->next;
			temp->next = node;
			
			prevParent->next->setFbtList(fbtHead);
			return;
		}
		else{
		temp = temp->next;
		}
	}
			
	//case3: if all else fails, append node to end of the list
	
	temp->next = node;
	prevParent->next->setFbtList(fbtHead);
	return;
}

//updates the FBT rank of the books inside the CATALOG list, by using the book information in the CART list.
void updateRank(BookNode* cartPtr){

	int thisBookId = cartPtr->getBookId();
	BookNode* prevBook = search(thisBookId,Catalog);
	FBTNode* fbtNodes = prevBook->next->getFbtList();
		
	//the first case is if a booknode does not have a fbtlist to begin with
	if(fbtNodes == NULL){
		addToFbt(thisBookId);
		return;
	}else{
		//second case checks for any matches between fbtnode ID's and cart ID's, if there is a match, then the fbtValue is incremented by 1.
		while(fbtNodes != NULL){
			int thisFbtId = fbtNodes->BookId;
			//the integer "matches" will tell us how many times a match was found to update the fbtValue
			int match = 0;
			BookNode* result = search(thisFbtId,cart);

			//if result of the search is not null, that means there is a match between a certain fbtid and the cart books
			//so the fbtnode value must be incremented
			if(result != NULL){
				fbtNodes->fbtValue +=1;
				match++;
			}
			//if we are at the end of the fbt list, and a match has not been found by any of the books in the cart,
			//we need to add them to the fbt list
			if(fbtNodes->next == NULL && match == 0){
				addToFbt(thisBookId);	
			}
		fbtNodes = fbtNodes->next;
		}
	}
	return;
}

//this method is used by the update rank method to add a fbtnode to a list.
//this method uses the bookId from the cart and puts it into the list with a fbtValue of 1.
void addToFbt(int cartBookId){

	BookNode* tempPtr = cart;
	while(tempPtr != NULL){
		int booksInCart = tempPtr->getBookId();
		FBTNode* temp = (struct FBTNode *) malloc(sizeof(struct FBTNode));
		temp->BookId = booksInCart;
		temp->fbtValue = 1;
		temp->next = NULL;

		//we are not adding any duplicate books to the cart at any given time
		//this condition makes sure that a book is not adding itself to its fbt list
		if(cartBookId != booksInCart){
			insert(cartBookId,temp);
		}
		
		tempPtr = tempPtr->next;	
	}
	return;
}

//rewrites the "FBT.txt" file when the program is quitting
void saveFbt(){
	FILE *fbtFile;
	FBTNode *node;
	BookNode* ListPtr = Catalog;
	int bookid;
	int value;
	fbtFile = fopen("FBT.txt","w");
	if(fbtFile != NULL){
		while(ListPtr != NULL){
			node = ListPtr->getFbtList();
			int parentid = ListPtr->getBookId();
			while(node != NULL){
				bookid = node->BookId;
				value = node->fbtValue;
				fwrite(&parentid,sizeof(int), 1, fbtFile);
				fwrite(&bookid,sizeof(int), 1, fbtFile);
				fwrite(&value,sizeof(int), 1, fbtFile);
				node = node->next;
			}
			ListPtr = ListPtr->next;
		}
	}
}

//deletes a linked list from memory
void deleteBookList(BookNode*p){
	if(p ==NULL){
		return;
	}else{
		deleteBookList(p->next);
		delete p;
	}
}

