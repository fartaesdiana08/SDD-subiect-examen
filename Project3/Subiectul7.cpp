#include<iostream>
#include<fstream>

using namespace std;

struct Camera {
	int nr;
	char* tip;
	int etaj;
};
//creare camera
Camera creareCamera(int nr, const char* tip, int etaj) {
	Camera c;
	c.nr = nr;
	c.tip = (char*)malloc(sizeof(char) * (strlen(tip) + 1));
	strcpy(c.tip, tip);
	c.etaj = etaj;
	return c;
}

//afisare camera
void afisareCamera(Camera c) {
	printf("Camera cu nr %d, este de tipul %s, si este plasata pe etajul %d\n", c.nr, c.tip, c.etaj);
}

//citirea unei camere din fisier
Camera citireCamera(FILE* f) {
	Camera c;
	fscanf(f, "%d", &c.nr); //citim un decimal care se duce in adresa lui c.nr;

	char buffer[30];
	fscanf(f, "%s", &buffer);
	c.tip = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(c.tip, buffer);

	fscanf(f, "%d", &c.etaj);

	return c;
}

//structura nod
struct nod {
	Camera info;
	nod* st;
	nod* dr;
};

//creare nod
nod* crearenod(Camera info, nod* st, nod* dr) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = info;
	nou->st = st;
	nou->dr = dr;
	return nou;
}
//inserare nod
nod* inserareABC(nod* root, Camera c) {
	if (root) {
		if (c.nr < root->info.nr) {
			root->st = inserareABC(root->st, c);
		}
		else if (c.nr > root->info.nr) {
			root->dr = inserareABC(root->dr, c);
		}
		return root;
	}
	else {
		nod* nou = crearenod(c, NULL, NULL);
		return nou;
	}
}
//afisarer srd
void SRD(nod* root) {
	if (root) {
		SRD(root->st);
		afisareCamera(root->info);
		SRD(root->dr);
	}
}

//initializare arbore
//intoarce arborele populat
nod* initializarearbore(FILE* f, int* nrelem) {
	nod* root = NULL;
	fscanf(f, "%d", nrelem);
	for (int i = 0;i < *nrelem;i++) {
		Camera c = citireCamera(f);
		root = inserareABC(root, c);
	}
	return root;
}

//functie care cauta etajul maxim
//este recursiva pentru ca suntem in arbore
void etajMaxim(nod* root, int* maxim) {
	if (root) {//rsd
		if (root->info.etaj > *maxim) {
			*maxim = root->info.etaj;
		}
		etajMaxim(root->st, maxim);
		etajMaxim(root->dr, maxim);
	}
}

//cate camere sunt pe un anumit etaj
void numaraCamerePeEtaj(nod* root, int etaj, int* nr) {
	//nr se va intoarce modificat;
	if (root) {//rsd
		if (root->info.etaj == etaj) {
			(*nr)++; //modifica valoare pt ca are paranteze;
		}
		numaraCamerePeEtaj(root->st, etaj, nr);
		numaraCamerePeEtaj(root->dr, etaj, nr);
	}
}
int* puneNrCamereInVector(nod* root, int* nr) {
	etajMaxim(root, nr);
	int* vect = (int*)malloc(sizeof(int) * (*nr + 1));

	for (int i = 0; i <= *nr;i++) {
		int k = 0;
		numaraCamerePeEtaj(root, i, &k);
		vect[i] = k;
	}
	return vect;
}
//pune in vector nr de camere de pe fiecare etaj

struct nodd {
	Camera info;
	nodd* prev;
	nodd* next;
};

nodd* creareNodd(Camera info, nodd* prev, nodd* next) {
	nodd* nou = (nodd*)malloc(sizeof(nodd));
	nou->info = info;
	nou->prev = prev;
	nou->next = next;
	return nou;
}

struct LDI {
	nodd* prim;
	nodd* ultim;
};

LDI inserareFinal(LDI lista, Camera c) {
	nodd* nou = creareNodd(c, NULL, NULL);
	if (lista.prim) {
		nou->prev = lista.ultim;
		lista.ultim->next = nou;
		lista.ultim = nou;
		return lista;
	}
	else {
		lista.prim = lista.ultim = nou;
		return lista;
	}
}

void afisareLista(LDI lista) {
	if (lista.prim) {
		for (nodd* p = lista.prim;p != NULL; p = p->next) {
			afisareCamera(p->info);
		}
	}
}

void mutadinABCinLDI(nod* root, LDI* lista, const char* tip) {
	if (root) {//rsd
		if (strcmp(root->info.tip, tip) == 0) {
			*lista = inserareFinal(*lista, creareCamera(root->info.nr, root->info.tip, root->info.etaj));
		}
		mutadinABCinLDI(root->st, lista, tip);
		mutadinABCinLDI(root->dr, lista, tip);
	}
}

LDI inserareDupaNod(LDI lista, Camera c, int nr) {
	nodd* p = lista.prim;
	while (p && p->info.nr != nr) {
		p = p->next;
	}
	if (p) {
		nodd* nou = creareNodd(c, NULL, NULL);
		nou->next = p->next;
		nou->prev = p;
		p->next->prev = nou;
		p->next = nou;
	}
	return lista;
}

void main() {
	nod* root = NULL;
	int nrelem = 0;
	FILE* f = fopen("Camere.txt", "r");
	root = initializarearbore(f, &nrelem);
	SRD(root);

	int nr = 0;
	int* vect = puneNrCamereInVector(root, &nr);
	for (int i = 0; i <= nr;i++) {
		printf("Pe etajul %d sunt %d camere.\n", i, vect[i]);
	}

	LDI lista;
	lista.prim = lista.ultim = NULL;
	mutadinABCinLDI(root, &lista, "economy");
	printf("\nAfisare lista LDI dupa mutare:\n");
	afisareLista(lista);
	//structuri care nu partajeaza zonele de memorie => deep copy
}