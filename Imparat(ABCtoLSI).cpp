#include<iostream>
using namespace std;

struct Imparat{
	char* nume;
	int anDomnie; //cheia de cautare
};
Imparat creareImparat(const char* nume, int anDomnie) {
	Imparat im;
	im.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(im.nume, nume);
	im.anDomnie = anDomnie;
	return im;
}
void afisareImparat(Imparat im) {
	printf("Imparatul %s si-a inceput domnia in anul %d.\n", im.nume, im.anDomnie);
}
Imparat citireImparatFisier(FILE* f) {
	Imparat im;

	char buffer[30];
	fscanf(f, "%s", &buffer);
	im.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(im.nume, buffer);

	fscanf(f, "%d", &im.anDomnie);

	return im;
}

//ARBORE BINAR DE CAUTARE
struct nodA {
	Imparat info;
	nodA* st;
	nodA* dr;
};
nodA* creareNod(Imparat im, nodA* st, nodA* dr) {
	nodA* nou = (nodA*)malloc(sizeof(nodA));
	nou->info = creareImparat(im.nume, im.anDomnie);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
//inseram in arbore in functie de cheia noastra de cautare
nodA* inserareABC(nodA* root, Imparat im) {
	if (root) {
		//valorile mai mici in stanga, valorile mai mari in dreapta
		if (im.anDomnie> root->info.anDomnie) {
			root->dr = inserareABC(root->dr, im);
		}
		else if (im.anDomnie < root->info.anDomnie) {
			root->st = inserareABC(root->st, im);
		}
		return root;
	}
	else {
		nodA* nou = creareNod(im, NULL, NULL);
		return nou;
	}
}

nodA* initializareArbore(FILE* f, int* nrelem) {
	nodA* root = NULL;
	fscanf(f, "%d", nrelem);
	for (int i = 0;i < *nrelem;i++) {
		Imparat im = citireImparatFisier(f);
		root = inserareABC(root, im);
	}
	return root;
}

void afisareArboreSRD(nodA* root) {
	if (root) {
		afisareArboreSRD(root->st);
		afisareImparat(root->info);
		afisareArboreSRD(root->dr);
	}
}

//cautare in ABC dupa an
Imparat cautareImparatDupaAn(nodA* rad, int anCautat) {
	if (rad) {
		if (rad->info.anDomnie == anCautat) {
			return rad->info;
		}
		else if (rad->info.anDomnie<anCautat) {
			return cautareImparatDupaAn(rad->dr, anCautat);
		}
		else {
			return cautareImparatDupaAn(rad->st, anCautat);
		}
	}
	else {
		return creareImparat("", -1);
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
nodA* stergereNod(nodA* root, int an) {
	if (root) {
		if (root->info.anDomnie == an) {
			if (root->st == NULL) {
				nodA* temp = root->dr;
				free(root->info.nume);
				free(root);
				return temp;
			}
			if (root->dr == NULL) {
				nodA* temp = root->st;
				free(root->info.nume);
				free(root);
				return temp;
			}

			nodA* temp = cautareMaxim(root->st);
			Imparat aux = root->info;
			root->info = temp->info;
			temp->info = aux;
			root->st = stergereNod(root->st, an);
			return root;

		}
		else if (root->info.anDomnie > an) {
			root->st = stergereNod(root->st, an);
		}
		else {
			root->dr = stergereNod(root->dr, an);
		}
	}
	return root;
}

//extragere nod=> este returnat nodul extras + nodul este eliminat din arbore
Imparat extrageImparatDinArbore(nodA** rad, int an) {
	if (*rad) {
		if (an > (*rad)->info.anDomnie) {
			return extrageImparatDinArbore(&(*rad)->dr, an);
		}
		else if (an < (*rad)->info.anDomnie) {
			return extrageImparatDinArbore(&(*rad)->st, an);
		}
		else {
			if ((*rad) -> st == NULL) {
				nodA* temp = (*rad)->dr;
				Imparat rezultat = (*rad)->info;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			if ((*rad)->dr == NULL) {
				nodA* temp = (*rad)->st;
				Imparat rezultat = (*rad)->info;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			nodA*temp = cautareMaxim((*rad)->st);
			Imparat aux = (*rad)->info;
			(*rad)->info = temp->info;
			temp->info = aux;
	
			return extrageImparatDinArbore(&(*rad)->st, an);
		}
	}
}

//copiere ABC in lista simpla inlantuita
struct nodLS {
	Imparat info;
	nodLS* next;
};

nodLS* crearenod(Imparat info, nodLS* next) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info = creareImparat(info.nume, info.anDomnie);//deep
	nou->next = next;
	return nou;
}

nodLS* inserareinceput(nodLS* cap, Imparat im) {
	nodLS* nou = crearenod(im, NULL);
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
//se copiaza in lista doar Imparatii cu anDomnie>1500;
void copiereImparatiInLista(nodLS** cap, nodA* rad) {
	if(rad){
		if (rad->info.anDomnie > 1500) {
			*cap = inserareinceput(*cap, rad->info);
		}
		copiereImparatiInLista(cap, rad->st);
		copiereImparatiInLista(cap, rad->dr);
	}
}
nodLS * ListaCuImparati(nodA* rad) {
	nodLS* cap = NULL;
	if (rad) {
		copiereImparatiInLista(&cap, rad);
		
	}
	return cap;
}
void afisareLista(nodLS* cap) {
	if (cap) {
		nodLS* p = cap;
		while (p) {
			afisareImparat(p->info);
			p = p->next;
		}
	}
}

//dezalocare arbore
void stergereArbore(nodA** rad) {
	if (*rad) {
		stergereArbore(&(*rad)->st);
		stergereArbore(&(*rad)->dr);
		free((*rad)->info.nume);
		free(*rad);
		*rad = NULL;
	}
}

//dezalocare LSI
nodLS* stergereLSI(nodLS* lista) {
	while (lista) {
		nodLS* aux = lista;
		lista = lista->next;
		free(aux->info.nume);
		free(aux);
	}
	return lista;
}


void main() {
	nodA* root = NULL;
	int nrelem = 0;
	FILE* f = fopen("imparat.txt", "r");
	root = initializareArbore(f, &nrelem);
	afisareArboreSRD(root);
	printf("\n----------------------------------------------\n");

	//apel cautare dupa an
	afisareImparat(cautareImparatDupaAn(root, 1600));
	printf("\n----------------------------------------------\n");


	//apel stergere nod
	afisareArboreSRD(root);
	stergereNod(root, 1600);
	printf("\n");
	afisareArboreSRD(root);
	printf("\n----------------------------------------------\n");

	//apel extragere
	afisareArboreSRD(root);
	printf("\n");
	afisareImparat(extrageImparatDinArbore(&root, 1400));
	printf("\n");
	afisareArboreSRD(root);
	printf("\n");
	printf("\n----------------------------------------------\n");

	//mutare in LSI
	nodLS* lista = NULL;
	lista = ListaCuImparati(root);
	afisareLista(lista);

	printf("\n----------------------------------------------\n");

	//apel dezalocare
	stergereArbore(&root);
	printf("\nAfisare arbore dupa dezalocare:\n");
	afisareArboreSRD(root);
	printf("\n----------------------------------------------\n");

	afisareLista(lista);
	lista = stergereLSI(lista);
	printf("\nAfisare lista dupa dezalocare:\n");
	afisareLista(lista);






}