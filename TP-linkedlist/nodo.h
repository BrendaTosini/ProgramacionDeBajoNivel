typedef struct nodo {
    int valor;
    struct nodo *proximo;
}Nodo;


Nodo *crearNodo(int valor);