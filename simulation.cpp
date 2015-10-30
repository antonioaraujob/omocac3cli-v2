#include "simulation.h"

#include <QFile>
//#include <QMessageBox>
#include <QTextStream>
#include <QStringList>

#include <random>
#include <chrono>

/**
 * @brief Funcion de comparacion de individuos con respecto al valor de desempeno de descubrimiento
 * @param p1 Individuo 1 a comparar
 * @param p2 Individuo 2 a comparar
 * @return Verdadero si p1 es menor que p2 con respecto a la funcion objetivo de descubrimiento
 */
inline static bool xLessThanF1(Individual *p1, Individual *p2)
{
    return p1->getPerformanceDiscovery() < p2->getPerformanceDiscovery();
}


/**
 * @brief Funcion de comparacion de individuos con respecto al valor de desempeno de latencia
 * @param p1 Individuo 1 a comparar
 * @param p2 Individuo 2 a comparar
 * @return Verdadero si p1 es menor que p2 con respecto a la funcion objetivo de latencia
 */
inline static bool xLessThanF2(Individual *p1, Individual *p2)
{
    return p1->getPerformanceLatency() < p2->getPerformanceLatency();
}

/**
 * @brief Funcion de comparacion de CTableGen con respecto al valor de channel
 * @param gen1 gen 1 a comparar
 * @param gen2 gen 2 a comparar
 * @return Verdadero si gen1 es menor que gen2 con respecto al canal
 */
inline static bool gen1LessThanGen2ByChannel(CTableGen * gen1, CTableGen * gen2)
{
    return gen1->getChannel() < gen2->getChannel();
}

/**
 * @brief Define e inicializa el miembro estatico individualIdCounter
 */
int Simulation::individualIdCounter = 0;


Simulation::Simulation(int population, int extFileSize, int generations, int subintervalsGrid, int genNormative,
                       int matches, int stdDev, int stdDevMin, int stdDevMax, bool dMutation, double dMutationProbability,
                       int indSize, int ctableWindow, int indToSortCTable, QString initSequence)
{
    populationSize = population;

    externalFileSize = extFileSize;

    generationsMax = generations;

    currentGeneration = 1;

    gridSubintervalsNumber = subintervalsGrid;

    gNormative = genNormative;

    matchesPerIndividuals = matches;

    stdDeviation = stdDev;

    stdDeviationMinChannelTime = stdDevMin;

    stdDeviationMaxChannelTime = stdDevMax;

    directedMutation = dMutation;

    directedMutationProbability = dMutationProbability;

    individualSize = indSize;

    normativePhenotipicPart = new NormativePhenotypicPart();

    externalFile = new ExternalFile(extFileSize);

    mutation = new Mutation();

    selection = new Selection();

    ctable = new CTable(individualSize, ctableWindow, indToSortCTable);

    indexToSortCTable = indToSortCTable;

    initialSequence = initSequence;

    qDebug("Simulation:");
    qDebug("    tamano de la poblacion: %d", populationSize);
    qDebug("    numero de generaciones: %d", generationsMax);
    qDebug("    Gnormative: %d", gNormative);
    qDebug("    numero de torneos por individuo: %d", matchesPerIndividuals);
    qDebug("    desviacion estandar: %d", stdDeviation);
    qDebug("    desviacion estandar min: %d", stdDeviationMinChannelTime);
    qDebug("    desviacion estandar max : %d", stdDeviationMaxChannelTime);
    qDebug("    tamano de ventana de CTable: %d", ctableWindow);
}


Simulation::~Simulation()
{
    delete normativePhenotipicPart;
    delete nGrid;
    delete externalFile;
    delete mutation;
}

int Simulation::getNewindividualId()
{
    int newId = individualIdCounter++;
    return newId;
}

void Simulation::initializePopulation()
{
    Individual * individuo;

    /*
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::initializePopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        qDebug("Simulation::initializePopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo");
        return;
    }
    QTextStream out(&file);
    out << endl << "Inicializacion de la poblacion." <<"\n";
    */


    // inicializacion de la poblacion
    for (int i = 0; i < populationSize; i++)
    {
        individuo = new Individual();

        individuo->printIndividual();
        individuo->getAverageOnFullScanning();

        individuo->printIndividual();
        individuo->getIndividualAsQString();
        qDebug("individualId: %d", individuo->getIndividualId());
        populationList.append(individuo);

        //out << individuo->getIndividualAsQString() << endl;
    }
    qDebug("tamano de la poblacion: %d",populationList.count());
    //return populationList;
}


void Simulation::initializeSmartPopulation()
{

/*
    // inicializacion de la poblacion  a partir de la cadena inteligente parametro del algoritmo y el mecanismo
    // aleatorio para asignacion de parametros
    createSmartPopulation();

    qDebug("tamano de la poblacion: %d",populationList.count());
    return;
*/


    // inicializacion de la poblacion a partir de la cadena inteligente parametro del algortimo y el mecanismo de
    // mutación de CTable
    createSmartPopulationWithCTable();

    qDebug("tamano de la poblacion: %d",populationList.count());
    return;




    Individual * individuo;
/*
    // inicializacion de la mita de la poblacion con condiciones inciales
    for (int i = 0; i < populationSize/2; i++)
    {
        individuo = new Individual(true);
        individuo->printIndividual();
        qDebug("individualId: %d", individuo->getIndividualId());
        populationList.append(individuo);
    }

    // agregar 3 individuos de 800/11
    for (int j = 0; j < 3; j++)
    {
        individuo = new Individual(true, "1,73,0,2,73,0,3,73,0,4,73,0,5,73,0,6,73,0,7,73,0,8,73,0,9,73,0,10,73,0,11,73,0");
        populationList.append(individuo);
    }

    // agregar 3 individuos de 400/11
    for (int k = 0; k < 3; k++)
    {
        individuo = new Individual(true, "1,36,0,2,36,0,3,36,0,4,36,0,5,36,0,6,36,0,7,36,0,8,36,0,9,36,0,10,36,0,11,36,0");
        populationList.append(individuo);
    }

    // agregar 3 individuos de 200/11
    for (int l = 0; l < 4; l++)
    {
        individuo = new Individual(true, "1,19,0,2,19,0,3,19,0,4,19,0,5,19,0,6,19,0,7,19,0,8,19,0,9,19,0,10,19,0,11,19,0");
        populationList.append(individuo);
    }
*/

/*
    // inicializacion de la poblacion
    for (int i = 0; i < populationSize-1; i++)
    {
        individuo = new Individual();

        individuo->printIndividual();
        individuo->getAverageOnFullScanning();

        individuo->printIndividual();
        individuo->getIndividualAsQString();
        qDebug("individualId: %d", individuo->getIndividualId());
        populationList.append(individuo);

        //out << individuo->getIndividualAsQString() << endl;
    }
*/

    if (individualSize == 1)
    {
        // agregar individuos de acuerdo a PIMRC tamano 1 latencia 8 ms
        /*
        for (int j = 0; j < 1; j++)
        {
            individuo = new Individual(true, "6,5,3");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }
        */


        // agregar individuos de acuerdo a PIMRC tamano 1 latencia 19 ms
        for (int j = 0; j < 1; j++)
        {
            individuo = new Individual(true, "6,9,10");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }



    }
    else if (individualSize == 2)
    {
        // agregar individuos de acuerdo a PIMRC tamano 2 latencia 25 ms
        for (int j = 0; j < 1/*populationSize*/; j++)
        {
            individuo = new Individual(true, "6,12,3,1,8,2");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }

    }
    else if (individualSize == 3)
    {
        // agregar individuos de acuerdo a PIMRC tamano 3 latencia 44 ms
        /*
        for (int j = 0; j < populationSize; j++)
        {
            individuo = new Individual(true, "6,15,5,1,7,3,11,7,7");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }
        */



        // agregar individuos de acuerdo a PIMRC tamano 3 latencia 68 ms
        for (int j = 0; j < 1/*populationSize*/; j++)
        {
            individuo = new Individual(true, "6,15,13,1,15,11,11,7,7");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }



    }
    else if (individualSize == 4)
    {
        // agregar individuos de acuerdo a PIMRC tamano 4 latencia 100 ms
        for (int j = 0; j < 1/*populationSize*/; j++)
        {
            individuo = new Individual(true, "6,15,13,1,15,13,11,15,13,9,8,8");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }


    }
    else if (individualSize == 8)
    {
        // agregar individuos de acuerdo a PIMRC tamano 8 latencia 160 ms
        for (int j = 0; j < 1/*populationSize*/; j++)
        {
            individuo = new Individual(true, "6,15,13,1,15,13,11,15,13,9,12,12,10,4,4,3,8,8,8,12,8,7,4,4");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }

    }
    else if (individualSize == 11)
    {
        /*
        // agregar individuos de acuerdo a PIMRC tamano 11 latencia
        for (int j = 0; j < 1; j++)
        {
            individuo = new Individual(true, "6,15,13,1,15,13,11,15,15,9,15,15,10,15,10,3,11,11,8,7,3,7,7,7,5,6,4,4,6,3,2,3,3");
            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }
        */

        // agregar individuos de acuerdo a la cadena
        // 11,15,135,1,15,135,6,15,135,9,17,1,5,17,1,2,17,1,10,17,1,8,17,1,4,17,1,7,17,1,3,17,1
        for (int j = 0; j < populationSize; j++)
        {
            // cadena contruida 150/150/150/18 con el orden de la proporcion de AP de la topologia
            //individuo = new Individual(true, "11,15,135,1,15,135,6,15,135,7,17,1,9,17,1,10,17,1,3,17,1,2,17,1,8,17,1,4,17,1,5,17,1");


            //individuo = new Individual(true, "11,15,135,1,15,135,6,15,135,9,17,1,5,17,1,2,17,1,10,17,1,8,17,1,4,17,1,7,17,1,3,17,1");

            // cadena Android
            individuo = new Individual(true, "1,53,1,2,53,1,3,53,1,4,53,1,5,53,1,6,53,1,7,53,1,8,53,1,9,53,1,10,53,1,11,53,1");

            individuo->getAverageOnFullScanning();
            populationList.append(individuo);
        }


    }
    else
    {
        QString error = "No se puede inicializar individuo con condiciones iniciales PIMRC de tamaño ";
        error.append(QString::number(individualSize));
        Q_ASSERT_X(false, "Simulation::initializeSmartPopulation()", qPrintable(error));
    }

    qDebug("tamano de la poblacion: %d",populationList.count());
}


QList<Individual *>  Simulation::getPopulationList()
{
    return populationList;
}

void Simulation::initializeNormativePhenotypicPart()
{

    // obtener la lista de los individuos no dominados de la poblacion inicial
    QList<Individual *> initialNonDominatedPopulation;

    initialNonDominatedPopulation = getNonDominatedPopulationApproach1();

    // ordenar los no dominados con respecto a la funcion objetivo 1 de menor a mayor
    qSort(initialNonDominatedPopulation.begin(), initialNonDominatedPopulation.end(), xLessThanF1);

    // tomar los limites inferior y superior
    double lF1 = initialNonDominatedPopulation.at(0)->getPerformanceDiscovery();
    double uF1 = initialNonDominatedPopulation.at(initialNonDominatedPopulation.count()-1)->getPerformanceDiscovery();

    // ordenar los no dominados con respecto a la funcion objetivo 2 de menor a mayor
    qSort(initialNonDominatedPopulation.begin(), initialNonDominatedPopulation.end(), xLessThanF2);

    double lF2 = initialNonDominatedPopulation.at(0)->getPerformanceLatency();
    double uF2 = initialNonDominatedPopulation.at(initialNonDominatedPopulation.count()-1)->getPerformanceLatency();

    // asigna los extremos de las funciones objetivo con respecto a los individuos no dominados
    normativePhenotipicPart->updateNormativePhenotypicPart(lF1, uF1, lF2, uF2);

    qDebug("| lF1: %f | uF1: %f | lF2: %f | uF2: %f |",
           normativePhenotipicPart->getLowerF1(),normativePhenotipicPart->getUpperF1(),
           normativePhenotipicPart->getLowerF2(), normativePhenotipicPart->getUpperF2());


    /*
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::initializeNormativePhenotypicPart(): No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        qDebug("Simulation::initializeNormativePhenotypicPart(): No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        return;
    }

    QTextStream out(&file);
    out << endl <<"Inicializacion de la parte normativa fenotipica." <<"\n";

    out << "| lF1: " << normativePhenotipicPart->getLowerF1() <<
           "| uF1: " << normativePhenotipicPart->getUpperF1() <<
           "| lF2: " << normativePhenotipicPart->getLowerF2() <<
           "| uF2: " << normativePhenotipicPart->getUpperF2() << "|" <<endl;
    */


}

void Simulation::initializeGrid()
{
    nGrid = new NormativeGrid(gridSubintervalsNumber, normativePhenotipicPart);

    /*
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::initializeGrid(): No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        qDebug("Simulation::initializeGrid(): No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        return;
    }

    QTextStream out(&file);
    out << endl <<"Inicializacion de la rejilla." <<"\n";
    */


}

/*
void Simulation::initializeCTable()
{
    // aqui se debe obtener el super individuo inicial

    QList<CTableGen *> superIndividualGenList;

    Individual * individual;
    CTableGen * gen;

    QList<double> indexFOValueList;

    for (int i=0; i<populationList.size(); i++)
    {
        individual = populationList.at(i);
        individual->getAverageOnFullScanning();
        //indexFOValueList.append(individual->getPerformanceDiscovery());
    }
    qSort(populationList.begin(), populationList.end(), xLessThanF1);
    int population = populationList.size();
    Individual * bestIndividual = populationList.at(population-1);

    // indice para ordenar
    // indexToSortCTable

    int ch = 0;
    double max = 0;
    double min = 0;
    double aps = 0;

    for (int j=0; j<individual->getIndividualSize(); j++)
    {
        ch = bestIndividual->getParameter(j*4);
        min = bestIndividual->getParameter((j*4)+1);
        max = bestIndividual->getParameter((j*4)+2);
        aps = bestIndividual->getParameter((j*4)+3);
        gen = new CTableGen(ch, min, max, aps, indexToSortCTable, 0, 0);
        superIndividualGenList.append(gen);
    }
    Q_ASSERT(superIndividualGenList.size() == individual->getIndividualSize());

    // el ordenamiento de CTable se hace internamente en la funcion
    // void addSuperIndividual(QList<CTableGen *> superIndividualGenList, int indexToSort);
    addSuperIndividual(superIndividualGenList, indexToSortCTable);

    return;
}
*/

void Simulation::initializeCTable()
{
    // aqui se debe obtener el super individuo inicial
    QList<CTableGen *> superIndividualGenList;

    QList<CTableGen *> individualConverted;
    QList< QList<CTableGen *> > populationConvertedList;

    Individual * individual;

    for (int i=0; i<populationList.size(); i++)
    {
        // obtener el individuo
        individual = populationList.at(i);
        // calcular el promedio de APi
        individual->getAverageOnFullScanning();

        // convertir el individuo a lista de genes
        individualConverted = ctable->convertIndividualToCTableGen(individual);
        // ordenar la lista de genes con respecto al canal
        qSort(individualConverted.begin(), individualConverted.end(), gen1LessThanGen2ByChannel);
        // calcular los valores de los indices indexA, indexB, indexC para cada canal
        for (int j=0; j<individualConverted.size(); j++)
        {
            individualConverted.at(j)->calculateIndexA();
            individualConverted.at(j)->calculateIndexB();
            individualConverted.at(j)->calculateIndexC();
        }
        populationConvertedList.append(individualConverted);
    }

    CTableGen * maxGen;
    CTableGen * auxGen;
    CTableGen * finalGen;

    for (int k=0; k<individual->getIndividualSize(); k++)
    {
        maxGen = populationConvertedList.at(0).at(k);
        auxGen = populationConvertedList.at(0).at(k);

        for (int l=0; l<populationConvertedList.size(); l++)
        {
            if (indexToSortCTable == 0)
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexA() > maxGen->getIndexA())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }else if (indexToSortCTable == 1)
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexB() > maxGen->getIndexB())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }else // 2 indexC
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexC() > maxGen->getIndexC())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }
        }
        // consegui el maximo del canal
        finalGen = new CTableGen(*maxGen);
        superIndividualGenList.append(finalGen);
    }
    // el ordenamiento de CTable se hace internamente en la funcion
    // void addSuperIndividual(QList<CTableGen *> superIndividualGenList, int indexToSort);
    ctable->addSuperIndividual(superIndividualGenList, indexToSortCTable);
}

void Simulation::updateNormativePhenotypicPart()
{
    qDebug("Simulation::updateNormativePhenotypicPart");

    QList<Individual *> extFileNonDominatedPopulation;

    // obtener la lista de individuos no dominados del archivo externo
    extFileNonDominatedPopulation = externalFile->getExternalFileList();

    // crear lista temporal de individuos no dominados del archivo externo unidos con
    // los individuos que cayeron fuera de la rejilla en la generacion actual
    QList<Individual *> extFileAndOutOfGridIndividualList;
    extFileAndOutOfGridIndividualList = externalFile->getExternalFileList();

    for (int i=0; i<getOutOfGridIndividualList().count();i++)
    {
        extFileAndOutOfGridIndividualList.append(getOutOfGridIndividualList().at(i));
    }


    // ordenarlos los no dominados con respecto a la funcion objetivo 1 de menor a mayor
    qSort(extFileAndOutOfGridIndividualList.begin(), extFileAndOutOfGridIndividualList.end(), xLessThanF1);

    // tomar los limites inferior y superior
    double lF1 = extFileAndOutOfGridIndividualList.at(0)->getPerformanceDiscovery();
    double uF1 = extFileAndOutOfGridIndividualList.at(extFileAndOutOfGridIndividualList.count()-1)->getPerformanceDiscovery();

    // ordenarlos los no dominados con respecto a la funcion objetivo 2 de menor a mayor
    qSort(extFileAndOutOfGridIndividualList.begin(), extFileAndOutOfGridIndividualList.end(), xLessThanF2);

    double lF2 = extFileAndOutOfGridIndividualList.at(0)->getPerformanceLatency();
    double uF2 = extFileAndOutOfGridIndividualList.at(extFileAndOutOfGridIndividualList.count()-1)->getPerformanceLatency();

    // asigna los extremos de las funciones objetivo con respecto a los individuos no dominados
    normativePhenotipicPart->updateNormativePhenotypicPart(lF1, uF1, lF2, uF2);

    qDebug("nueva parte fenotipica normativa:");
    qDebug("| lF1: %f | uF1: %f | lF2: %f | uF2: %f |",
           normativePhenotipicPart->getLowerF1(),normativePhenotipicPart->getUpperF1(),
           normativePhenotipicPart->getLowerF2(), normativePhenotipicPart->getUpperF2());

    // Reconstruir la rejilla con los nuevos valores de lowerF1, upperF1, lowerF2, upperF2.
    delete nGrid;

    // Reinicializar todos los contadores de la rejilla en cero.
    nGrid = new NormativeGrid(gridSubintervalsNumber, normativePhenotipicPart);

    nGrid->printGrid();

    // Agregar todos los individuos del archivo externo al contador de su celda correspondiente.
    // De esta manera el espacio de creencias está listo de nuevo para su uso.
    updateGrid(extFileNonDominatedPopulation);

    qDebug("++++++grid despues de actualizada con los individuos del archivo externo");
    nGrid->printGrid();
    qDebug("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    extFileAndOutOfGridIndividualList.clear();
    outOfGridIndividualList.clear();



/*
    qDebug("Simulation::updateNormativePhenotypicPart");

    QList<Individual *> extFileNonDominatedPopulation;

    // obtener la lista de individuos no dominados del archivo externo
    extFileNonDominatedPopulation = externalFile->getExternalFileList();

    // ordenarlos los no dominados con respecto a la funcion objetivo 1 de menor a mayor
    qSort(extFileNonDominatedPopulation.begin(), extFileNonDominatedPopulation.end(), xLessThanF1);

    // tomar los limites inferior y superior
    int lF1 = extFileNonDominatedPopulation.at(0)->getPerformanceDiscovery();
    int uF1 = extFileNonDominatedPopulation.at(extFileNonDominatedPopulation.count()-1)->getPerformanceDiscovery();

    // ordenarlos los no dominados con respecto a la funcion objetivo 2 de menor a mayor
    qSort(extFileNonDominatedPopulation.begin(), extFileNonDominatedPopulation.end(), xLessThanF2);

    int lF2 = extFileNonDominatedPopulation.at(0)->getPerformanceLatency();
    int uF2 = extFileNonDominatedPopulation.at(extFileNonDominatedPopulation.count()-1)->getPerformanceLatency();

    // asigna los extremos de las funciones objetivo con respecto a los individuos no dominados
    normativePhenotipicPart->updateNormativePhenotypicPart(lF1, uF1, lF2, uF2);

    qDebug("nueva parte fenotipica normativa:");
    qDebug("| lF1: %f | uF1: %f | lF2: %f | uF2: %f |",
           normativePhenotipicPart->getLowerF1(),normativePhenotipicPart->getUpperF1(),
           normativePhenotipicPart->getLowerF2(), normativePhenotipicPart->getUpperF2());

    // Reconstruir la rejilla con los nuevos valores de lowerF1, upperF1, lowerF2, upperF2.
    delete nGrid;

    // Reinicializar todos los contadores de la rejilla en cero.
    nGrid = new NormativeGrid(gridSubintervalsNumber, normativePhenotipicPart);

    nGrid->printGrid();

    // Agregar todos los individuos del archivo externo al contador de su celda correspondiente.
    // De esta manera el espacio de creencias está listo de nuevo para su uso.
    updateGrid(extFileNonDominatedPopulation);

    qDebug("++++++grid despues de actualizada con los individuos del archivo externo");
    nGrid->printGrid();
    qDebug("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
*/
}


void Simulation::updateGrid(QList<Individual *> nonDominated)
{

    // Para actualizar la rejilla simplemente se incrementan los contadores de los
    // individuos no dominados con todos los individuos recién agregados al
    // archivo externo durante la generación actual.

/*
    Individual * ind = nonDominated.at(0);
    nGrid->addIndividualToGrid(ind);

    ind = nonDominated.at(1);
    nGrid->addIndividualToGrid(ind);
*/
    qDebug("Simulation::updateGrid");
    Individual * auxIndividual;

    qDebug("cantidad de individuos no dominados recien agregados al archivo externo");
    qDebug(qPrintable(QString::number(nonDominated.count())));

    // verificar si existen individuos recientemente agregados
    if (nonDominated.count() == 0)
    {
        qDebug("    nonDominated.count() == 0");
        return;
    }

    for (int i=0; i<nonDominated.count(); i++)
    {
        auxIndividual = nonDominated.at(i);

        if(nGrid->individualInsideGrid(auxIndividual))
        {
            nGrid->addIndividualToGrid(auxIndividual);
        }

        /*
        // comentado porque si el individuo no pertenece a la grid no se hace nada
        if(!nGrid->individualInsideGrid(auxIndividual))
        {
            // TODO: revisar esto:
            qDebug("%%%%%%%% el individuo no pertenece a la grid");
            //auxIndividual->printIndividual();
            //outOfGridIndividualList.append(auxIndividual);
            Q_ASSERT_X(false, "Simulation::updateGrid()", "el individuo que se desea agregar no pertenece a la grid");
        }
        else
        {
            nGrid->addIndividualToGrid(auxIndividual);
        }
        */
    }
}


void Simulation::updateCTable(QList<Individual *> newNonDominatedIndividualsFromEF)
{
    // aqui se debe actualizar el superindividuo de CTable tomando los nuevos individuos agregados
    // al archivo externo; similar a la actualizacion de la grid


    //verificar si existen individuos recientemente agregados
    if (newNonDominatedIndividualsFromEF.count() == 0)
    {
        qDebug("Simulation::updateCTable: no hay individuos recientemente agregados");
        return;
    }


    // aqui se debe obtener el super individuo inicial
    QList<CTableGen *> superIndividualGenList;

    QList<CTableGen *> individualConverted;
    QList< QList<CTableGen *> > populationConvertedList;

    Individual * individual;

    for (int i=0; i<newNonDominatedIndividualsFromEF.size(); i++)
    {
        // obtener el individuo
        individual = newNonDominatedIndividualsFromEF.at(i);
        // calcular el promedio de APi
        individual->getAverageOnFullScanning();

        // convertir el individuo a lista de genes
        individualConverted = ctable->convertIndividualToCTableGen(individual);
        // ordenar la lista de genes con respecto al canal
        qSort(individualConverted.begin(), individualConverted.end(), gen1LessThanGen2ByChannel);
        // calcular los valores de los indices indexA, indexB, indexC para cada canal
        for (int j=0; j<individualConverted.size(); j++)
        {
            individualConverted.at(j)->calculateIndexA();
            individualConverted.at(j)->calculateIndexB();
            individualConverted.at(j)->calculateIndexC();
        }

        populationConvertedList.append(individualConverted);

    }

    CTableGen * maxGen;
    CTableGen * auxGen;
    CTableGen * finalGen;

    for (int k=0; k<individual->getIndividualSize(); k++)
    {
        maxGen = populationConvertedList.at(0).at(k);
        auxGen = populationConvertedList.at(0).at(k);

        for (int l=0; l<populationConvertedList.size(); l++)
        {
            if (indexToSortCTable == 0)
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexA() > maxGen->getIndexA())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }else if (indexToSortCTable == 1)
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexB() > maxGen->getIndexB())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }else // 2 indexC
            {
                auxGen = populationConvertedList.at(l).at(k);
                if (auxGen->getIndexC() > maxGen->getIndexC())
                {
                    maxGen = populationConvertedList.at(l).at(k);
                }
            }
        }
        // consegui el maximo del canal
        finalGen = new CTableGen(*maxGen);
        superIndividualGenList.append(finalGen);
    }

    // ejecutar el proceso de limpiar (registrar antiguo super individuo) en CTable
    if (newNonDominatedIndividualsFromEF.size()>0)
    {
        ctable->clearCTable();
        // el ordenamiento de CTable se hace internamente en la funcion
        ctable->addSuperIndividual(superIndividualGenList, indexToSortCTable);
    }
}


void Simulation::printGrid()
{
    nGrid->printGrid();
}

void Simulation::mutatePopulation()
{
    // utilizar la mutación dirigida
    if (directedMutation)
    {
        //mutation->doDirectedMutation(populationList, getStdDeviation(), stdDeviationMinChannelTime, stdDeviationMaxChannelTime,
        //                             directedMutationProbability, nGrid);
        mutation->doDirectedMutation(populationList, getStdDeviation(), stdDeviationMinChannelTime, stdDeviationMaxChannelTime,
                                     directedMutationProbability, ctable);
    }
    else
    {
        mutation->doMutation(populationList, getStdDeviation(), stdDeviationMinChannelTime, stdDeviationMaxChannelTime);
    }

    mutatedPopulationList = mutation->getNewPopulation();

    mutation->printNewPopulation();

    /*
    // agregar resultados a archivo
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::mutatePopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        qDebug("Simulation::mutatePopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        //msg.exec();
        return;
    }
    QTextStream out(&file);
    out << endl << "Mutacion de la poblacion." <<"\n";
    out << endl;


    Individual * auxIndividual;
    for (int i=0; i<mutatedPopulationList.count(); i++)
    {
        auxIndividual = mutatedPopulationList.at(i);
        out << auxIndividual->getIndividualAsQString() << endl;
    }
    */
}

void Simulation::selectPopulation()
{
    selection->doSelection(mutatedPopulationList, matchesPerIndividuals, nGrid);

    populationList = selection->getSelectedPopulation();

    //
    outOfGridIndividualList = selection->getOutOfGridList();

    for (int i=0; i<selection->getOutOfGridList().count(); i++)
    {
        outOfGridIndividualList.append(selection->getOutOfGridList().at(i));
    }


    /*
    // agregar resultados a archivo
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::selectPopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        qDebug("Simulation::selectPopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        return;
    }
    QTextStream out(&file);
    out << endl << "Seleccion de poblacion de tamano P." <<"\n";
    out << endl;

    Individual * auxIndividual;
    for (int i=0; i<populationList.count(); i++)
    {
        auxIndividual = populationList.at(i);
        out << auxIndividual->getIndividualAsQString() << endl;
    }
    */

}


void Simulation::addNonDominatedIndividualsToExternalFile(QList<Individual *> ndIndividualList)
{
    externalFile->addNonDominatedIndividuals(ndIndividualList, nGrid);

    /*
    // agregar resultados a archivo
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("Simulation::addNonDominatedIndividualsToExternalFile(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        qDebug("Simulation::selectPopulation(): No se pudo abrir el archivo /tmp/algorithmResult.txt\n para escribir resultados de la ejecucion del algoritmo.");
        return;
    }
    QTextStream out(&file);
    out << endl << "Individuos del archivo externo." <<"\n";
    out << endl;

    Individual * auxIndividual;
    for (int i=0; i<externalFile->getExternalFileList().count(); i++)
    {
        auxIndividual = externalFile->getExternalFileList().at(i);
        out << auxIndividual->getIndividualAsQString() << endl;
    }
    */

}

void Simulation::incrementGeneration()
{
    currentGeneration++;
}

int Simulation::getCurrentGenerationNumber()
{
    return currentGeneration;
}

bool Simulation::stopEvolution()
{
    if (currentGeneration > generationsMax)
        return true;
    else
        return false;
}



QList<Individual *> Simulation::getNonDominatedPopulationApproach1()
{
    qDebug("...Simulation::getNonDominatedPopulationApproach1");

    // populationList es la lista de individuos
    QList<Individual *> nonDominatedPopulation;
    int p = populationList.count();

    Individual * individualI;
    Individual * individualJ;

    for (int i=0; i<p; i++)
    {
        bool dominated = false;
        individualI = populationList.at(i);

        for (int j=0; ((j<p) && (!dominated)); j++)
        {

            if (i==j)
            {
                continue;
            }
            individualJ = populationList.at(j);
            if (individualDominate(individualJ, individualI))
            {
                dominated = true;
            }
        }
        if (!dominated)
        {
            nonDominatedPopulation.append(individualI);
        }
    }
    return nonDominatedPopulation;


}

QList<Individual *> Simulation::getNonDominatedPopulationApproach2()
{
    qDebug("...Simulation::getNonDominatedPopulationApproach2");

    int p = populationList.count();

    Individual * individualI;
    Individual * individualJ;

    // populationList es la lista de individuos
    QList<Individual *> nonDominatedPopulation;
    nonDominatedPopulation.append(populationList.at(0));

    int nonDP = nonDominatedPopulation.count();

    int i = 1;
    int j;

    while(i < p)
    {
        individualI = populationList.at(i);
        j = 0;

        while (j < nonDP)
        {
            individualJ = populationList.at(j);
            if ( individualDominate(individualI, individualJ) )
            {
                nonDominatedPopulation.removeAt(j);
            }
            else if (individualDominate(individualJ, individualI)) {
                continue;
            }
            j++;
        }
        if (j == nonDP)
        {
            nonDominatedPopulation.append(individualI);
        }
        i++;
    }

    return nonDominatedPopulation;
}


bool Simulation::individualDominate(Individual * xj, Individual * xi)
{
    //qDebug("... ... Simulation::individualDominate");

    // a solution xj is said to dominate another solution xi, and we write xj <| xi if both
    // the following conditions are true:
    //
    // condition a: Fi(xj) <= Fi(xi) to i E 1,2
    //
    // confition b: Exists j 1, 2 such that Fj(xj) < Fj(xi)

    bool conditionA = false;
    bool conditionB = false;


    // condition a
    if ( (xj->getPerformanceDiscovery() >= xi->getPerformanceDiscovery()) &&
         (xj->getPerformanceLatency() <= xi->getPerformanceLatency()) )
    {
        conditionA = true;
    }

    // condition b
    if ( (xj->getPerformanceDiscovery() > xi->getPerformanceDiscovery()) ||
         (xj->getPerformanceLatency() < xi->getPerformanceLatency()) )
    {
        conditionB = true;
    }

    if ( (conditionA) && (conditionB) )
    {
        return true;
    }else
    {
        return false;
    }
}


void Simulation::setExternalFile(ExternalFile * extFile)
{
    externalFile = extFile;
}

ExternalFile * Simulation::getExternalFile()
{
    return externalFile;
}


double Simulation::getStdDeviation()
{
    return stdDeviation;
}


void Simulation::printPopulation()
{
    for (int i = 0; i < populationList.count(); i++)
    {
        populationList.at(i)->printIndividual();
    }
}


int Simulation::getgNormative()
{
    return gNormative;
}

QList<Individual *> Simulation::getOutOfGridIndividualList()
{
    return outOfGridIndividualList;
}


void Simulation::addIndividualToOutOfGridIndividualList(Individual * outOfGridIndividual)
{
    // verificar que el individuo no exista; si no existe se agrega en caso contrario se ignora

    Individual * auxIndividual;

    for (int i=0; i<outOfGridIndividualList.count(); i++)
    {
        auxIndividual = outOfGridIndividualList.at(i);

        if (auxIndividual->getIndividualId() == outOfGridIndividual->getIndividualId())
        {

        }
        else
        {
            outOfGridIndividualList.append(outOfGridIndividual);
        }
    }
}

void Simulation::evaluateIndividuals()
{
    qDebug("MainWindow::evaluateIndividuals()");

    Individual * individual;
    for (int i=0; i<populationList.count();i++)
    {
        individual = populationList.at(i);
        individual->calculateDiscoveryValue();
        individual->calculateLatencyValue();
    }
}

void Simulation::printList(QList<Individual*> list)
{
    for (int i = 0; i < list.count(); i++)
    {
        list.at(i)->printIndividual();
    }
}


void Simulation::reportCTableHistory(QString resultsDirectory)
{
    ctable->reportCTableHistory(resultsDirectory);
}


void Simulation::createSmartPopulation()
{

    QStringList parameters;
    parameters = initialSequence.split(",");

    Individual * individual;


    std::default_random_engine generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    std::normal_distribution<double>  minChannelTimeDistribution(0, stdDeviationMinChannelTime);

    std::normal_distribution<double>  maxChannelTimeDistribution(0, stdDeviationMaxChannelTime);


    double min = 0;
    double max = 0;
    double random = 0;

    QString aux;

    for (int j = 0; j < populationSize; j++)
    {
        individual = new Individual(true, initialSequence);

        // iterar por cada canal del individuo
        for (int i = 0; i < individual->getIndividualSize(); i++)
        {
            // canal
            aux = parameters.at((i*3));
            individual->setParameter((i*4), aux.toDouble());

            // min
            aux = parameters.at((i*3+1));
            random = qRound(minChannelTimeDistribution(generator));
            min = aux.toDouble() + random;

            if (min < 5)
            {
                while(min < 5)
                {
                    min = aux.toDouble() + qRound(minChannelTimeDistribution(generator));
                }
            }

            // max
            aux = parameters.at((i*3+2));
            random = qRound(maxChannelTimeDistribution(generator));
            max = aux.toDouble() + random;

            if (max < 1)
            {
                while(max < 1)
                {
                    max = aux.toDouble() + qRound(maxChannelTimeDistribution(generator));
                }
            }

            // chequear que la latencia no sea mayor a la asignada para el canal

            // aps se deja como construyo el individuo arriba y abajo se hara el scanning 30 veces
            //aux = parameters.at((i*4+3));
            //individual->setParameter((i*4+3), aux.toDouble());

            // asignar min
            individual->setParameter((i*4+1), min);

            // asignar max
            individual->setParameter((i*4+2), max);

        }

        individual->getAverageOnFullScanning();
        populationList.append(individual);
    }
}



void Simulation::createSmartPopulationWithCTable()
{

    Individual * individuo;
    // agregar 10 individuos identicos a la cadena de referencia de iOS
    for (int i=0; i<10; i++)
    {
        //individuo = new Individual(true, "1,39,0,2,39,0,3,39,0,4,39,0,5,39,0,6,39,0,7,39,0,8,39,0,9,39,0,10,39,0,11,39,0");
        individuo = new Individual(true, initialSequence);
        individuo->getAverageOnFullScanning();
        populationList.append(individuo);
    }


    // agregar 10 individuos creados a partir de la cadena de referencia con el proceso de CTable
    Individual * superIndividual = new Individual(true, initialSequence);
    superIndividual->getAverageOnFullScanning();

    ctable->addSuperIndividual(superIndividual);

    Individual * father = new Individual();
    father->getAverageOnFullScanning();

    Individual * offspring;

    for (int i=0; i<10; i++)
    {

        offspring = ctable->getNewIndividualFromCTable(father);
        offspring->getAverageOnFullScanning();

        populationList.append(offspring);
    }
    ctable->clearCTable(false);

}
