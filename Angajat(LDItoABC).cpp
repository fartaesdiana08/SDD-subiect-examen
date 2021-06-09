#include<iostream>
using namespace std;

struct Angajat {
	int id;
	char* nume;
	char* functie;
	float salariu;
	int varsta;
};
Angajat creareAngajat(int id, const char* nume, const char* functie, float salariu, int varsta) {
	Angajat a;
	a.id = id;
	a.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(a.nume, nume);
	a.functie = (char*)malloc(sizeof(char) * (strlen(functie) + 1));
	strcpy(a.functie, functie);
	a.salariu = salariu;
	a.varsta = varsta;
	return a;
}
void afisareAngajat(Angajat a) {
	printf("\n Angajatul cu id ul %d cu numele %s si functia %s are salariul %5.2f si varsta %d", 
		a.id, a.nume, a.functie, a.salariu, a.varsta);
}
Angajat citireAngajatFisier(FILE* f) {
	Angajat a;

	fscanf(f, "%d", &a.id);

	char buffer[30];
	fscanf(f, "%s", &buffer);
	a.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(a.nume, buffer);

	char buffer2[30];
	fscanf(f, "%s", &buffer2);
	a.functie = (char*)malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(a.functie, buffer2);

	fscanf(f, "%f", &a.salariu);

	fscanf(f, "%d", &a.varsta);

	return a;
}
//structura LDI
struct nodd {
	Angajat info;
	nodd* prev;
	nodd* next;
};

nodd* creareNodd(Angajat info, nodd* prev, nodd* next) {
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

LDI inserareInceput(LDI lista, Angajat s) {
	nodd* nou = creareNodd(s, NULL, NULL);
	if (lista.prim) {
		nou->next = lista.prim;
		lista.prim->prev = nou;
		lista.prim = nou;
		return lista;
	}
	else {
		lista.prim = lista.ultim = nou;
		return lista;
	}
}
void afisareinceputfinal(LDI lista) {
	if (lista.prim) {
		nodd* p = lista.prim;
		while (p) {
			afisareAngajat(p->info);
			p = p->next;
		}
	}
}
void afisarefinalinceput(LDI lista) {
	if (lista.prim) {
		for (nodd* p = lista.ultim;p != NULL;p = p->prev) {
			afisareAngajat(p->info);
		}
	}
}
//stergere din lista pe baza unui criteriu
Angajat extragereDinListaDupaFunctie(LDI lista, const char* functie) {
	while (lista.prim && strcmp(lista.prim->info.functie,functie)!=0) {
		lista.prim = lista.prim->next;
	}
	if (lista.prim) {
		Angajat a = lista.prim->info;
		if (lista.prim->prev) {
			lista.prim->prev->next = lista.prim->next;
			lista.prim->next->prev = lista.prim->prev;
			free(lista.prim);
		}
		else {
			nodd *temp = lista.prim;
			lista.prim = lista.prim->next;
			free(temp);
		}
		return a;
	}
	else {
		return creareAngajat(-1, "", "", 0, 0);
	}
}
void stergereDinListaDupaVarsta(LDI &lista, int varsta) {
	//STERGE ANGAJATII CU VARSTA PESTE CEA DATA
	nodd* temp = lista.prim;
	while (temp->info.salariu <= varsta) {
		temp = temp->next;
	}
	if (temp) {
		nodd* temp2 = temp;
		if (temp->prev) {
			temp->prev->next = temp->next;
			if (temp->next) {
				temp->next->prev = temp->prev;
			}
			else {
				lista.ultim = temp->prev;
			}
		}
		else {
			temp->next->prev = NULL;
			lista.prim = temp->next;
		}
		free(temp->info.nume);
		free(temp);
	}
}
int numaraElementeCriteriu(LDI lista, int criteriu) {
	if (lista.prim) {
		int nr = 0;
		for (nodd* i = lista.prim; i != NULL; i = i->next) {
			if (i->info.varsta > criteriu) {
				nr++;
			}
		}
		return nr;
	}
	else return 0;
}

//structura ABC
struct nodA {
	Angajat info;
	nodA* st;
	nodA* dr;
};
nodA* creareNod(Angajat info, nodA* st, nodA* dr) {
	nodA* nou = (nodA*)malloc(sizeof(nodA));
	nou->info = creareAngajat(info.id, info.nume, info.functie, info.salariu, info.varsta);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nodA* inserareArbore(nodA* root, Angajat info) { //id este cheie 
	if (root) {
		if (info.id < root->info.id) {
			root->st = inserareArbore(root->st, info);
		}
		else if (info.id > root->info.id) {
			root->dr = inserareArbore(root->dr, info);
		}
		return root;
	}
	else {
		nodA* nou = creareNod(info, NULL, NULL);
		return nou;
	}
}

nodA* inserareListaDublaInArbore(LDI lista, int varsta) {
	nodA* arbore = NULL;
	nodd* p = lista.prim;
	while (p) {
		if (p->info.varsta < varsta) {
			arbore = inserareArbore(arbore, p->info);
		}
		p = p->next;
	}
	return arbore;
}

void afisareArboreSRD(nodA* root) {
	if (root) {
		afisareArboreSRD(root->st);
		afisareAngajat(root->info);
		afisareArboreSRD(root->dr);
	}
}

//deazlocari
void dezalocareListaDubla(LDI* lista) {
	while (lista->prim) {
		nodd* aux = lista->prim;
		lista->prim = lista->prim->next;
		free(aux->info.nume);
		free(aux->info.functie);
		free(aux);
	}
	lista->ultim = NULL;
	printf("\nS-a dezalocat lista dubla!\n");
}

void dezalocareArbore(nodA** rad) {
	if (*rad) {
		dezalocareArbore(&(*rad)->st);
		dezalocareArbore(&(*rad)->dr);
		free((*rad)->info.nume);
		free((*rad)->info.functie);
		free(*rad);
		*rad = NULL;
		printf("\nS-a dezalocat arborele!\n");
	}
}

void dezalocareVector(Angajat* vectorAngajati, int* nr) {
	for (int i = 0; i < *nr; i++)
	{
		free(vectorAngajati[i].nume);
		free(vectorAngajati[i].functie);
	}
	free(vectorAngajati);
	printf("\nS-a dezalocat vectorul!\n");
}

void main() {
	//1. apel citire angajati
	LDI lista;
	lista.prim = lista.ultim = NULL;

	FILE* f = fopen("angajat.txt", "r");
	while (!feof(f)) {
		Angajat a = citireAngajatFisier(f);
		lista = inserareInceput(lista, a);
	}
	fclose(f);
	afisareinceputfinal(lista);
	printf("\n-----------------------------------------\n");

	//2.apel stergere din lista pe baza unui criteriu char
	afisareAngajat(extragereDinListaDupaFunctie(lista, "Functie3"));
	printf("\n");
	afisarefinalinceput(lista);
	printf("\n");
	afisareinceputfinal(lista);
	printf("\n-----------------------------------------\n");

	//2. varianta pt criteriu tip int
	int criteriu = 34;
	int nr = numaraElementeCriteriu(lista, criteriu);
	for (int i = 0; i < nr; i++) {
		stergereDinListaDupaVarsta(lista, criteriu);
	}
	printf("\n");
	afisarefinalinceput(lista);
	printf("\n");
	afisareinceputfinal(lista);
	printf("\n-----------------------------------------\n");

	//3.mutare in ABC angajatii cu varsta mai mica de 35
	nodA* root = NULL;
	root= inserareListaDublaInArbore(lista, 35);
	afisareArboreSRD(root);
	printf("\n-----------------------------------------\n");

	//4. dezalocari
	afisarefinalinceput(lista);
	dezalocareListaDubla(&lista);
	printf("Lista dupa dezalocare: ");
	afisarefinalinceput(lista);

	printf("\n-------------\n");

	afisareArboreSRD(root);
	dezalocareArbore(&root);
	printf("Lista dupa dezalocare: ");
	afisareArboreSRD(root);

}