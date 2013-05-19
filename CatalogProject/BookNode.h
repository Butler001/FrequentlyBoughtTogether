#include <iostream>
using namespace std;



class BookNode {

private:
	char title[1024];		// Book name
	int bookId;					// Book id or ISBN number
	float price;			// Book price
	struct FBTNode* fbtHead;
	

public:
	BookNode* next;
	BookNode* type;

	BookNode(void){ fbtHead = NULL;}
	
	BookNode(char pTitle[], int pBookId, float pPrice){
		strcpy_s(title, pTitle);
		bookId = pBookId;
		price = pPrice;
		fbtHead = NULL;
		type = NULL;
	}


	char* getTitle(){
		return title;
	}

	int getBookId(){
		return bookId;
	}

	float getPrice(){
		return price;
	}

	FBTNode* getFbtList(){
		return fbtHead;
	}
	void setFbtList(FBTNode* insertFBT){
		fbtHead = insertFBT;
	}
	void virtual display(){
		cout.precision(2);
		cout<< fixed << "ID: "<<this->bookId << " Title: "<<this->title << " for $"<< this->price;
		if(this->type != NULL){
			this->type->display();
		}else{
			cout << endl;
		}
	}
	
	
};
BookNode* Catalog;
BookNode* cart;

//classes FictionNode, MagazineNode, and TextbookNode inherit from the parent class BookNode.
//in order to display a specific book, each of these classes use the polymorphic function display()
//each has a constructor for initialization and is linked to the booknode object through the pointer "type"

class FictionNode: public BookNode{
private:
	char author[1024];
public:
	FictionNode(char pAuthor[]){
		strcpy_s(author, pAuthor);
	}
	void display(){
		cout << " Author: " << author << endl;
	}
};

class MagazineNode: public BookNode{
private:
	int issue;
public:
	MagazineNode(int pIssue){
		issue = pIssue;
	}
	void display(){
		cout << " Issue: " << issue << endl;
	}
};
class TextbookNode: public BookNode{
private:
	char author[1024];
	int edition;
	char isbn[1024];
public:
	TextbookNode(char pAuthor[], int pEdition, char pIsbn[]){
		strcpy_s(author, pAuthor);
		edition = pEdition;
		strcpy_s(isbn, pIsbn);
	}
	void display(){
		cout << " Author: " << author << " Edition: "<< edition << " isbn:" << isbn << endl;
	}
};







