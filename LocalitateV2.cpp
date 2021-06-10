#include<iostream>

using namespace std;		// BILET #15

struct Localitate {
	int id;
	char* denumire;
	int nrLocuitori;
};

Localitate creareLocalitate(int id, char* denumire, int nrLocuitori) {
	Localitate l;
	l.id = id;
	l.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(l.denumire, denumire);
	l.nrLocuitori = nrLocuitori;

	return l;
}

Localitate citireLocalitateDinFisier(FILE* f) {
	Localitate l;
	fscanf(f, "%d", &l.id);
	char buffer[25];
	fscanf(f, "%s", buffer);
	l.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(l.denumire, buffer);
	fscanf(f, "%d", &l.nrLocuitori);

	return l;
}

void afisareLocalitate(Localitate l) {
	printf("\nLocalitatea cu id-ul %d are denumirea %s si %d locuitori ", l.id, l.denumire, l.nrLocuitori);
}

struct nodPrincipal;

struct nodSecundar {
	nodPrincipal* info;
	nodSecundar* next;
};

struct nodPrincipal {
	Localitate info;
	nodPrincipal* next;
	nodSecundar* vecini;
};

nodSecundar* creareNodSecundar(nodPrincipal* info, nodSecundar* next) {
	nodSecundar* nou = (nodSecundar*)malloc(sizeof(nodSecundar));
	nou->info = info;
	nou->next = next;

	return nou;
}

nodPrincipal* creareNodPrincipal(Localitate info, nodPrincipal* next, nodSecundar* vecini) {
	nodPrincipal* nou = (nodPrincipal*)malloc(sizeof(nodPrincipal));
	nou->info = info;
	nou->next = next;
	nou->vecini = vecini;

	return nou;
}

nodSecundar* inserareNodSecundar(nodSecundar* cap, nodPrincipal* info) {
	nodSecundar* nou = creareNodSecundar(info, NULL);
	if (cap) {
		nodSecundar* p = cap;
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

nodPrincipal* inserareNodPrincipal(nodPrincipal* cap, Localitate info) {
	nodPrincipal* nou = creareNodPrincipal(info, NULL, NULL);
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

nodPrincipal* inserareLocalitatiCititeDinFisierInGraf(FILE* f) {
	nodPrincipal * graf = NULL;
	int nr = 0;
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		graf = inserareNodPrincipal(graf, citireLocalitateDinFisier(f));

	}
	return graf;
}

nodPrincipal* cautareDupaId(nodPrincipal* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareArcInGraf(nodPrincipal* graf, int idStart, int idStop) {
	if (graf) {
		nodPrincipal* nodStart = cautareDupaId(graf, idStart);
		nodPrincipal* nodStop = cautareDupaId(graf, idStop);
		if (nodStart && nodStop) {
			nodStart->vecini = inserareNodSecundar(nodStart->vecini, nodStop);
			nodStop->vecini = inserareNodSecundar(nodStop->vecini, nodStart);
		}
	}
}

void afisareNodSecundar(nodSecundar* cap) {
	if (cap) {
		while (cap) {
			printf("\n %d %s", cap->info->info.id, cap->info->info.denumire);
			cap = cap->next;
		}
	}
}

void afisareNodPrincipal(nodPrincipal* cap) {
	if (cap) {
		while (cap) {
			afisareLocalitate(cap->info);
			printf("\nVecinii : \n");
			afisareNodSecundar(cap->vecini);
			cap = cap->next;
		}
	}
}

Localitate extragereLocalitateDinGraf(nodPrincipal** graf, int id) {
	if (*graf) {
		if ((*graf)->info.id == id) {
			Localitate rezultat = (*graf)->info;
			nodPrincipal* temp = (*graf);
			(*graf) = (*graf)->next;
			free(temp);
			return rezultat;
		}
		else {
			nodPrincipal* copie1 = (*graf);
			nodPrincipal* copie2 = (*graf)->next;
			while (copie2 && (*graf)->info.id == id) {
				copie1 = copie1->next;
				copie2 = copie2->next;
			}
			if (copie2) {
				Localitate rezultat = copie2->info;
				nodPrincipal* temp = copie2;
				copie1->next = copie2->next;
				free(temp);
				return rezultat;
			}
		}
	}
}

int nrNoduriSecundare(nodSecundar* cap) {
	int nr = 0;
	while (cap) {
		nr++;
		cap = cap->next;
	}
	return nr;
}

int cautaNrMaximConexiuni(nodPrincipal* graf) {
	int maxim = 0;
	while (graf) {
		nodSecundar* p = graf->vecini;
		if (maxim < nrNoduriSecundare(p)) {
			maxim = nrNoduriSecundare(p);
		}
		graf = graf->next;
	}
	return maxim;
}

int numaraElementeCuNrMaximDeConexiuni(nodPrincipal* graf) {
	int nr = 0;
	int maxim = cautaNrMaximConexiuni(graf);
	while (graf) {
		nodSecundar* p = graf->vecini;
		if (maxim == nrNoduriSecundare(p)) {
			nr++;
		}
		graf = graf->next;
	}
	return nr;
}

char** vectorLocalitatiMaximConexiuni(nodPrincipal* graf) {
	int nr = numaraElementeCuNrMaximDeConexiuni(graf);
	char** vector = (char**)malloc(sizeof(char*)*nr);
	int maxim = cautaNrMaximConexiuni(graf);
	int contor = 0;
	while (graf) {
		nodSecundar* p = graf->vecini;
		if (maxim == nrNoduriSecundare(p)) {
			vector[contor] = (char*)malloc(sizeof(char)*(strlen(graf->info.denumire) + 1));
			strcpy(vector[contor], graf->info.denumire);
			contor++;
		}
		graf = graf->next;
	}
	return vector;
}

struct nodArbore {
	Localitate info;
	nodArbore* st;
	nodArbore* dr;
};

nodArbore* creareNodArbore(Localitate info, nodArbore* st, nodArbore* dr) {
	nodArbore* nou = (nodArbore*)malloc(sizeof(nodArbore));
	nou->info = creareLocalitate(info.id, info.denumire, info.nrLocuitori);
	nou->st = st;
	nou->dr = dr;

	return nou;
}

nodArbore* inserareInArbore(nodArbore* root, Localitate l) {
	if (root) {
		if (l.nrLocuitori < root->info.nrLocuitori) {
			root->st = inserareInArbore(root->st, l);
		}
		else if (l.nrLocuitori > root->info.nrLocuitori) {
			root->dr = inserareInArbore(root->dr, l);
		}
		return root;
	}
	else {
		return creareNodArbore(l, NULL, NULL);
	}
}

void afisareRSD(nodArbore* root) {
	if (root) {
		afisareLocalitate(root->info);
		afisareRSD(root->st);
		afisareRSD(root->dr);
	}
}

void copiereDinGrafInArbore(nodPrincipal* graf, nodArbore** root,  char litera) {
	nodPrincipal* temp = graf;
	while (temp) {
		if (temp->info.denumire[0] == litera) {
			*root = inserareInArbore(*root, temp->info);
		}
		temp = temp->next;
	}
}

void numaraNoduriFrunzaArbore(nodArbore* root, int*nr) {
	
	if (root) {
		numaraNoduriFrunzaArbore(root->st, nr);
		numaraNoduriFrunzaArbore(root->dr, nr);
		if (root->st == NULL && root->dr == NULL) {
			(*nr)++;
		}
		
	}
	
}

void puneNoduriFrunzaDinArboreInVector(nodArbore* root, Localitate** vector, int* contor) {
	if (root) {
		if (root->st == NULL && root->dr == NULL) {
			(*vector)[*contor] = creareLocalitate(root->info.id,root->info.denumire,root->info.nrLocuitori);
			(*contor)++;
		}
		puneNoduriFrunzaDinArboreInVector(root->st, vector,contor);
		puneNoduriFrunzaDinArboreInVector(root->dr, vector,contor);	
	}
}

nodArbore* cautareMaxim(nodArbore* rad) {
	if (rad) {
		while (rad->dr) {
			rad = rad->dr;
		}
		return rad;
	}
}

void stergereNodArbore(nodArbore** root, int nrlocuitori) {
		if (*root) {
			if (nrlocuitori < (*root)->info.nrLocuitori) {
				stergereNodArbore(&(*root)->st, nrlocuitori);
			}
			else if (nrlocuitori> (*root)->info.nrLocuitori) {
				stergereNodArbore(&(*root)->dr, nrlocuitori);
			}
			else {
				if ((*root)->st == NULL) {
					nodArbore* temp = (*root)->dr;
					free((*root)->info.denumire);
					free((*root));
					(*root) = temp;
				}
				else if ((*root)->dr == NULL) {
					nodArbore* temp = (*root)->st;
					free((*root)->info.denumire);
					free((*root));
					(*root) = temp;
				}
				else {
					nodArbore* temp = cautareMaxim((*root)->st);
					Localitate aux = (*root)->info;
					(*root)->info = temp->info;
					temp->info = aux;
					stergereNodArbore(&(*root)->st, nrlocuitori);
				}
			}
		}
}

void extragereLocalitateDinArbore(nodArbore** root, Localitate l) {
	if (*root) {
		if (l.nrLocuitori < (*root)->info.nrLocuitori) {
			extragereLocalitateDinArbore(&(*root)->st, l);
		}
		else if (l.nrLocuitori > (*root)->info.nrLocuitori) {
			extragereLocalitateDinArbore(&(*root)->dr, l);
		}

		else {
			if ((*root)->st == NULL) {
				Localitate rez = (*root)->info;
				nodArbore* temp = (*root)->dr;
				free((*root)->info.denumire);
				free(*root);
				*root = temp;
			}
			else if ((*root)->dr == NULL) {
				Localitate rez = (*root)->info;
				nodArbore* temp = (*root)->st;
				free((*root)->info.denumire);
				free(*root);
				*root = temp;
			}
			else {
				nodArbore* temp = cautareMaxim((*root)->st);
				Localitate aux = (*root)->info;
				(*root)->info = temp->info;
				temp->info = aux;
				extragereLocalitateDinArbore(&(*root)->st, l);
			}
		}
	}
}

void stergeNodurileFrunza(nodArbore** root, Localitate* vector, int nrloc) {
	for (int i = 0; i < nrloc; i++) {
		stergereNodArbore(root, vector[i].nrLocuitori);
	}
}
void afisareVector(Localitate* vector, int nr) {
	printf("\n Afisare vector");
	for (int i = 0; i < nr; i++) {
		afisareLocalitate(vector[i]);
	}

}

nodPrincipal* dezalocareListaPrincipala(nodPrincipal* cap) {
	while (cap) {
		nodPrincipal*aux = cap;
		cap = cap->next;
		free(aux->info.denumire);
		free(aux);
	}
	return cap;
}

nodSecundar* dezalocareListaSecundara(nodSecundar* cap) {
	while (cap) {
		nodSecundar*aux = cap;
		cap = cap->next;
		free(aux->info->info.denumire);
		free(aux);
	}
	return cap;
}

void dezalocareArbore(nodArbore** root) {
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

	printf("\n\n-------------------------- Exercitiul 1 ------------------------------\n");
	FILE* f = fopen("Localitati.txt", "r");
	nodPrincipal* graf = NULL;
	graf = inserareLocalitatiCititeDinFisierInGraf(f);

	inserareArcInGraf(graf, 1, 2);
	inserareArcInGraf(graf, 1, 3);
	inserareArcInGraf(graf, 3, 4);
	inserareArcInGraf(graf, 3, 5);
	inserareArcInGraf(graf, 5, 6);

	printf("\nAfisare GRAF dupa citirea din fisier \n");
	afisareNodPrincipal(graf);


	printf("\n\n-------------------------- Exercitiul 2 ------------------------------\n");
	printf("\nAfisare Vector localitati maxim conexiuni \n");
	int nrr = numaraElementeCuNrMaximDeConexiuni(graf);
	char** vect = vectorLocalitatiMaximConexiuni(graf);
	for (int i = 0;i < nrr;i++) {
		printf(" %s,", vect[i]);
	}

	printf("\n\n-------------------------- Exercitiul 3 ------------------------------\n");
	nodArbore* root = NULL;
	copiereDinGrafInArbore(graf, &root, 'a');
	printf("\nAfisare ARBORE dupa copiere din graf \n");
	afisareRSD(root);
	
	 
	printf("\n\n-------------------------- Exercitiul 4 ------------------------------\n");
	int nr1 = 0;
	numaraNoduriFrunzaArbore(root,&nr1);
	Localitate*vector=(Localitate*)malloc(sizeof(Localitate)*nr1);
	int contor = 0;
	puneNoduriFrunzaDinArboreInVector(root, &vector,&contor);
	stergeNodurileFrunza(&root, vector, nr1);
	printf("\nAfisare VECTOR dupa copiere din arbore \n");
	afisareVector(vector, nr1);
	printf("\n\n");
	printf("\n Afisare arbore dupa extragere : \n");
	afisareRSD(root);


	printf("\n\n-------------------------- Exercitiul extra ------------------------------\n");
	printf("\n\n");
	printf("\nAfisare localitate extrasa din graf\n");
	Localitate extras = extragereLocalitateDinGraf(&graf, 1);
	afisareLocalitate(extras);
	
	
	printf("\n\n-------------------------- Exercitiul 5 ------------------------------\n");
	graf = dezalocareListaPrincipala(graf);
	printf("\nAfisare GRAF dupa dezalocare \n");
	afisareNodPrincipal(graf);

	dezalocareArbore(&root);
	printf("\nAfisare ARBORE dupa dezalocare \n");
	afisareRSD(root);

	dezalocareVector(&vector, &nr1);
	printf("\nAfisare VECTOR dupa dezalocare \n");
	afisareVector(vector, nr1);


}