//Como crearemos un laberinto???
// Muy buena pregunta, sabiendo que somos principiantes y nos gusta los retos
// Tienes 14hs desde ya para terminar el reto


// Empezamos por a base del proyecto, que es el entorno (matriz/tablero)

//Importamos todas las librerias a usar
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random> 

using namespace std;

class Maze{
    /*Creamos la clase laberinto, en el cual configuraremos las celdas*/
    public:
        Maze(int ANCHO, int ALTO);
        void display();
        void solve(); //Funcion para poder resolver

    private:
        int ANCHO, ALTO;
        vector<vector<int>>maze;
        void genereteMaze();
        bool esSeguro(int x, int y);
        bool resolverMaze(int x, int y, vector<vector<int>>& solucion);
        void DFS(int x, int y);
        vector<pair<int, int>> vecinos(int x, int y);
};

Maze::Maze(int ANCHO, int ALTO) : ANCHO(ANCHO), ALTO(ALTO){
    maze.resize(ALTO, vector<int>(ANCHO,0));
    genereteMaze(); //Inicializamos el laberinto
}

void Maze::display(){
    // Crea el tablero iterando las filas y celdas, se le asigna un valor a la celda(1)
    for (const auto& fila : maze) {
        for (int celda : fila) {
            cout << (celda == 1 ? " " : "#");
        }
        cout << endl;
    }
}

void Maze::genereteMaze(){
    srand(time(0));
    DFS(1,1); // Iniciar DFS desde la entrada del laberinto
    maze[1][1]=1; // Asegurar que la entrada esté abierta
    maze[ANCHO-2][ALTO-2] = 1; // Asegurar que la salida esté abierta

}

void Maze::DFS(int x, int y){
    maze[x][y] = 1;
    auto vecs = vecinos(x, y);

    random_device rd;
    mt19937 g(rd());
    shuffle(vecs.begin(), vecs.end(), g);

    for (auto [nx, ny] : vecs) {
        int wx = (x + nx) / 2;
        int wy = (y + ny) / 2;
        if (esSeguro(nx, ny) && maze[wx][wy] == 0){
            maze[wx][wy] = 1;
            DFS(nx, ny);
        }
    }
}

vector<pair<int, int>> Maze::vecinos(int x, int y) {
    vector<pair<int, int>> vec = {{x + 2, y}, {x - 2, y}, {x, y + 2}, {x, y - 2}};
    vec.erase(remove_if(vec.begin(), vec.end(), [this](pair<int, int> p) {
        int nx = p.first, ny = p.second;
        return nx < 0 || nx >= ALTO || ny < 0 || ny >= ANCHO;
    }), vec.end());
    return vec;
}


bool Maze::esSeguro(int x, int y){
    // Es un validador 
    return (x >= 0 && x < ALTO && y >= 0 && y < ANCHO && maze[x][y] == 0);
}

bool Maze::resolverMaze(int x, int y, vector<vector<int>>& solucion){

    if (x == ALTO-1 && y == ANCHO-1){
        solucion[x][y] = 1;
        return true;
    }

    if(esSeguro(x, y) && maze[x][y] ==1){
        solucion[x][y] = 1;
        maze [x][y] = 2; // Marca como visitado
        if (resolverMaze(x+1, y, solucion)) return true;
        if (resolverMaze(x, y+1, solucion)) return true;
        if (resolverMaze(x-1, y, solucion)) return true;
        if (resolverMaze(x, y-1, solucion)) return true;
        solucion[x][y]=0;
        return false;
    }
    return false;
}

void Maze::solve(){
    vector<vector<int>> solucion(ALTO, vector<int>(ANCHO, 0));
    if (resolverMaze(0,0, solucion)){
        cout << "SOLUCION ENCONTRADA!" << endl;
        for (const auto& fila : solucion){
            for (int celda : fila){
                cout << (celda == 1 ? "  ": "##");
            }
            cout << endl;
        }
    }else{
        cout << "NO SE ENCONTRO SOLUCION! " << endl;
    }

}

int main(){
    int ANCHO, ALTO;
    cout <<"Ingrese el nro de filas: ";
    cin >> ALTO;
    cout << "Ingrese el nro de columnas: ";
    cin >> ANCHO;
    Maze maze(ANCHO,ALTO);
    cout << "LABERINTO GENERADO:" << endl;
    maze.display();
    cout << endl << "RESOLVIENDO LABERINTO..." << endl;
    maze.solve();

    return 0;
}