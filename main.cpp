
/*FABRIZIO ALCARAZ ESCOBAR & DAVID DEL CERRO DOMINGUEZ*/



#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iomanip>
#include <time.h>


using namespace std;

const int Persona = 1;
const int Maquina = 2;
const int NumCartasMazo = 8;
const int MaxCartas = 40;

typedef int tCartas[MaxCartas], tCartasPorAparecer[NumCartasMazo];
struct tConjuntoCartas {
	tCartas cartas;
	int cont;
};

int menu();

int determinarGanador(double p1, double p2 , int cartasJ, int cartasM);			//Recibe los puntos, y en el caso de la version 3 el numero de cartas robadas y devuelve el ganador

void decirGanador(int ganador);

bool abrirArchivo(ifstream & mazo);			//Devuelve true si se ha podido abrir el archivo, devuelve false en caso contrario

int generarAleatorio(int a, int b);

float determinarPuntuacion(float valor);


void version1(char modo);

double modoA(ifstream & mazo, int max_cartas);

double modoB(ifstream & mazo, int numCartas, int turno, int PuntosContrario);


void version2();

double modoC(ifstream & mazo, tCartasPorAparecer cartas, int turno, double puntosContrario);

bool esProbablePasarse(double puntosMaquina, const tCartasPorAparecer cartas);

void restarCarta(int carta, tCartasPorAparecer cartas);			//Resta la carta que ha salido del array

void iniciarCartasPorAparecer(tCartasPorAparecer cartas);		// Inicia el array tCartasPorAparecer

int cartasPorSalir(const tCartasPorAparecer cartas);		 //Recibe el array y calcula las cartas que quedan en total


void version3(int & numeroPartida);

void iniciar(tConjuntoCartas & mazo);

double modoD(tConjuntoCartas & mazo, tCartasPorAparecer cartas, int turno, double puntosContrario, tConjuntoCartas & cartasJugador);

void mostrar(tConjuntoCartas & mazo);			//Muestra las cartas en el mazo 

void crearMazo(tConjuntoCartas & mazo);			

void barajar(tConjuntoCartas & mazo);			

void sacar(tConjuntoCartas & mazo, int & carta);

void incluir(tConjuntoCartas & mazo, int carta);

bool cargarMazo(ifstream & flin, tConjuntoCartas & mazo);

void guardarResultado(int ganador, double totalJugador, double totalMaquina, tConjuntoCartas cartasJugador, tConjuntoCartas cartasMaquina, int & numeroPartida);



int main()
{
	int opcion;
	int numeroPartida = 1;					
	srand(time(nullptr));

	opcion = menu();

	while (opcion != 0)
	{
		if (opcion == 1) {
			version1('A');
		}
		else if (opcion == 2) {
			version1('B');
		}
		else if (opcion == 3) {
			version2();
		}
		else if (opcion == 4) {
			version3(numeroPartida);
		}

		opcion = menu();
	}
	
	return 0;
}

int  menu() {

	int op;

	cout << "0: Salir" << endl;
	cout << "1: Modo A" << endl;
	cout << "2: Modo B" << endl;
	cout << "3: Modo C" << endl;
	cout << "4: Modo D" << endl;
	cout << "Elige una opcion: ";

	cin >> op;
	cin.ignore(INT_MAX, '\n');

	while((op < 0) || (op > 4)) {

		cout << "Opcion no valida" << endl;
		cout << "Introduzca una opcion: ";
		cin >> op;
		cin.ignore(INT_MAX, '\n');
		
	}
	return op;
}

int determinarGanador(double p1, double p2 , int cartasJ, int cartasM) {
	int gana;

	if (p1 <= 7.5 && p2 <= 7.5) {
		if (p1 > p2) {
			gana = Persona;
		}
		else if (p2 > p1) {
			gana = Maquina;
		}
		else if (p2 == p2) {
			if (cartasJ < cartasM) {
				gana = Persona;				
			}
			else if (cartasM < cartasJ) {
				gana = Maquina;				
			}
			else gana = generarAleatorio(1, 2);	
		}
	}
	else if (p2 > 7.5) {
		gana = Persona;
	}
	else if (p1 > 7.5) {
		gana = Maquina;
	}

	
	return gana;

}

void decirGanador(int ganador) {
	if (ganador == Persona) {
		cout << "HA GANADO EL JUGADOR" << endl;
	}
	else if (ganador == Maquina) {
		cout << "HA GANADO LA MAQUINA" << endl;
	}
}

bool abrirArchivo(ifstream & mazo) {
	string archivo;
	bool abierto;

	cout << "Nombre del archivo: ";
	cin >> archivo;
	cin.ignore(INT_MAX,'\n');

	mazo.open(archivo);
	if (mazo.is_open()) {
		abierto = true;
	}
	else {
		cout << "No se pudo abrir el archivo" << endl << endl;
		abierto = false;
	}
	return abierto;
}

int generarAleatorio(int a, int b) {
	int aleatorio;

	aleatorio = (a + rand() % (b));
	return aleatorio;
}

float determinarPuntuacion(float valor) {
	if (valor == 10 || valor == 11 || valor == 12) {
		valor = 0.5;	
	}
	return valor;
}

void sacarCarta(ifstream & mazo ,int & carta, double & total){
	double puntuacion;
	mazo >> carta;
	puntuacion = determinarPuntuacion(carta);
	total = total + puntuacion;

	cout << "Carta: " << setw(3) << carta;
	cout << setw(20) << "Puntuacion: " << setw(3) << total << endl;

}


void version1(char modo) {
	double totalJugador, totalMaquina;
	int max_carta, ganador;
	ifstream mazo;
	
	totalMaquina = 0;
	max_carta = generarAleatorio(3, 3);

	if (abrirArchivo(mazo)) {

		cout << setfill('=')  << setw(33) << "" << endl << setfill(' ');			//setfill('=') para rellenar los espacios con un =, y al terminar setfill(' ') para rellenar con espacios los sigueintes setw()
		cout << setw(8) << " " << "Turno del jugador" << endl << endl;
		if (modo == 'A') {
			totalJugador = modoA(mazo, max_carta);
		}
		else if (modo == 'B') {
			totalJugador = modoB(mazo, max_carta, Persona, 0);
		}
		cout << endl << "Puntuacion del jugador: " << totalJugador  << endl;
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
		if (totalJugador <= 7.5) {
			cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
			cout << setw(7)  << " " << "Turno de la maquina" << endl << endl;
			if (modo == 'A') {
				totalMaquina = modoA(mazo, max_carta);
			}
			else if(modo == 'B') {
				totalMaquina = modoB(mazo, max_carta, Maquina, totalJugador);
			}
			cout << endl << "Puntuacion de la maquina: " << totalMaquina << endl;
			cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
		}

		ganador=determinarGanador(totalJugador, totalMaquina,0,0);
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ') << setw(6) << " ";			// setw(6) << " " para centrar la siguiente frase
		decirGanador(ganador);
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
	}
}

double modoA(ifstream & mazo, int max_cartas) {
	int cont = 0;
	double total, puntuacion;
	int carta;

	total = 0;

	while ((cont < max_cartas) && (total < 7.5)) {
		sacarCarta(mazo,carta,total);
		cont++;
	}

	return total;
}

double modoB(ifstream & mazo, int numCartas, int turno, int PuntosContrario) {
	int cont = 0;
	char a;
	int carta;
	double puntuacion, total;
	bool seguir = true;

	total = 0;

	while ((cont < numCartas) && (total < 7.5) && (seguir)) {
		sacarCarta(mazo, carta, total);
		if ((turno == Persona) && (total < 7.5)) {
			cout << "Continuar? (Y/N) ";
			cin >> a;
			cin.ignore(INT_MAX, '\n');
			if (toupper(a) == 'N') seguir = false;
		}
		else if ((turno == Maquina) && (total > PuntosContrario)) {
			seguir = false;
		}
		cont++;
	}
	return total;
}

void version2() {
	ifstream mazo;
	tCartasPorAparecer cartas;
	double totalJugador, totalMaquina;
	int ganador;

	totalMaquina = 0;						// TotalMaquina=0 para inicializar determinarGanador en el caso en el que el jugador se pase de 7.5
	iniciarCartasPorAparecer(cartas);

	if (abrirArchivo(mazo)) {
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ');							//setfill('=') para rellenar los espacios con un =, y al terminar setfill(' ') para rellenar con espacios los sigueintes setw()
		cout << setw(8) << " " << "Turno del jugador" << endl << endl;
		totalJugador = modoC(mazo, cartas, Persona, 0);
		cout << endl << "Puntuacion del jugador: " << totalJugador << endl;
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ');

		if (totalJugador <= 7.5) {
			cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
			cout << setw(7) << " " << "Turno de la maquina" << endl<< endl;
			totalMaquina = modoC(mazo, cartas, Maquina, totalJugador);
			cout << endl << "Puntuacion de la maquina: " << totalMaquina << endl;
			cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
		}

		ganador = determinarGanador(totalJugador, totalMaquina, 0, 0);
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ') << setw(6) << " ";					// setw(6) << " " para centrar la siguiente frase
		decirGanador(ganador);
		cout << setfill('=') << setw(33) << "" << endl << setfill(' ');
	}
}

double modoC(ifstream & mazo, tCartasPorAparecer cartas, int turno, double puntosContrario) {
	int carta;
	double puntuacion, total;
	char a;
	bool seguir = true;
	bool esProbable = false;

	total = 0;

	while ((total < 7.5) && (seguir) && (!esProbable)) {
		sacarCarta(mazo, carta, total);
		restarCarta(carta, cartas);
		if ((turno == Persona) && (total < 7.5)) {
			cout << "Continuar? (Y/N) ";
			cin >> a;
			cin.ignore(INT_MAX, '\n');
			if (toupper(a) == 'N') seguir = false;
		}
		else if (turno == Maquina) {
			if (puntosContrario == total){
				esProbable = esProbablePasarse(total, cartas);
			}
			if (total > puntosContrario){
				seguir = false;
			}
		}
	}
	
	return total;
}

int cartasPorSalir(const tCartasPorAparecer cartas) {
	int total;
	total = 0;
	for (int i = 0; i < NumCartasMazo; i++) {
		total = cartas[i] + total;
	}
	return total;
}

bool esProbablePasarse(double puntosMaquina, const tCartasPorAparecer cartas) {
	double puntosQueQuedan, probabilidad;
	float cartasSalir;						//Float necesario para realizar la division
	int numCartas;
	bool pasarse;

	numCartas = 0;
	puntosQueQuedan = 7.5 - puntosMaquina;
	cartasSalir = cartasPorSalir(cartas);

	for (int i = 0 ; i < puntosQueQuedan; i++) {
		numCartas = cartas[i] + numCartas;
	}

	probabilidad =	1	- (numCartas / cartasSalir);
	pasarse = probabilidad > 0.5;
	// cout << "Probabilidad de pasarse: " << probabilidad << endl;

	return pasarse;
}

void restarCarta(int carta, tCartasPorAparecer cartas) {
	if (carta > 7) {
		cartas[0] = cartas[0] - 1;
	}
	else {
		cartas[carta] = cartas[carta] - 1;
	}
}

void iniciarCartasPorAparecer(tCartasPorAparecer cartas) {
	cartas[0] = 12;									// Aqui tCartasPorAparecer se inicia
	for (int i = 1; i < NumCartasMazo; i++) {
		cartas[i] = 4;
	}
}



void version3(int & numeroPartida) {

	bool depurar = false;						// Activa o desactiva el modo depuracion
	ifstream flin;

	tConjuntoCartas mazo, cartasJugador, cartasMaquina;
	tCartasPorAparecer cartas;
 
	double totalJugador, totalMaquina;
	int ganador;
	totalMaquina = 0;

	iniciar(mazo); iniciar(cartasJugador); iniciar(cartasMaquina);
	iniciarCartasPorAparecer(cartas);
	

	if (!depurar) {
		crearMazo(mazo);
		barajar(mazo);
		
	}
	else {
		srand(1);
		cargarMazo(flin, mazo);
		barajar(mazo);
		cout << "Mazo Barajeado: ";
		mostrar(mazo);
	}
	cout << setfill('=') << setw(64) << "" << endl << setfill(' ');							//setfill('=') para rellenar los espacios con un =, y al terminar setfill(' ') para rellenar con espacios los sigueintes
	cout << setw(21) << " " << "Turno del jugador" << endl;
	totalJugador = modoD(mazo, cartas, Persona, 0, cartasJugador);
	cout << endl << "Puntuacion del jugador: " << totalJugador << endl;
	cout << setfill('=') << setw(64) << "" << endl << setfill(' ');
	if (totalJugador <= 7.5) {
		cout << setfill('=') << setw(64) << "" << endl << setfill(' ');
		cout << setw(20) << " " << "Turno de la maquina" << endl;
		totalMaquina = modoD(mazo, cartas, Maquina, totalJugador, cartasMaquina);
		cout << endl << "Puntuacion de la maquina: " << totalMaquina << endl;
		cout << setfill('=') << setw(64) << "" << endl << setfill(' ');
	}

	ganador = determinarGanador(totalJugador, totalMaquina, cartasJugador.cont, cartasMaquina.cont);

	cout << setfill('=') << setw(64) << "" << endl << setfill(' ') << setw(20) << " ";					// setw(64) << " " para centrar la siguiente frase
	decirGanador(ganador);
	cout << setfill('=') << setw(64) << "" << endl << setfill(' ');

	guardarResultado(ganador, totalJugador, totalMaquina, cartasJugador, cartasMaquina, numeroPartida);
}

bool cargarMazo(ifstream & flin, tConjuntoCartas & mazo) {
	string archivo;
	bool cargado;
	int num;
	cargado = false;

	cout << "MODO DEPURAR ACTIVADO" << endl;
		while (!cargado) {
			if (abrirArchivo(flin)) {
				flin >> num;
				cout << "Mazo Original: ";
				for (int i = 0; (!flin.fail()) && (i < MaxCartas); i++) {			// Rellena cada hueco de Cartas con el numero que aparece en el archivo 
					mazo.cartas[i] = num;
					cout << mazo.cartas[i] << " ";
					flin >> num;
					mazo.cont++;
				}
				cout << endl;
				cargado = true;
			}
		}
	return cargado;
}

void iniciar(tConjuntoCartas & mazo) {
	mazo.cont = 0;						
	for (int i = 0; i < MaxCartas; i++) {
		mazo.cartas[i] = 0;
	}
}

void crearMazo(tConjuntoCartas & mazo) {
	int puntuacion = 1;
	int aux;
	
	for (int i = 0; i < MaxCartas; i++) {
		mazo.cartas[i] = puntuacion;									
		if (puntuacion % 12 == 0) { puntuacion = 0; }
		puntuacion++;
		if (puntuacion == 8) { puntuacion = 10; };
		mazo.cont++;
	}
}

void barajar(tConjuntoCartas & mazo) {
	int aleat1, aleat2;
	int aux;
	for (int i = 0; i < mazo.cont; i++) {
		aleat1 = generarAleatorio(0, mazo.cont);
		aleat2 = generarAleatorio(0, mazo.cont);
		aux = mazo.cartas[aleat1];
		mazo.cartas[aleat1] = mazo.cartas[aleat2];
		mazo.cartas[aleat2] = aux;
	}
}

void mostrar(tConjuntoCartas & mazo) {				     
	for (int i = 0; i < mazo.cont; i++) {
		cout << mazo.cartas[i] << " ";
	}
	cout << endl;
}

void sacar(tConjuntoCartas & mazo, int & carta) {
	carta = mazo.cartas[mazo.cont - 1];
	mazo.cont--;
}

double modoD(tConjuntoCartas & mazo, tCartasPorAparecer cartas, int turno, double puntosContrario, tConjuntoCartas & cartasJugador) {
	
	bool esProbable, seguir;
	double total, puntuacion;
	int carta;
	char a;
	esProbable = false;
	seguir = true;
	total = 0;

	
	while ((total < 7.5) && (seguir) && (!esProbable)) {
		
		sacar(mazo, carta);
		restarCarta(carta, cartas);						
		puntuacion = determinarPuntuacion(carta);
		total = total + puntuacion;
		cout << "Carta: " << setw(3) << carta;
		cout << setw(20) << "Puntuacion: " << setw(4) << total;
		incluir(cartasJugador, carta);
		cout << setw(25) <<"Cartas en la mano: ";
		mostrar(cartasJugador);
		if (turno == Persona) {
			if (total < 7.5) {
			cout << "Continuar? (Y/N) ";
			cin >> a;
			cin.ignore(INT_MAX, '\n');
			if (toupper(a) == 'N') seguir = false;
			}
		}
		else if (turno == Maquina) {
			if (puntosContrario == total) {
				esProbable = esProbablePasarse(total, cartas);
			}
			if (total > puntosContrario) {
				seguir = false;
			}
		}
	}
	
	return total;						
}

void incluir(tConjuntoCartas & mazo, int carta) {
	mazo.cartas[mazo.cont] = carta;
	mazo.cont++;
}

void guardarResultado(int ganador, double totalJugador, double totalMaquina, tConjuntoCartas cartasJugador, tConjuntoCartas cartasMaquina, int & numeroPartida){
	ofstream fout;
	string archivo;        

	archivo = "partida" + to_string(numeroPartida) + ".txt";									

	fout.open(archivo);
	if (fout.is_open())
	{
		fout << "Numero de partida: "<<setw(3)<< numeroPartida << endl;
		fout << "Ganador: ";
			if (ganador == Persona) fout << "Humano" << endl;
			else fout << "Maquina" << endl;
		fout << "Humano. Puntuacion: " << totalJugador << endl;
		fout << "Cartas: ";
			for (int i = 0; i < cartasJugador.cont; i++) {
				fout << cartasJugador.cartas[i] << " ";
			}
			fout << endl;
		fout << "Maquina. Puntuacion: " << totalMaquina << endl;
		fout << "Cartas: ";
			for (int i = 0; i < cartasMaquina.cont; i++) {
				fout << cartasMaquina.cartas[i] << " ";
			}
			fout << endl;
		fout.close();
	}
	else cout << "ERROR";
	numeroPartida++;
}