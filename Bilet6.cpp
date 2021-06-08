#include<iostream>
using namespace std;

struct Frigider {
	int pret;//cheie cautare
	char* firma;
	char* model;
	float greutate;
};

Frigider creareFrigider(int pret, const char* firma, const char* model, float greutate) {
	Frigider f;
	f.pret = pret;
	f.firma = (char*)malloc(sizeof(char) * (strlen(firma) + 1));
	strcpy(f.firma, firma);
	f.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(f.model, model);
	f.greutate = greutate;
	return f;
}
void afisareFrigider(Frigider f) {
	printf("\n Frigiderul %s %s costa %d lei si are greutatea %5.2f", f.firma, f.model, f.pret, f.greutate);
}
Frigider citireFrigider(FILE* f) {
	Frigider a;

	fscanf(f, "%d", &a.pret);

	char buffer[30];
	fscanf(f, "%s", &buffer);
	a.firma = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(a.firma, buffer);

	char buffer2[30];
	fscanf(f, "%s", &buffer2);
	a.model = (char*)malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(a.model, buffer2);

	fscanf(f, "%f", &a.greutate);

	return a;
}

struct nod {
	Frigider info;
	nod* st;
	nod* dr;
};
nod* creareNod(Frigider f, nod* st, nod* dr) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareFrigider(f.pret, f.firma, f.model, f.greutate);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nod* inserareABC(nod* root, Frigider f) /*unde inseram, de unde*/ {
	if (root) {
		//valorile mai mici in stanga, valorile mai mari in dreapta
		if (f.pret > root->info.pret) {
			root->dr = inserareABC(root->dr, f);
		}
		else if (f.pret < root->info.pret) {
			root->st = inserareABC(root->st, f);
		}
		return root;
	}
	else {
		nod* nou = creareNod(f, NULL, NULL);
		return nou;
	}
}

nod* initializarearbore(FILE* f, int* nrelem) {
	nod* root = NULL;
	fscanf(f, "%d", nrelem);
	for (int i = 0;i < *nrelem;i++) {
		Frigider c = citireFrigider(f);
		root = inserareABC(root, c);
	}
	return root;
}

void SRD(nod* root) {
	if (root) {
		SRD(root->st);
		afisareFrigider(root->info);
		SRD(root->dr);
	}
}

//2. pretul aparatului electrocasnic cu pretul cel mai mic
void pretMinim(nod* root, int* minim) {
	if (root) {//rsd
		if (root->info.pret < * minim) {
			*minim = root->info.pret;
		}
		pretMinim(root->st, minim);
		pretMinim(root->dr, minim);
	}
}

//3.extrage aparatul electrocasnic cu pretul cel mai mare din abc
void pretMaxim(nod* root, int* maxim) {
	if (root) {//rsd
		if (root->info.pret > *maxim) {
			*maxim = root->info.pret;
		}
		pretMaxim(root->st, maxim);
		pretMaxim(root->dr, maxim);
	}
}

nod* cautareMaxim(nod* arbore) {
	if (arbore) {
		if (arbore->dr) {
			return cautareMaxim(arbore->dr);
		}
		else {
			return arbore;
		}
	}
	else {
		return NULL;
	}
}

//fct de extragere
Frigider extragereFrigiderDinArbore(nod* * rad, int pretCautat) {
	if (*rad) {
		if (pretCautat > (*rad)->info.pret) {
			return extragereFrigiderDinArbore(&(*rad)->dr, pretCautat);
		}
		else if (pretCautat < (*rad)->info.pret) {
			return extragereFrigiderDinArbore(&(*rad)->st, pretCautat);
		}
		else {
			if ((*rad)->st == NULL) {
				Frigider rezultat = 
					creareFrigider((*rad)->info.pret, (*rad)->info.firma, (*rad)->info.model, (*rad)->info.greutate);
				free((*rad)->info.firma);
				free((*rad)->info.model);
				nod* temp = (*rad)->dr;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			else if ((*rad)->dr == NULL) {
				Frigider rezultat = 
					creareFrigider((*rad)->info.pret, (*rad)->info.firma, (*rad)->info.model, (*rad)->info.greutate);
				free((*rad)->info.firma);
				free((*rad)->info.model);
				nod* temp = (*rad)->st;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			else {
				nod* temp = cautareMaxim((*rad)->st);
				Frigider aux = (*rad)->info;
				(*rad)->info = temp->info;
				temp->info = aux;
				return extragereFrigiderDinArbore(&(*rad)->st, pretCautat);
			}
		}
	}
}

//4.
//functia de cautare
//cauta nodul in functie de pretul dat de mine
Frigider cautareInArbore(nod* arbore, int pretMaxim) {
	if (arbore) {
		if (pretMaxim < arbore->info.pret) {
			return cautareInArbore(arbore->st, pretMaxim);

		}
		else if (pretMaxim > arbore->info.pret) {
			return cautareInArbore(arbore->dr, pretMaxim);
		}
		return arbore->info;
	}
	else {
		return creareFrigider(0, "Fictiv","Fictiv", 0);
	}
}
//functia de modificare
nod* modificaPret(nod* arbore, int pretMinim) {
	if (arbore) {
		int maxim = 0;
		pretMaxim(arbore, &maxim);
		Frigider rez = cautareInArbore(arbore, maxim);
		rez.pret = pretMinim -1;
		arbore = inserareABC(arbore, rez);
	}
	return arbore;
}
//5. copiere in lsi

struct nodLS {
	Frigider info;
	nodLS* next;
};

nodLS* creareNod(Frigider info, nodLS* next) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info = creareFrigider(info.pret, info.firma, info.model, info.greutate);
	nou->next = next;
	return nou;
}
nodLS* inserareInceput(nodLS* cap, Frigider f) { //unde, ce
	nodLS* nou = creareNod(f, NULL);
	if (cap) {
		nou->next = cap;
		cap = nou;
		return cap;
	}
	else {
		cap = nou;
		return cap;
	}
}

//insereaza in lista doar nodurile cu greutatea peste cea data in main;
nodLS* inserareABCInLSI(nodLS* lista, nod* arbore, float greutate) {
	if (arbore) {
		if (arbore->info.greutate > greutate) {
			lista = inserareInceput(lista, arbore->info);
		}
		lista = inserareABCInLSI(lista, arbore->st, greutate);
		lista = inserareABCInLSI(lista, arbore->dr, greutate);
	}
	return lista;
}


void afisareLista(nodLS* cap) {
	if (cap) {
		nodLS* p = cap;
		while (p) {
			afisareFrigider(p->info);
			p = p->next;
		}
	}
}

//6. DEZALOCARI
nodLS* stergereLista(nodLS* cap) {
	while (cap) {
		nodLS* aux = cap;
		cap = cap->next;
		free(aux->info.firma);
		free(aux->info.model);
		free(aux);
	}
	return cap;
}

void stergereArbore(nod** root) {
	if (*root) {
		stergereArbore(&(*root)->st);
		stergereArbore(&(*root)->dr);
		free((*root)->info.firma);
		free((*root)->info.model);
		free(*root);
		*root = NULL;
	}
}


void main() {
	//1
	nod* root = NULL;
	int nrelem = 0;
	FILE* f = fopen("bilet6.txt", "r");
	root = initializarearbore(f, &nrelem);
	SRD(root);
	printf("\n----------------------------------------------\n");

	//2
	int min = INT_MAX;
	pretMinim(root, &min);
	printf("Pretul minim: %d", min);
	printf("\n----------------------------------------------\n");

	//3
	int maxim = 0;
	pretMaxim(root, &maxim);
	Frigider rez = extragereFrigiderDinArbore(&root, maxim);
	afisareFrigider(rez);
	printf("\n----------------------------------------------\n");

	//4
	int maximm = 0;
	pretMaxim(root, &maximm);
	Frigider fr = cautareInArbore(root, maximm);
	afisareFrigider(fr);
	//exista o problema la cautare

	root = modificaPret(root, min);
	SRD(root);
	printf("\n----------------------------------------------\n");

	//5
	nodLS* lista=NULL;
	lista = inserareABCInLSI(lista, root, 50);
	afisareLista(lista);
	printf("\n----------------------------------------------\n");
	
	//6
	stergereArbore(&root);
	printf("\nAfisare dupa dezalocare\n");
	SRD(root);
	stergereLista(lista);
	printf("\n----------------------------------------------\n");
}