#include<iostream>
using namespace std;

//1
struct Depozit {
	int id; //cheie de inserare
	char* denumire;
	char* adresa;
	int nrAngajati;
};

Depozit creareDepozit(int id, const char* denumire, const char* adresa, int nrAngajati) {
	Depozit d;
	d.id = id;
	d.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(d.denumire, denumire);
	d.adresa = (char*)malloc(sizeof(char) * (strlen(adresa) + 1));
	strcpy(d.adresa, adresa);
	d.nrAngajati = nrAngajati;
	return d;
}

void afisareMasina(Depozit d) {
	printf("\n Depozitul %s cu id-ul %d este la adresa %s si are %d angajati.", d.denumire, d.id, d.adresa, d.nrAngajati);
}

Depozit citireDepozitFisier(FILE* f) {
	Depozit d;

	fscanf(f, "%d", &d.id);

	char buffer[30];
	fscanf(f, "%s", &buffer);
	d.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(d.denumire, buffer);

	char buffer2[30];
	fscanf(f, "%s", &buffer2);
	d.adresa = (char*)malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(d.adresa, buffer2);

	fscanf(f, "%d", &d.nrAngajati);

	return d;
}

//arbore binar de cautare
struct nodA {
	Depozit info;
	nodA* st;
	nodA* dr;
};
nodA* creareNod(Depozit d, nodA* st, nodA* dr) {
	nodA* nou = (nodA*)malloc(sizeof(nodA));
	nou->info = creareDepozit(d.id, d.denumire, d.adresa, d.nrAngajati);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nodA* inserareABC(nodA* root, Depozit d){
	if (root) {
		//valorile mai mici in stanga, valorile mai mari in dreapta
		if (d.id > root->info.id) {
			root->dr = inserareABC(root->dr, d);
		}
		else if (d.id < root->info.id) {
			root->st = inserareABC(root->st, d);
		}
		return root;
	}
	else {
		nodA* nou = creareNod(d, NULL, NULL);
		return nou;
	}
}

nodA* initializarearbore(FILE* f, int* nrelem) {
	nodA* root = NULL;
	fscanf(f, "%d", nrelem);
	for (int i = 0;i < *nrelem;i++) {
		Depozit d = citireDepozitFisier(f);
		root = inserareABC(root, d);
	}
	return root;
}

void afisareABC(nodA* root) { //inordine SRD
	if (root) {
		afisareABC(root->st);
		afisareMasina(root->info);
		afisareABC(root->dr);
	}
}
void main() {
	//1
	nodA* root = NULL;
	int nrelem = 0;
	FILE* f = fopen("bilet13.txt", "r");
	root = initializarearbore(f, &nrelem);
	afisareABC(root);
	printf("\n----------------------------------------------\n");
}