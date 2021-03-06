// ExempluMeniuAndMainArgs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>

using namespace std;
class Student {
protected:
	char* nume;
	bool integralist;
public:
	Student() {
		this->nume = new char[strlen("Anonim") + 1];
		strcpy(this->nume, "Anonim");
		this->integralist = false;
	}
	Student(char* v_nume, bool v_integralist) {
		this->nume = new char[strlen(v_nume) + 1];
		strcpy(this->nume, v_nume);
		this->integralist = v_integralist;
	}
	Student(const Student& sursa) {
		this->nume = new char[strlen(sursa.nume) + 1];
		strcpy(this->nume, sursa.nume);
		this->integralist = sursa.integralist;
	}
	~Student() {
		if (this->nume != NULL) {
			delete[] this->nume;
		}
	}
	Student& operator=(const Student& sursa) {
		if (this->nume != NULL) {
			delete[] this->nume;
		}
		this->nume = new char[strlen(sursa.nume) + 1];
		strcpy(this->nume, sursa.nume);
		this->integralist = sursa.integralist;
		return *this;
	}

	friend ostream & operator<<(ostream & out, Student& s) {
		out << "Nume: " << s.nume << endl;
		out << "Integralist: " << s.integralist << endl;
		return out;
	}

	char* getNume() {
		return this->nume;
	}
};


void main(int argc, char* argv[])
{
	cout << "Numar argumente: " << argc << endl;
	for (int i = 0; i < argc; i++) {
		cout << "Argumentul " << i + 1 << ": " << argv[i] << endl;
	}
	cout << "**************************************************" << endl;
	cout << "Aplicatia pune la dispozitie urmatoarele optiuni: " << endl;
	cout << "1: Crearea unui nou student." << endl;
	cout << "2: Afisarea studentului creat." << endl;
	cout << "3: Terminare aplicatie." << endl;
	cout << "**************************************************" << endl;

	int optiune;
	cout << "Dati optiunea dorita: ";
	cin >> optiune;

	Student s;

	while (optiune != 3) {

		switch (optiune) {
		case 1:
			cout << "Dati nume: ";
			char buffer[50];
			cin.ignore();
			cin.getline(buffer, 50);
			s = Student(buffer, true);
			break;
		case 2:
			if (strcmp(s.getNume(), "Anonim") != 0) {
				cout << s;
			}
			else {
				cout << "Pentru afisare trebuie sa aveti un obiect creat!" << endl;
			}
			break;
		default:
			cout << "Optiunea aleasa nu exista. Consultati manualul aplicatiei." << endl;
			break;
		}

		//Primesc o noua optiune
		cout << "Dati optiunea dorita: ";
		cin >> optiune;
	}
	cout << "Aplicatia se inchide..." << endl;
}

