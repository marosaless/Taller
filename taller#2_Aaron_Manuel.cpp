#include <iostream>
#include <fstream>
using namespace std;

const int infinito = 1000000000;

struct Nodo {
    int destino;
    int peso;
    Nodo* sig;
};

Nodo** grafo;
int vertices;



void inicializargrafo() {
    grafo = new Nodo*[vertices];
    for (int i = 0; i < vertices; i++) {
        grafo[i] = nullptr;
    }
}

int rango(int v) {
    return (v >= 0 && v < vertices);
}

void agregarArista(int u, int v, int peso) {
    if (!rango(u) || !rango(v)) return;

    grafo[u] = new Nodo{v, peso, grafo[u]};
    grafo[v] = new Nodo{u, peso, grafo[v]};
}


void cargarArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    archivo >> vertices;
    inicializargrafo();

    int u, v, peso;
    while (archivo >> u >> v >> peso) {
        agregarArista(u, v, peso);
    }

    archivo.close();
}

void prim(int origen) {
    int* visitado = new int[vertices];
    int* costo = new int[vertices];
    int* padre = new int[vertices];
    int cont=0;

    for (int i = 0; i < vertices; i++) {
        visitado[i] = 0;
        costo[i] = infinito;
        padre[i] = -1;
    }
    costo[origen] = 0;

    for (int count = 0; count < vertices; count++) {
        int u = -1;
        for (int j = 0; j < vertices; j++) {
            if (!visitado[j] && (u == -1 || costo[j] < costo[u])) {
                u = j;
            }
        }

        if (u == -1) break;
        visitado[u] = 1;

        Nodo* actual = grafo[u];
        while (actual) {
            int v = actual->destino;
            int peso = actual->peso;
            if (!visitado[v] && peso < costo[v]) {
                costo[v] = peso;
                padre[v] = u;
            }
            actual = actual->sig;
        }
    }

    cout << "Arbol minimo (Prim):" << endl;
    for (int i = 0; i < vertices; i++) {
        if (padre[i] != -1) {
            cout << padre[i] << " - " << i << " (peso: " << costo[i] << ")" << endl;
            cont += costo[i];
        }
    }
    cout << "Costo total del arbol minimo: " << cont << endl;

    delete[] visitado;
    delete[] costo;
    delete[] padre;
}

void dijkstra(int origen) {
    int* distancia = new int[vertices];
    int* padre = new int[vertices];
    int* visitado = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distancia[i] = infinito;
        padre[i] = -1;
        visitado[i] = 0;
    }
    distancia[origen] = 0;

    for (int count = 0; count < vertices; count++) {
        int u = -1;
        for (int j = 0; j < vertices; j++) {
            if (!visitado[j] && (u == -1 || distancia[j] < distancia[u])) {
                u = j;
            }
        }

        if (u == -1) break;
        visitado[u] = 1;

        Nodo* actual = grafo[u];
        while (actual) {
            int v = actual->destino;
            int peso = actual->peso;
            if (distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                padre[v] = u;
            }
            actual = actual->sig;
        }
    }

    cout << "Distancias desde el vertice " << origen << " (Dijkstra):" << endl;
    for (int i = 0; i < vertices; i++) {
        cout << "A " << i << ": ";
        if (distancia[i] == infinito) {
            cout << "INFINITO\n";
        } else {
            cout << distancia[i] << " | Camino: ";
            int* camino = new int[vertices];
            int k = 0, actual = i;
            while (actual != -1) {
                camino[k++] = actual;
                actual = padre[actual];
            }
            for (int j = k - 1; j >= 0; j--) {
                cout << camino[j];
                if (j > 0) cout << " -> ";
            }
            cout << "\n";
            delete[] camino;
        }
    }

    delete[] distancia;
    delete[] padre;
    delete[] visitado;
}

void dijkstraEntre(int origen, int destino) {
    if (!rango(origen) || !rango(destino)) {
        cout << "Vertices fuera de rango" << endl;
        return;
    }

    int* distancia = new int[vertices];
    int* padre = new int[vertices];
    int* visitado = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distancia[i] = infinito;
        padre[i] = -1;
        visitado[i] = 0;
    }
    distancia[origen] = 0;

    for (int count = 0; count < vertices; count++) {
        int u = -1;
        for (int j = 0; j < vertices; j++) {
            if (!visitado[j] && (u == -1 || distancia[j] < distancia[u])) {
                u = j;
            }
        }

        if (u == -1) break;
        visitado[u] = 1;

        Nodo* actual = grafo[u];
        while (actual) {
            int v = actual->destino;
            int peso = actual->peso;
            if (distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                padre[v] = u;
            }
            actual = actual->sig;
        }
    }

    if (distancia[destino] == infinito) {
        cout << "No existe camino de " << origen << " a " << destino << endl;
    } else {
        cout << "Distancia minima de " << origen << " a " << destino << ": " << distancia[destino] << endl;
        cout << "Camino: ";
        int* camino = new int[vertices];
        int k = 0, actual = destino;
        while (actual != -1) {
            camino[k++] = actual;
            actual = padre[actual];
        }
        for (int i = k - 1; i >= 0; i--) {
            cout << camino[i];
            if (i > 0) cout << " -> ";
        }
        cout << "\n";
        delete[] camino;
    }

    delete[] distancia;
    delete[] padre;
    delete[] visitado;
}

void colorearGrafo() {
    int* color = new int[vertices];
    for (int i = 0; i < vertices; i++) color[i] = -1;
    color[0] = 0;

    for (int i = 1; i < vertices; i++) {
        int* disponible = new int[vertices];
        for (int j = 0; j < vertices; j++) disponible[j] = 1;

        Nodo* actual = grafo[i];
        while (actual) {
            int v = actual->destino;
            if (color[v] != -1) disponible[color[v]] = 0;
            actual = actual->sig;
        }

        for (int c = 0; c < vertices; c++) {
            if (disponible[c]) {
                color[i] = c;
                break;
            }
        }
        delete[] disponible;
    }

    cout << "Coloracion del grafo:\n";
    for (int i = 0; i < vertices; i++) {
        cout << "Vertice " << i << " -> Color " << color[i] << "\n";
    }

    delete[] color;
}

void liberarGrafo() {
    for (int i = 0; i < vertices; i++) {
        Nodo* actual = grafo[i];
        while (actual) {
            Nodo* siguiente = actual->sig;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] grafo;
}

int main() {
    string nombrearchivo = "taller2-ejemplo.txt";
    cargarArchivo(nombrearchivo);
    prim(0);
    cout << "\n";
    dijkstra(0);
    cout << "\n";
    colorearGrafo();
    cout << "\n";
    dijkstraEntre(0, 5);
    liberarGrafo();
    return 0;
}
