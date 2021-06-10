#include<iostream>

using namespace std;

struct Localitate {
	int id;
	char* denumire;
	char* numePrimar;
	float suprafata;
};
Localitate creareLocalitate(int id, const char* denumire, const char* numePrimar, float suprafata) {
	Localitate l;
	l.id = id;
	l.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(l.denumire, denumire);
	l.numePrimar = (char*)malloc(sizeof(char) * (strlen(numePrimar) + 1));
	strcpy(l.numePrimar, numePrimar);
	l.suprafata = suprafata;
	return l;
}

void afisareLocalitate(Localitate l) {
	printf("\n Localitatea %s cu id-ul %d are primarul %s si suprafata %5.2f.", l.denumire, l.id, l.numePrimar, l.suprafata);
}

Localitate citireLocalitateFisier(FILE* f) {
	Localitate l;

	fscanf(f, "%d", &l.id);

	char buffer[30];
	fscanf(f, "%s", &buffer);
	l.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(l.denumire, buffer);

	char buffer2[30];
	fscanf(f, "%s", &buffer2);
	l.numePrimar = (char*)malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(l.numePrimar, buffer2);

	fscanf(f, "%f", &l.suprafata);

	return l;
}

//structura pentru graf
struct  nodPrincipal;

struct nodSecundar {
	nodPrincipal* info;
	nodSecundar* next;
};

struct nodPrincipal {
	nodSecundar* vecini;
	Localitate info;
	nodPrincipal* next;
};

nodPrincipal* creareNodPrincipal(nodSecundar* vecini, Localitate info, nodPrincipal* next) {
	nodPrincipal* nou = (nodPrincipal*)malloc(sizeof(nodPrincipal));
	nou->vecini = vecini;
	nou->info = info;
	nou->next = next;
	return nou;
}

nodSecundar* creareNodSecundar(nodPrincipal* info, nodSecundar* next) {
	nodSecundar* nou = (nodSecundar*)malloc(sizeof(nodSecundar));
	nou->info = info;
	nou->next = next;
	return nou;
}

nodPrincipal* inserareListaPrincipala(nodPrincipal* cap, Localitate info) {
	nodPrincipal* nou = creareNodPrincipal(NULL, info, NULL);
	if (cap) {
		nodPrincipal* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
		return cap;
	}
	else {
		cap = nou;
		return cap;
	}

}

nodSecundar* inserareListaSecundara(nodSecundar* cap, nodPrincipal* info) {
	nodSecundar* nou = creareNodSecundar(info, NULL);
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

void afisareListaSecundara(nodSecundar* cap) {
	while (cap) {
		printf("%d, ", cap->info->info.id);
		cap = cap->next;
	}
}

void afisareListaPrincipala(nodPrincipal* cap) {
	while (cap) {
		afisareLocalitate(cap->info);
		printf("\nVecinii: \n");
		afisareListaSecundara(cap->vecini);
		cap = cap->next;
	}
}

nodPrincipal* cautaredupaID(nodPrincipal* cap, int idcautat) {
	while (cap && cap->info.id != idcautat) {
		cap = cap->next;
	}
	return cap;
}

void inserareArcInGraf(nodPrincipal* graf, int idStart, int idStop) {
	if (graf) {
		nodPrincipal* nodStart = cautaredupaID(graf, idStart);
		nodPrincipal* nodStop = cautaredupaID(graf, idStop);

		if (nodStart && nodStop) {
			nodStart->vecini = inserareListaSecundara(nodStart->vecini, nodStop);
			nodStop->vecini = inserareListaSecundara(nodStop->vecini, nodStart);
		}

	}
}
//citire din fisier
nodPrincipal* citireCuInserare(FILE* f) {
	nodPrincipal* graf = NULL;
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		graf = inserareListaPrincipala(graf, citireLocalitateFisier(f));

	}
	return graf;
}

//determinarea loc cu nr maxim de vecini
struct nod {
	int id;
	nod* next;
};

nod* creareNod(int id, nod* next) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->id = id;
	nou->next = next;
	return nou;
}

nod* pushCoada(nod* cap, int id) {
	nod* nou = creareNod(id, NULL);
	if (cap) {
		nod* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
		return cap;
	}
	else {
		cap = nou;
		return cap;
	}
}

int popCoada(nod** cap) {
	if (*cap) {
		int rez = (*cap)->id;
		nod* temp = (*cap);
		(*cap) = (*cap)->next;
		free(temp);
		return rez;

	}
	else {
		return -1;
	}
}

int calculNumarNoduri(nodPrincipal* cap) {
	int nr = 0;
	while (cap) {
		nr++;
		cap = cap->next;
	}
	return nr;
}

void parcurgereLatime(nodPrincipal* graf, int idNodStart) {
	if (graf) {
		nod* coada = NULL;
		int nrNoduri = calculNumarNoduri(graf);
		int* vizitate = (int*)malloc(sizeof(int) * nrNoduri);
		for (int i = 0; i < nrNoduri; i++) {
			vizitate[i] = 0;
		}

		coada = pushCoada(coada, idNodStart);
		vizitate[idNodStart] = 1;

		while (coada) {
			int idNodCurent = popCoada(&coada);
			nodPrincipal* nodCurent = cautaredupaID(graf, idNodCurent);
			afisareLocalitate(nodCurent->info);
			nodSecundar* p = nodCurent->vecini;
			while (p) {
				if (vizitate[p->info->info.id] == 0) {
					coada = pushCoada(coada, p->info->info.id);
					vizitate[p->info->info.id] = 1;
				}
				p = p->next;
			}
		}

	}

}

//cate noduri are lista simpla
int nrNoduriVecini(nodSecundar* cap) {
	int  nr = 0;
	while (cap) {
		nr++;
		cap = cap->next;
	}
	return nr;
}

int cautaMaximDeVecini(nodPrincipal* cap) {
	nodPrincipal* p = cap;
	int maxim = -1;
	while (p) {
		if (maxim < nrNoduriVecini(p->vecini)) {
			maxim = nrNoduriVecini(p->vecini);
		}
		p = p->next;
	}
	return maxim;
}

void afiseazaLocalitatiCuNrDeVeciniMaxim(nodPrincipal* cap) {
	nodPrincipal* p = cap;
	int maxim = cautaMaximDeVecini(cap);
	while (p) {
		if (nrNoduriVecini(p->vecini) == maxim) {
			printf("\n %s", p->info.denumire);
		}
		p = p->next;
	}
}

//exercitiul 3 abc

struct nodA {
	Localitate info;
	nodA* st;
	nodA* dr;
};
nodA* creareNod(Localitate l, nodA* st, nodA* dr) {
	nodA* nou = (nodA*)malloc(sizeof(nodA));
	nou->info = creareLocalitate(l.id, l.denumire, l.numePrimar, l.suprafata);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nodA* inserareABC(nodA* root, Localitate l) {
	if (root) {
		//valorile mai mici in stanga, valorile mai mari in dreapta
		if (l.id > root->info.id) {
			root->dr = inserareABC(root->dr, l);
		}
		else if (l.id < root->info.id) {
			root->st = inserareABC(root->st, l);
		}
		return root;
	}
	else {
		nodA* nou = creareNod(l, NULL, NULL);
		return nou;
	}
}
void conversie(nodPrincipal* graf, nodA** root, char primaLitera) {
	nodPrincipal* temp = graf;
	while (temp) {
		if (temp->info.denumire[0] == primaLitera) {
			*root = inserareABC(*root, temp->info);
		}
		temp = temp->next;
	}
}
void afisareArboreSRD(nodA* root) {
	if (root) {
		afisareArboreSRD(root->st);
		afisareLocalitate(root->info);
		afisareArboreSRD(root->dr);
	}
}

//4. extragerea nodurilor frunza in vector (mutare)

void numaraNoduriFrunzaArbore(nodA* root, int* nr) {

	if (root) {
		numaraNoduriFrunzaArbore(root->st, nr);
		numaraNoduriFrunzaArbore(root->dr, nr);
		if (root->st == NULL && root->dr == NULL) {
			(*nr)++;
		}

	}
}
void puneNoduriFrunzaDinArboreInVector(nodA* root, Localitate** vector, int* contor) {
	if (root) {
		if (root->st == NULL && root->dr == NULL) {
			(*vector)[*contor] = creareLocalitate(root->info.id, root->info.denumire, root->info.numePrimar, root->info.suprafata);
			(*contor)++;
		}
		puneNoduriFrunzaDinArboreInVector(root->st, vector, contor);
		puneNoduriFrunzaDinArboreInVector(root->dr, vector, contor);
	}
}
nodA* cautareMaxim(nodA* rad) {
	if (rad) {
		while (rad->dr) {
			rad = rad->dr;
		}
		return rad;
	}
}
void stergereNodArbore(nodA** root, int nrlocuitori) {
	if (*root) {
		if (nrlocuitori < (*root)->info.id) {
			stergereNodArbore(&(*root)->st, nrlocuitori);
		}
		else if (nrlocuitori > (*root)->info.id ) {
			stergereNodArbore(&(*root)->dr, nrlocuitori);
		}
		else {
			if ((*root)->st == NULL) {
				nodA* temp = (*root)->dr;
				free((*root)->info.denumire);
				free((*root));
				(*root) = temp;
			}
			else if ((*root)->dr == NULL) {
				nodA* temp = (*root)->st;
				free((*root)->info.denumire);
				free((*root));
				(*root) = temp;
			}
			else {
				nodA* temp = cautareMaxim((*root)->st);
				Localitate aux = (*root)->info;
				(*root)->info = temp->info;
				temp->info = aux;
				stergereNodArbore(&(*root)->st, nrlocuitori);
			}
		}
	}
}
void stergeNodurileFrunza(nodA** root, Localitate* vector, int nrloc) {
	for (int i = 0; i < nrloc; i++) {
		stergereNodArbore(root, vector[i].id);
	}
}
void afisareVector(Localitate* vector, int nr) {
	printf("\n Afisare vector");
	for (int i = 0; i < nr; i++) {
		afisareLocalitate(vector[i]);
	}
}

//dezalocari
nodPrincipal* dezalocareListaPrincipala(nodPrincipal* cap) {
	while (cap) {
		nodPrincipal* aux = cap;
		cap = cap->next;
		free(aux->info.denumire);
		free(aux);
	}
	return cap;
}

nodSecundar* dezalocareListaSecundara(nodSecundar* cap) {
	while (cap) {
		nodSecundar* aux = cap;
		cap = cap->next;
		free(aux->info->info.denumire);
		free(aux);
	}
	return cap;
}

void dezalocareArbore(nodA** root) {
	if (*root) {
		dezalocareArbore(&(*root)->st);
		dezalocareArbore(&(*root)->dr);
		free((*root)->info.denumire);
		free(*root);
		*root = NULL;
	}
}

void dezalocareVector(Localitate** vector, int* dim) {
	*dim = 0;
	free(*vector);
	*vector = NULL;
}

void main() {
	printf("\n-----------------------Exercitiul 1--------------------------------\n");
	nodPrincipal* graf = NULL;
	FILE* f = fopen("localitate.txt", "r");
	graf = citireCuInserare(f);

	inserareArcInGraf(graf, 0, 1);
	inserareArcInGraf(graf, 2, 3);
	inserareArcInGraf(graf, 2, 4);
	inserareArcInGraf(graf, 4, 5);
	inserareArcInGraf(graf, 5, 3);
	inserareArcInGraf(graf, 6, 4);
	inserareArcInGraf(graf, 6, 7);
	inserareArcInGraf(graf, 7, 8);
	inserareArcInGraf(graf, 7, 9);

	afisareListaPrincipala(graf);

	printf("\n-----------------------Exercitiul 2--------------------------------\n");
	printf("\n\nAfiseaza localitati cu nr de vecini maaxim: \n");
	afiseazaLocalitatiCuNrDeVeciniMaxim(graf);

	printf("\n-----------------------Exercitiul 3--------------------------------\n");
	nodA* arbore = NULL;
	conversie(graf, &arbore, 'e');
	printf("\nAfisare arbore\n");
	afisareArboreSRD(arbore);

	printf("\n-----------------------Exercitiul 4--------------------------------\n");
	int nr1 = 0;
	numaraNoduriFrunzaArbore(arbore, &nr1);
	Localitate* vector = (Localitate*)malloc(sizeof(Localitate) * nr1);
	int contor = 0;
	puneNoduriFrunzaDinArboreInVector(arbore, &vector, &contor);
	stergeNodurileFrunza(&arbore, vector, nr1);
	printf("\nAfisare VECTOR dupa copiere din arbore \n");
	afisareVector(vector, nr1);
	printf("\n\n");
	printf("\n Afisare arbore dupa extragere : \n");
	afisareArboreSRD(arbore);
}