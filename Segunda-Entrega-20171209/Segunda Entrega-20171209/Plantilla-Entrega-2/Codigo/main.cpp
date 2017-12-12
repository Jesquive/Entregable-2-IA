#include <iostream>

//Para Strins y StringStreams
#include <sstream>
#include <string>

#include <fstream>

#include <random>
#include <vector>
#include <algorithm>
//#include <time>
#include <limits>

static int HOME_STAND = 3;
static int ROAD_TRIP = 3;
static int CANT_POBL = 10;
static int CANT_GENS = 10;
static std::string NL[16] = {"ATL", "NYM", "PHI", "MON", "FLA", "PIT", "CIN", "CHI", "STL", "MIL", "HOU", "COL", "SF", "SD", "LA", "ARI"};
static std::string Super[14] = {"BFN", "AKL", "CAN", "PRE", "HLM", "SYD", "JOH", "CHC", "BRI", "DUR", "DUN", "PER", "CPT", "WLG"};
static std::string NFL[32] = {"BOS", "MIA", "BUF", "NYJ", "CIN", "PIT", "BAL", "CLE", "IND", "JAC", "NAS", "HOU", "DEN", "SAN","KAN", "OAK", "NYG", "DAL", "WAS", "PHI", "CHI", "MIN", "DET", "GBY", "CHA", "TAM", "ATL", "NOR", "SEA", "STL", "PHO", "SFO"};
static std::string INSTANCIA;
static int** MATRIZDIST;

int** CrearMatriz(int fila, int columna)
{
  int** matriz = new int*[fila];
  for (int i = 0; i < columna; i++) {
    matriz[i] = new int[columna];
  }
  return matriz;
}

void BorrarMatriz(int** matriz,int fila)
{
  for (int i = 0; i < fila ; i++) {
    delete [] matriz[i];
  delete [] matriz;
  }
}

int sgn(int number)
{
  if (number>0) {
    return 1;
  }
  if (number<0) {
    return -1;
  }
  return 0;
}

using namespace std;
class Itinerario
{
  public:
    int Costo;
    int Penalidad;
    int Value;
    int cantEquipos;
    vector< vector<int> > A;
    Itinerario(vector<int> v, int n);
    Itinerario(int cost, int pen, int val, int n);
    void CalcularPenitencia_Costo(int cantEq);

    //vector<int> Cromosoma(Tamano,0);
};
//Constructor inicial de poblacion
Itinerario::Itinerario( vector<int> v, int n) : cantEquipos(n),   A(n, vector<int>(n-1))
{
  Costo = 0;
  Penalidad = 0;
  Value = 0;

  int Indicefijo = v[0];
  int ValorReal = v[0];

  for (int i = 0; i <  n; i++)
  {
      //El fijo con el segundo siempre van, i define la columna rde la actual ronda
      A[Indicefijo][i] = v[1];
      A[v[1]][i] = ValorReal;
      /*for ( int L = 0; L < n; L++) {
        for (  int O = 0;O < n-1; O++) {
          std::cout << A[L][O] << "  ";
        }
        std::cout << '\n';
      }*/
      int TEMPVALUE = 0;
      for (int j = 2; j < (n-1); j++) {
        int indiceBajo = 0 ;
        //el primero con el ultimo, cerrandose
        if(j%2 != 0)
        {
           indiceBajo = ((j+1)/2)+1;
           if(indiceBajo == TEMPVALUE){
             indiceBajo = TEMPVALUE +1;
           }
           if(TEMPVALUE > indiceBajo){
             indiceBajo = TEMPVALUE+1;
           }
           TEMPVALUE = indiceBajo;
        } else
        {
           indiceBajo = (j/2)+1;

           if(indiceBajo == TEMPVALUE){
             indiceBajo = TEMPVALUE +1;
           }
           if(TEMPVALUE > indiceBajo){
             indiceBajo = TEMPVALUE+1;
           }
           TEMPVALUE = indiceBajo;
        }

        int indiceAlto = n - j + 1;

        if(indiceAlto == indiceBajo)
        {
          break;
        }
        if(indiceAlto < indiceBajo)
        {
          break;
        }
        //std::cout << "indice alto y bajo " << indiceBajo << " -- " << indiceAlto <<'\n';
        A[v[indiceBajo]][i] =  v[indiceAlto];
        A[v[indiceAlto]][i] =  v[indiceBajo];

        //std::cout << "Segundas Asignaciones 1: " <<   A[v[indiceBajo]][i] << "--" <<A[v[indiceAlto]][i] << " Indices : "<< indiceBajo << "--"<< indiceAlto << "--"<< j <<'\n';

        /*for ( int L = 0; L < n; L++) {
          for (  int O = 0;O < n-1; O++) {
            std::cout << A[L][O] << "  ";
          }
          std::cout << '\n';
        }*/
      }
    //  std::cout << "#######################" << '\n';
      //Reorganizar el vector
      int nuevoUltimo = v[1];
      for (int k = 1; k < n; k++)
      {
        //Cuando no se salga del largo del arreglo
        if(k+1 != n)
        {
          v[k] = v[k+1];
        } else
        {
          v[n-1] = nuevoUltimo;
        }
      }
  }

  int contRoadTrip = 0;
  int contHomeStand = 1;
  int signoActual = 1;
  int randomNumb;

  //SUMARLE 1 A TODOS
  for ( int i = 0; i < n; i++) {
    for (  int j = 0; j < n-1; j++)
    {
      //std::cout << "print 1" << '\n';
      A[i][j] =  (A[i][j]+1);
    }
  }

  //Cambiarle los signos SWAPHOMES
  for ( int i = 0; i < n; i++) {
    for (  int j = 0; j < n-1; j++)
    {
      //Generar num randomentre 0 y 100 y comparar para ver si se cambia el estado
      randomNumb = rand()%100;
      //std::cout << "Numero random "<< randomNumb << '\n';
      if(signoActual == 1)
      {
        int prob = (contHomeStand * 100)/HOME_STAND;
        //std::cout << "prob  "<< prob << '\n';

        if(randomNumb < prob)
        {
          contHomeStand = 0;
          signoActual = -1;
          contRoadTrip++;
        }
      }
      if(signoActual == -1)
      {
        int prob = (contRoadTrip * 100)/ROAD_TRIP;
        if(randomNumb < prob)
        {
          contRoadTrip = 0;
          signoActual = 1;
          contHomeStand++;
        }
      }

      //Recorrer el arreglo por la fila, revisar si yo o el otro somos negativos
      //, si no es asi modificarme
      int Actual = A[i][j];
      if(sgn(Actual) == -1)
      {
        int ActualPositivo = -Actual;
        int Otro = A[ActualPositivo-1][j];

      } else  if(sgn(Actual) == 1)
      {
        int Otro = A[Actual-1][j];
        if (sgn(Otro) == 1)
        {
          A[i][j] = Actual * signoActual;
           A[Actual-1][j] = Otro * -signoActual;
        }
      }
      if(signoActual == -1)
      {
        contRoadTrip++;
      } else
      {
        contHomeStand++;
      }
    }
  }
}

//Constructor
Itinerario::Itinerario(int cost, int pen, int val, int n): cantEquipos(n),   A(n, vector<int>(n-1))
{
  Costo = cost;
  Penalidad = pen;
  Value = val;
}
void Itinerario::CalcularPenitencia_Costo(int cantEq) {
  std::vector<int> ConCualesTermine(cantEq);


  //PrimeraMitadDelCosto
  for (int i = 0; i < cantEq; i++) {
    //std::cout << "****************************************" << '\n';
    int DondeEstoyAhora = i;
    for (int j = 0; j < cantEq-1; j++)
     {
       //std::cout << "DondeEstoyAhora "<< DondeEstoyAhora  << '\n';
      int indice = A[i][j];
      //EL PRIMERO
      if (j == 0) {
        if(sgn(indice)==-1){ //Soy visita, me fui de casa
          Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
          DondeEstoyAhora = (-indice)-1;
        }
        //std::cout << "COSTO 1: " << Costo << " -- indice  :" << indice << '\n';

      } else
      {
         //LOS DEMAS
        if(sgn(indice)==-1){
          if(DondeEstoyAhora != i){
            Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
            //std::cout << "COSTO 2.1: " << Costo << " -- indice  :" << indice << '\n';
            DondeEstoyAhora = (-indice)-1;
          } else {
            //std::cout << "MATRIZDIST[DondeEstoyAhora][(indice)-1]; " << DondeEstoyAhora[0][3] << '\n';
            Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
            DondeEstoyAhora =  (-indice)-1;
            //std::cout << "COSTO 2.2: " << Costo << " -- indice  :" << indice << '\n';
          }
        } else if(sgn(indice) == 1){
          if (DondeEstoyAhora != i) {
            Costo += MATRIZDIST[DondeEstoyAhora][i];
            DondeEstoyAhora = i;
          }
          //std::cout << "COSTO 3: " << Costo<< " -- indice  :" << indice << '\n';
        }
      }

      if(j == cantEq-2)
      {
        ConCualesTermine[i] = (DondeEstoyAhora);
      }
    }
    //std::cout << "Dondeqiedealfinal "<< DondeEstoyAhora  << '\n';
  }


  //SegundaMitadDelCosto
  for (int i = 0; i < cantEq; i++) {
    //std::cout << "****************************************" << '\n';
    int DondeEstoyAhora = ConCualesTermine[i];
    for (int j = 0; j < cantEq-1; j++)
     {
       //std::cout << "DondeEstoyAhora "<< DondeEstoyAhora  << '\n';
      int indice = -(A[i][j]);
      //EL PRIMERO
      if (j == 0) {
        if(sgn(indice)==-1){ //Soy visita, me fui de casa
          Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
          DondeEstoyAhora = (-indice)-1;
        }
        //std::cout << "COSTO 1: " << Costo << " -- indice  :" << indice << '\n';

      } else
      {
         //LOS DEMAS
        if(sgn(indice)==-1){
          if(DondeEstoyAhora != i){
            Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
            //std::cout << "COSTO 2.1: " << Costo << " -- indice  :" << indice << '\n';
            DondeEstoyAhora = (-indice)-1;
          } else {
            //std::cout << "MATRIZDIST[DondeEstoyAhora][(indice)-1]; " << DondeEstoyAhora[0][3] << '\n';
            Costo += MATRIZDIST[DondeEstoyAhora][(-indice)-1];
            DondeEstoyAhora =  (-indice)-1;
            //std::cout << "COSTO 2.2: " << Costo << " -- indice  :" << indice << '\n';
          }
        } else if(sgn(indice) == 1){
          if (DondeEstoyAhora != i) {
            Costo += MATRIZDIST[DondeEstoyAhora][i];
            DondeEstoyAhora = i;
          }
          //std::cout << "COSTO 3: " << Costo<< " -- indice  :" << indice << '\n';
        }
      }

      if(j == cantEq-2)
      {
        Costo += MATRIZDIST[DondeEstoyAhora][i];
      }
    }
    //std::cout << "Dondeqiedealfinal "<< DondeEstoyAhora  << '\n';
  }

  vector< vector<int> > ScheduleEntero(cantEq,vector<int>((cantEq-1)*2));
  for (int i = 0; i < cantEq; i++) {
    for (int j = 0; j < (cantEq-1) ;j++) {
      ScheduleEntero[i][j] = A[i][j];
    }
  }
  for (int i = 0; i < cantEq; i++) {
    for (int j = cantEq-1; j < (cantEq-1)*2 ;j++) {
      ScheduleEntero[i][j] = - A[i][j-(cantEq-1)];
    }
  }

  int conthomestand=0;
  int controadtrip=0;
  for (int i = 0; i < cantEq; i++) {
    for (int j = 0; j < ((cantEq-1)*2); j++) {
      if(sgn(ScheduleEntero[i][j]) == 1)
      {
        conthomestand+=1;
        controadtrip=0;
        if (conthomestand > HOME_STAND) {
          Penalidad+=1;
          conthomestand=1;
        }
      } else
      {
        conthomestand=0;
        controadtrip+=1;
        if (controadtrip > ROAD_TRIP) {
          Penalidad+=1;
          controadtrip=1;
        }
      }
    }
  }

  Value = Costo + (5000 * Penalidad);
}


static std::vector<Itinerario> POBLACION;
vector<int> fisherYatesShuffling(int *arr, int n)
{
    vector<int> v(n);
    int a[n];
    int ind[n];
    for (int i = 0; i < n; i++)
        ind[i] = 0;
    int index;
    for (int i = 0; i < n; i++)
    {
        do
        {
            index = rand() % n;
        }
        while (ind[index] != 0);
        ind[index] = 1;
        a[i] = *(arr + index);
    }
    for (int i = 0; i < n; i++)
    {
        v[i]=a[i];
    }
    return v;
}

//vector<Itinerario>
void CrearPoblacion(int CantEquipos)
{
  int CantidadRondas = CantEquipos -1; // * 2
  int arreglo[CantEquipos];

  for (int i = 0; i < CantEquipos; i++) {
    arreglo[i] = i;
  }

  for (int j = 0; j < CANT_POBL; j++) {
    vector<int> vect(CantEquipos);
    vect = fisherYatesShuffling(arreglo,CantEquipos);
    Itinerario itinerario(vect,CantEquipos);
    itinerario.CalcularPenitencia_Costo(CantEquipos);
    POBLACION.push_back(itinerario);
  }
  std::cout << "$$$$$$$$$$$$$$$$$$$$$$$" << '\n';
  for (int i = 0; i < CANT_POBL; i++) {
    std::cout << "HIJO N: " << i << " COSTO : " << POBLACION[i].Costo << " Penalidad : " << POBLACION[i].Penalidad << " Valor : " << POBLACION[i].Value << '\n';
    /*
    for (int j = 0; j < CantEquipos; j++) {
      for (int l = 0; l < CantEquipos -1 ; l++) {
        //std::cout << "indice j y l" << j << "--"<< l << '\n';
        std::cout << POBLACION[i].A[j][l] << ' ';
      }
      std::cout << '\n';
    }
    std::cout << "$$" << '\n';
    */
  }
}

Itinerario Mutacion(Itinerario padre1, Itinerario padre2, int n)
{
  std::cout << "MUTACION 1" << '\n';
  std::vector<int> nuevoVectorItinerario;
  std::vector<int> YaUsados;

  std::cout << "$$$$$$$$$$" << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << padre1.A[i][0] << ' ';
  }
  std::cout << "\n$$$$$$$$$$" << '\n';
  std::cout << "$$$$$$$$$$" << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << padre2.A[i][0] << ' ';
  }

  std::cout << "\n$$$$$$$$$$vectror si haay" << '\n';
  //Lista que sera usada para saber que numeros ya he usado para no repetir
  for (int i = 1; i < n+1; i++) {
    YaUsados.push_back(i);
  }
  for (int i = 0; i < n; i++) {
      std::cout << YaUsados[i];
  }

  std::cout << '\n';
  std::cout << "MUTACION 2" << '\n';

  //agregar genes de cada padre, sin repetir, hasta tener la cantidad necesarias
  int cantAgregados = 0;
  do {
    int cualPadre = rand()%2;
    if( cualPadre == 1)
    {
      int IndicedeFila= rand()%n;
      //std::cout << "IndicedeFila A: " << IndicedeFila << '\n';
      int GenActual = padre1.A[IndicedeFila][0];
      //std::cout << "GEN ACTUAL A:  "<< GenActual << '\n';

      if(sgn(GenActual) == -1) {
        GenActual = (-GenActual)-1;
      } else{
        GenActual -= 1;
      }

      //Si no esta usado, agregarlo
      if(YaUsados[GenActual] != -1)
      {
        YaUsados[GenActual] = -1;
        nuevoVectorItinerario.push_back(GenActual+1);
        cantAgregados +=1;
        std::cout << "agregadoA : " << GenActual << '\n';
      }
    } else
    {
      int IndicedeFila= rand()%(n-1);
      int GenActual = padre2.A[IndicedeFila][0];

      if(sgn(GenActual) == -1) {
        GenActual = (-GenActual)-1;
      } else{
        GenActual -= 1;
      }

      //Si no esta usado, agregarlo
      if(YaUsados[GenActual] != -1)
      {
        YaUsados[GenActual] = -1;
        nuevoVectorItinerario.push_back(GenActual+1);
        cantAgregados +=1;
        std::cout << "agregadoB : " << GenActual << '\n';
      }
    }
  } while(cantAgregados != n);
  /*do {
    int random = rand()%2;
    //Elegir al azar a uno de los padres
    int cantGenes = rand()%3; ;

    if(random == 1)
    {
    //std::cout << "MUTACION 3" << '\n';
      if (cantGenes == 0) cantGenes = 1;
      int indice = rand()%(n-1);
      do {

        int genactual = (padre1.A[0][indice]);
        std::cout << "GEN PADRE 1 -- INDICE " <<  genactual << " -- " << indice << '\n';


        if(sgn(genactual) == -1) {
          genactual = (-genactual)-1;
        } else{
          std::cout << "GEN ACTUAL 1 " <<  genactual << '\n';

          genactual -= 1;

          std::cout << "GEN ACTUAL 1.2 " <<  genactual << '\n';

        }
        bool usado = false;
        //Revisar si ya lo use
        if (YaUsados[genactual] == -1) {
            usado = true;
        }
        if (usado == false) {
          nuevoVectorItinerario.push_back(genactual);
          cantAgregados +=1;
          std::cout << "MUTACION CANT AGREG " << cantAgregados << '\n';
          YaUsados[genactual] = -1;
        }

        indice += 1;
        cantGenes -= 1;
      } while(cantGenes > 0);

    } else
    {
      //std::cout << "MUTACION 4" << '\n';

      if (cantGenes == 0) cantGenes = 1;
      int indice = rand()%(n-1);
      do {
        int genactual = (padre2.A[0][indice]);
        std::cout << "GEN PADRE 2 -- INDICE " <<  genactual << " -- " << indice << '\n';

        if(sgn(genactual) == -1) {
          genactual = (-genactual)-1;
        } else{
          std::cout << "GEN ACTUAL 2 " <<  genactual << '\n';
          genactual -= 1;
          std::cout << "GEN ACTUAL  2.2 " <<  genactual << '\n';
        }
        bool usado = false;
        //Revisar si ya lo use
        if (YaUsados[genactual] == -1) {
            usado = true;
        }
        if (usado == false) {
          nuevoVectorItinerario.push_back(genactual);
          cantAgregados +=1;
          std::cout << "MUTACION CANT AGREG" << cantAgregados << '\n';
          YaUsados[genactual] = -1;
        }

        indice += 1;
        cantGenes -= 1;
        std::cout << "MUTACION 5" << '\n';

      } while(cantGenes > 0);

    }
  } while(cantAgregados != (n-1)); */
  for (int i = 0; i < n; i++) {
    std::cout << YaUsados[i] << " " ;
  }
  std::cout << "\n#######" << '\n';
std::cout << "END" << '\n';
  for (int m = 0; m < n; m++) {
    nuevoVectorItinerario[m] = nuevoVectorItinerario[m]-1;
    std::cout << nuevoVectorItinerario[m] << " ";
  }
  std::cout << "END2" << '\n';

  //Crear hijo, calcular sus valores, retornan
  Itinerario hijo(nuevoVectorItinerario, n);
  std::cout << "END2.5" << '\n';

  hijo.CalcularPenitencia_Costo(n);
  std::cout << "END3" << '\n';

  return hijo;
}



int main()
{
  stringstream ss;
  string TipoDeInstancia;
  string NumeroDeInstancia;
  string filepath;
  cout << "Insertar Instancia (NFL, NL, SUPER): ";
  cin >> TipoDeInstancia;

  cout << TipoDeInstancia << '\n'; //debuglog

  if(TipoDeInstancia.compare("NFL")==0)
  {
    cout << "Insertar numero (16,18): ";
  } else if(TipoDeInstancia.compare("NL")==0)
  {
    cout << "Insertar numero (4,6,8,10,12,14): ";
  } else if(TipoDeInstancia.compare("SUPER")==0)
  {
    cout << "Insertar numero (4,6,8,10): ";
  }
  cin >> NumeroDeInstancia;
  ss << "instances/" << TipoDeInstancia << "/" << TipoDeInstancia << NumeroDeInstancia << ".txt";
  filepath = ss.str();
  cout << filepath << '\n'; //debuglog
  INSTANCIA = TipoDeInstancia;
  ifstream infile(filepath);

  int indice = stoi( NumeroDeInstancia );
  int** MatrizDeDistancias = CrearMatriz(indice,indice);

  string s;
  int filas = 0;
  while (getline(infile,s))
  {
    //std::cout << line << '\n';
    size_t pos = 0;
    string token;
    string delimiter = " ";
    int colum = 0;

    while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0,pos);
      MatrizDeDistancias[filas][colum] = stoi( token );
      s.erase(0, pos + delimiter.length());
      colum++;
    }
    size_t pos2 = s.find("\n");
    token = s.substr(0,pos2);
    MatrizDeDistancias[filas][colum] = stoi( token );
    filas++;;


  }
  MATRIZDIST = MatrizDeDistancias;
  CrearPoblacion(indice);

  //comenzar el algoritmo
  std::cout << "ALGORTIMO 1" << '\n';
  //POR CANTIDAD DE GENERACIONES
  for (int i = 0; i < CANT_GENS; i++) {
    vector<Itinerario> POBLACIONNUEVA;
    int indiceMin1;
    int indiceMin2;
    int min1 = numeric_limits<int>::max();
    int min2 = numeric_limits<int>::max();
    int promedio = 0;
    int IndividuosEnPoblacionNueva = 2;
    std::cout << "ALGORTIMO 2" << '\n';

    //Guardar los 2 mejores de la poblacion
    for (int j = 0; j < CANT_POBL; j++)
    {
      int Actual = POBLACION[j].Value;
      promedio += Actual;
      if(Actual < min1)
      {
        min1 = Actual;
        indiceMin1 = j;
      }
    }
    std::cout << "INDICE 1 Y 2 " << indiceMin1 << " " <<  indiceMin2<< '\n';

    for (int j = 0; j < CANT_POBL; j++)
    {
      int Actual = POBLACION[j].Value;
      if(Actual < min2 && Actual != min1)
      {
        min2 = Actual;
        indiceMin2 = j;
      }
    }
    std::cout << "INDICE 1 Y 2 " << indiceMin1 << " " <<  indiceMin2<< '\n';
    promedio = promedio/CANT_POBL;
    POBLACIONNUEVA.push_back(POBLACION[indiceMin1]);
    POBLACIONNUEVA.push_back(POBLACION[indiceMin2]);
    std::cout << "ALGORTIMO 3 Y PROMEDIO "<< promedio << '\n';

    //POR CANTIDAD DE INDIVIDUOS
    do {
      //Elegir 2 miembros al azar y aplicar mutacion
      int random = rand()%CANT_POBL;
      Itinerario padre1 = POBLACION[random];
      std::cout << "ALGORTIMO 3.1" << '\n';

      random = rand()%CANT_POBL;
      Itinerario padre2 = POBLACION[random];
      std::cout << "ALGORTIMO 3.2" << '\n';
      bool hijoCreado = false;
      random = rand()%CANT_POBL;

      //Criterio de mutacion random>25 y si el valor de el hijo es mayor al promedio
      if (random > 0 && (padre2.Value > promedio) || (padre1.Value > promedio)) {
        hijoCreado = true;
        std::cout << "MUTACION" << '\n';
        Itinerario hijo = Mutacion(padre1,padre2,indice);

        POBLACIONNUEVA.push_back(hijo);
        IndividuosEnPoblacionNueva += 1;

      }

      //Aplicar busqueda local al Hijo

      //Evaluar

      //Agregar a La nueva poblacion


    } while(IndividuosEnPoblacionNueva != CANT_POBL);
    std::cout << "ALGORTIMO 4" << '\n';

    POBLACION = POBLACIONNUEVA;
    std::cout << "ALGORTIMO 5" << '\n';

  }

  return 0;
}
