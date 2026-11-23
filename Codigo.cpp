/* =============================================================
   Curso      : Estructura de Datos (24UC00428)
   Evaluacion : C2 - Unidad 4 - Semana 15
   Proyecto   : Nido, la red social del campus (kit base)
   Institucion: Universidad Continental
   Docente    : Carlos del Carpio
   Semestre   : 2026-20
   Compilador : Dev-C++ (MinGW / GCC, C++11 o superior)
   NRC        : 
   Grupo      : G
   Integrantes: juan carlos chani
   juan dennys cayllahua apaza
   ============================================================= */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const int MAX = 12;
const string ARCHIVO = "nido.txt";

struct NodoUsuario {
    string usuario;
    int    id;
    NodoUsuario *izq, *der;
};

int sigue[MAX][MAX];
NodoUsuario *porId[MAX];
int nUsuarios = 0;

// ===== FICHA ===== (copiar tal cual en nido.cpp)

const string COMUNIDAD = "nido-zorzal";
const int N_U = 10;
string fUsuario[N_U] = {"@iglu", "@cometa", "@neon", "@vinilo", "@kayak",
 "@zenit", "@glitch", "@haiku", "@nova", "@turbo"};
const int N_S = 16;
string fSigue[N_S][2] = {{"@vinilo", "@glitch"}, {"@glitch", "@vinilo"},
 {"@glitch", "@cometa"}, {"@neon", "@zenit"},
 {"@kayak", "@nova"}, {"@glitch", "@kayak"},
 {"@iglu", "@cometa"}, {"@vinilo", "@cometa"},
 {"@neon", "@vinilo"}, {"@glitch", "@haiku"},
 {"@haiku", "@turbo"}, {"@cometa", "@glitch"},
 {"@kayak", "@cometa"}, {"@cometa", "@iglu"},
 {"@zenit", "@neon"}, {"@neon", "@turbo"}};

// =================
NodoUsuario* crearUsuario(string usuario, int id) {
    NodoUsuario *n = new NodoUsuario;
    n->usuario = usuario; n->id = id;
    n->izq = n->der = nullptr;
    return n;
}

bool esUsuarioValido(string u) {
    if (u.size() < 2 || u.size() > 16 || u[0] != '@') return false;
    for (int i = 1; i < (int)u.size(); i++)
        if (!((u[i] >= 'a' && u[i] <= 'z') || (u[i] >= '0' && u[i] <= '9'))) return false;
    return true;
}

void imprimirMarcados(string titulo, bool marca[]) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) k++;
    cout << titulo << " (" << k << "):";
    if (k == 0) cout << " (nadie)";
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) cout << " " << porId[x]->usuario;
    cout << endl;
}

/* ---------- Arbol binario de busqueda (todo recursivo) ---------- */

NodoUsuario* buscar(NodoUsuario *r, string u) {
    // TODO R1a
    if (r == nullptr || r->usuario == u) return r;
    if (u < r->usuario) return buscar(r->izq, u);
    return buscar(r->der, u);
    return nullptr;
}

bool insertar(NodoUsuario *&r, NodoUsuario *nuevo) {
    // TODO R1b
    if (r == nullptr) { r = nuevo; return true; }
    if (nuevo->usuario == r->usuario) return false;
    if (nuevo->usuario < r->usuario) return insertar(r->izq, nuevo);
    return insertar(r->der, nuevo);
    return false;
}

bool registrarUsuario(NodoUsuario *&raiz, string u) {
    // TODO R1c
    if (!esUsuarioValido(u)) return false;
    if (nUsuarios == MAX || buscar(raiz, u) != nullptr) return false;

    int id = nUsuarios;
    NodoUsuario *nuevo = crearUsuario(u, id);
    if (!insertar(raiz, nuevo)) return false;

    porId[nUsuarios] = nuevo;
    nUsuarios++;
    return true;
    return false;
}

void enorden(NodoUsuario *r) {
    // TODO R2a
    if (r == nullptr) return;
    enorden(r->izq);
    cout << r->usuario << "\n";
    enorden(r->der);
}

void preorden(NodoUsuario *r) {
    // TODO R2b
    if (r == nullptr) return;
    cout << r->usuario << "\n";
    preorden(r->izq);
    preorden(r->der);
}

int contar(NodoUsuario *r) {
    // TODO R2c
     if (r == nullptr) return 0;
    return 1 + contar(r->izq) + contar(r->der);
    return 0;
}

int altura(NodoUsuario *r) {
    // TODO R2d
    if (r == nullptr) return 0;
    int izq = altura(r->izq);
    int der = altura(r->der);
    return 1 + (izq > der ? izq : der);
    return 0;
}

int hojas(NodoUsuario *r) {
    // TODO R2e
    if (r == nullptr) return 0;
    if (r->izq == nullptr && r->der == nullptr) return 1;
    return hojas(r->izq) + hojas(r->der);
    return 0;
}

/* ---------- Grafo dirigido: matriz de adyacencia sigue ---------- */

int contarSiguiendo(int id) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (sigue[id][x] == 1) k++;
    return k;
}

int contarSeguidores(int id) {
    // TODO R3a
    int c = 0;
    for (int x = 0; x < nUsuarios; x++) if (sigue[x][id] == 1) c++;
    return c;
    return 0;
}

void difundir(int v, bool visto[], bool imprimir) {
    // TODO R5a
    visto[v] = true;
    for (int x = 0; x < nUsuarios; x++) {
        if (sigue[x][v] == 1 && !visto[x]) {
            if (imprimir) cout << " " << porId[x]->usuario;
            difundir(x, visto, imprimir);
        }
    }
}
/* ---------- Archivos y memoria ---------- */

void guardarPreorden(NodoUsuario *r, ofstream &f) {
    // TODO R6a
 if (r == nullptr) return;

f << "U " << r->usuario << " " << r->id << endl;

guardarPreorden(r->izq, f);

guardarPreorden(r->der, f);
}
int liberar(NodoUsuario *r) {
    // TODO R7a
     if (r == nullptr) return 0;
    int k = liberar(r->izq) + liberar(r->der);
    delete r;
    return k + 1;
}

/* ---------- Ordenes ---------- */

void leerOrden(string linea, string &orden, string &a1, string &a2) {
    string t[6]; int n = 0;
    stringstream ss(linea);
    while (n < 6 && ss >> t[n]) n++;
    orden = a1 = a2 = "";
    if (n == 0) return;
    string c = t[0];
    if (c == "signup") c = "registrar";
    else if (c == "whois") c = "perfil";
    else if (c == "users") c = "usuarios";
    else if (c == "tree") c = "arbol";
    else if (c == "follow") c = "seguir";
    else if (c == "unfollow") c = "dejar";
    else if (c == "followers") c = "seguidores";
    else if (c == "following") c = "siguiendo";
    else if (c == "mutuals") c = "mutuos";
    else if (c == "trending") c = "popular";
    else if (c == "reach") c = "alcance";
    else if (c == "reaches") c = "llega";
    else if (c == "save") c = "guardar";
    else if (c == "load") c = "cargar";
    else if (c == "stats") c = "reporte";
    else if (c == "help" || c == "man") c = "ayuda";
    else if (c == "exit" || c == "quit") c = "salir";
    orden = c;
    if (n > 1) a1 = t[1];
    if (n > 2) a2 = t[2];
}

void ayuda() {
    cout << "  ORDEN                   EN INGLES            QUE HACE" << endl;
    cout << "  registrar @u            signup @u            crear una cuenta (ABB: insertar)" << endl;
    cout << "  perfil @u               whois @u             ver una cuenta (ABB: buscar)" << endl;
    cout << "  usuarios                users                cuentas en orden alfabetico (enorden)" << endl;
    cout << "  arbol                   tree                 preorden, nodos, altura y hojas" << endl;
    cout << "  seguir @a @b            follow @a @b         @a empieza a seguir a @b" << endl;
    cout << "  dejar @a @b             unfollow @a @b       @a deja de seguir a @b" << endl;
    cout << "  seguidores @u           followers @u         quienes siguen a @u" << endl;
    cout << "  siguiendo @u            following @u         a quienes sigue @u" << endl;
    cout << "  mutuos @u               mutuals @u           se siguen los dos" << endl;
    cout << "  popular                 trending             cuenta con mas seguidores" << endl;
    cout << "  alcance @u              reach @u             a quienes llega un post de @u (DFS)" << endl;
    cout << "  llega @a @b             reaches @a @b        un post de @a llega a @b?" << endl;
    cout << "  guardar / cargar        save / load          archivo nido.txt" << endl;
    cout << "  reporte                 stats                resumen de la comunidad" << endl;
    cout << "  salir                   exit                 terminar" << endl;
}

int main() {
    NodoUsuario *raiz = nullptr;
    for (int i = 0; i < MAX; i++) for (int j = 0; j < MAX; j++) sigue[i][j] = 0;

    for (int i = 0; i < N_U; i++) registrarUsuario(raiz, fUsuario[i]);
    for (int i = 0; i < N_S; i++) {
        NodoUsuario *a = buscar(raiz, fSigue[i][0]);
        NodoUsuario *b = buscar(raiz, fSigue[i][1]);
        if (a != nullptr && b != nullptr) sigue[a->id][b->id] = 1;
    }

    cout << "Nido - comunidad " << COMUNIDAD << " (" << nUsuarios << " cuentas). Escribe ayuda." << endl;

    string linea;
    while (true) {
        cout << "nido@" << COMUNIDAD << ":~$ ";
        if (!getline(cin, linea)) break;
        string orden, a1, a2;
        leerOrden(linea, orden, a1, a2);
        if (orden == "") continue;
        cout << "> " << linea << endl;

        if (orden == "registrar") {
  
            // TODO R1d
            string u;
            cin >> u;
           if (registrarUsuario(raiz, u)) {
                cout << "OK\n";
           } else {
                cout << "ERROR\n";
           }
            
        }
        else if (orden == "perfil") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else cout << u->usuario << " | id " << u->id << " | seguidores " << contarSeguidores(u->id)
                      << " | siguiendo " << contarSiguiendo(u->id) << endl;
        }
        else if (orden == "usuarios") {
            cout << "Usuarios (enorden):";
            enorden(raiz);
            cout << endl;
        }
        else if (orden == "arbol") {
            cout << "Preorden:";
            preorden(raiz);
            cout << endl;
            cout << "Nodos: " << contar(raiz) << " | altura: " << altura(raiz) << " | hojas: " << hojas(raiz) << endl;
        }
        else if (orden == "seguir") {
    // TODO R3b
    int idOrigen, idDestino;
    cin >> idOrigen >> idDestino;
    if (idOrigen >= 0 && idOrigen < nUsuarios &&
        idDestino >= 0 && idDestino < nUsuarios &&
        idOrigen != idDestino) {
        sigue[idOrigen][idDestino] = 1;
        cout << "OK\n";
    } else {
        cout << "ERROR\n";
    }
}

        else if (orden == "dejar") {
            // TODO R3c
        }
        else if (orden == "siguiendo") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else {
                bool marca[MAX] = {false};
                for (int x = 0; x < nUsuarios; x++) if (sigue[u->id][x] == 1) marca[x] = true;
                imprimirMarcados(u->usuario + " sigue a", marca);
            }
        }
        else if (orden == "seguidores") {
            // TODO R3d
            int id;
            cin >> id;
            if (id < 0 || id >= nUsuarios) {
                cout << "ERROR\n";
            } else {
                cout << contarSeguidores(id) << "\n";
        }
        }
        else if (orden == "mutuos") {
    // TODO R4a
    int id;
    cin >> id;
    if (id < 0 || id >= nUsuarios || porId[id] == nullptr) {
        cout << "ERROR\n";
    } else {
        cout << "Mutuos de " << porId[id]->usuario << ":";
        for (int x = 0; x < nUsuarios; x++) {
            if (x != id && sigue[id][x] == 1 && sigue[x][id] == 1) {
                cout << " " << porId[x]->usuario;
            }
        }
        cout << "\n";
    }
}

        
        else if (orden == "alcance") {
    // TODO R5b
    int id;
    cin >> id;
    if (id < 0 || id >= nUsuarios) {
        cout << "ERROR\n";
    } else {
        bool visto[MAX] = {false};
        difundir(id, visto, false);
        int alcance = 0;
        for (int x = 0; x < nUsuarios; x++) {
            if (x != id && visto[x]) alcance++;
        }
        cout << alcance << "\n";
    }
}

        else if (orden == "llega") {
            // TODO R5c
            int origen, destino;
            cin >> origen >> destino;
            if (origen < 0 || origen >= nUsuarios || destino < 0 || destino >= nUsuarios) {
                cout << "ERROR\n";
            } else if (sigue[origen][destino] == 1) {
                cout << "SI\n";
            } else {
                cout << "NO\n";
            }
                
        }
        else if (orden == "guardar") {
            ofstream f(ARCHIVO);
            int m = 0;
            guardarPreorden(raiz, f);
            // TODO R6b
            
            f.close();
            cout << "Guardado: " << contar(raiz) << " cuentas y " << m << " seguimientos en " << ARCHIVO << endl;
        }
        else if (orden == "cargar") {
            ifstream f(ARCHIVO);
            if (!f) cout << "E: No existe " << ARCHIVO << endl;
            else {
                int m = 0;
                // TODO R6c
                cout << "Cargado: " << contar(raiz) << " cuentas y " << m << " seguimientos | altura "
                     << altura(raiz) << endl;
            }
        }
        else if (orden == "reporte") {
            // TODO R7b
        }
        else if (orden == "ayuda") ayuda();
        else if (orden == "salir") break;
        else cout << orden << ": orden no encontrada (escribe ayuda)" << endl;
    }

    int liberados = liberar(raiz);
    raiz = nullptr;
    cout << "Nodos liberados: " << liberados << ". Hasta pronto." << endl;
    return 0;
}
