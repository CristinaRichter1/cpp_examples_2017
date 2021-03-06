﻿// Seminar 8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
using namespace std;

class Restaurant {
public:
	char* nume;
	bool deschis;
	int nr_meniuri;
	float* preturi;
public:
	Restaurant() {
		this->nume = new char[strlen("Anonim") + 1];
		strcpy(this->nume, "Anonim");
		this->deschis = false;
		this->nr_meniuri = 0;
		this->preturi = NULL;
	}

	Restaurant(char* v_nume, bool v_deschis, int v_nr_meniuri, float* v_preturi) {
		this->nume = new char[strlen(v_nume) + 1];
		strcpy(this->nume, v_nume);
		this->deschis = v_deschis;
		this->nr_meniuri = v_nr_meniuri;
		this->preturi = new float[this->nr_meniuri];
		for (int i = 0; i < this->nr_meniuri; i++) {
			this->preturi[i] = v_preturi[i];
		}
	}

	Restaurant(const Restaurant& sursa) {
		this->nume = new char[strlen(sursa.nume) + 1];
		strcpy(this->nume, sursa.nume);
		this->deschis = sursa.deschis;
		this->nr_meniuri = sursa.nr_meniuri;
		this->preturi = new float[this->nr_meniuri];
		for (int i = 0; i < this->nr_meniuri; i++) {
			this->preturi[i] = sursa.preturi[i];
		}
	}

	Restaurant& operator=(const Restaurant& sursa) {
		if (this->nume != NULL) {
			delete[] this->nume;
		}
		if (this->preturi != NULL) {
			delete[] this->preturi;
		}
		this->nume = new char[strlen(sursa.nume) + 1];
		strcpy(this->nume, sursa.nume);
		this->deschis = sursa.deschis;
		this->nr_meniuri = sursa.nr_meniuri;
		this->preturi = new float[this->nr_meniuri];
		for (int i = 0; i < this->nr_meniuri; i++) {
			this->preturi[i] = sursa.preturi[i];
		}
		return *this;
	}

	~Restaurant() {
		if (this->nume != NULL) {
			delete[] this->nume;
		}
		if (this->preturi != NULL) {
			delete[] this->preturi;
		}

	}
	friend ostream& operator<<(ostream& out, Restaurant& r) {
		out << "Nume:"<< r.nume << endl;
		out <<"Deschis:"<< r.deschis << endl;
		out << "Nr meniuri:"<<r.nr_meniuri << endl;
		if (r.nr_meniuri > 0) {
			for (int i = 0; i < r.nr_meniuri; i++) {
				out << "Pretul pentru meniul " << i + 1 << " este: " << r.preturi[i] << endl;;
			}
		}
		return out;
	}
	friend ofstream& operator<<(ofstream& out, Restaurant& r) {
		out << r.nume << endl;
		out << r.deschis << endl;
		out << r.nr_meniuri << endl;
		if (r.nr_meniuri > 0) {
			for (int i = 0; i < r.nr_meniuri; i++) {
				out << r.preturi[i] << " ";
			}
			out << endl;
		}
		return out;
	}

	friend ifstream& operator>>(ifstream& in, Restaurant& r) {
		char buffer[50];
		in >> buffer;
		if (r.nume != NULL) {
			delete[] r.nume;
		}
		r.nume = new char[strlen(buffer) + 1];
		strcpy(r.nume, buffer);
		in >> r.deschis;
		in >> r.nr_meniuri;
		if (r.preturi != NULL) {
			delete[] r.preturi;
		}
		r.preturi = new float[r.nr_meniuri];
		for (int i = 0; i < r.nr_meniuri; i++) {
			in >> r.preturi[i];
		}
		return in;
	}
};

void generareFisierBinar(Restaurant* restaurante, int nr, char* numeFisier) {
	ofstream fisier;
	fisier.open(numeFisier, ofstream::out | ofstream::binary);
	if (fisier.is_open()) {
		//scriem nr de restaurante
		//adresa care se scrie si numarul de bytes ocupati
		fisier.write((char*)&nr, sizeof(int));
		for (int i = 0; i < nr; i++) {
			//scriem lungimea sirului de caractere
			int lungime = strlen(restaurante[i].nume) + 1;
			fisier.write((char*)&lungime, sizeof(int));
			//scriem numele
			fisier.write(restaurante[i].nume, sizeof(char)*lungime);
			//scriem daca e deschis
			fisier.write((char*)&restaurante[i].deschis, sizeof(bool));
			//scriem nr de meniuri
			fisier.write((char*)&restaurante[i].nr_meniuri, sizeof(int));
			//scriem preturile
			for (int j = 0; j < restaurante[i].nr_meniuri; j++) {
				fisier.write((char*)&restaurante[i].preturi[j], sizeof(float));
			}
		}
		fisier.close();
	}
	else {
		cout << "Eroare la scierea fisierului..."<<endl;

	}
}

void incarcaFisierBinar(Restaurant*& restaurante, int& nr, char* numeFisier) {
	ifstream fisier;
	fisier.open(numeFisier, ifstream::in | ifstream::binary);
	if (fisier.is_open()) {
		fisier.read((char*)&nr, sizeof(int));
		restaurante = new Restaurant[nr];
		for (int i = 0; i < nr; i++) {
			int lungime;
			fisier.read((char*)&lungime, sizeof(int));
			char nume[100];
			fisier.read(nume, sizeof(char)*lungime);
			bool deschis;
			fisier.read((char*)&deschis, sizeof(bool));
			int nr_meniuri;
			fisier.read((char*)&nr_meniuri, sizeof(int));
			float* preturi = new float[nr_meniuri];
			for (int j = 0; j < nr_meniuri; j++) {
				fisier.read((char*)&preturi[j], sizeof(float));
			}
			restaurante[i] = Restaurant(nume, deschis, nr_meniuri, preturi);
		}
		fisier.close();
	}
	else {
		cout << "Eroare la citirea fisierului..." << endl;
	}

}
void main()
{
	Restaurant r1;
	float v[2] = {18.5,20};
	float z[3] = { 15.5,28,22.5 };
	Restaurant* r2 = new Restaurant("Magic", true, 2, v);
	Restaurant r3 ("Victoriei", false, 3, z);

	Restaurant restaurante[3] = { r1,*r2,r3 };
	/*ofstream fisierOutTxt;
	fisierOutTxt.open("restaurante.txt", ofstream::out);
	for (int i = 0; i < 3; i++) {
		fisierOutTxt << restaurante[i];
	}
	fisierOutTxt.close();*/

	ifstream fisierInTxt;
	fisierInTxt.open("restaurante.txt", ifstream::in);
	if (fisierInTxt.is_open()) {
		while (!fisierInTxt.eof()) {
			Restaurant temp;
			fisierInTxt >> temp;
			cout << temp;
		}
	}
	fisierInTxt.close();

	generareFisierBinar(restaurante,3, "restaurante.dat");
	Restaurant* restauranteDinBinar=NULL;
	int nr_res=0;
	incarcaFisierBinar(restauranteDinBinar, nr_res, "restaurante.dat");
	if(nr_res > 0) {
		cout << "\tRestaurante incarcate din binar: " << endl;
		for (int i = 0; i < nr_res; i++) {
			cout << restauranteDinBinar[i] << endl;
		}
	}
}

