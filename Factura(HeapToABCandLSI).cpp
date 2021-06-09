#include<string>
#include<iostream>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

//1.
struct Factura {
	int cod;
	char* denumireSocietate;
	char* numeClient;
	int nrZile; //cheia de prioriatte
	float valoare;
};
Factura creareFactura(int cod, const char* denumireSocietate, const char*numeClient, int nrZile, float valoare) {
	Factura fc;
	fc.cod = cod;
	fc.denumireSocietate = (char*)malloc(sizeof(char*) * (strlen(denumireSocietate) + 1));
	strcpy(fc.denumireSocietate, denumireSocietate);
	fc.numeClient = (char*)malloc(sizeof(char*) * (strlen(numeClient) + 1));
	strcpy(fc.numeClient, numeClient);
	fc.nrZile = nrZile;
	fc.valoare = valoare;
	return fc;
}

void afisareFactura(Factura fc) {
	printf("\nFactura %d de la %s a clientului %s trebuie platita in %d zile si are valoarea %5.2f.", fc.cod, fc.denumireSocietate, fc.numeClient, fc.nrZile, fc.valoare);
}

//citire factura din fisier
Factura citireFacturaFisier(FILE* f) {
	Factura fc;

	fscanf(f, "%d", &fc.cod);

	char buffer[30];
	fscanf(f, "%s", &buffer);
	fc.denumireSocietate = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(fc.denumireSocietate, buffer);

	char buffer2[30];
	fscanf(f, "%s", &buffer2);
	fc.numeClient = (char*)malloc(sizeof(char) * (strlen(buffer2) + 1));
	strcpy(fc.numeClient, buffer2);

	fscanf(f, "%d", &fc.nrZile);

	fscanf(f, "%f", &fc.valoare);

	return fc;
}

//structura heap
struct Heap {
	Factura* vector;
	int dim;
};

Heap creareHeap(int dim) {
	Heap h;
	h.dim = dim;
	h.vector = (Factura*)malloc(sizeof(Factura) * dim);
	return h;
}

void filtrare(Heap h, int poz) {
	if (h.vector) {
		if (poz <= (h.dim - 1) / 2) {
			int fiuS = 2 * poz + 1;
			int fiuD = 2 * poz + 2;
			int pozMax = poz;
			if (fiuS<h.dim && h.vector[fiuS].nrZile>h.vector[pozMax].nrZile) {
				pozMax = fiuS;
			}
			if (fiuD<h.dim && h.vector[fiuD].nrZile>h.vector[pozMax].nrZile) {
				pozMax = fiuD;
			}

			if (pozMax != poz) {
				Factura aux = h.vector[poz];
				h.vector[poz] = h.vector[pozMax];
				h.vector[pozMax] = aux;
				if (pozMax <= (h.dim - 1) / 2) {
					filtrare(h, pozMax);
				}
			}
		}
	}
}
void afisareHeap(Heap h) {
	if (h.vector) {
		for (int i = 0; i < h.dim; i++) {
			afisareFactura(h.vector[i]);
		}
	}
}
Heap inserareheap(Heap h, Factura fc) {
	int dimensiune = h.dim + 1;
	Factura* copie = (Factura*)malloc(sizeof(Factura) * dimensiune);
	for (int i = 0;i < h.dim;i++) {
		copie[i] = h.vector[i];

	}
	copie[dimensiune - 1] = fc;

	h.dim = dimensiune;
	free(h.vector);
	h.vector = (Factura*)malloc(sizeof(Factura) * dimensiune);
	for (int i = 0;i < h.dim;i++) {
		h.vector[i] = copie[i];
	}
	for (int i = (h.dim - 1) / 2;i >= 0;i--) {
		filtrare(h, i);
	}
	return h;
}

//extragere factura
Factura extragereHeap(Heap& heap) {
	if (heap.dim > 0) {
		Factura rezultat = heap.vector[0];
		Factura* temp = (Factura*)malloc(sizeof(Factura) * (heap.dim - 1));
		for (int i = 1; i < heap.dim; i++) {
			temp[i - 1] = heap.vector[i];
		}
		free(heap.vector);
		heap.vector = temp;
		heap.dim--;
		for (int i = (heap.dim - 2) / 2; i >= 0; i--) {
			filtrare(heap, i);
		}
		return rezultat;
	}
	else {
		return creareFactura(0, "", "", -1, 0);
	}
}

void afisareConditionata(Heap& heap) {
	while (heap.vector[0].nrZile >= 15) { //mai mici de 15
		Factura fc = extragereHeap(heap);
		afisareFactura(fc);
		free(fc.denumireSocietate);
		free(fc.numeClient);
	}
}

//structura abc
struct nodA {
	Factura info;
	nodA* st;
	nodA* dr;
};
nodA* creareNod(Factura info, nodA* st, nodA* dr) {
	nodA* nou = (nodA*)malloc(sizeof(nodA));
	nou->info = creareFactura(info.cod, info.denumireSocietate, info.numeClient, info.nrZile, info.valoare);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
//inseram in arbore in functie de cheia noastra de cautare
nodA* inserareArbore(nodA* root, Factura info) {
	if (root) {
		if (info.cod < root->info.cod) {
			root->st = inserareArbore(root->st, info);
		}
		else if (info.cod > root->info.cod) {
			root->dr = inserareArbore(root->dr, info);
		}
		return root;
	}
	else {
		nodA* nou = creareNod(info, NULL, NULL);
		return nou;
	}
}
nodA* inseraredinHeapinarbore(Heap h, int n) {
	if (h.vector) {
		nodA* root = NULL;
		if (n <= h.dim) {

			for (int i = 0; i < n; i++) {
				if (h.vector[i].cod < 103) {
					root = inserareArbore(root, h.vector[i]);
				}

			}
		}
		return root;

	}
	else return NULL;
}

void afisareArboreSRD(nodA* root) {
	if (root) {
		afisareArboreSRD(root->st);
		afisareFactura(root->info);
		afisareArboreSRD(root->dr);
	}
}

//mutare in lista simpla
struct nodLS {
	Factura info;
	nodLS* next;
};

nodLS* creareNod(Factura info, nodLS* next) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info = creareFactura(info.cod, info.denumireSocietate, info.numeClient, info.nrZile, info.valoare);
	nou->next = next;
	return nou;
}
nodLS* inserareInceput(nodLS* cap, Factura f) { //unde, ce
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
void mutaHeapInListaSimpla(nodA** arbore, nodLS** cap, int cod) {
	if (*arbore) {
		//rsd
		if (((*arbore)->info).cod < cod) {

		*cap = inserareInceput(*cap, (*arbore)->info);
		}

		mutaHeapInListaSimpla(&(*arbore)->st, cap, cod);
		mutaHeapInListaSimpla(&(*arbore)->dr, cap, cod);
	}
}
void afisareLista(nodLS* cap) {
	if (cap) {
		nodLS* p = cap;
		while (p) {
			afisareFactura(p->info);
			p = p->next;
		}
	}
}

//dezalocari
nodLS* stergereLista(nodLS* cap) {
	while (cap) {
		nodLS* aux = cap;
		cap = cap->next;
		free(aux->info.numeClient);
		free(aux->info.denumireSocietate);
		free(aux);
	}
	return cap;
}

void stergereArbore(nodA** root) {
	if (*root) {
		stergereArbore(&(*root)->st);
		stergereArbore(&(*root)->dr);
		free((*root)->info.denumireSocietate);
		free((*root)->info.numeClient);
		free(*root);
		*root = NULL;
	}
}

void dezalocareHeap(Heap& h)
{
	if (h.vector) {
		for (int i = 0; i < h.dim; i++) {
			free(h.vector[i].denumireSocietate);
			free(h.vector[i].numeClient);
		}
		free(h.vector);
	}
}
void main() {
	Factura fc = creareFactura(100, "Enel", "Popescu", 2, 679.5);
	afisareFactura(fc);
	printf("\n----------------------------------------------\n");


	//1.citire facturi din fisier text
	Heap h = creareHeap(5);
	FILE* f = fopen("factura.txt", "r");
	int i = 0;
	while (!feof(f)) {
		Factura fact = citireFacturaFisier(f);
		h.vector[i] = fact;
		i++;
	}
	fclose(f);
	afisareHeap(h);
	printf("\n----------------------------------------------\n");

	//afisare elemente dupa filtrare
	for (int i = (h.dim - 1) / 2;i >= 0;i--) {
		filtrare(h, i);
	}
	printf("\nAfisare heap dupa filtrare");
	afisareHeap(h);
	printf("\n----------------------------------------------\n");

	//2. extragere
	//afisareConditionata(h);
	//printf("\n----------------------------------------------\n");

	//3. to abc
	nodA* root = NULL;
	root = inseraredinHeapinarbore(h, 5);
	printf("\nAfisare arbore: \n");
	afisareArboreSRD(root);
	printf("\n----------------------------------------------\n");

	//4.
	nodLS* lista = NULL;
	mutaHeapInListaSimpla(&root, &lista, 102);
	afisareLista(lista);
	printf("\n----------------------------------------------\n");

	//5.
	stergereArbore(&root);
	stergereLista(lista);
	dezalocareHeap(h);
	printf("\n----------------------------------------------\n");
}