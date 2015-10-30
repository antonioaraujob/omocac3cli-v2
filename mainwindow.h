#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simulation.h"
//#include "qcustomplot.h"

#include "gen.h"
#include "scan.h"
#include "emulator.h"

#include <QSettings>

class MainWindow
{


private:

    /**
     * @brief Tamano de la poblacion para el algoritmo cultural
     */
    int population;

    /**
     * @brief Numero maximo de generaciones
     */
    int generationsNumber;


    /**
     * @brief Desviacion estandar de la mutacion
     */
    int standarDeviation;

    /**
     * @brief Desviacion estandar de MinChannelTime
     */
    int standarDeviationMinChannelTime;

    /**
     * @brief Desviacion estandar de MaxChannelTime
     */
    int standarDeviationMaxChannelTime;


    /**
     * @brief Tamano maximo del archivo externo
     */
    int externalFileSize;

    /**
     * @brief Numero de subintervalos de la rejilla del espacio de creencias
     */
    int gridSubintervalnumber;

    /**
     * @brief Frecuencia de actualizacion de parte normativa
     */
    int updateFrequency;

    /**
     * @brief Numero de encuentros por individuos en torneos
     */
    int matchesPerIndividual;

    /**
     * @brief Probabilidad de mutacion dirigida
     */
    double mutationProbability;

    /**
     * @brief Verdadero si se debe ejecutar la mutacion dirigida
     */
    bool doDirectedMutation;

    /**
     * @brief Verdadero si se debe ejecutar el algoritmo original
     */
    bool doOriginalAlgorithm;

    /**
     * @brief Numero de repeticiones para ejecutar la comparacion de algoritmos
     */
    int simulationRepetitions;

    /**
     * @brief Verdadero si se debe realizar la comparacion de los algoritmos
     */
    bool doComparationOfAlgorithms;

    /**
     * @brief Objeto Simulation que abstrae todo el algoritmo cultural
     */
    Simulation * simulation;

    /**
     * @brief Lista de soluciones no dominadas resultantes de la ejecucion del algoritmo cultural original
     */
    QList<Individual *> genericAlgorithmSolutions;

    /**
     * @brief Lista de soluciones no dominadas resultantes de la ejecucion del algoritmo cultural modificado
     *
     * En la modificacion se esta utilizando
     */
    QList<Individual *> modificatedAlgorithmSolutions;

    /**
     * @brief repeatedSolutionList
     */
    QList< QList<Individual *> > repeatedSolutionList;

    /**
     * @brief repeatedOriginalSolutionList
     */
    QList< QList<Individual *> > repeatedOriginalSolutionList;

    /**
     * @brief repeatedModificatedSolutionList
     */
    QList< QList<Individual *> > repeatedModificatedSolutionList;

    /**
     * @brief Tamano del individuo con respecto a la cadena de scanning (numero de canales a escanear)
     */
    static int individualSize;

    /**
     * @brief Verdadero si se desea emular el proceso de scanning de acuerdo a la campana de recoleccion
     * de datos del Profesor Laudin. En caso contrario se utiliza el modelo matematico descrito en el
     * articulo PIMRC 13
     */
    static bool emulateScanning;

    /**
     * @brief Objeto ScanningCampaing que simula la campana de scanning para obtener valores APs de acuerdo
     * a MinChannelTime y MaxChannelTime
     */
    static ScanningCampaing * scanningCampaing;

    /**
     * @brief Especifica si en el proceso de inicializacion de la poblacion se
     * utiliza una secuencia y temporizadores como condiciones iniciales para los
     * individuos
     */
    bool useSmartIndividual;

    /**
     * @brief Sequence de referencia para crear individuos inteligentes
     */
    QString smartIndividualSequence;


    /**
     * @brief Tamano de la ventana de la tabla C del espacio de creencias para la mutacion dirigida
     */
    int ctableWindow;

    /**
     * @brief Indice para ordenar la tabla C del espacio de creencias para la mutacion dirigida
     */
    static int indexToSortCTable;

    /**
     * @brief Limite inferior del MinChannelTime
     */
    static double lowerMinChannelTime;

    /**
     * @brief Limite superior del MinChannelTime
     */
    static double upperMinChannelTime;

    /**
     * @brief Limite inferior del MaxChannelTime
     */
    static double lowerMaxChannelTime;

    /**
     * @brief Limite superior del MaxChannelTime
     */
    static double upperMaxChannelTime;

    /**
     * @brief Especifica si se debe utilizar el indice de FO1 APmin/min + APmax/max
     */
    static bool useOF1Index;

    /**
     * @brief Especifica la latencia del individuo inteligente pasado para la creacion
     * de individuos de la poblacion.
     */
    static double smartIndividualLatency;

    /**
     * @brief Cadena que especifica el directorio de resultados de la ejecucion del algoritmo
     */
    QString resultsDirectory;

    /**
     * @brief Emulador de la topologia para consultar APs
     */
    Emulator * topologyEmulator;

public:

    /**
     * @brief Constructor de la clase
     */
    MainWindow();

    /**
     * @brief Destructor de la clase
     */
    ~MainWindow();


    /**
     * @brief Completa el widget ListView con los individuos no dominados del
     * archivo externo al final de la ejecución del algoritmo cultural
     */
    void populateListView();

    /**
     * @brief Configura el widget para generar el grafico de los individuos no dominados del
     * archivo externo.
     *  Se esta utilizando QCustomPlot
     *
     * @param customPlot
     */
    //void setupCustomPlot(QCustomPlot *customPlot);

    /**
     * @brief Valida los parametros antes de ejecutar el algoritmo
     *
     *  @return Verdadero si todos los campos son validos
     */
    bool validateFields();

    //void newSetupCustomPlot(QCustomPlot *customPlot);

    //void setupCustomPlot2(QCustomPlot *customPlot);

    void plotSolutions();


    void storeExecutionSolution();

    /**
     * @brief Retorna el numero de individuos no dominados de todas las repeticiones
     * del algoritmo
     * @return
     */
    int getCountOfNonDominatedInRepetitions();

    /**
     * @brief Retorna el numero de individuos no dominados de todas las repeticiones
     * del algoritmo original
     * @return
     */

    int getCountOfNonDominatedInOriginalRepetitions();

    /**
     * @brief Retorna el numero de individuos no dominados de todas las repeticiones
     * del algoritmo modificado
     * @return
     */
    int getCountOfNonDominatedInModificatedRepetitions();



    /**
     * @brief Retorna el promedio de la funcion objetivo de todos los individuos no dominados
     * luego de las repeticiones del algoritmo
     * @param fo identificador de la funcion objetivo: 1 para descubierta, 2 para latencia
     * @param list lista de individuos no dominados sobre la cual calcular el promedio
     *
     * @return promedio de la funcion objetivo
     */
    double getMeanOfObjectiveFunction(int fo, QList<QList<Individual *> > list , int type);

    /**
     * @brief retorna la desviacion estandar de la funcion objetivo pasada como argumento
     * @param mean media de la funcion objetivo seleccionada
     * @param fo funcion objetivo seleccionada: 1 para descubierta, 2 para latencia
     * @param list lista de individuos no dominados sobre la cual calcular el promedio
     * @return desviacion estandar de la funcion objetivo pasada como argumento
     */
    double getStandardDeviation(double mean, int fo, QList<QList<Individual *> >list, int type);


    /**
     * @brief Retorna el tiempo promedio de ejecucion de 30 repeticiones
     * @param l lista de los tiempos de ejecucion de las 30 repeticiones
     * @return tiempo promedio de ejecucion de 30 repeticiones
     */
    double getMeanExecutionTime(QList<double> l);

    /**
     * @brief Retorna la desviacion estandar del tiempo de ejecucion
     * @param l lista de los tiempos de ejecucion de las 30 repeticiones
     * @param mean tiempo promedio
     * @return
     */
    double getStdDeviationExecutionTime(QList<double> l, double mean);


    /**
     * @brief Retorna la lista de individuos no dominados de la lista pasada como argumento
     * @param list lista de individuos de los cuales obtener los no dominados
     * @return lista de individuos no dominados
     */
    QList<Individual*> getNonDominatedIndividualsFromList(QList<QList<Individual*> >list);

    /**
     * @brief Retorna la lista de individuos no dominados de las 30 ejecuciones del algoritmo
     * @param original verdadero si se debe utilizar la lista del algoritmo original; falso en caso
     * de usar la lista del algoritmo modificado.
     * @return lista de individuos no dominados (aproximacion del frente de Pareto)
     */
    QList<Individual*> getNonDominatedIndivualsFromRepetitions(bool original);


    /**
     * @brief Funcion para generar un archivo con los individuos de la lista pasada como
     * argumento
     * @param list lista de individuos para escribir en el archivo
     * @param fileName nombre del archivo que almacenara los individuos. La ruta del archivo
     * sera: /tmp/fileName.txt
     */
    void reportIndividualAsFile(QList<Individual*> list, QString fileName);

    /**
     * @brief Funcion para generar un archivo con los individuos de la lista pasada como
     * argumento
     * @param list lista de individuos para escribir en el archivo
     * @param resultsSubdirectory cadena con la ruta absoluta del subdirectorio para almacenar los
     * resultados
     * @param fileName nombre del archivo que almacenara los individuos. La ruta del archivo
     * sera: /tmp/fileName.txt
     */
    void reportIndividualAsFile(QList<Individual*> list, QString resultsSubdirectory, QString fileName);


    /**
     * @brief genera un archivo con los individuos de la lista pasado como argumento en la que cada
     * individuo esta ordenado de acuerdo a la latencia en genes de mayor a menor
     * @param list lista de individuos para escribir en el archivo
     * @param resultsSubdirectory cadena con la ruta absoluta del subdirectorio para almacenar los
     * resultados
     * @param fileName nombre del archivo que almacenara los individuos
     */
    void reportIndividualOrderedByLatencyInGenes(QList<Individual*> list, QString resultsSubdirectory, QString fileName);

    /**
     * @brief genera un archivo con los individuos de la lista pasada como argumento en la que cada
     * individuo esta ordenado de acuerdo al numero de APs en genes de menor a mayor
     * @param list lista de individuos para escribir en el archivo
     * @param resultsSubdirectory cadena con la ruta absoluta del subdirectorio para almacenar los
     * resultados
     * @param fileName nombre del archivo que almacenara los individuos
     * @param ascending bandera para el ordenamiento: true->ascendente; false->descendente
     */
    void reportIndividualOrderedByApInGenes(QList<Individual*> list, QString resultsSubdirectory, QString fileName, bool ascending=true);

    /**
     * @brief reordena la lista de genes pasada como argumento con respecto a latencia en genes que tienen igual numero de APs
     * @param list lista de genes a ordenar
     * @param ascending bandera para especificar si el orden es ascendente o descendente
     */
    QList<Gen*> reorderAP(QList<Gen*> originalList, bool ascending=true);

    /**
     * @brief Funcion para colocar las cadenas de individuos en los listview de la
     * pestana de cadenas de la interfaz grafica
     * @param list lista de individuos a colocar en el listview
     * @param listView listview donde se colocan los individuos
     */
    //void populateAListView(QList<Individual*> list, QListView * listView);

//public slots:

    /**
     * @brief Slot para ejecutar el algoritmo cultural al presionar un boton en
     * la interfaz grafica
     */
    void executeAlgorithm();

    /**
     * @brief Ejecuta el algoritmo cultural original repetido el numero de veces que se
     * especifique en el archivo settings.ini
     */
    void executeAlgorithmRepeated();

    /**
     * @brief Ejecuta el algoritmo cultural modificadop repetido el numero de veces que se
     * especifique en el archivo settings.ini
     */
    void executeModificatedAlgorithm();

    /**
     * @brief Slot para habilitar o deshabilitar el campo para introducir el valor de la
     *  probabilidad de mutacion dirigida
     * @param state Estado del check box
     */
    void activateDirectedMutation(int state);

    /**
     * @brief Slot para cambiar el valor del numero de encuentros por individuo en el torneo
     * @param str
     */
    void checkPopulationSize(const QString & str);

    /**
     * @brief Slot para habilitar el boton de comparacion de los algoritmos genericos y modificado
     * @param state
     */
    void activateComparationButton(int state);

    /**
     * @brief Slot para ejecutar el algoritmo PSO modificado al presionar un boton en la
     * interfaz grafica
     */
    void compareAlgorithms();



    void repeatAlgorithm();

    void view();

    void viewAll();

    /**
     * @brief slot para comparacion la ejecucion repetida 30 veces de cada uno de los algoritmos
     *
     * Se ejecuta primero el algorimo original y almacenan todos los individuos no dominados,
     * luego se ejecuta el algoritmo con la mutación dirigida y se
     *
     */
    void compareAlgorithmRepeated();


    /**
     * @brief Funcion para crear la estructura del directorio resultados en el directorio actual del
     * programa con subdirectorios para cada una de las ejecuciones que se realicen con el siguiente
     * formato de nombre: dd.MM.yyyy_hh.mm.ss
     *
     * @return cadena con la ruta absoluta al subdirectorio donde se deben almacenar los resultados
     */
    QString createResultsDirectory();

    /**
     * @brief Retorna verdadero si se debe realizar la comparacion de los algoritmos de acuerdo al
     * parametro leido del archivo de configuracion
     * @return verdadero si se debe realizar la comparacion de algoritmos
     */
    bool getDoComparationOfAlgorithms();

    /**
     * @brief Retorna verdadero si se debe realizar la ejecucion del algoritmo original
     * @return verdadero si se debe realizar la ejecucion del algoritmo original
     */
    bool getDoOriginalAlgorithm();

    /**
     * @brief Retorna el tamano del individuo con respecto a la secuencia de canales
     * @return tamano del individuo con respecto a la secuencia de canales
     */
    static int getIndividualSize();


    /**
     * @brief Retorna si se debe utilizar la emulacion de scanning de acuerdo a la campana de mediciones
     * @return si se debe utilizar la emulacion de scanning de acuerdo a la campana de mediciones
     */
    static bool getEmulateScanning();

    /**
     * @brief getRandomScan
     * @param channel
     * @param minChannelTime
     * @param maxChannelTime
     * @return
     */
    static ScanningCampaing::ScanResults getRandomScan(int channel, int minChannelTime, int maxChannelTime);

    /**
     * @brief Obtiene el numero de APs encontrados dados los parametros pasados como argumento
     * @param channel
     * @param minChannelTime
     * @param maxChannelTime
     * @return numero de APs encontrados dados los parametros pasados como argumento
     */
    static int getAPs(int channel, int minChannelTime, int maxChannelTime);

    /**
     * @brief Retorna si el paramatro del algoritmo de usar individuos inteligentes para
     * el proceso de inicializar la poblacion esta establecido
     * @return si se deben usar individuos inteligentes para el proceso de inicializar la poblacion
     */
    bool getUseSmartIndividual();

    /**
     * @brief Retorna el limite inferior del MinChannelTime
     * @return limite inferior del MinChannelTime
     */
    static double getLowerMinChannelTime();

    /**
     * @brief Retorna el limite superior del MinChannelTime
     * @return limite superio del MinChannelTime
     */
    static double getUpperMinChannelTime();

    /**
     * @brief Retorna el limite inferior del MaxChannelTime
     * @return limite inferior del MaxChannelTime
     */
    static double getLowerMaxChannelTime();

    /**
     * @brief Retorna el limite superior del MaxChannelTime
     * @return limite superio del MaxChannelTime
     */
    static double getUpperMaxChannelTime();

    /**
     * @brief Retorna el indice para ordenar la tabla C del espacio de creencias para la mutacion dirigida
     * @return indice para ordenar la tabla C del espacio de creencias para la mutacion dirigida
     */
    static int getIndexToSortCTable();

    /**
     * @brief Procesa los resultados de la ejecucion con el operador de mutacion original
     * El procesamiento incluye ejecutar 30 full scanning en el emulador por cada cadena del
     * algoritmo cultural. Con estos valores se construye un intervalo de confianza para
     * el numero de APs descubiertos que luego se comparan con las cadenas de los dispositivos
     * de referencia.
     */
    void generateAPResultsOfOriginalMutation();


    /**
     * @brief Procesa los resultados de la ejecucion con el operador de mutacion dirigida
     * El procesamiento incluye ejecutar 30 full scanning en el emulador por cada cadena del
     * algoritmo cultural. Con estos valores se construye un intervalo de confianza para
     * el numero de APs descubiertos que luego se comparan con las cadenas de los dispositivos
     * de referencia.
     */
    void generateAPResultsOfDirectedMutation();

    /**
     * @brief Procesa una cadena resultante del algoritmo cultural y obtiene el intervalo de
     * confianza del 95% para el numero promedio de APs
     *
     * Solo deja los valores de ch,min,max para todos los canales.
     *
     * @param line cadena a la que se desea calcular el intervalo de confianza
     * @return intervalo de confianza para el numero promedio de APs  en una lista <min, avg, max>
     */
    QList<double> processLine(QString line, bool individualBase = false);

    /**
     * @brief Calcula la varianza de la muestra
     * @param sampleList lista de muestras
     * @param mean media de las muestras
     * @return varianza de la muestra
     */
    double computeSampleVariance(QList<double> sampleList, double mean);

    /**
     * @brief Calculo el valor que se suma y resta al promedio para construir el intervalo de confianza
     * @param sampleList lista de muestras
     * @param mean media de las muestras
     * @return valor que se suma y resta al promedio para construir el intervalo de confianza
     */
    double computeConfidenceInterval(QList<double> sampleList, double mean);


    /**
     * @brief Genera un script para el programa gnuplot que luego generara el grafico de comparacion
     * @param sequences numero de cadenas resultantes de la simulacion
     * @param maxLatency latencia maxima para establecer el valor maximo del eje Y
     */
    void generateGnuplotFile(int sequences, int maxLatency);

    /**
     * @brief Genera un script generateGraphic.sh que al ejecutar construye el gráfico de comparacion
     * de cadenas resultantes y las cadenas de referencia a traves de gnuplot
     */
    void generateGraphicOfSequences();

    /**
     * @brief Escribe un archivo en el directorio resultado de cada ejecucion con los superindidivuos
     * generados en la CTable
     */
    void reportCTableHistory();

    /**
     * @brief Genera un archivo pdf con las tablas de individuos resultantes de la simulacion con
     *mutacion dirigida de acuerdo al tamano de los individuos
     */
    void generateMutatedResultsTable();

    /**
     * @brief Genera un archivo pdf con las tablas de individuos resultantes de la simulacion con
     * mutacion original de acuerdo al tamano de los individuos
     */
    void generateResultsTable();


    double askTopology(int channel, double min, double max);



    /**
     * @brief Retorna el número de aps del individuo
     * @return
     */
    QString askTopologyPerformanceSmartIndividual();

    /**
     * @brief Retorna el valor del parametro del algoritmo que especifica si se debe usar
     * indice de la FO1
     */
    static bool getUseOF1Index();

    /**
     * @brief Retorna la latencia del individuo inteligente utilizado para generar la población inicial
     * @return latencia del individuo inteligente utilizado para generar la población inicial
     */
    static double getSmartIndividualLatency();

};

#endif // MAINWINDOW_H
