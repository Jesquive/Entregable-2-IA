#include <iostream>
#include <utility>
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
static int CANT_POBL = 200;
static int CANT_GENS = 10;
static std::string NL[16] = {"ATL", "NYM", "PHI", "MON", "FLA", "PIT", "CIN", "CHI", "STL", "MIL", "HOU", "COL", "SF", "SD", "LA", "ARI"};
static std::string Super[14] = {"BFN", "AKL", "CAN", "PRE", "HLM", "SYD", "JOH", "CHC", "BRI", "DUR", "DUN", "PER", "CPT", "WLG"};
static std::string NFL[32] = {"BOS", "MIA", "BUF", "NYJ", "CIN", "PIT", "BAL", "CLE", "IND", "JAC", "NAS", "HOU", "DEN", "SAN","KAN", "OAK", "NYG", "DAL", "WAS", "PHI", "CHI", "MIN", "DET", "GBY", "CHA", "TAM", "ATL", "NOR", "SEA", "STL", "PHO", "SFO"};
static std::string INSTANCIA;
static int** MATRIZDIST;
static int MAXTEMPERATURA = 100;
static int DISMTEMPERATURA = 10;
static int MOV1SA = 100;
static int MOV2SA = 85;
static int MOV3SA = 100;



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
    vector<int> VectorGenerador;
    vector<int> VectorGenerador2;
    vector< vector<int> > A;
    Itinerario(vector<int> v, vector<int> v2, int n);
    Itinerario(int cost, int pen, int val, int n);
    void CalcularPenitencia_Costo(int cantEq);
    void SWAPHOMES(void);
    void SWAPMATCH(int x, int i, int val);
    void ImprimirRespuesta(int n);


};
//Constructor inicial de poblacion
Itinerario::Itinerario( vector<int> v, vector<int> v2, int n) : cantEquipos(n),   A(n, vector<int>((n-1) * 2))
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

  VectorGenerador.swap(v);

  //////////////////////////////////////////////////////////////////////////////
  Indicefijo = v2[0];
  ValorReal = v2[0];

  for (int i = n-1; i <  (n-1)*2; i++)
  {
      //El fijo con el segundo siempre van, i define la columna rde la actual ronda
      A[Indicefijo][i] = v2[1];
      A[v2[1]][i] = ValorReal;
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
        A[v2[indiceBajo]][i] =  v2[indiceAlto];
        A[v2[indiceAlto]][i] =  v2[indiceBajo];

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
      int nuevoUltimo = v2[1];
      for (int k = 1; k < n; k++)
      {
        //Cuando no se salga del largo del arreglo
        if(k+1 != n)
        {
          v2[k] = v2[k+1];
        } else
        {
          v2[n-1] = nuevoUltimo;
        }
      }
  }

  //SUMARLE 1 A TODOS
  for ( int i = 0; i < n; i++) {
    for (  int j = n-1; j < (n-1)*2; j++)
    {
      //std::cout << "print 1" << '\n';
      A[i][j] =  (A[i][j]+1);
    }
  }

  int SignoAPoner = 1;
  //Cambiarle los signos SWAPHOMES
  for ( int i = 0; i < n; i++) {
    for (  int j = n-1; j < (n-1)*2; j++)
    {
      int valorActual = A[i][j];
      //Revisa la parte anterior y pone el signo contrario
      for (int l = 0; l < n-1; l++) {
        int ValorAnterior = A[i][l];

        //std::cout << "VALOR ANTERIOR ; "<<ValorAnterior << " VALORACTUAL " << valorActual << '\n';

        if(valorActual == ValorAnterior)
        {
          if(sgn(ValorAnterior) == 1){
            SignoAPoner = -1;
          } else {
            SignoAPoner = 1;
          }
          break;
        } else if(valorActual == -ValorAnterior){
          if(sgn(ValorAnterior) == 1){
            SignoAPoner = -1;
          } else {
            SignoAPoner = 1;
          }
        }
      }
      //Cambiar Signo
      A[i][j] = valorActual * SignoAPoner;

    }
  }

  VectorGenerador2.swap(v2);

  ///////////////////////////
  int col1 = n-2;
  int col2 = n-1;
  int cantRepetears = 0;
  for (int i = 0; i < n; i++) {
    //std::cout << "cols " <<A[i][col1] << " " <<A[i][col2] << '\n';
    if(A[i][col1] == -A[i][col2]){
      cantRepetears += 1;
    }
  }
  if (cantRepetears > 0) {
    int minCantRep = cantRepetears;
    int colAIntercambiar = -1;
    for (int b = n; b < ((n-1)*2); b++) {
      int cantRepetearsNew = 0;
      for (int i = 0; i < n; i++) {
        //std::cout << "cols " <<A[i][col1] << " " <<A[i][col2] << '\n';
        if(A[i][col1] == -A[i][b]){
          cantRepetearsNew += 1;
        }
      }

      if (cantRepetearsNew < minCantRep) {
        //std::cout << "repetears--------3----------3------3-------------- "  << cantRepetearsNew<< '\n';
        minCantRep = cantRepetearsNew;
        colAIntercambiar = b;
      }
    }
    if (colAIntercambiar == -1) {
      Penalidad +=1;
    } else {
      //std::cout << "----------------------" << '\n';
      //std::cout << colAIntercambiar <<" COLUMNA" << '\n';

      for (int i = 0; i < n; i++) {
        //std::cout << "cols " <<A[i][col1] << " " <<A[i][col2] << '\n';
        //std::cout << A[i][colAIntercambiar] << " ";
        int temp = A[i][col2];
        A[i][col2]  = A[i][colAIntercambiar];
        A[i][colAIntercambiar] = temp;
      }
      //std::cout << "\n----------------------" << '\n';

    }
  }
  ///////////////////////////

}

Itinerario Mutacion(Itinerario padre1, Itinerario padre2, int n)
{
  //std::cout << "MUTACION 1" << '\n';
  std::vector<int> nuevoVectorItinerario;
  std::vector<int> nuevoVectorItinerario2;
  std::vector<int> YaUsados;
  std::vector<int> YaUsados2;



  //std::cout << "\n$$$$$$$$$$ si haay" << '\n';
  //Lista que sera usada para saber que numeros ya he usado para no repetir
  for (int i = 1; i < n+1; i++) {
    YaUsados.push_back(i);
    YaUsados2.push_back(i);
  }

  //agregar genes de cada padre, sin repetir, hasta tener la cantidad necesarias
  int cantAgregados = 0;
  do {
    int cualPadre = rand()%2;
    if( cualPadre == 1)
    {
      int IndicedeFila= rand()%n;
      //std::cout << "IndicedeFila A: " << IndicedeFila << '\n';
      int GenActual = padre1.VectorGenerador[IndicedeFila];

      //std::cout << "GEN ACTUAL A:  "<< GenActual << '\n';

      //Si no esta usado, agregarlo
      if(YaUsados[GenActual] != -1)
      {
        YaUsados[GenActual] = -1;
        nuevoVectorItinerario.push_back(GenActual+1);
        cantAgregados +=1;
        //std::cout << "agregadoA : " << GenActual << '\n';
      }
    } else
    {
      int IndicedeFila= rand()%(n-1);
      int GenActual = padre2.VectorGenerador[IndicedeFila];



      //Si no esta usado, agregarlo
      if(YaUsados[GenActual] != -1)
      {
        YaUsados[GenActual] = -1;
        nuevoVectorItinerario.push_back(GenActual+1);
        cantAgregados +=1;
        //std::cout << "agregadoB : " << GenActual << '\n';
      }
    }
  } while(cantAgregados != n);

  for (int m = 0; m < n; m++) {
    nuevoVectorItinerario[m] = nuevoVectorItinerario[m]-1;
    //std::cout << nuevoVectorItinerario[m] << " ";
  }
  ///////////////////////////////////////////////////////
  cantAgregados = 0;
  do {
    int cualPadre = rand()%2;
    if( cualPadre == 1)
    {
      int IndicedeFila= rand()%n;
      //std::cout << "IndicedeFila A: " << IndicedeFila << '\n';
      int GenActual = padre1.VectorGenerador2[IndicedeFila];

      //std::cout << "GEN ACTUAL A:  "<< GenActual << '\n';

      //Si no esta usado, agregarlo
      if(YaUsados2[GenActual] != -1)
      {
        YaUsados2[GenActual] = -1;
        nuevoVectorItinerario2.push_back(GenActual+1);
        //std::cout << "GENAGREGADO " << GenActual+1 << '\n';

        cantAgregados +=1;
        //std::cout << "agregadoA : " << GenActual << '\n';
      }
    } else
    {
      int IndicedeFila= rand()%(n-1);
      int GenActual = padre2.VectorGenerador2[IndicedeFila];



      //Si no esta usado, agregarlo
      if(YaUsados2[GenActual] != -1)
      {
        YaUsados2[GenActual] = -1;
        nuevoVectorItinerario2.push_back(GenActual+1);
        //std::cout << "GENAGREGADO " << GenActual+1 << '\n';
        cantAgregados +=1;
        //std::cout << "agregadoB : " << GenActual << '\n';
      }
    }
  } while(cantAgregados != n);

  for (int m = 0; m < n; m++) {
    nuevoVectorItinerario2[m] = nuevoVectorItinerario2[m]-1;
    //std::cout << nuevoVectorItinerario[m] << " ";
  }

  //Crear hijo, calcular sus valores, retornan
  Itinerario hijo(nuevoVectorItinerario, nuevoVectorItinerario2,n);
  //std::cout << "END2.5" << '\n';

  hijo.CalcularPenitencia_Costo(n);
  //std::cout << "END3" << '\n';

  return hijo;
}
vector<int> CalcularPenCost(int cantEq, vector<vector<int>> aEvaluar )
{
  int Costo = 0;
  int Penalidad = 0;
  int Value = 0;
  vector<int> Respuesta;

  //PrimeraMitadDelCosto
  for (int i = 0; i < cantEq; i++) {
    //std::cout << "****************************************" << '\n';
    int DondeEstoyAhora = i;
    for (int j = 0; j < (cantEq-1)*2; j++)
     {
       //std::cout << "DondeEstoyAhora "<< DondeEstoyAhora  << '\n';
      int indice = aEvaluar[i][j];
      //std::cout << "INDICE ANTES DE CAMBIO " <<indice << '\n';

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
          //std::cout << "DONDE ESTOY AHOR E INDICE **" <<DondeEstoyAhora <<"**"<< (-indice)-1 <<'\n';
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
    }
    if (DondeEstoyAhora != i) {
      Costo += MATRIZDIST[DondeEstoyAhora][i];
      DondeEstoyAhora = i;
    }
  }

  int conthomestand=0;
  int controadtrip=0;
  for (int i = 0; i < cantEq; i++) {
    conthomestand=0;
    controadtrip=0;
    for (int j = 0; j < ((cantEq-1)*2); j++) {
      if(sgn(aEvaluar[i][j]) == 1)
      {
        conthomestand+=1;
        controadtrip=0;
        if (conthomestand > HOME_STAND) {
          //std::cout << "home stand detectado EN FILA -->" <<i << '\n';
          Penalidad+=1;
          conthomestand-=1;
        }
      } else
      {
        conthomestand=0;
        controadtrip+=1;
        if (controadtrip > ROAD_TRIP) {
          //std::cout << "ROAD TRIP detectado EN FILA -->" << i<< '\n';
          Penalidad+=1;
          controadtrip-=1;
        }
      }
      //std::cout << "{{{{{{{{{{{{}}}}}}}}}}}}  conthomestand-->"<< conthomestand << '\n';
      //std::cout << "{{{{{{{{{{{{}}}}}}}}}}}}  controadtrip-->"<< controadtrip << '\n';

    }
  }
  for (int i = 0; i < cantEq; i++) {
    if (aEvaluar[i][cantEq-2] == -aEvaluar[i][cantEq-1]) {
      //std::cout << "REPETEAR EVAL" << '\n';
      Penalidad+=1;

    }
  }
  Value = Costo + (5000 * Penalidad);

  //std::cout << "penlaidad " <<Penalidad  <<'\n';
  Respuesta.push_back(Costo);
  Respuesta.push_back(Penalidad);
  Respuesta.push_back(Value);
  return Respuesta;

}

void Itinerario::SWAPHOMES(void)
{
  //Se hara SA con 3 movimientos, teniendo mas probabilidad de elegir el mas simple per efectivo
  int temperatura = MAXTEMPERATURA;
  int choice = rand()%100;

  //Se crea la arreglo B, la cual se le haran los cambios de los movimientos
  // y el arreglo C que siempre tendra al mejor hasta entonces
  vector < vector <int>> B(cantEquipos , vector<int>((cantEquipos-1)*2));
  vector < vector <int>> C(cantEquipos , vector<int>((cantEquipos-1)*2));
  //std::cout << "+++CLON+++" << '\n';
  for (int i = 0; i < cantEquipos; i++) {
    for (int j = 0; j < (cantEquipos-1)*2; j++) {
      B[i][j] = A[i][j];
      C[i][j] = A[i][j];
      //std::cout << B[i][j] << ' ';
    }
    //std::cout << '\n';
  }
  //std::cout << "++++++++++" << '\n';
  /*
  std::cout << "<-----------------------> " << '\n';

  for (int i = 0; i < cantEquipos; i++) {
    for (int j = 0; j < (cantEquipos-1)*2; j++) {
      std::cout << C[i][j] << "   ";
    }
    std::cout << '\n';
  }
  std::cout << "<-----------------------> " << '\n';
  */
  vector<int> datos; //(Costo , penalidad , value)
  vector<int> datosC;
  datosC.push_back(Costo);
  datosC.push_back(Penalidad);
  datosC.push_back(Value);

  //MOVIMIENTO SWAṔHOMES
  if(choice < MOV1SA)
  {
    //SWAPHOMES/////////////////////////77
    bool BreakingPoint = false;
    int VecesEstancados = 0;
    int VecesSinCambios = 0;
    while( VecesEstancados <= 3){

      for (int k = 0; k < cantEquipos; k++) {
        for (int l = 0; l < cantEquipos-1; l++) {
          int indice = B[k][l];
          int reset1 = indice;
          int reset3;
          int reset4;
          int OtraCol;
          int indice2;
          if(sgn(indice)==-1){
            indice = -(indice) -1 ;
          } else {
            indice -= 1;
          }
          int reset2 = B[indice][l];

          for (int p = 0; p < (cantEquipos-1)*2; p++) {
          //  std::cout << "swaprevealer -- FOR -- B[k][l] -->" << B[k][l] << "  - B[k][p]--> " << - B[k][p]  << '\n';

            if(B[k][l] == - B[k][p]){
              OtraCol = p;
              indice2 = B[k][p];
              reset3 = indice2;
              if(sgn(indice2)==-1){
                indice2 = -(indice2) -1 ;
              } else {
                indice2 -= 1;
              }
              reset4 = B[indice2][p];
              break;
            }
          }
          //std::cout << "swaprevealer -- OUTFOR" << '\n';

          //Cambiar los signos de los 2
          //std::cout << "CAMBIO DE INDICES -- " <<-(indice) << " * " << -(B[indice][l]) << '\n';
          B[k][l] = -(B[k][l]);
          //std::cout << "swaprevealer -- ASIGNACION1" << '\n';

          B[indice][l] = -(B[indice][l]);

           //std::cout << "swaprevealer -- ASIGNACION2" << '\n';

          B[k][OtraCol] = -(B[k][OtraCol]);

          // std::cout << "swaprevealer -- ASIGNACION3" << '\n';

          B[indice2][OtraCol] = -(B[indice2][OtraCol]);


          //Recalcular costo-penalidad-value
          /*
          std::cout << "-------------SWAPHOM-----------------------" << '\n';
          for (size_t i = 0; i < cantEquipos; i++) {
            for (size_t j = 0; j < (cantEquipos-1)*2; j++) {
              std::cout << B[i][j] << "  ";
            }
            std::cout << '\n';
          }
          std::cout << "------------------------------------" << '\n';
            */

          //std::cout << "CALCULAR NUEVOS COSTOS" << '\n';
          //std::cout << "MODIFICAR COSTOS DE INDICES " << k << "**" <<  l<< '\n';
          datos = CalcularPenCost(cantEquipos,B);
          //std::cout << "COSTO -> "<<datos[0] <<"  COSTOC--> "<<datosC[0] << '\n';

          //std::cout << "PENALIDAD -> "<<datos[1] <<"  PENALIDADC--> "<<datosC[1] << '\n';

          //std::cout << "VALUE -> "<<datos[2] <<"  VALUEC--> "<<datosC[2] << '\n';
          //Comparar si mejoro
          if (datos[2] < datosC[2]) {
          //std::cout << "SE ENCONTRO UNO MEJOR" << '\n';
            for (int o = 0; o < 3; o++) {
              datosC[o] = datos[o];
              //std::cout << "datos[o]  " << datos[o]<< '\n';
            }
            C[k][l] = B[k][l];
            C[indice][l] = B[indice][l];
            C[k][OtraCol] = B[k][OtraCol];
            C[indice2][OtraCol] = B[indice2][OtraCol];
            BreakingPoint = true;
            break;

          } else {
            B[k][l] = reset1;
            B[indice][l] = reset2 ;
            B[k][OtraCol] = reset3;
            B[indice2][OtraCol] = reset4;
            VecesSinCambios += 1;
            //std::cout << "VECES SIN CAMBIO " << VecesSinCambios<< '\n';
          }
        }

        //std::cout << "BREAKINGPOINT -- " << BreakingPoint << '\n';
        if (VecesSinCambios >= (cantEquipos)*(cantEquipos-1)) {
          VecesEstancados+=1;
        }
        if(BreakingPoint == true)
        {
          break;
        }
      }
    }

    Costo = datosC[0];
    Penalidad =datosC[1];
    Value =datosC[2];

    // std::cout << "/* message */" << '\n';
    for (int i = 0; i < cantEquipos; i++) {
      for (int j = 0; j < (cantEquipos-1)*2; j++) {
        A[i][j] = C[i][j];
        //std::cout << A[i][j] << ' ';
      }
      //std::cout << '\n';
    }
    //std::cout << "/* message */" << '\n';


  }


}


void Itinerario::SWAPMATCH(int x, int y, int val){
  vector < vector <int>> B(cantEquipos , vector<int>((cantEquipos-1)*2));
  vector < vector <int>> C(cantEquipos , vector<int>((cantEquipos-1)*2));
  vector < vector <int>> USED(cantEquipos , vector<int>((cantEquipos-1)*2));

  for (int i = 0; i < cantEquipos; i++) {
    for (int j = 0; j < (cantEquipos-1)*2; j++) {
      B[i][j] = A[i][j];
      C[i][j] = A[i][j];
      USED[i][j] = A[i][j];
      //std::cout << B[i][j] << ' ';
    }
    //std::cout << '\n';
  }

  vector<int> datos; //(Costo , penalidad , value)
  vector<int> datosC;
  datosC.push_back(Costo);
  datosC.push_back(Penalidad);
  datosC.push_back(Value);

  int ColSinErrores;
  //std::cout << "CANTEQ --> " << cantEquipos-1 << '\n';
  if(y < cantEquipos-1){

    //Cuadrante1
    //Obligatorio
    int temp = B[x][y];
    int indice = B[x][y];
    int Columna2;
    if(sgn(indice) == -1){
      indice = -(indice) - 1;
    } else
    {
      indice -=1;
    }
    B[x][y] = val;

    for (int i = 0; i < cantEquipos-1; i++) {
      if(C[x][i] ==  val || C[x][i] ==  -val){
        B[x][i] = temp;
        Columna2 = i;
        //std::cout << "TEMP- -> " << temp << '\n';
        break;
      }
    }

    //Agregar el primer cambio
    int temp2 = B[indice][y];

    //B[indice][y] = B[indice][Columna2];

    //B[indice][Columna2] =temp2;

    USED[x][y] = -500;
    USED[x][Columna2] = -500;



    do {
      /*
      std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
      for (size_t i = 0; i < cantEquipos; i++) {
        for (size_t j = 0; j < cantEquipos-1; j++) {
          std::cout << "   "<< B[i][j] << "   ";
        }
        std::cout << '\n';
      }
      std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
      */

      ColSinErrores = 0;
      bool flag1 = false;
      int filaError;
      int valorError;
      int valorFaltante;
      int colError;
      //Recorrer columnas, buscar el error, si pos error no esta usada, buscar el
      //valor faltante en la fila e intercambiarlos
      for (int i = 0; i < cantEquipos-1; i++) { //COLUMNAS
        std::vector<int> Encontrados(cantEquipos); //Vector de 0 que va aumentando si encuentra clones
        for (int j = 0; j < cantEquipos; j++) {//FILAS

          if (USED[j][i] == -500)
          {
            int indiceDelUsado = B[j][i];

            if(sgn(indiceDelUsado) == -1){
              indiceDelUsado = -(indiceDelUsado) - 1;
            } else {indiceDelUsado -= 1;}

            //std::cout << "INDICE DEL USADO 1--> "<< indiceDelUsado+1  << '\n';

            Encontrados[indiceDelUsado] +=1 ;

            if (Encontrados[indiceDelUsado] == 2) {
              colError = i;
              for (size_t filas = 0; filas < cantEquipos; filas++) {
                if (B[filas][i] == B[j][i] ||  B[filas][i] == -(B[j][i])) {
                  valorError = B[filas][i];
                  filaError = filas;
                  break;
                }
              }
            }

          }
          else
          {
            int indiceDelUsado = B[j][i];
            if(sgn(indiceDelUsado) == -1){
              indiceDelUsado = -(indiceDelUsado) - 1;
            } else {indiceDelUsado -= 1;}
            //std::cout << "INDICE DEL USADO 2-2--> "<< indiceDelUsado+1  << '\n';

            Encontrados[indiceDelUsado] +=1 ;
            if (Encontrados[indiceDelUsado] == 2) {
              valorError = B[j][i];
              filaError = j;
              colError = i;
            }
          }
        }


        for (size_t j = 0; j < cantEquipos; j++) {
          //std::cout << Encontrados[j] << '/';
          if (Encontrados[j] == 0) {
            valorFaltante = j+1;
            flag1 = true;
          }
        }
        //std::cout << '\n';

        if (flag1) {
          break;
        }

        ColSinErrores +=1;
      }
       //TENGI EL VALOR A CAMBIAR, LA FILA DONDE CAMBIARLO Y EL VALOR QUE BUSCO.
       //BUSCO EL VALOR A TRAVES DE LA FILA Y LO INTERCAMBIO CON EL ERROR
      //std::cout << "VALOR REPETIDO -->"<< valorError<<"  VALOR FALTANTE -->" << valorFaltante<< "  FILA CON DONDE BUSCAR -->" << filaError+1<< '\n';
      //std::cout << "COLSINERR -->"<<ColSinErrores << '\n';
      if (ColSinErrores == cantEquipos-1) {
      //std::cout << "NO ERRORES, VOLVER FINALIZAR" << '\n';
      }
      else {
        for (size_t colu = 0; colu < cantEquipos-1; colu++) {
          if(B[filaError][colu] == valorFaltante || -(B[filaError][colu]) == valorFaltante){
            //std::cout << "cambio" << '\n';
            /*
            int indTemp1 = B[filaError][colu];
            if(sgn(indTemp1) == -1){
              indTemp1 = -(indTemp1) - 1;
            } else {indTemp1 -= 1;}

            int indTemp2 = B[filaError][colError];
            if(sgn(indTemp2) == -1){
              indTemp2 = -(indTemp2) - 1;
            } else {indTemp2 -= 1;}
            */
            int valTemp =  B[filaError][colu];
            //int valTemp2 =  B[indTemp1][colu];

            //B[indTemp1][colu] = B[indTemp2][colError] ;
            B[filaError][colu] = valorError;
            //B[indTemp2][colError] = valTemp2;
            B[filaError][colError] = valTemp;

            USED[filaError][colu] = -500;
            USED[filaError][colError] = -500;

          }
        }
      }
    ///// VOLVER A EMPEZAR, TERMINAR SI ES QUE NO ENCUENTRA UN VALOR ERROR
    } while(ColSinErrores < cantEquipos-1);
  }
  else{

    //Cuadrante2
    //Obligatorio
    int temp = B[x][y];
    int indice = B[x][y];
    int Columna2;
    if(sgn(indice) == -1){
      indice = -(indice) - 1;
    }else
    {
      indice -=1;
    }
    B[x][y] = val;
    //std::cout << "val- -> " << val << '\n';

    for (int i = cantEquipos-1; i < (cantEquipos-1)*2; i++) {
      //std::cout << "C[x][i]- -> " << C[x][i] << '\n';
      // 0 4 -2
      if(C[x][i] ==  val || C[x][i] ==  -val){
        B[x][i] = temp;
        Columna2 = i;
        break;
      }
    }
    //Agregar el primer cambio
    //int temp2 = B[indice][y];
    //B[indice][y] = B[indice][Columna2];
    //B[indice][Columna2] =temp2;
    USED[x][y] = -500;
    USED[x][Columna2] = -500;

    /*
    std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
    for (size_t i = 0; i < cantEquipos; i++) {
      for (size_t j = cantEquipos-1; j < (cantEquipos-1) * 2; j++) {
        std::cout << "   "<< USED[i][j] << "   ";
      }
      std::cout << '\n';
    }
    std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
    */
    do {
      /*
      std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
      for (size_t i = 0; i < cantEquipos; i++) {
        for (size_t j = cantEquipos-1; j < (cantEquipos-1) * 2; j++) {
          std::cout << "   "<< B[i][j] << "   ";
        }
        std::cout << '\n';
      }
      std::cout << "°°°°°°°°°°°°°°°°°°°°°°°°" << '\n';
      */
      ColSinErrores = 0;
      bool flag1 = false;
      int filaError;
      int valorError;
      int valorFaltante;
      int colError;
      //Recorrer columnas, buscar el error, si pos error no esta usada, buscar el
      //valor faltante en la fila e intercambiarlos
      for (int i = cantEquipos-1; i < (cantEquipos-1)*2; i++) { //COLUMNAS
        std::vector<int> Encontrados(cantEquipos); //Vector de 0 que va aumentando si encuentra clones
        for (int j = 0; j < cantEquipos; j++) {//FILAS

          if (USED[j][i] == -500)
          {
            int indiceDelUsado = B[j][i];

            if(sgn(indiceDelUsado) == -1){
              indiceDelUsado = -(indiceDelUsado) - 1;
            } else {indiceDelUsado -= 1;}

            //std::cout << "INDICE DEL USADO 1--> "<< indiceDelUsado+1  << '\n';

            Encontrados[indiceDelUsado] +=1 ;

            if (Encontrados[indiceDelUsado] == 2) {
              colError = i;
              for (size_t filas = 0; filas < cantEquipos; filas++) {
                if (B[filas][i] == B[j][i] ||  B[filas][i] == -(B[j][i])) {
                  valorError = B[filas][i];
                  filaError = filas;
                  break;
                }
              }
            }

          }
          else
          {
            int indiceDelUsado = B[j][i];
            if(sgn(indiceDelUsado) == -1){
              indiceDelUsado = -(indiceDelUsado) - 1;
            } else {indiceDelUsado -= 1;}
            //std::cout << "INDICE DEL USADO 2-2--> "<< indiceDelUsado+1  << '\n';

            Encontrados[indiceDelUsado] +=1 ;
            if (Encontrados[indiceDelUsado] == 2) {
              valorError = B[j][i];
              filaError = j;
              colError = i;
            }
          }
        }


        for (size_t j = 0; j < cantEquipos; j++) {
          //std::cout << Encontrados[j] << '/';
          if (Encontrados[j] == 0) {
            valorFaltante = j+1;
            flag1 = true;
          }
        }
        //std::cout << '\n';

        if (flag1) {
          break;
        }

        ColSinErrores +=1;
      }
       //TENGI EL VALOR A CAMBIAR, LA FILA DONDE CAMBIARLO Y EL VALOR QUE BUSCO.
       //BUSCO EL VALOR A TRAVES DE LA FILA Y LO INTERCAMBIO CON EL ERROR
      //std::cout << "VALOR REPETIDO -->"<< valorError<<"  VALOR FALTANTE -->" << valorFaltante<< "  FILA CON DONDE BUSCAR -->" << filaError+1<< '\n';
      //std::cout << "COLSINERR -->"<<ColSinErrores << '\n';
      if (ColSinErrores == cantEquipos-1) {
      //std::cout << "NO ERRORES, VOLVER FINALIZAR" << '\n';
      }
      else {
        for (size_t colu = cantEquipos-1; colu < (cantEquipos-1)*2; colu++) {
          if(B[filaError][colu] == valorFaltante || -(B[filaError][colu]) == valorFaltante){
            //std::cout << "cambio" << '\n';
            /*
            int indTemp1 = B[filaError][colu];
            if(sgn(indTemp1) == -1){
              indTemp1 = -(indTemp1) - 1;
            } else {indTemp1 -= 1;}

            int indTemp2 = B[filaError][colError];
            if(sgn(indTemp2) == -1){
              indTemp2 = -(indTemp2) - 1;
            } else {indTemp2 -= 1;}
              */
            int valTemp =  B[filaError][colu];
            //int valTemp2 =  B[indTemp1][colu];

            //B[indTemp1][colu] = B[indTemp2][colError] ;
            B[filaError][colu] = valorError;
            //B[indTemp2][colError] = valTemp2;
            B[filaError][colError] = valTemp;

            USED[filaError][colu] = -500;
            USED[filaError][colError] = -500;
            //USED[indTemp1][colu] = -500;
            //USED[indTemp2][colError] = -500;
          }
        }
      }
    ///// VOLVER A EMPEZAR, TERMINAR SI ES QUE NO ENCUENTRA UN VALOR ERROR
    } while(ColSinErrores < cantEquipos-1);
  }
  /*
  std::cout << '\n';
  for (int i = 0; i < cantEquipos; i++) {
    for (int j = 0; j < (cantEquipos-1)*2; j++) {
      std::cout << B[i][j] << "   ";
    }
    std::cout << '\n';
  }
  std::cout << '\n';
  */

  for (size_t i = 0; i < cantEquipos; i++) {
    for (size_t j = 0; j < (cantEquipos-1)*2; j++) {
      //std::cout << USED[i][j] << "  ";
      if(USED[i][j] == -500)
      {
          int indtemp = B[i][j];
          int signTemp = -1;
          if(sgn(indtemp) == -1){
            signTemp = 1;
            indtemp = -(indtemp) - 1;
          } else {indtemp -= 1;}

          B[indtemp][j] = (i+1) * signTemp;
      }
    }
    //std::cout << '\n';
  }
  //std::cout << '\n';
  /*
  std::cout << "FFINALAJASDJFJASDFASD"<< '\n';
  for (int i = 0; i < cantEquipos; i++) {
    for (int j = 0; j < (cantEquipos-1)*2; j++) {
      std::cout << B[i][j] << "   ";
    }
    std::cout << '\n';
  }
  std::cout << '\n';
  */
  datos = CalcularPenCost(cantEquipos,B);
  Costo = datos[0];
  Penalidad = datos[0];
  Value = datos[0];

  /*
  std::cout << "FINAL, value = " << datos[2]<< '\n';
  for (size_t i = 0; i < cantEquipos; i++) {
    for (size_t j = 0; j < (cantEquipos-1)*2; j++) {
      std::cout << B[i][j] << "   ";
    }
    std::cout << '\n';
  }
  */
  if(y < cantEquipos-1)
  {
    for (size_t i = 0; i < cantEquipos; i++) {
      for (size_t j = 0; j < (cantEquipos-1); j++) {
        A[i][j] = B[i][j];
      }
    }
  } else
  {
    for (size_t i = 0; i < cantEquipos; i++)
    {
      for (size_t j = (cantEquipos-1); j < ((cantEquipos-1)*2); j++)
       {
        A[i][j] = B[i][j];
       }
    }
  }

}


void Itinerario::CalcularPenitencia_Costo(int cantEq) {
  Costo = 0;
  Penalidad = 0;
  Value = 0;
  //PrimeraMitadDelCosto
  for (int i = 0; i < cantEq; i++) {
    //std::cout << "****************************************" << '\n';
    int DondeEstoyAhora = i;
    for (int j = 0; j < (cantEq-1)*2; j++)
     {
       //std::cout << "DondeEstoyAhora "<< DondeEstoyAhora  << '\n';
      int indice = A[i][j];
      //std::cout << "INDICE ANTES DE CAMBIO " <<indice << '\n';

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
          //std::cout << "DONDE ESTOY AHOR E INDICE **" <<DondeEstoyAhora <<"**"<< (-indice)-1 <<'\n';
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



    }
    if (DondeEstoyAhora != i) {
      Costo += MATRIZDIST[DondeEstoyAhora][i];
      DondeEstoyAhora = i;
    }
    //std::cout << "Dondeqiedealfinal "<< DondeEstoyAhora  << '\n';
  }

  int conthomestand=0;
  int controadtrip=0;
  for (int i = 0; i < cantEq; i++) {
    conthomestand=0;
    controadtrip=0;
    for (int j = 0; j < ((cantEq-1)*2); j++) {
      if(sgn(A[i][j]) == 1)
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
  for (int i = 0; i < cantEq; i++) {
    if (A[i][cantEq-2] == -A[i][cantEq-1]) {
      //std::cout << "REPETEAR EVAL" << '\n';
      Penalidad+=1;

    }
  }
  Value = Costo + (5000 * Penalidad);
}


void Itinerario::ImprimirRespuesta(int n){
  std::cout << "Slot  ";
  for (int i = 0; i < n; i++) {
    std::cout << NL[i] << "   ";
  }
  std::cout << '\n';
  std::cout << "------------------------------" << '\n';
  for (int i = 0; i < (n-1)*2; i++) {
    std::cout << "  " << i << "  ";
    for (int j = 0; j < n; j++) {
      int indice = A[j][i];
      if(sgn(indice) == -1)
      {
        std::cout << " " << "@"<< NL[(-indice) -1] << " ";
      } else {
        std::cout << " " << NL[(indice) -1] << "  ";

      }
    }
    std::cout << '\n';
  }
  std::cout << "***********************************" << '\n';

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
    vector<int> vect2(CantEquipos);
    vect = fisherYatesShuffling(arreglo,CantEquipos);
    vect2 = fisherYatesShuffling(arreglo,CantEquipos);
    Itinerario itinerario(vect,vect2,CantEquipos);
    itinerario.CalcularPenitencia_Costo(CantEquipos);
    POBLACION.push_back(itinerario);
  }
  std::cout << "$$$$$$$$$$$$$$$$$$$$$$$" << '\n';
  for (int i = 0; i < CANT_POBL; i++) {
    std::cout << "HIJO N: " << i << " COSTO : " << POBLACION[i].Costo << " Penalidad : " << POBLACION[i].Penalidad << " Valor : " << POBLACION[i].Value << '\n';
  }
}


int main()
{
  stringstream ss;
  string TipoDeInstancia;
  string NumeroDeInstancia;
  string filepath;
  cout << "Insertar Instancia (NFL, NL, Super): ";
  cin >> TipoDeInstancia;

  cout << TipoDeInstancia << '\n'; //debuglog

  if(TipoDeInstancia.compare("NFL")==0)
  {
    cout << "Insertar numero (16,18): ";
  } else if(TipoDeInstancia.compare("NL")==0)
  {
    cout << "Insertar numero (4,6,8,10,12,14): ";
  } else if(TipoDeInstancia.compare("Super")==0)
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
  //std::cout << "ALGORTIMO 1" << '\n';
  //POR CANTIDAD DE GENERACIONES

  for (int i = 0; i < CANT_GENS; i++) {
    vector<Itinerario> POBLACIONNUEVA;
    int indiceMin1;
    int indiceMin2;
    int min1 = numeric_limits<int>::max();
    int min2 = numeric_limits<int>::max();
    int promedio = 0;
    int IndividuosEnPoblacionNueva = 2;
    //std::cout << "ALGORTIMO 2" << '\n';

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
    //std::cout << "INDICE 1 Y 2 " << indiceMin1 << " " <<  indiceMin2<< '\n';

    for (int j = 0; j < CANT_POBL; j++)
    {
      int Actual = POBLACION[j].Value;
      if(Actual < min2 && Actual != min1)
      {
        min2 = Actual;
        indiceMin2 = j;
      }
    }
    //std::cout << "INDICE 1 Y 2 " << indiceMin1 << " " <<  indiceMin2<< '\n';
    promedio = promedio/CANT_POBL;
    POBLACIONNUEVA.push_back(POBLACION[indiceMin1]);
    POBLACIONNUEVA.push_back(POBLACION[indiceMin2]);
    //std::cout << "ALGORTIMO 3 Y PROMEDIO "<< promedio << '\n';

    //POR CANTIDAD DE INDIVIDUOS
    std::cout << " ################ GENERACION "<< i << "  ################"<< '\n';
    std::cout << "HIJO ELITISTA " <<1 << " COSTO : "<< POBLACION[indiceMin1].Costo <<" PENALIDAD :  "<< POBLACION[indiceMin1].Penalidad << " Valor : " << POBLACION[indiceMin1].Value << '\n';
    std::cout << "HIJO ELITISTA " <<2 << " COSTO : "<< POBLACION[indiceMin2].Costo <<" PENALIDAD :  "<< POBLACION[indiceMin2].Penalidad << " Valor : " << POBLACION[indiceMin2].Value << '\n';

    do {
      //Elegir 2 miembros al azar y aplicar mutacion
      int random = rand()%CANT_POBL;
      Itinerario padre1 = POBLACION[random];
      //std::cout << "ALGORTIMO 3.1" << '\n';

      random = rand()%CANT_POBL;
      Itinerario padre2 = POBLACION[random];
      //std::cout << "ALGORTIMO 3.2" << '\n';
      bool hijoCreado = false;
      random = rand()%CANT_POBL;

      //Criterio de mutacion random>25 y si el valor de el hijo es mayor al promedio
      if (random > 15 && (padre2.Value < promedio) || (padre1.Value < promedio)) {
        hijoCreado = true;
        //std::cout << "MUTACION" << '\n';
        Itinerario hijo = Mutacion(padre1,padre2,indice);

        //Aplicar busqueda local al Hijo Y Evaluar
        std::cout << "*********VALUEANTES: " << hijo.Value << '\n';
        hijo.SWAPHOMES();


        for (size_t i = 0; i < ((indice/2)-1); i++) {
          int index1 = rand()%indice;
          int index2 = rand()%(indice-1);
          int index3 = rand()%(indice-1);
          do {
             index3 = rand()%(indice-1);
          } while(index2 == index3);
          int ValorEnIndice = hijo.A[index1][index3];
          hijo.SWAPMATCH(index1,index2,ValorEnIndice);
        }

        for (size_t i = 0; i < ((indice/2)-1); i++) {
          int index1 = rand()%indice;
          int index2 = (rand()%(indice-1)) +(indice-1);
          int index3 = (rand()%(indice-1)) +(indice-1);
          do {
             index3 = rand()%(indice-1)+(indice-1);
          } while(index2 == index3);
          int ValorEnIndice = hijo.A[index1][index3];
          hijo.SWAPMATCH(index1,index2,ValorEnIndice);
        }





        hijo.SWAPHOMES();

        std::cout << "*********VALUEDESPUES: " << hijo.Value << '\n';
        hijo.CalcularPenitencia_Costo(indice);

        //Agregar a La nueva poblacion
        std::cout << "HIJO " <<IndividuosEnPoblacionNueva+1 << " COSTO : "<< hijo.Costo <<" PENALIDAD :  "<< hijo.Penalidad << " Valor : " << hijo.Value << '\n';
        POBLACIONNUEVA.push_back(hijo);
        IndividuosEnPoblacionNueva += 1;

      }



    } while(IndividuosEnPoblacionNueva != CANT_POBL);
    //std::cout << "ALGORTIMO 4" << '\n';

    POBLACION = POBLACIONNUEVA;
    //std::cout << "ALGORTIMO 5" << '\n';

  }

  int menor = numeric_limits<int>::max();;
  int indiceDelMenor;
  for (size_t i = 0; i < CANT_POBL; i++) {
    if(POBLACION[i].Value < menor){
      menor = POBLACION[i].Value;
      indiceDelMenor = i;
    }
  }

  std::cout << " \n EL MEJOR MIEMBRO DE LA ULTIMA POBLACION, TIENE UN VALOR DE : " << POBLACION[indiceDelMenor].Value << '\n';
  POBLACION[indiceDelMenor].ImprimirRespuesta(indice);
    std::cout << "///////////////////////////////////////" << '\n';
    for (int i = 0; i < indice; i++) {
      for (int j = 0; j < (indice-1)*2; j++) {
        std::cout << POBLACION[indiceDelMenor].A[i][j] << "   ";
      }
      std::cout << '\n';
    }
    std::cout << "///////////////////////////////////////" << '\n';

    POBLACION[indiceDelMenor].CalcularPenitencia_Costo(indice);




  return 0;
}
