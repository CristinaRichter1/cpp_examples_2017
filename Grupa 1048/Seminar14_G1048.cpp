// Grupa_1048.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

class InterfataAdresaWeb {
public:
	virtual bool esteValida() = 0;

	virtual void afisare() = 0;
};
class PaginaWeb {
private:
	char adresaUrl[100];
	int rating; //max 5
	static int RATING_MAXIM;

public:
	PaginaWeb(char v_adresaUrl[100], int v_rating) {
		strcpy(this->adresaUrl, v_adresaUrl);
		this->setRating(v_rating);
	}

	friend ostream& operator<<(ostream& out, PaginaWeb& pw) {
		out << "Adresa url: " << pw.adresaUrl << endl;
		out << "Rating: " << pw.rating << endl;
		return out;
	}

	friend istream& operator>>(istream& in, PaginaWeb& pw) {
		cout << "Dati adresa url: ";
		in >> pw.adresaUrl;
		cout << "Dati rating-ul: ";
		int rating;
		in >> rating;
		pw.setRating(rating);

		return in;
	}

	void setRating(int v_rating) {
		if (v_rating >= 0 && v_rating <= RATING_MAXIM) {
			this->rating = v_rating;
		}
		else if (v_rating > RATING_MAXIM) {
			cout << "Rating peste maxim....va fi setat cu maxim!" << endl;
			this->rating = RATING_MAXIM;
		}
		else {
			cout << "Rating negativ....va fi setat pe 0!" << endl;
			this->rating = 0;
		}

	}

	void setAdresa(char v_adresaUrl[100]) {
		strcpy(this->adresaUrl, v_adresaUrl);
	}

	PaginaWeb& operator+=(int v_rating) {
		this->setRating(this->rating + v_rating);
		return *this;
	}

	int getRating() {
		return this->rating;
	}

	char* getAdresaUrl() {
		return this->adresaUrl;
	}

	static int getRatingMaxim() {
		return PaginaWeb::RATING_MAXIM;
	}

	friend ofstream& operator<<(ofstream& out, PaginaWeb& pw) {
		out <<  pw.adresaUrl << endl;
		out <<  pw.rating << endl;
		return out;
	}
};

int PaginaWeb::RATING_MAXIM = 5;

class Bookmark: public InterfataAdresaWeb {
private:
	const int idIntrare;
	PaginaWeb* paginaWeb;
	char* descriere;

public:
	int getIdIntrare() {
		return this->idIntrare;
	}
	bool esteValida() {
		char* url = this->paginaWeb->getAdresaUrl();
		string urlS = url;
		if (urlS.length() >= 8 &&
			urlS.find("www",0) != -1
			&& urlS.find(".", 0) != -1) {
			return true;
		}
		else {
			throw "Url invalid";
		}
	}

	void afisare() {
		cout << *this;
	}
	Bookmark(int v_idIntrare, PaginaWeb* v_paginaWeb)
	:idIntrare(v_idIntrare){
		this->paginaWeb = v_paginaWeb;
		this->descriere = new char[strlen("") + 1];
		strcpy(this->descriere, "");
	}

	Bookmark(int v_idIntrare, PaginaWeb* v_paginaWeb, char* v_descriere)
		:idIntrare(v_idIntrare) {
		this->paginaWeb = v_paginaWeb;
		this->descriere = new char[strlen(v_descriere) + 1];
		strcpy(this->descriere, v_descriere);
	}

	Bookmark(const Bookmark& sursa) :idIntrare(sursa.idIntrare) {
		this->paginaWeb = sursa.paginaWeb;
		this->descriere = new char[strlen(sursa.descriere) + 1];
		strcpy(this->descriere, sursa.descriere);
	}
	
	Bookmark& operator=(const Bookmark& sursa){
		if (this->descriere != NULL) {
			delete[] this->descriere;
		}
		this->paginaWeb = sursa.paginaWeb;
		this->descriere = new char[strlen(sursa.descriere) + 1];
		strcpy(this->descriere, sursa.descriere);

		return *this;
	}

	~Bookmark() {
		if (this->descriere != NULL) {
			delete[] this->descriere;
		}
	}

	void setDescriere(char* v_descriere) {
		if (this->descriere != NULL) {
			delete[] this->descriere;
		}
		this->descriere = new char[strlen(v_descriere) + 1];
		strcpy(this->descriere, v_descriere);
	}

	bool operator>(Bookmark& b) {
		return this->paginaWeb->getRating() > b.paginaWeb->getRating();
	}

	char* getUrl() {
		return this->paginaWeb->getAdresaUrl();
	}

	friend ostream& operator<<(ostream& out, Bookmark& b) {
		out << "Id intrare: " << b.idIntrare << endl;
		out << *b.paginaWeb;
		out << "Descriere: " << b.descriere << endl;

		return out;
	}

	friend ofstream& operator<<(ofstream& out, Bookmark& b) {
		out << b.idIntrare << endl;
		out << *b.paginaWeb;
		out << b.descriere << endl;

		return out;
	}

	int getRating() {
		return this->paginaWeb->getRating();
	}

	explicit operator int() {
		return strlen(this->paginaWeb->getAdresaUrl());
	}

	friend istream& operator>>(istream& in, Bookmark& b) {
		PaginaWeb* p = new PaginaWeb("", 0);
		in >> *p;
		b.paginaWeb = p;
		cout << "Dati descrierea: ";
		char buffer[50];
		in.ignore();
		in.getline(buffer,50);
		b.setDescriere(buffer);
		return in;
	}

	static void printLista(Bookmark** bookmarks, int nr, char* numeFisier, int rating) {
		ofstream file;
		file.open(numeFisier, fstream::out);
		if (file.is_open()) {
			for (int i = 0; i < nr; i++) {
				if (bookmarks[i]->getRating() >= rating) {
					file << *bookmarks[i];
				}
			}
			file.close();
		}
	}
};

class History : public Bookmark {
private:
	string dataUltimeiAccesari;
public:
	History(int v_idIntrare, PaginaWeb* v_paginaWeb, char* v_descriere, string v_dataUltimeiAccesari)
	:Bookmark(v_idIntrare,v_paginaWeb, v_descriere){
		this->dataUltimeiAccesari = v_dataUltimeiAccesari;
	}

	void afisare() {
		cout << (Bookmark)*this;
		cout << "Data ultimei accesari: " << this->dataUltimeiAccesari << endl;
	}
};

class MyBookmarks {
private:
	map<char*, Bookmark*> continut;
public:
	friend ostream& operator<<(ostream& out, MyBookmarks& mb) {
		map<char*, Bookmark*>::iterator it;
		for (it = mb.continut.begin(); it != mb.continut.end(); it++) {
			out << "Key: " << it->first << endl;
			out << "Value: " << endl;
			out << *it->second << endl;
		}
		return out;
	}

	MyBookmarks operator+(Bookmark* b) {
		MyBookmarks copie = *this;
		copie.continut.insert(make_pair(b->getUrl(), b));

		return copie;
	}

	Bookmark* getBookmark(char* URL) {
		map<char*, Bookmark*>::iterator it;
		it = this->continut.find(URL);
		if (it != this->continut.end()) {
			return it->second;
		}
		else {
			return NULL;
		}
	}
};
void main()
{
	PaginaWeb p2("www.ase.ro", 2);
	PaginaWeb p3 = p2;
	PaginaWeb p4 = p3;
	//cin >> p4;
	cout << p2 << p3 << p4;
	p2.setAdresa("www.csie.ase.ro");
	p2 += 1; //incrementare rating
	cout << p2 << p3 << p4;

	Bookmark b1(1, &p2);
	Bookmark b2(2, &p3, "Pagina Ase");
	b1.setDescriere("Pagina Web ASE");
	if (b2 > b1) {
		cout << endl << b2.getUrl() << "are rating mai mare" << endl;
	}
	else {
		cout << endl << b1.getUrl() << "are rating mai mare" << endl;
	}
	cout << b1;
	
	//cin >> b1;
	cout << "Lungime url: " << (int)b1 << endl;
	cout << b1.getUrl() << " are ratingul " << b1.getRating() << " din maxim " << PaginaWeb::getRatingMaxim() << endl;

	Bookmark** bookmarks;
	int nrBookmarks=0;
	//initializare vector de pointeri cu minim 3 pointeri cititi de la consola
	cout << "Dati nr de bookmarkuri: ";
	//cin >> nrBookmarks;
	bookmarks = new Bookmark*[nrBookmarks];
	for (int i = 0; i < nrBookmarks; i++) {
		Bookmark* b = new Bookmark(0, new PaginaWeb("", 0));
		//cin >> *b;
		bookmarks[i] = b;
	}

	//afisare vector
	/*for (int i = 0; i < nrBookmarks; i++) {
		cout << *bookmarks[i];
	}*/

	//Bookmark::printLista(bookmarks, nrBookmarks, "Favorite.txt", 3);

	//Bookmark b3(3, new PaginaWeb("adadadada", 4));
	Bookmark b3(3, new PaginaWeb("www.irina.com", 4));
	try {
		if (b3.esteValida() == true) {
			cout << "Adresa url valida." << endl;
		}
	}
	catch (char* mesaj) {
		cout << mesaj << endl;
	}

	InterfataAdresaWeb* adrese[2] = { &b3,new History(4,&p3,"Pagina speciala","08/01/2018") };
	for (int i = 0; i < 2; i++) {
		adrese[i]->afisare();
	}

	MyBookmarks mb;
	mb = mb + &b1;
	mb = mb + &b2;
	cout << mb;
	Bookmark* b = mb.getBookmark(b1.getUrl());
	//Bookmark* b = mb.getBookmark("www.irina.ro");
	if (b!= NULL) {
		cout << *b;
	}
	else {
		cout << "Negasit."<<endl;
	}
}

