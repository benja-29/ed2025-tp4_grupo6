#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
// ============================== CLASE LIBRO ==============================
class Libro {
private:
    string titulo;
    string autor;
    string ISBN;
    bool disponible;

public:
    Libro(string t, string a, string i) : titulo(t), autor(a), ISBN(i), disponible(true) {}

    string getTitulo() const { return titulo; }
    string getAutor() const { return autor; }
    string getISBN() const { return ISBN; }
    bool estaDisponible() const { return disponible; }

    void prestar() { disponible = false; }
    void devolver() { disponible = true; }

    virtual void mostrar() const {
        cout << "Título: " << titulo << ", Autor: " << autor << ", ISBN: " << ISBN;
        cout << ", Estado: " << (disponible ? "Disponible" : "Prestado") << endl;
    }
};

class LibroDigital : public Libro {
private:
    string url;
public:
    LibroDigital(string t, string a, string i, string u) : Libro(t, a, i), url(u) {}
    void mostrar() const override {
        Libro::mostrar();
        cout << "URL de descarga: " << url << endl;
    }
};
// ============================== TDA PILA DE LIBROS ==============================
const int MAX = 10;
struct Pila {
    Libro* libros[MAX];
    int cima;
};

void initStack(Pila &p) { p.cima = -1; }
bool isEmptyStack(Pila p) { return p.cima == -1; }
bool isFullStack(Pila p) { return p.cima == MAX - 1; }

void pushStack(Pila &p, Libro* libro) {
    if (!isFullStack(p)) p.libros[++p.cima] = libro;
}

Libro* popStack(Pila &p) {
    if (!isEmptyStack(p)) return p.libros[p.cima--];
    return nullptr;
}
// ============================== CLASE USUARIO ==============================
class Usuario {
private:
    string nombre;
    int ID;
    Pila historial;

public:
    Usuario(string n, int id) : nombre(n), ID(id) {
        initStack(historial);
    }

    string getNombre() const { return nombre; }
    int getID() const { return ID; }

    void agregarHistorial(Libro* libro) {
        pushStack(historial, libro);
    }

    void mostrarHistorial() {
        Pila copia = historial;
        cout << "Historial de préstamos de " << nombre << ":\n";
        while (!isEmptyStack(copia)) {
            Libro* libro = popStack(copia);
            cout << " - " << libro->getTitulo() << endl;
        }
    }
};
// ============================== CLASE BIBLIOTECA ==============================
class Biblioteca {
private:
    vector<Libro*> libros;
    vector<Usuario*> usuarios;

public:
    ~Biblioteca() {
        for (auto libro : libros) delete libro;
        for (auto usuario : usuarios) delete usuario;
    }

    void registrarLibro(Libro* libro) {
        libros.push_back(libro);
    }

    void registrarUsuario(Usuario* usuario) {
        usuarios.push_back(usuario);
    }

    Libro* buscarLibroPorTitulo(string titulo) {
        for (auto libro : libros)
            if (libro->getTitulo() == titulo)
                return libro;
        return nullptr;
    }

    vector<Libro*> buscarLibrosPorAutor(string autor) {
        vector<Libro*> encontrados;
        for (auto libro : libros)
            if (libro->getAutor() == autor)
                encontrados.push_back(libro);
        return encontrados;
    }

    Usuario* buscarUsuarioPorID(int id) {
        for (auto usuario : usuarios)
            if (usuario->getID() == id)
                return usuario;
        return nullptr;
    }

    void prestarLibro(string titulo, int userID) {
        Libro* libro = buscarLibroPorTitulo(titulo);
        Usuario* usuario = buscarUsuarioPorID(userID);
        if (libro && usuario && libro->estaDisponible()) {
            libro->prestar();
            usuario->agregarHistorial(libro);
            cout << "Libro prestado correctamente.\n";
        } else {
            cout << "No se pudo realizar el préstamo.\n";
        }
    }

    void devolverLibro(string titulo) {
        Libro* libro = buscarLibroPorTitulo(titulo);
        if (libro && !libro->estaDisponible()) {
            libro->devolver();
            cout << "Libro devuelto correctamente.\n";
        } else {
            cout << "No se pudo devolver el libro.\n";
        }
    }

    void mostrarTodosLosLibros() {
        cout << "Total de libros registrados: " << libros.size() << endl;
        for (auto libro : libros)
            libro->mostrar();
    }

    void mostrarTodosLosUsuarios() {
        cout << "Total de usuarios registrados: " << usuarios.size() << endl;
        for (auto u : usuarios)
            cout << "Usuario: " << u->getNombre() << ", ID: " << u->getID() << endl;
    }
};

bool esNumero(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

int obtenerIDSeguro() {
    string entrada;
    int id;
    while (true) {
        getline(cin, entrada);
        if (esNumero(entrada)) {
            stringstream(entrada) >> id;
            return id;
        } else {
            cout << "ID inválido. Ingrese un número entero válido: ";
        }
    }
}

// ========================== MAIN ==============================
int main() {
    Biblioteca bib;
    int opcion;

    do {
        cout << "\n==== MENÚ DE BIBLIOTECA ====\n";
        cout << "1. Registrar nuevo libro\n";
        cout << "2. Registrar nuevo usuario\n";
        cout << "3. Buscar libro por título\n";
        cout << "4. Buscar libros por autor\n";
        cout << "5. Prestar libro\n";
        cout << "6. Devolver libro\n";
        cout << "7. Mostrar todos los libros\n";
        cout << "8. Mostrar todos los usuarios\n";
        cout << "9. Ver historial de un usuario\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        string titulo, autor, isbn, url, nombreUsuario;
        int id;

        switch(opcion) {
        case 1: {
            char tipo;
            do {
                cout << "¿Es un libro digital? (s/n): ";
                cin >> tipo;
                cin.ignore();
                cout << "Título: "; getline(cin, titulo);
                cout << "Autor: "; getline(cin, autor);
                cout << "ISBN: "; getline(cin, isbn);
                if (tipo == 's' || tipo == 'S') {
                    cout << "URL de descarga: "; getline(cin, url);
                    bib.registrarLibro(new LibroDigital(titulo, autor, isbn, url));
                } else {
                    bib.registrarLibro(new Libro(titulo, autor, isbn));
                }
                cout << "Libro registrado correctamente.\n";
                cout << "¿Desea registrar otro libro? (s/n): ";
                cin >> tipo;
                cin.ignore();
            } while (tipo == 's' || tipo == 'S');
            break;
        }

        case 2: {
            char respuesta;
            do {
                cout << "Nombre del usuario: "; getline(cin, nombreUsuario);
                cout << "ID del usuario: ";
                id = obtenerIDSeguro();
                bib.registrarUsuario(new Usuario(nombreUsuario, id));
                cout << "Usuario registrado correctamente.\n";
                cout << "¿Desea registrar otro usuario? (s/n): ";
                cin >> respuesta;
                cin.ignore();
            } while (respuesta == 's' || respuesta == 'S');
            break;
        }

        case 3:
            cout << "Título del libro: "; getline(cin, titulo);
            if (Libro* l = bib.buscarLibroPorTitulo(titulo)) l->mostrar();
            else cout << "Libro no encontrado.\n";
            break;

        case 4:
            cout << "Autor: "; getline(cin, autor);
            {
                auto lista = bib.buscarLibrosPorAutor(autor);
                if (lista.empty()) cout << "No se encontraron libros de ese autor.\n";
                else for (auto l : lista) l->mostrar();
            }
            break;

        case 5:
            cout << "Título del libro: "; getline(cin, titulo);
            cout << "ID del usuario: ";
            id = obtenerIDSeguro();
            bib.prestarLibro(titulo, id);
            break;

        case 6:
            cout << "Título del libro: "; getline(cin, titulo);
            bib.devolverLibro(titulo);
            break;

        case 7:
            bib.mostrarTodosLosLibros();
            break;

        case 8:
            bib.mostrarTodosLosUsuarios();
            break;

        case 9:
            cout << "ID del usuario: ";
            id = obtenerIDSeguro();
            if (Usuario* u = bib.buscarUsuarioPorID(id))
                u->mostrarHistorial();
            else
                cout << "Usuario no encontrado.\n";
            break;

        case 0:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
