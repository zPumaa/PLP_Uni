#include "library.h"

#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory> 
#include <fstream>
#include <iostream>
#include <string>

#include <cassert>

//Method to update the title of a document
void Document::updateTitle(const std::string newTitle) {
  _title = newTitle;
}

//Method to update the quantity of a document
void Document::updateQuantity(int newQuantity) { 
  _quantity = newQuantity;
  }

//Method to update the year of a document  
void Document::updateYear(int newYear) { 
  _year = newYear;
  }

//Method to get the title of a document  
std::string Document::getTitle() {
  return _title;
  }
  
//Method to get the year of a document  
int Document::getYear() {
  return _year;
  }

//Method to get the quantity of a document  
int Document::getQuantity() {
  return _quantity;
  }



//Method to check whether a document can be borrowed or not
bool Document::borrowDoc() {
  if (_quantity > 0) {
    _quantity--;
    return true;
  }
  return false;
}

//Method to return a document
void Document::returnDoc() {
   _quantity++;
  }

//Constructor for a novel
Novel::Novel(const std::string title, const std::string author, int year, int quantity) {
  _title = title;
  _author = author;
  _year = year;
  _quantity = quantity;
}

//Function to get the type of a document returns DOC_NOVEL
DocType Novel::getDocType() { return DOC_NOVEL; }

//Method to print the details of a novel
void Novel::print() {
  std::cout << "Novel, title: " << _title << ", author: " << _author << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

//Method to update the author of a novel
void Novel::updateAuthor(const std::string newAuthor) {
  _author = newAuthor;
}

//Method to get the author of a novel
std::string Novel::getAuthor() { 
  return _author;
  }

//Constructor for a textbook
Comic::Comic(const std::string title, const std::string author, int issue, int year,
             int quantity) {
  _title = title;
  _author = author;
  _year = year;
  _quantity = quantity;
  _issue = issue;
}

//Method to get the type of a document returns DOC_COMIC
DocType Comic::getDocType() { return DOC_COMIC; }

//Method to print the details of a comic
void Comic::print() {
  std::cout << "Comic, title: " << _title <<  ", author: " << _author << ", issue: " << _issue << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

//Method to update the author of a comic
void Comic::updateAuthor(const std::string newAuthor) {
  _author = newAuthor;
}

//Method to update the issue of a comic
void Comic::updateIssue(int newIssue) { 
  _issue = newIssue;
  }

//Method to get the author of a comic
std::string Comic::getAuthor() {
  return _author;
  }

//Method to get the issue of a comic  
int Comic::getIssue() {
  return _issue;
  }

//Constructor for a magazine
Magazine::Magazine(const std::string title, int issue, int year, int quantity) {
  _title = title;
  _year = year;
  _quantity = quantity;
  _issue = issue;
}

//Method to get the type of a document returns DOC_MAGAZINE
DocType Magazine::getDocType() { return DOC_MAGAZINE; }

//Method to print the details of a magazine
void Magazine::print() {
  std::cout << "Magazine, title: " << _title << ", issue: " << _issue << ", year: " << _year << ", quantity: " << _quantity << "\n";
}

//Method to update the issue of a magazine
void Magazine::updateIssue(int newIssue) {
  _issue = newIssue;
  }

//Method to get the issue of a magazine  
int Magazine::getIssue() {
  return _issue; 
  }

//Constructor for a library
Library::Library() {};//{ _docs = std::vector<Document *>();}

//Method to add a document to the library
bool Library::addDocument(DocType t, const std::string title, const std::string author,
                         int issue, int year, int quantity) {
 // Document *d = nullptr;
  switch (t) {
  case DOC_NOVEL: {
    //d = (Document *)new Novel(title, author, year, quantity);
    auto d = std::make_shared<Novel>(title, author, year, quantity);
    return addDocument(d);
    break;
  }
  case DOC_COMIC: {
    //d = (Document *)new Comic(title, author, issue, year, quantity);
    auto d = std::make_shared<Comic>(title, author, issue, year, quantity);
    return addDocument(d);
    break;
  }
  case DOC_MAGAZINE: {
    //d = (Document *)new Magazine(title, issue, year, quantity);
    auto d = std::make_shared<Magazine>(title, issue, year, quantity);
    return addDocument(d);
    break;
  }
  default:
    return false;
  }
  return false;
}

//Method to add a document to the library

bool Library::addDocument(Document *d) {
  for (auto& i : _docs )
    if ((i->getTitle() == d->getTitle())) {
      return false;
    }

  _docs.push_back(std::shared_ptr<Document>(d));
  return true;
}

bool Library::addDocument(std::shared_ptr<Document> d) {
  for (auto& i : _docs )
    if ((i->getTitle() == d->getTitle())) {
      return false;
    }

  _docs.push_back(d);
  return true;
}

bool Library::delDocument(const std::string title) {
  int index = -1;
  for (auto& i : _docs){
    index++;
    if (i->getTitle() == title) {
      _docs.erase(_docs.begin() + index);
      return true;
    }
  }
  return false;
}


int Library::countDocumentOfType(DocType t) {
  int res = 0;
  for (auto& i : _docs)
    if (i->getDocType() == t) {
      res++;
    }
  return res;
}


Document *Library::searchDocument(const std::string title) {
  for (auto& i : _docs)
    if ((i->getTitle() == title)) {
      return i.get();
    }
  return nullptr;
}

//Method to print the details of all the documents in the library
void Library::print() {
  for (auto& i : _docs)
    i->print();
}

//Method to borrow a document from the library
bool Library::borrowDoc(const std::string title) {
  Document *d = searchDocument(title);
  if (d)
    return d->borrowDoc();
  return false;
}

//Method to return a document to the library
bool Library::returnDoc(const std::string title) {
  Document *d = searchDocument(title);
  if (d) {
    d->returnDoc();
    return true;
  }
  return false;
}

//Function that writes documents to a file
bool Library::dumpCSV(const std::string filename) {
  std::fstream theFile;
  theFile.open(filename, std::ios::out);

  for (auto& i : _docs) {
    //Document *d = i;
    switch (i->getDocType()) {
    case DOC_NOVEL: {
      Novel *n = dynamic_cast<Novel *>(i.get());
      theFile << "novel," << n->getTitle() << "," << n->getAuthor() << "," << "," << n->getYear() << "," << n->getQuantity() << "\n";
      break;
    }
    case DOC_COMIC: {
      Comic *c = dynamic_cast<Comic *>(i.get());
      theFile << "comic," << c->getTitle() << "," << c->getAuthor() << "," << c->getIssue() << "," << c->getYear() << "," << c->getQuantity() << "\n";
      break;
    }
    case DOC_MAGAZINE: {
      Magazine *m = dynamic_cast<Magazine *>(i.get());
      theFile << "magazine," << m->getTitle() << "," << "," << m->getIssue() << "," << m->getYear() << "," << m->getQuantity() << "\n";
      break;
    }
    default:
      return false;
    }
  }
  theFile.close();
  return true;
}
