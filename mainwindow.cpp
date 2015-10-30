#include "mainwindow.h"

#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QDir>

#include "individual.h"
#include "simulation.h"




/**
 * @brief Funcion de comparacion de individuos con respecto al valor de desempeno de latencia
 * @param p1 Individuo 1 a comparar
 * @param p2 Individuo 2 a comparar
 * @return Verdadero si p1 es menor que p2 con respecto a la funcion objetivo de latencia
 */
inline static bool xLessThanLatency(Individual *p1, Individual *p2)
{
    return p1->getPerformanceLatency() < p2->getPerformanceLatency();
}

/**
 * @brief funcion de comparacion de genes con respecto al valor de la latencia
 * @param g1 gen1 a comparar
 * @param g2 gen2 a comparar
 * @return
 */
inline static bool genLessThanLatency(Gen *g1, Gen *g2)
{
    return g1->getLatency() < g2->getLatency();
}

/**
 * @brief funcion de comparacion de genes con respecto al valor de APs descubiertos
 * @param g1 gen1 a comparar
 * @param g2 gen2 a comparar
 * @return
 */
inline static bool genLessThanAPs(Gen *g1, Gen *g2)
{
    return g1->getValue(3) < g2->getValue(3);
}


/**
 * @brief Define e inicializa el miembro estatico individualSize
 */
int MainWindow::individualSize = 0;

/**
 * @brief Define e inicializa el miembro estatico emulateScanning
 */

bool MainWindow::emulateScanning = true;

/**
 * @brief MainWindow::scanningCampaing
 */
ScanningCampaing * MainWindow::scanningCampaing = 0;

/**
 * @brief Define e inicializa el miembro estatico lowerMinChannelTime
 */
double MainWindow::lowerMinChannelTime = 0;

/**
 * @brief Define e inicializa el miembro estatico upperMinChannelTime
 */
double MainWindow::upperMinChannelTime = 0;

/**
 * @brief Define e inicializa el miembro estatico lowerMaxChannelTime
 */
double MainWindow::lowerMaxChannelTime = 0;

/**
 * @brief Define e inicializa el miembro estatico upperMaxChannelTime
 */
double MainWindow::upperMaxChannelTime = 0;

/**
 * @brief Define e inicializa el miembro estatico indexToSortCTable
 */
int MainWindow::indexToSortCTable = 0;

/**
 * @brief Define e inicializa el miembro estatico useOF1Index
 */
bool MainWindow::useOF1Index = false;

/**
 * @brief Define e inicializa el miembro estatico smartIndividualLatency
 */
double MainWindow::smartIndividualLatency = 0;

MainWindow::MainWindow()
{
    // lectura de los parametros del algoritmo cultural a partir del archivo ini
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("CONF");

    population = settings.value("poblacion").toInt();
    qDebug(qPrintable(QString::number(population)));

    generationsNumber = settings.value("numeroDeGeneraciones").toInt();
    qDebug(qPrintable(QString::number(generationsNumber)));

    standarDeviation = settings.value("desviacionEstandar").toInt();
    qDebug(qPrintable(QString::number(standarDeviation)));

    standarDeviationMinChannelTime = settings.value("desviacionEstandarMinChannelTime").toInt();
    qDebug(qPrintable(QString::number(standarDeviationMinChannelTime)));

    standarDeviationMaxChannelTime = settings.value("desviacionEstandarMaxChannelTime").toInt();
    qDebug(qPrintable(QString::number(standarDeviationMaxChannelTime)));

    externalFileSize = settings.value("tamanoArchivoExterno").toInt();
    qDebug(qPrintable(QString::number(externalFileSize)));

    gridSubintervalnumber = settings.value("numeroDeSubintervalosDeRejilla").toInt();
    qDebug(qPrintable(QString::number(gridSubintervalnumber)));

    updateFrequency = settings.value("frecuenciaDeActualizacion").toInt();
    qDebug(qPrintable(QString::number(updateFrequency)));

    matchesPerIndividual = settings.value("numeroDeEncuentrosPorIndividuo").toInt();
    qDebug(qPrintable(QString::number(matchesPerIndividual)));

    mutationProbability = settings.value("probabilidadDeMutacionDirigida").toDouble();
    qDebug(qPrintable(QString::number(mutationProbability)));

    doDirectedMutation = settings.value("ejecutarMutacionDirigida").toBool();
    if (doDirectedMutation)
        qDebug("doDirectMutation");
    else
        qDebug("NO doDirectMutation");

    doOriginalAlgorithm = settings.value("ejecutarAlgoritmoOriginal").toBool();

    simulationRepetitions = settings.value("numeroDeRepeticionesDeSimulacion").toInt();

    doComparationOfAlgorithms = settings.value("ejecutarComparacionDeAlgoritmos").toBool();

    individualSize = settings.value("tamanoDeIndividuo").toInt();
    qDebug("individualSize %s", qPrintable(QString::number(individualSize)));

    emulateScanning = settings.value("emularScanning").toBool();

    useSmartIndividual = settings.value("usarIndividuosInteligentes").toBool();

    smartIndividualSequence = settings.value("cadenaIndividuoInteligente").toString();

    smartIndividualLatency = settings.value("latenciaIndividuoInteligente").toDouble();

    ctableWindow = settings.value("tamanoDeVentanaCTable").toInt();

    indexToSortCTable = settings.value("indiceParaOrdenarTablaC").toInt();

    lowerMinChannelTime = settings.value("limiteInferiorMinChannelTime").toDouble();

    upperMinChannelTime = settings.value("limiteSuperiorMinChannelTime").toDouble();

    lowerMaxChannelTime = settings.value("limiteInferiorMaxChannelTime").toDouble();

    upperMaxChannelTime = settings.value("limiteSuperiorMaxChannelTime").toDouble();

    useOF1Index = settings.value("usarIndiceFO1").toBool();

    resultsDirectory = "";

    // base de datos sqlite
    QString database("test_18.1.db");
    //QString database("database.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");
    scanningCampaing = new ScanningCampaing(database.toStdString(),experiment.toStdString(), 0);
    scanningCampaing->init();
    scanningCampaing->prepareIRD();

    Q_ASSERT_X(ctableWindow <= individualSize, "MainWindow::MainWindow()",
               "No se puede iniciar la simulacion con tamano de ventana de CTable mayor que el tamano del individuo.");

    topologyEmulator = new Emulator();
    double aps = 0;
    for (int i=1; i<12; i++)
    {
        aps = topologyEmulator->getAPs(11, 15, 135);
    }

    QString smart = askTopologyPerformanceSmartIndividual();
    qDebug(qPrintable(smart));

}


MainWindow::~MainWindow()
{

}



void MainWindow::executeAlgorithm()
{
    /*
    if (!validateFields())
    {
        return;
    }
    */

    // creacion del objeto simulacion
    simulation = new Simulation(population,
                                externalFileSize,
                                generationsNumber,
                                gridSubintervalnumber,
                                updateFrequency,
                                matchesPerIndividual,
                                standarDeviation,
                                standarDeviationMinChannelTime,
                                standarDeviationMaxChannelTime,
                                doDirectedMutation,
                                mutationProbability,
                                individualSize,
                                ctableWindow,
                                indexToSortCTable,
                                smartIndividualSequence);

    qsrand((uint)QTime::currentTime().msec());


    /*
    QFile file("/tmp/algorithmResult.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        //QMessageBox msg;
        //msg.setText("No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        qDebug("No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
        //msg.exec();
        return;
    }
    QTextStream out(&file);
    out << "Inicia ejecucion del algoritmo cultural." <<"\n";

    out << endl;
    out << "Parametros de la ejecucion" << endl;
    out << "Tamano de la poblacion: " << QString::number(population) << endl;
    out << "Tamano del archivo externo: " << QString::number(externalFileSize) << endl;
    out << "Maximo numero de generaciones: " << QString::number(generationsNumber) << endl;
    out << "Numero de subintervalos para la rejilla: " << QString::number(gridSubintervalnumber) << endl;
    out << "Numero de generaciones para actualizar parte normativa: " << QString::number(updateFrequency) << endl;
    out << "Numero de encuentros por individuo en un torneo: " << QString::number(population/2) << endl;
    out << "Desviacion estandar de la mutacion gausiana: " << QString::number(standarDeviation) << endl;
    out << "Numero de APs desplegados: " << 25 << endl;
    */

    // inicializar poblacion de tamano P
    if (useSmartIndividual)
    {
        simulation->initializeSmartPopulation();
    }
    else
    {
        simulation->initializePopulation();
    }

    // evaluar poblacion inicial
    simulation->evaluateIndividuals();

    // inicializar parte fenotipica normativa del espacio de creencias
    simulation->initializeNormativePhenotypicPart();
    qDebug("...se acaba de inicializar la parte normativa fenotipica del espacio de creencias");

    // inicializar rejilla del espacio de creencias
    simulation->initializeGrid();
    qDebug("...se acaba de inicializar la grid");

    // inicializar la tabla C del espacio de creencias si se va a usar mutacion dirigida
    if (doDirectedMutation)
    {
        simulation->initializeCTable();
    }

    // lista de individuos no dominados
    QList<Individual *> nonDominatedList;

    // contador de generaciones para la actualizacion de la parte fenotipica normativa
    int countOfGenerations = 1;

    // repetir por el numero maximo de generaciones
    do{
        qDebug("...generacion: %d", simulation->getCurrentGenerationNumber());

        /*
        QFile file("/tmp/algorithmResult.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            //QMessageBox msg;
            //msg.setText("No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
            qDebug("No se pudo abrir el archivo /tmp/algorithmResult.txt para escribir \nresultados de la ejecucion del algoritmo.");
            //msg.exec();
            return;
        }
        QTextStream out(&file);
        out << endl<< "Generacion: "<< simulation->getCurrentGenerationNumber() <<"\n";
        */

        // mutacion de la poblacion
        simulation->mutatePopulation();
        qDebug("...despues de simulation->mutatePopulation()");


        // evaluar hijos
        simulation->evaluateIndividuals();

        // realizar torneos y seleccionar poblacion
        simulation->selectPopulation();
        qDebug("...despues de simulation->selectPopulation()");

        // imprimir poblacion para depuracion
        simulation->printPopulation();

        // obtener los individuos no dominados nueva poblacion P
        nonDominatedList = simulation->getNonDominatedPopulationApproach1();
        qDebug("...Numero de individuos en la poblacion no dominada: %d", nonDominatedList.count());


        qDebug("-------INDIVIDUOS no dominados antes de insertarlos en el archivo externo-------");
        simulation->printList(nonDominatedList);
        qDebug("--------------------------------------------------------------------------------");
        qDebug("...despues de obtener los individuos no dominados");

        // agregar los individuos no dominados al archivo externo
        simulation->addNonDominatedIndividualsToExternalFile(nonDominatedList);
        //simulation->addNonDominatedIndividualsToExternalFile(simulation->getPopulationList());

        qDebug("Cantidad de INDIVIDUOS no dominados despues de insertarlos en el archivo externo: %d",
               simulation->getExternalFile()->getExternalFileList().count());


        // actualizar la rejilla con todos los individuos no dominados recien agregados al archivo externo
        // durante la generación actual
        simulation->updateGrid(simulation->getExternalFile()->getCurrentGenerationIndividualList());

        // actualizar el espacio de creencias con los individos aceptados
        if (countOfGenerations == simulation->getgNormative())
        {
            qDebug("MainWindow.cpp: numero de generaciones transcurridas: %d ahora actualizar parte normativa", countOfGenerations);
            simulation->updateNormativePhenotypicPart();
        }

        // actualizar la tabla C del espacio de creencias todos los individuos no dominados recien agregados al archivo externo
        // durante la generación actual si se va a utilizar la mutacion dirigida
        if (doDirectedMutation)
        {
            simulation->updateCTable(simulation->getExternalFile()->getCurrentGenerationIndividualList());
        }

        simulation->getExternalFile()->resetCurrentGenerationIndividualList();
        qDebug("...despues de actualizar la rejilla");

        qDebug("generacion actual: %d", simulation->getCurrentGenerationNumber());

        qDebug("****************************************************************************");
        qDebug("Individuos del archivo externo");
        simulation->printList(simulation->getExternalFile()->getExternalFileList());
        qDebug("****************************************************************************");

        //QMessageBox msg;
        //QString string = "Ver el Archivo externo al final de la generacion ";
        //string.append(QString::number(simulation->getCurrentGenerationNumber()));
        //msg.setText(string);
        //msg.exec();


        simulation->incrementGeneration();

        // incrementar contador de generaciones para actualizar parte fenotipica normativa
        countOfGenerations++;

    }while(!simulation->stopEvolution()); // fin de la repeticion

    qDebug("*********");
    qDebug("TERMINO EL ALGORITMO CULTURAL!");


    // poblar la lista de individuos no dominados del archivo externo
    //populateListView();


    //if (ui->checkBoxDirectedMutation->isChecked())
    if (doDirectedMutation)
    {
        modificatedAlgorithmSolutions = simulation->getExternalFile()->getExternalFileList();
    }
    else
    {
        genericAlgorithmSolutions = simulation->getExternalFile()->getExternalFileList();
    }


    // generar el grafico
    //setupCustomPlot(ui->customPlot);

    // generar el grafico
    //plotSolutions();

    //storeExecutionSolution();

    // *****************************************************************************
    /*
    if (!doComparationOfAlgorithms)
    {
        // escribir los resultados de la simulacion
        QList<Individual *> finalResultsList;
        QString outputFileName = "";
        // cadena con el nombre del subdirectorio que almacenara los resultados
        QString resultsDirectory = createResultsDirectory();

        if (doDirectedMutation)
        {
            finalResultsList = modificatedAlgorithmSolutions;
            outputFileName = "individuosFrenteParetoModificado";
        }
        else
        {
            finalResultsList = genericAlgorithmSolutions;
            outputFileName = "individuosFrenteParetoOriginal";
        }

        // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
        qSort(finalResultsList.begin(), finalResultsList.end(), xLessThanLatency);

        // escribir en un archivo los individuos del frente de pareto encontrado en un archivo
        reportIndividualAsFile(finalResultsList, resultsDirectory, outputFileName);


        // escribir en un archivo los individuos del frente de pareto encontrado ordenados por latencia en genes en un archivo
        reportIndividualOrderedByLatencyInGenes(finalResultsList, resultsDirectory, "individuosFrenteParetoOriginalPorLatencia");
    }
    */
    // *****************************************************************************


    qDebug("antes de salir executeAlgorithm");
}


void MainWindow::executeAlgorithmRepeated()
{
    qDebug("MainWindow::compareAlgorithmRepeated()");

    QTime timer;
    QList<double> executionTimeList;



    // ejecuciones del algoritmo original
    for (int i=0; i<simulationRepetitions; i++)
    {
        timer.start();
        executeAlgorithm();
        executionTimeList.append(timer.elapsed());
        QList<Individual*> list(genericAlgorithmSolutions);
        repeatedOriginalSolutionList.append(list);
    }

    // calcular el tiempo promedio de ejecucion del algoritmo original
    double meanExecutionTimeOriginal = getMeanExecutionTime(executionTimeList);
    double stdExecutionTimeOriginal = getStdDeviationExecutionTime(executionTimeList, meanExecutionTimeOriginal);
    executionTimeList.clear();

    qDebug("------");
    qDebug("Promedio de tiempo de ejecución original:");
    qDebug(qPrintable(QString::number(meanExecutionTimeOriginal)+" ms, "+QString::number(stdExecutionTimeOriginal)));

    qDebug("Promedio de numero de individuos no dominados algoritmo original:");
    int totalIndividuals = 0;
    for (int j=0; j<repeatedOriginalSolutionList.count(); j++)
    {
        totalIndividuals = totalIndividuals + repeatedOriginalSolutionList.at(j).count();
    }
    double meanNonDominatedIndividuals1 = totalIndividuals/repeatedOriginalSolutionList.count();
    qDebug(qPrintable(QString::number(meanNonDominatedIndividuals1)+" individuos"));

    double meanF1Original = getMeanOfObjectiveFunction(1, repeatedOriginalSolutionList, 1);
    qDebug("Promedio de Fo1 original: %s", qPrintable(QString::number(meanF1Original)));
    qDebug("STD de Fo1 original: %s", qPrintable(QString::number(getStandardDeviation(meanF1Original, 1, repeatedOriginalSolutionList, 1))));
    double meanF2Original = getMeanOfObjectiveFunction(2, repeatedOriginalSolutionList, 1);
    qDebug("Promedio de Fo2 original: %s", qPrintable(QString::number(meanF2Original)));
    qDebug("STD de Fo2 original: %s", qPrintable(QString::number(getStandardDeviation(meanF2Original, 2, repeatedOriginalSolutionList, 1))));

    // cadena con el nombre del subdirectorio que almacenara los resultados
    //QString resultsDirectory = createResultsDirectory();
    resultsDirectory = createResultsDirectory();

    QFile file(resultsDirectory+"/resultadosFinalesEjecucionRepetida.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text /*| QIODevice::Append*/))
    {
        //QMessageBox msg;
        //msg.setText("No se pudo abrir el archivo /tmp/resultadosFinalesComparacion.txt para escribir \nresultados de la comparacion de algoritmos.");
        //msg.exec();
        qDebug("No se pudo abrir el archivo /tmp/resultadosFinalesEjecucionRepetida.txt para escribir \nresultados de la ejecucion repetida");
        return;
    }
    QTextStream out(&file);

    out << "Resultados de ejecucion repetida de algoritmo original" << endl;
    out << "Promedio de tiempo de ejecución original: " << QString::number(meanExecutionTimeOriginal) << " ms, std: " <<
           QString::number(stdExecutionTimeOriginal) << endl;

    out << endl;
    out << "Promedio de numero de individuos no dominados algoritmo original: " << QString::number(meanNonDominatedIndividuals1) << " individuos" << endl;

    out << endl;
    out << "Promedio de Fo1 original: " << QString::number(meanF1Original) << endl;
    out << "STD de Fo1 original: " << QString::number(getStandardDeviation(meanF1Original, 1, repeatedOriginalSolutionList, 1)) << endl;
    out << "Promedio de Fo2 original: " << QString::number(meanF2Original) << endl;
    out << "STD de Fo2 original: " << QString::number(getStandardDeviation(meanF2Original, 2, repeatedOriginalSolutionList, 1)) << endl;
    out << endl;


    // prueba de obtener los no dominados de todas las ejecuciones del algoritmo
    QList<Individual*> myList = getNonDominatedIndivualsFromRepetitions(true);
    qDebug("--------");
    qDebug("individuos no dominados del algoritmo original: %s", qPrintable(QString::number(myList.count())));
/*
    QVector<double> discoveryParetoOriginal(myList.count()), latencyParetoOriginal(myList.count());
    Individual * paretoIndividual;
    int vectorPosition = 0;

    for (int i=0; i<myList.count();i++)
    {
        paretoIndividual = myList.at(i);
        discoveryParetoOriginal[vectorPosition] = paretoIndividual->getPerformanceDiscovery();
        latencyParetoOriginal[vectorPosition] = paretoIndividual->getPerformanceLatency();
        vectorPosition++;
    }
*/

    // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
    qSort(myList.begin(), myList.end(), xLessThanLatency);


    // escribir en un archivo los individuos del frente de pareto encontrado en un archivo
    reportIndividualAsFile(myList, resultsDirectory, "individuosFrenteParetoOriginal");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por latencia en genes en un archivo
    reportIndividualOrderedByLatencyInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorLatencia");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma ascendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorAPsAscendente", true);

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma descendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorAPsDescendente", false);

    // limpiar lista de resultados
    myList.clear();

}


void MainWindow::executeModificatedAlgorithm()
{
    qDebug("MainWindow::executeModificatedAlgorithm()");

    QTime timer;
    QList<double> executionTimeList;


    doDirectedMutation = true;

    for (int i=0; i<simulationRepetitions; i++)
    {
        timer.start();
        executeAlgorithm();
        executionTimeList.append(timer.elapsed());
        QList<Individual*> list(modificatedAlgorithmSolutions);
        repeatedModificatedSolutionList.append(list);
    }

    // calcular el tiempo promedio de ejecucion del algoritmo modificado
    double meanExecutionTimeModificated = getMeanExecutionTime(executionTimeList);
    double stdExecutionTimeModificated = getStdDeviationExecutionTime(executionTimeList, meanExecutionTimeModificated);


    qDebug("Promedio de tiempo de ejecución modificado:");
    qDebug(qPrintable(QString::number(meanExecutionTimeModificated)+" ms, "+QString::number(stdExecutionTimeModificated)));
    qDebug("------");

    qDebug("Promedio de numero de individuos no dominados algoritmo modificado:");
    int totalIndividuals2 = 0;
    for (int j=0; j<repeatedModificatedSolutionList.count(); j++)
    {
        totalIndividuals2 = totalIndividuals2 + repeatedModificatedSolutionList.at(j).count();
    }
    double meanNonDominatedIndividuals2 = totalIndividuals2/repeatedModificatedSolutionList.count();
    qDebug(qPrintable(QString::number(meanNonDominatedIndividuals2)+" individuos"));
    qDebug("------");

    double meanF1Modificated = getMeanOfObjectiveFunction(1, repeatedModificatedSolutionList, 2);
    qDebug("Promedio de Fo1 modificada: %s", qPrintable(QString::number(meanF1Modificated)));
    qDebug("STD de Fo1 modificada: %s", qPrintable(QString::number(getStandardDeviation(meanF1Modificated, 1, repeatedModificatedSolutionList, 2))));
    double meanF2Modificated = getMeanOfObjectiveFunction(2, repeatedModificatedSolutionList, 2);
    qDebug("Promedio de Fo2 modificada: %s", qPrintable(QString::number(meanF2Modificated)));
    qDebug("STD de Fo2 modificada: %s", qPrintable(QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 2))));

    // cadena con el nombre del subdirectorio que almacenara los resultados
    //QString resultsDirectory = createResultsDirectory();
    resultsDirectory = createResultsDirectory();

    QFile file(resultsDirectory+"/resultadosFinalesEjecucionRepetidaMutDir.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text /*| QIODevice::Append*/))
    {
        //QMessageBox msg;
        //msg.setText("No se pudo abrir el archivo /tmp/resultadosFinalesComparacion.txt para escribir \nresultados de la comparacion de algoritmos.");
        //msg.exec();
        qDebug("No se pudo abrir el archivo /tmp/resultadosFinalesEjecucionRepetidaMutDir.txt para escribir \nresultados de la comparacion de algoritmos");
        return;
    }
    QTextStream out(&file);

    out << "Resultados de ejejcucion de algoritmo modificado" << endl;

    out << "Promedio de tiempo de ejecución modificado: " << QString::number(meanExecutionTimeModificated) << " ms, std: " <<
           QString::number(stdExecutionTimeModificated) << endl;
    out << endl;

    out << "Promedio de numero de individuos no dominados algoritmo modificado: " << QString::number(meanNonDominatedIndividuals2) << " individuos" << endl;
    out << endl;

    out << "Promedio de Fo1 modificada: " << QString::number(meanF1Modificated) << endl;
    out << "STD de Fo1 modificada: " << QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 1)) << endl;
    out << "Promedio de Fo2 modificada: " << QString::number(meanF2Modificated) << endl;
    out << "STD de Fo2 modificada: " << QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 2)) << endl;



    //---------------------------------------------------------------------------
    // prueba de obtener los no dominados de todas las ejecuciones del algoritmo
    QList<Individual*> myList = getNonDominatedIndivualsFromRepetitions(false);

    qDebug("--------");
    qDebug("individuos no dominados del algoritmo modificado: %s", qPrintable(QString::number(myList.count())));

    /*
    QVector<double> discoveryParetoModificated(myList.count()), latencyParetoModificated(myList.count());
    vectorPosition = 0;

    for (int i=0; i<myList.count();i++)
    {
        paretoIndividual = myList.at(i);
        discoveryParetoModificated[vectorPosition] = paretoIndividual->getPerformanceDiscovery();
        latencyParetoModificated[vectorPosition] = paretoIndividual->getPerformanceLatency();
        vectorPosition++;
    }
*/

    // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
    qSort(myList.begin(), myList.end(), xLessThanLatency);


    // agregar el individuo base para mostrarlo en la tabla de APs encontrados
    Individual * smartInd;
    if (useSmartIndividual)
    {
        smartInd = new Individual(true, smartIndividualSequence);
        smartInd->getAverageOnFullScanning();
        myList.insert(0, smartInd);
    }



    // escribir en un archivo los individuos del frente de pareto encontrado en un archivo
    reportIndividualAsFile(myList, resultsDirectory, "individuosFrenteParetoModificado");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por latencia en genes en un archivo
    reportIndividualOrderedByLatencyInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorLatencia");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma ascendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorAPsAscendente", true);

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma descendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorAPsDescendente", false);


    qDebug("--------");
    //---------------------------------------------------------------------------
}


void MainWindow::compareAlgorithmRepeated()
{

    qDebug("MainWindow::compareAlgorithmRepeated()");

    QTime timer;
    QList<double> executionTimeList;



    // ejecuciones del algoritmo original
    for (int i=0; i<simulationRepetitions; i++)
    {
        timer.start();
        executeAlgorithm();
        executionTimeList.append(timer.elapsed());
        QList<Individual*> list(genericAlgorithmSolutions);
        repeatedOriginalSolutionList.append(list);
    }
    // calcular el tiempo promedio de ejecucion del algoritmo original
    double meanExecutionTimeOriginal = getMeanExecutionTime(executionTimeList);
    double stdExecutionTimeOriginal = getStdDeviationExecutionTime(executionTimeList, meanExecutionTimeOriginal);
    executionTimeList.clear();

    // ejecuciones del algoritmo modificado
    //ui->checkBoxDirectedMutation->setChecked(true);
    doDirectedMutation = true;

    for (int i=0; i<simulationRepetitions; i++)
    {
        timer.start();
        executeAlgorithm();
        executionTimeList.append(timer.elapsed());
        QList<Individual*> list(modificatedAlgorithmSolutions);
        repeatedModificatedSolutionList.append(list);
    }

    // calcular el tiempo promedio de ejecucion del algoritmo modificado
    double meanExecutionTimeModificated = getMeanExecutionTime(executionTimeList);
    double stdExecutionTimeModificated = getStdDeviationExecutionTime(executionTimeList, meanExecutionTimeModificated);

    qDebug("------");
    qDebug("Promedio de tiempo de ejecución original:");
    qDebug(qPrintable(QString::number(meanExecutionTimeOriginal)+" ms, "+QString::number(stdExecutionTimeOriginal)));

    qDebug("Promedio de tiempo de ejecución modificado:");
    qDebug(qPrintable(QString::number(meanExecutionTimeModificated)+" ms, "+QString::number(stdExecutionTimeModificated)));
    qDebug("------");

    qDebug("Promedio de numero de individuos no dominados algoritmo original:");
    int totalIndividuals = 0;
    for (int j=0; j<repeatedOriginalSolutionList.count(); j++)
    {
        totalIndividuals = totalIndividuals + repeatedOriginalSolutionList.at(j).count();
    }
    double meanNonDominatedIndividuals1 = totalIndividuals/repeatedOriginalSolutionList.count();
    qDebug(qPrintable(QString::number(meanNonDominatedIndividuals1)+" individuos"));

    qDebug("Promedio de numero de individuos no dominados algoritmo modificado:");
    int totalIndividuals2 = 0;
    for (int j=0; j<repeatedModificatedSolutionList.count(); j++)
    {
        totalIndividuals2 = totalIndividuals2 + repeatedModificatedSolutionList.at(j).count();
    }
    double meanNonDominatedIndividuals2 = totalIndividuals2/repeatedModificatedSolutionList.count();
    qDebug(qPrintable(QString::number(meanNonDominatedIndividuals2)+" individuos"));
    qDebug("------");



    double meanF1Original = getMeanOfObjectiveFunction(1, repeatedOriginalSolutionList, 1);
    qDebug("Promedio de Fo1 original: %s", qPrintable(QString::number(meanF1Original)));
    qDebug("STD de Fo1 original: %s", qPrintable(QString::number(getStandardDeviation(meanF1Original, 1, repeatedOriginalSolutionList, 1))));
    double meanF2Original = getMeanOfObjectiveFunction(2, repeatedOriginalSolutionList, 1);
    qDebug("Promedio de Fo2 original: %s", qPrintable(QString::number(meanF2Original)));
    qDebug("STD de Fo2 original: %s", qPrintable(QString::number(getStandardDeviation(meanF2Original, 2, repeatedOriginalSolutionList, 1))));


    double meanF1Modificated = getMeanOfObjectiveFunction(1, repeatedModificatedSolutionList, 2);
    qDebug("Promedio de Fo1 modificada: %s", qPrintable(QString::number(meanF1Modificated)));
    qDebug("STD de Fo1 modificada: %s", qPrintable(QString::number(getStandardDeviation(meanF1Modificated, 1, repeatedModificatedSolutionList, 2))));
    double meanF2Modificated = getMeanOfObjectiveFunction(2, repeatedModificatedSolutionList, 2);
    qDebug("Promedio de Fo2 modificada: %s", qPrintable(QString::number(meanF2Modificated)));
    qDebug("STD de Fo2 modificada: %s", qPrintable(QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 2))));

    // cadena con el nombre del subdirectorio que almacenara los resultados
    QString resultsDirectory = createResultsDirectory();

    QFile file(resultsDirectory+"/resultadosFinalesComparacion.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text /*| QIODevice::Append*/))
    {
        //QMessageBox msg;
        //msg.setText("No se pudo abrir el archivo /tmp/resultadosFinalesComparacion.txt para escribir \nresultados de la comparacion de algoritmos.");
        //msg.exec();
        qDebug("No se pudo abrir el archivo /tmp/resultadosFinalesComparacion.txt para escribir \nresultados de la comparacion de algoritmos");
        return;
    }
    QTextStream out(&file);

    out << "Resultados de comparacion de algoritmos" << endl;
    out << "Promedio de tiempo de ejecución original: " << QString::number(meanExecutionTimeOriginal) << " ms, std: " <<
           QString::number(stdExecutionTimeOriginal) << endl;
    out << "Promedio de tiempo de ejecución modificado: " << QString::number(meanExecutionTimeModificated) << " ms, std: " <<
           QString::number(stdExecutionTimeModificated) << endl;
    out << endl;
    out << "Promedio de numero de individuos no dominados algoritmo original: " << QString::number(meanNonDominatedIndividuals1) << " individuos" << endl;
    out << "Promedio de numero de individuos no dominados algoritmo modificado: " << QString::number(meanNonDominatedIndividuals2) << " individuos" << endl;
    out << endl;
    out << "Promedio de Fo1 original: " << QString::number(meanF1Original) << endl;
    out << "STD de Fo1 original: " << QString::number(getStandardDeviation(meanF1Original, 1, repeatedOriginalSolutionList, 1)) << endl;
    out << "Promedio de Fo2 original: " << QString::number(meanF2Original) << endl;
    out << "STD de Fo2 original: " << QString::number(getStandardDeviation(meanF2Original, 2, repeatedOriginalSolutionList, 1)) << endl;
    out << endl;
    out << "Promedio de Fo1 modificada: " << QString::number(meanF1Modificated) << endl;
    out << "STD de Fo1 modificada: " << QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 1)) << endl;
    out << "Promedio de Fo2 modificada: " << QString::number(meanF2Modificated) << endl;
    out << "STD de Fo2 modificada: " << QString::number(getStandardDeviation(meanF2Modificated, 2, repeatedModificatedSolutionList, 2)) << endl;



    //---------------------------------------------------------------------------
    // prueba de obtener los no dominados de todas las ejecuciones del algoritmo
    QList<Individual*> myList = getNonDominatedIndivualsFromRepetitions(true);
    qDebug("--------");
    qDebug("individuos no dominados del algoritmo original: %s", qPrintable(QString::number(myList.count())));
/*
    QVector<double> discoveryParetoOriginal(myList.count()), latencyParetoOriginal(myList.count());
    Individual * paretoIndividual;
    int vectorPosition = 0;

    for (int i=0; i<myList.count();i++)
    {
        paretoIndividual = myList.at(i);
        discoveryParetoOriginal[vectorPosition] = paretoIndividual->getPerformanceDiscovery();
        latencyParetoOriginal[vectorPosition] = paretoIndividual->getPerformanceLatency();
        vectorPosition++;
    }
*/

    // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
    qSort(myList.begin(), myList.end(), xLessThanLatency);


    // escribir en un archivo los individuos del frente de pareto encontrado en un archivo
    reportIndividualAsFile(myList, resultsDirectory, "individuosFrenteParetoOriginal");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por latencia en genes en un archivo
    reportIndividualOrderedByLatencyInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorLatencia");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma ascendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorAPsAscendente", true);

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma descendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoOriginalPorAPsDescendente", false);

    // colocar las cadenas en la pestana de cadenas de la interfaz grafica
    //populateAListView(myList, ui->listViewPFOriginal);

    myList.clear();

    // registrar los resultados del algoritmo modificado

    myList = getNonDominatedIndivualsFromRepetitions(false);
    qDebug("--------");
    qDebug("individuos no dominados del algoritmo modificado: %s", qPrintable(QString::number(myList.count())));

    /*
    QVector<double> discoveryParetoModificated(myList.count()), latencyParetoModificated(myList.count());
    vectorPosition = 0;

    for (int i=0; i<myList.count();i++)
    {
        paretoIndividual = myList.at(i);
        discoveryParetoModificated[vectorPosition] = paretoIndividual->getPerformanceDiscovery();
        latencyParetoModificated[vectorPosition] = paretoIndividual->getPerformanceLatency();
        vectorPosition++;
    }
*/

    // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
    qSort(myList.begin(), myList.end(), xLessThanLatency);

    // escribir en un archivo los individuos del frente de pareto encontrado en un archivo
    reportIndividualAsFile(myList, resultsDirectory, "individuosFrenteParetoModificado");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por latencia en genes en un archivo
    reportIndividualOrderedByLatencyInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorLatencia");

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma ascendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorAPsAscendente", true);

    // escribir en un archivo los individuos del frente de pareto encontrado ordenados por APs encontrados en genes de
    // forma descendente
    reportIndividualOrderedByApInGenes(myList, resultsDirectory, "individuosFrenteParetoModificadoPorAPsDescendente", false);

    // colocar las cadenas en la pestana de cadenas de la interfaz grafica
    //populateAListView(myList, ui->listViewPFModificated);

    qDebug("--------");
    //---------------------------------------------------------------------------
}


void MainWindow::reportIndividualAsFile(QList<Individual*> list, QString fileName)
{
    QFile file("/tmp/"+fileName+".txt");
    if (file.exists())
    {
        file.remove();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/"+fileName+".txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&file);
    out << "/tmp/"+fileName+".txt - Individuos encontrados luego de ejecutar el algoritmo cultural: " << "\n";

    for(int i=0; i<list.count(); i++)
    {
        out << list.at(i)->getIndividualAsQString() << "\n";
    }
}

void MainWindow::reportIndividualAsFile(QList<Individual*> list, QString resultsSubdirectory, QString fileName)
{
    QFile file(resultsSubdirectory+"/"+fileName+".txt");
    if (file.exists())
    {
        file.remove();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/"+fileName+".txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&file);
    //out << resultsSubdirectory +"/"+fileName+".txt - Individuos encontrados luego de ejecutar el algoritmo cultural: " << "\n";

    for(int i=0; i<list.count(); i++)
    {
        out << list.at(i)->getIndividualAsQString() << "\n";
    }
}


void MainWindow::reportIndividualOrderedByLatencyInGenes(QList<Individual*> list, QString resultsSubdirectory, QString fileName)
{
    QFile file(resultsSubdirectory+"/"+fileName+".txt");
    if (file.exists())
    {
        file.remove();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/"+fileName+".txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&file);

    //out << resultsSubdirectory +"/"+fileName+".txt - Individuos encontrados luego de ejecutar el algoritmo cultural: " << "\n";
    //for(int i=0; i<list.count(); i++)
    //{
    //    out << list.at(i)->getIndividualAsQString() << "\n";
    //}

    // variable auxiliar para recorrer los genes del individuo
    int aux = 1;

    // lista de genes a ordenar
    QList<Gen*> genList;

    QString str;

    // iterar sobre los individuos de la lista
    for (int h=0; h<list.count();h++)
    {
        Individual * ind = list.at(h);

        // iterar sobre el tamano del individuo
        for (int i=0; i<individualSize; i++)
        {
            Gen * gen = new Gen();

            //qDebug("i: %s", qPrintable(QString::number(i)));
            // iterar sobre los parametros de cada gen
            for (int j=0; j<4; j++)
            {
                //qDebug("j: %s", qPrintable(QString::number(j)));
                if (aux == 1)
                {
                    //individualString.append("<");
                    //individualString.append(QString::number(getParameter(i*4)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setChannel(ind->getParameter(i*4));
                    aux++;
                }
                else if (aux == 2)
                {
                    //individualString.append(QString::number(getParameter(i*4+1)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setMinChannelTime(ind->getParameter(i*4+1));
                    aux++;
                }
                else if (aux == 3)
                {
                    //individualString.append(QString::number(getParameter(i*4+2)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setMaxChannelTime(ind->getParameter(i*4+2));
                    aux++;
                }
                else if (aux == 4)
                {
                    //individualString.append(QString::number(getParameter(i*4+3)));
                    //individualString.append(">");
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setAPs(ind->getParameter(i*4+3));
                    aux = 1;
                }

            } // fin de parametros de gen

            // insertar el gen en la lista
            genList.append(gen);

        } // fin de iterar sobre el tamano del individuo

        // ordenar la lista en orden ascendente de acuerdo a la latencia (F2)
        qSort(genList.begin(), genList.end(), genLessThanLatency);


        for(int i=genList.count(); i>0; i--)
        {
            for (int j=0; j<4; j++)
            {
                str.append(QString::number(genList.at(i-1)->getValue(j)));
                str.append(",");
            }

        }
        str.append(QString::number(ind->getPerformanceDiscovery()));
        str.append(",");
        str.append(QString::number(ind->getPerformanceLatency()));

        // para reportar nscans
        str.append(",");
        str.append(QString::number(ind->getNscanForMutation()));

        str.append("\n");
        out << str;
        str.clear();
        genList.clear();


    } // fin de iterar sobre individuos


}


void MainWindow::reportIndividualOrderedByApInGenes(QList<Individual*> list, QString resultsSubdirectory, QString fileName, bool ascending)
{
    QFile file(resultsSubdirectory+"/"+fileName+".txt");
    if (file.exists())
    {
        file.remove();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/"+fileName+".txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&file);

    // variable auxiliar para recorrer los genes del individuo
    int aux = 1;

    // lista de genes a ordenar
    QList<Gen*> genList;

    // cadena que almacenara todos los parametros del individuo
    QString str;

    // valor del indice FONC del individuo
    double sumFonc = 0;

    // iterar sobre los individuos de la lista
    for (int h=0; h<list.count();h++)
    {
        Individual * ind = list.at(h);

        // iterar sobre el tamano del individuo
        for (int i=0; i<individualSize; i++)
        {
            Gen * gen = new Gen();

            //qDebug("i: %s", qPrintable(QString::number(i)));
            // iterar sobre los parametros de cada gen
            for (int j=0; j<4; j++)
            {
                //qDebug("j: %s", qPrintable(QString::number(j)));
                if (aux == 1)
                {
                    //individualString.append("<");
                    //individualString.append(QString::number(getParameter(i*4)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setChannel(ind->getParameter(i*4));
                    aux++;
                }
                else if (aux == 2)
                {
                    //individualString.append(QString::number(getParameter(i*4+1)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setMinChannelTime(ind->getParameter(i*4+1));
                    aux++;
                }
                else if (aux == 3)
                {
                    //individualString.append(QString::number(getParameter(i*4+2)));
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setMaxChannelTime(ind->getParameter(i*4+2));
                    aux++;
                }
                else if (aux == 4)
                {
                    //individualString.append(QString::number(getParameter(i*4+3)));
                    //individualString.append(">");
                    //individualString.append(",");
                    //qDebug("aux %s: %s", qPrintable(QString::number(aux)), qPrintable(individualString));

                    gen->setAPs(ind->getParameter(i*4+3));
                    aux = 1;
                }

            } // fin de parametros de gen

            // insertar el gen en la lista
            genList.append(gen);

        } // fin de iterar sobre el tamano del individuo

        // ordenar la lista en orden de acuerdo al numero de ap
        qSort(genList.begin(), genList.end(), genLessThanAPs);

        if (ascending)
        {

            // reordenar la lista en caso de existan genes con igual numero de APs con respecto a la latencia
            QList<Gen*> reorderListByAP = reorderAP(genList, true);

            // recorrer la lista de genes de forma ascendente
            //for(int i=0; i<genList.count(); i++)
            for(int i=0; i<reorderListByAP.count(); i++)
            {
                for (int j=0; j<4; j++)
                {
                    str.append(QString::number(reorderListByAP.at(i)->getValue(j)));
                    //str.append(QString::number(genList.at(i)->getValue(j)));
                    str.append(",");
                }

            }


        }
        else // orden descendente
        {
            // recorrer la lista de genes de forma descendente
            /*
            for(int i=genList.count(); i>0; i--)
            {
                for (int j=0; j<4; j++)
                {
                    str.append(QString::number(genList.at(i-1)->getValue(j)));
                    str.append(",");
                }

            }
            */

            // lista en orden descendente
            QList<Gen*> invertedList;
            Gen * gen;
            for(int i=genList.count(); i>0; i--)
            {
                gen = genList.at(i-1);
                invertedList.append(gen);
            }

            // reordenar la lista en caso de existan genes con igual numero de APs con respecto a la latencia
            QList<Gen*> reorderListByAP = reorderAP(invertedList, true);


            //for(int i=0; i<invertedList.size(); i++)
            for(int i=0; i<reorderListByAP.count(); i++)
            {
                for (int j=0; j<4; j++)
                {
                    str.append(QString::number(reorderListByAP.at(i)->getValue(j)));
                    //str.append(QString::number(invertedList.at(i)->getValue(j)));
                    str.append(",");
                }

                // obtener el valor del indice FONC asociado al gen y agregarlo a la cadena del individuo
                double genFonc = reorderListByAP.at(i)->getFONC();
                str.append(QString::number(genFonc));
                str.append(",");
                sumFonc = sumFonc + genFonc;
            }

        }

        double discovery = ind->getPerformanceDiscovery();
        str.append(QString::number(discovery));
        str.append(",");
        str.append(QString::number(ind->getPerformanceLatency()));

        // para reportar nscans
        //str.append(",");
        //str.append(QString::number(ind->getNscanForMutation()));

        // para reportar suma simple de APs
        //str.append(",");
        //str.append(QString::number(discovery));

        // para reportar el indico FONC del individuo
        str.append(",");
        str.append(QString::number(sumFonc));
        sumFonc = 0;

        str.append("\n");
        out << str;
        str.clear();
        genList.clear();


    } // fin de iterar sobre individuos


}

QList<Gen*> MainWindow::reorderAP(QList<Gen*> originalList, bool ascending)
{
    int i=0;


    QList<Gen*> auxList;
    QList<Gen*> orderedList;

    Gen * gen1;

    // si solo hay un gen no se ordena nada
    if (originalList.size()==1)
    {
        return originalList;
    }



    while(i<originalList.count())
    {

        if (auxList.isEmpty())
        {
            gen1 = originalList.at(i);
            auxList.append(gen1);
            i++;
            continue;
        }
        gen1 = originalList.at(i);
        if (gen1->getValue(3)== auxList.at(auxList.count()-1)->getValue(3))
        {
            if ((i+1)==originalList.count())
            {
                auxList.append(gen1);
                qSort(auxList.begin(), auxList.end(), genLessThanLatency);
                for (int i=0;i<auxList.count();i++)
                {
                    orderedList.append(auxList.at(i));
                }
            }
            else
            {
                auxList.append(gen1);
            }
        }
        else
        {
            // verificar si auxList tiene un solo elemento
            if (auxList.count() == 1)
            {
                if ((i+1)==originalList.count())
                {
                    orderedList.append(auxList.at(0));
                    orderedList.append(gen1);
                    auxList.clear();

                }
                else
                {
                    orderedList.append(auxList.at(0));
                    auxList.clear();
                    auxList.append(gen1);
                }

            }
            else // la lista auxList tiene varios elementos entonces ordenarlos
            {
                if ((i+1)==originalList.count())
                {
                    qSort(auxList.begin(), auxList.end(), genLessThanLatency);
                    for (int i=0;i<auxList.count();i++)
                    {
                        orderedList.append(auxList.at(i));
                    }
                    auxList.clear();
                    orderedList.append(gen1);
                }
                else
                {
                    qSort(auxList.begin(), auxList.end(), genLessThanLatency);
                    for (int i=0;i<auxList.count();i++)
                    {
                        orderedList.append(auxList.at(i));
                    }
                    auxList.clear();
                    auxList.append(gen1);
                }


            }
        }
        i++;

    }

    return orderedList;
}

double MainWindow::getMeanExecutionTime(QList<double> l)
{

    double sumOfExecutionTime= 0;

    for (int j=0; j<l.count(); j++)
    {
        sumOfExecutionTime = sumOfExecutionTime + l.at(j);

    }
    return sumOfExecutionTime/l.count();

}


double MainWindow::getStdDeviationExecutionTime(QList<double> l, double mean)
{
    double numerator = 0;
    double denominator = l.count();

    for (int j=0; j<l.count(); j++)
    {
        numerator = numerator + pow((l.at(j)-mean),2);

    }
    return sqrt(numerator/denominator);
}


int MainWindow::getCountOfNonDominatedInRepetitions()
{
    // identificar el número total de individuos de todas las ejecuciones
    int counter = 0;
    for (int x=0; x<repeatedSolutionList.count(); x++)
    {
        counter = counter + repeatedSolutionList.at(x).count();
    }
    return counter;
}

double MainWindow::getMeanOfObjectiveFunction(int fo, QList<QList<Individual *> >list, int type)
{
    int counter = 0; //getCountOfNonDominatedInRepetitions();

    if (type==0)
    {
        counter = getCountOfNonDominatedInRepetitions();
    }
    else if (type==1)
    {
        counter = getCountOfNonDominatedInOriginalRepetitions();
    }else if (type==2)
    {
        counter = getCountOfNonDominatedInModificatedRepetitions();
    }

    double sumOfDiscovery = 0;
    double sumOfLatency = 0;

    QList<Individual *> listOfNonDominated;
    Individual * individual;

    //for (int j=0; j<repeatedSolutionList.count(); j++)
    for (int j=0; j<list.count(); j++)
    {
        //listOfNonDominated = repeatedSolutionList.at(j);
        listOfNonDominated = list.at(j);
        for (int k=0; k<listOfNonDominated.count(); k++)
        {
            individual = listOfNonDominated.at(k);
            sumOfDiscovery = sumOfDiscovery + individual->getPerformanceDiscovery();
            sumOfLatency = sumOfLatency + individual->getPerformanceLatency();

        }
    }

    if (fo==1)
    {
        return sumOfDiscovery/counter;
    }
    else if (fo==2)
    {
        return sumOfLatency/counter;
    }
    else
    {
        return 0;
    }

    //qDebug("Promedio de Fo1: %s", qPrintable(QString::number(sumOfDiscovery/counter)));
    //qDebug("Promedio de Fo2: %s", qPrintable(QString::number(sumOfLatency/counter)));
}


double MainWindow::getStandardDeviation(double mean, int fo, QList<QList<Individual *> >list, int type)
{
    double numerator = 0;
    double denominator = 0; // getCountOfNonDominatedInRepetitions();

    if (type==0)
    {
        denominator = getCountOfNonDominatedInRepetitions();
    }
    else if (type==1)
    {
        denominator = getCountOfNonDominatedInOriginalRepetitions();
    }else if (type==2)
    {
        denominator = getCountOfNonDominatedInModificatedRepetitions();
    }


    QList<Individual *> listOfNonDominated;
    Individual * individual;

    //for (int j=0; j<repeatedSolutionList.count(); j++)
    for (int j=0; j<list.count(); j++)
    {
        //listOfNonDominated = repeatedSolutionList.at(j);
        listOfNonDominated = list.at(j);
        for (int k=0; k<listOfNonDominated.count(); k++)
        {
            individual = listOfNonDominated.at(k);

            if (fo == 1)
            {
                numerator = numerator + pow((individual->getPerformanceDiscovery()-mean),2);

            }
            else if (fo == 2)
            {
                numerator = numerator + pow((individual->getPerformanceLatency()-mean),2);

            }
            else
            {
                return 0;
            }
        }
    }
    return sqrt(numerator/denominator);

}


QList<Individual*> MainWindow::getNonDominatedIndivualsFromRepetitions(bool original)
{
    QList<Individual*> auxiliaryList;
    if (original)
    {
        auxiliaryList = getNonDominatedIndividualsFromList(repeatedOriginalSolutionList);
    }
    else
    {
        auxiliaryList = getNonDominatedIndividualsFromList(repeatedModificatedSolutionList);
    }

    QList<Individual*> nonDominatedListToReturn;

    ExternalFile auxiliaryExternalFile;
    int p = auxiliaryList.count();

    Individual * individualI;
    Individual * individualJ;

    for (int i=0; i<p; i++)
    {
        bool dominated = false;
        individualI = auxiliaryList.at(i);

        for (int j=0; ((j<p) && (!dominated)); j++)
        {

            if (i==j)
            {
                continue;
            }
            individualJ = auxiliaryList.at(j);
            if (auxiliaryExternalFile.individualDominate(individualJ, individualI))
            {
                dominated = true;
            }
        }
        if (!dominated)
        {
            nonDominatedListToReturn.append(individualI);
        }
    }
    return nonDominatedListToReturn;


}


int MainWindow::getCountOfNonDominatedInOriginalRepetitions()
{
    // identificar el número total de individuos de todas las ejecuciones
    int counter = 0;
    for (int x=0; x<repeatedOriginalSolutionList.count(); x++)
    {
        counter = counter + repeatedOriginalSolutionList.at(x).count();
    }
    return counter;
}

int MainWindow::getCountOfNonDominatedInModificatedRepetitions()
{
    // identificar el número total de individuos de todas las ejecuciones
    int counter = 0;
    for (int x=0; x<repeatedModificatedSolutionList.count(); x++)
    {
        counter = counter + repeatedModificatedSolutionList.at(x).count();
    }
    return counter;
}

QList<Individual*> MainWindow::getNonDominatedIndividualsFromList(QList<QList<Individual *> > list)
{
    QList<Individual *> individualList;

    QList<Individual*> auxiliaryList;
    Individual * individual;
    for (int j=0; j<list.count(); j++)
    {
        auxiliaryList = list.at(j);
        for (int k=0; k<auxiliaryList.count(); k++)
        {
            individual = auxiliaryList.at(k);
            individualList.append(individual);
        }
    }
    return individualList;
}



QString MainWindow::createResultsDirectory()
{

    QString resultsDir = QDir::currentPath() + "/resultados";
    qDebug(qPrintable(resultsDir));

    QString outputDir = resultsDir + "/" + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss");
    qDebug(qPrintable(outputDir));


    QDir dir(resultsDir);
    if (!dir.exists())
    {
        qDebug("no existe");
        if (dir.mkdir(resultsDir))
            qDebug("se acaba de crear");
        else
            qDebug("no se pudo crear");
    }
    else
    {
        qDebug("existe");
    }

    // crear el directorio con la fecha
    QDir tmpDir(outputDir);
    if (!tmpDir.exists())
    {
        qDebug("no existe el directorio de ejecucion");
        if (tmpDir.mkdir(outputDir))
            qDebug("creado el directorio de la ejecucion");
        else
            qDebug("fallo la creacion del directorio de ejecucion");

    }
    return outputDir;

}


bool MainWindow::getDoComparationOfAlgorithms()
{
    return doComparationOfAlgorithms;
}

bool MainWindow::getDoOriginalAlgorithm()
{
    return doOriginalAlgorithm;
}

int MainWindow::getIndividualSize()
{
    return individualSize;
}


bool MainWindow::getEmulateScanning()
{
    return emulateScanning;
}

ScanningCampaing::ScanResults MainWindow::getRandomScan(int channel, int minChannelTime, int maxChannelTime)
{
    return scanningCampaing->randomScan(channel, minChannelTime, maxChannelTime);

}

int MainWindow::getAPs(int channel, int minChannelTime, int maxChannelTime)
{

    int low = 1;
    int high = 100;
    int random = qrand() % ((high + 1) - low) + low;

    //return scanningCampaing->getAPs(channel, minChannelTime, maxChannelTime, random);
    return 1;
}

bool MainWindow::getUseSmartIndividual()
{
    return useSmartIndividual;
}

double MainWindow::getLowerMinChannelTime()
{
    return lowerMinChannelTime;
}

double MainWindow::getUpperMinChannelTime()
{
    return upperMinChannelTime;
}

double MainWindow::getLowerMaxChannelTime()
{
    return lowerMaxChannelTime;
}

double MainWindow::getUpperMaxChannelTime()
{
    return upperMaxChannelTime;
}

int MainWindow::getIndexToSortCTable()
{
    return indexToSortCTable;
}



void MainWindow::generateAPResultsOfOriginalMutation()
{
    // resultsDirectory cadena que tiene la ruta absoluta al directorio de resultados

    // abrir el archivo individuosFrenteParetoOriginalPorAPsDescendente
    QFile inputFile(resultsDirectory+"/individuosFrenteParetoOriginalPorAPsDescendente.txt");
    //QFile inputFile("/home/antonio/Copy/2015/pgcomp/tesis/algoCultural/automatizarResultados/resultados/03.06.2015_13.19.01/individuosFrenteParetoOriginalPorAPsDescendente.txt");
    //QFile inputFile("/home/aaraujo/desarrollo/iaa/omocac3cli/resultados/03.06.2015_13.19.01/individuosFrenteParetoOriginalPorAPsDescendente.txt");
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Q_ASSERT_X(false, "MainWindow::generateAPResultsOfOriginalMutation()",
                   "no se pudo abrir el archivo individuosFrenteParetoOriginalPorAPsDescendente.txt para procesar.");
    }



    // crear archivo de salida
    QFile outputFile(resultsDirectory+"/dataToPlot.txt");
    //QFile outputFile("/tmp/dataToPlot.txt");
    if (outputFile.exists())
    {
        outputFile.remove();
    }
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/.txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&outputFile);

    QList<double> sequenceConfidenceInterval;
    QString intervalLine;

    // en el caso de utilizar un individuo inteligente como base para la creación de la poblacion
    // agregar los datos del individuo en la salida de dataToPlot.txt
    if (useSmartIndividual)
    {
        QString base = smartIndividualSequence;
        sequenceConfidenceInterval = processLine(base, true);

        intervalLine.append("indBase");
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(0)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(1)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(2)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append("\n");

        out << intervalLine ;
        intervalLine.clear();
        sequenceConfidenceInterval.clear();

    }


    QTextStream in(&inputFile);

    // contador de cadenas resultantes
    int i=1;

    // valor de latencia maxima para luego generar el grafico con este limite
    int maxLatency = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        //QString line = "1,39.02,0,2,39.02,0,3,39.02,0,4,39.02,0,5,39.02,0,6,39.02,0,7,39.02,0,8,39.02,0,9,39.02,0,10,39.02,0,11,39.02,0";
        qDebug("linea leida %s", qPrintable(line));
        sequenceConfidenceInterval = processLine(line);

        intervalLine.append("cadena"+QString::number(i));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(0)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(1)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(2)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append("\n");

        out << intervalLine ;
        intervalLine.clear();
        // incrementar contador de cadenas
        i++;

        // verificar la latencia maxima de las cadenas
        if (sequenceConfidenceInterval.at(3) > maxLatency)
        {
            maxLatency = sequenceConfidenceInterval.at(3);
        }
    }

    intervalLine = "Debian 7.633333 6.810066 8.456600 113 113 113\n";
    out << intervalLine ;
    intervalLine = "Windows 10.63333 9.764601 11.50206 168 168 168\n";
    out << intervalLine ;
    intervalLine = "Meego 10.7 9.607376 11.79262 184 184 184\n";
    out << intervalLine ;
    intervalLine = "iOS 16.4 14.96683 17.83317 429 429 429\n";
    out << intervalLine ;
    intervalLine = "Android 18.83333 17.60040 20.06627 601 601 601\n";
    out << intervalLine ;

    if (maxLatency <= 601)
    {
        maxLatency = 700;
    }

    outputFile.close();

    // hasta este punto se genero el archivo resultsDirectory/dataToPlot.txt
    generateGnuplotFile(i , maxLatency);

    QString program = "/bin/chmod +x "+ resultsDirectory + "/gnuplotFile.gp";
    system(qPrintable(program));


    //program = resultsDirectory + "/gnuplotFile.gp";
    //system(qPrintable(program));


    // generar archivo generateGraphic.sh
    generateGraphicOfSequences();

    // asignarle permiso de ejecucion
    program = "/bin/chmod +x "+ resultsDirectory + "/generateGraphic.sh";
    system(qPrintable(program));

    // ejecutar el script
    program = resultsDirectory + "/generateGraphic.sh";
    system(qPrintable(program));


    QString h = QDir::currentPath();

    // copiar el archivo settings.ini al directorio de resultados
    program = "cp "+ h + "/settings.ini "+ resultsDirectory ;
    system(qPrintable(program));


    return;
}


QList<double> MainWindow::processLine(QString line, bool individualBase)
{
    //qDebug("MainWindow::processLine(QString line)");

    QStringList tmpList;
    QStringList newList;
    tmpList = line.split(",");

    for (int i=0; i<individualSize; i++)
    {
        if (individualBase)
        {
            newList.append(tmpList.at(i*3));
            newList.append(tmpList.at(i*3+1));
            newList.append(tmpList.at(i*3+2));
        }
        else
        {
            /*
            newList.append(tmpList.at(i*4));
            newList.append(tmpList.at(i*4+1));
            newList.append(tmpList.at(i*4+2));
            */

            // (i*5) porque se esta considerando la cadena <ch,min,max,aps,fonc>...<aps,L,fonc_total>
            newList.append(tmpList.at(i*5));
            newList.append(tmpList.at(i*5+1));
            newList.append(tmpList.at(i*5+2));
        }


    }

    QString latency;

    if (individualBase)
    {
        double auxLatency = 0;
        for (int i = 0; i<individualSize; i++)
        {
            auxLatency = auxLatency + newList.at(i*3+1).toDouble() + newList.at(i*3+2).toDouble();
        }
        latency = QString::number(auxLatency);
    }
    else
    {
        //latency = tmpList.at(individualSize*4+1);

        // en el caso de <ch,min,max,aps,fonc>
        latency = tmpList.at(individualSize*5+1);

    }



    QString z;
    for (int i=0; i<newList.size(); i++)
    {
        z.append(newList.at(i));
        z.append(",");
    }
    qDebug("cadena: %s", qPrintable(z));

    ScanningCampaing scan("test_18.1.db", "full", 0);
    scan.init();

    scan.prepareIRD();

    // vector de canales de la secuencia
    std::vector<int> channelList;

    // vector de MinChannelTime de la secuencia
    std::vector<int> minChannelTimeList;

    // vector de MaxChannelTime de la secuencia
    std::vector<int> maxChannelTimeList;

    for (int i=0; i<individualSize; i++)
    {
        channelList.push_back(newList.at(i*3).toInt());
        minChannelTimeList.push_back(newList.at(i*3+1).toDouble());
        maxChannelTimeList.push_back(newList.at(i*3+2).toDouble());
    }

    //printf("canales: ");
    for (int j=0; j<channelList.size(); j++)
    {
        //printf("%d ", channelList.at(j));
    }
    //printf("\n");

    //printf("min: ");
    for (int j=0; j<minChannelTimeList.size(); j++)
    {
        //printf("%d ", minChannelTimeList.at(j));
    }
    //printf("\n");

    //printf("max: ");
    for (int j=0; j<maxChannelTimeList.size(); j++)
    {
        //printf("%d ", maxChannelTimeList.at(j));
    }
    //printf("\n");

    //printf("**************************************************\n");

    int aps = 0;
    double sum = 0;

    // lista que mantiene los APs obtenidos en cada una de las 30 repeticiones
    QList<double> apList;

    for (int j=0; j<30; j++)
    {
        aps = scan.getAPsForSequence(channelList, minChannelTimeList, maxChannelTimeList);
        apList.append(aps);
        //printf("%d,", aps);
        sum = sum + aps;
    }
    double apsAvg = sum/30;
    qDebug("\npromedio de APs en 30 ejecuciones: %f ", apsAvg);

    double variance = computeSampleVariance(apList, apsAvg);
    qDebug("varianza de APs en 30 ejecuciones: %f ", variance);

    double intervalValue = computeConfidenceInterval(apList, apsAvg);

    double minConfidenceInterval = apsAvg - intervalValue;

    double maxConfidenceInterval = apsAvg + intervalValue;

    qDebug("intervalo de confianza 95 para promedio de APs: %f %f %f ", apsAvg, minConfidenceInterval, maxConfidenceInterval);

    QList<double> confidenceInterval;
    confidenceInterval.append(apsAvg);
    confidenceInterval.append(minConfidenceInterval);
    confidenceInterval.append(maxConfidenceInterval);
    confidenceInterval.append(latency.toDouble());

    return confidenceInterval;
}


void MainWindow::generateAPResultsOfDirectedMutation()
{
    // resultsDirectory cadena que tiene la ruta absoluta al directorio de resultados

    // abrir el archivo individuosFrenteParetoOriginalPorAPsDescendente
    QFile inputFile(resultsDirectory+"/individuosFrenteParetoModificadoPorAPsDescendente.txt");
    //QFile inputFile("/home/antonio/Copy/2015/pgcomp/tesis/algoCultural/automatizarResultados/resultados/03.06.2015_13.19.01/individuosFrenteParetoOriginalPorAPsDescendente.txt");
    //QFile inputFile("/home/aaraujo/desarrollo/iaa/omocac3cli/resultados/03.06.2015_13.19.01/individuosFrenteParetoOriginalPorAPsDescendente.txt");
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Q_ASSERT_X(false, "MainWindow::generateAPResultsOfOriginalMutation()",
                   "no se pudo abrir el archivo individuosFrenteParetoOriginalPorAPsDescendente.txt para procesar.");
    }



    // crear archivo de salida
    QFile outputFile(resultsDirectory+"/dataToPlot.txt");
    //QFile outputFile("/tmp/dataToPlot.txt");
    if (outputFile.exists())
    {
        outputFile.remove();
    }
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/.txt";
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&outputFile);

    QList<double> sequenceConfidenceInterval;
    QString intervalLine;

    // en el caso de utilizar un individuo inteligente como base para la creación de la poblacion
    // agregar los datos del individuo en la salida de dataToPlot.txt
    if (useSmartIndividual)
    {
        QString base = smartIndividualSequence;
        sequenceConfidenceInterval = processLine(base, true);

        intervalLine.append("indBase");
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(0)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(1)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(2)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append("\n");

        out << intervalLine ;
        intervalLine.clear();
        sequenceConfidenceInterval.clear();

    }


    QTextStream in(&inputFile);

    // contador de cadenas resultantes
    int i=1;

    // primera linea leida
    bool first = true;

    // valor de latencia maxima para luego generar el grafico con este limite
    int maxLatency = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (useSmartIndividual && first)
        {
            first = false;
            continue;
        }

        //QString line = "1,39.02,0,2,39.02,0,3,39.02,0,4,39.02,0,5,39.02,0,6,39.02,0,7,39.02,0,8,39.02,0,9,39.02,0,10,39.02,0,11,39.02,0";
        qDebug("linea leida %s", qPrintable(line));
        sequenceConfidenceInterval = processLine(line);

        intervalLine.append("cadena"+QString::number(i));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(0)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(1)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(2)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append(" ");
        intervalLine.append(QString::number(sequenceConfidenceInterval.at(3)));
        intervalLine.append("\n");

        out << intervalLine ;
        intervalLine.clear();
        // incrementar contador de cadenas
        i++;

        // verificar la latencia maxima de las cadenas
        if (sequenceConfidenceInterval.at(3) > maxLatency)
        {
            maxLatency = sequenceConfidenceInterval.at(3);
        }
    }

    intervalLine = "Debian 7.633333 6.810066 8.456600 113 113 113\n";
    out << intervalLine ;
    intervalLine = "Windows 10.63333 9.764601 11.50206 168 168 168\n";
    out << intervalLine ;
    intervalLine = "Meego 10.7 9.607376 11.79262 184 184 184\n";
    out << intervalLine ;
    intervalLine = "iOS 16.4 14.96683 17.83317 429 429 429\n";
    out << intervalLine ;
    intervalLine = "Android 18.83333 17.60040 20.06627 601 601 601\n";
    out << intervalLine ;

    if (maxLatency <= 601)
    {
        maxLatency = 700;
    }

    outputFile.close();

    // hasta este punto se genero el archivo resultsDirectory/dataToPlot.txt
    generateGnuplotFile(i , maxLatency);

    QString program = "/bin/chmod +x "+ resultsDirectory + "/gnuplotFile.gp";
    system(qPrintable(program));


    //program = resultsDirectory + "/gnuplotFile.gp";
    //system(qPrintable(program));


    // generar archivo generateGraphic.sh
    generateGraphicOfSequences();

    // asignarle permiso de ejecucion
    program = "/bin/chmod +x "+ resultsDirectory + "/generateGraphic.sh";
    system(qPrintable(program));

    // ejecutar el script
    program = resultsDirectory + "/generateGraphic.sh";
    system(qPrintable(program));


    QString h = QDir::currentPath();

    // copiar el archivo settings.ini al directorio de resultados
    program = "cp "+ h + "/settings.ini "+ resultsDirectory ;
    system(qPrintable(program));


    return;

}



double MainWindow::computeSampleVariance(QList<double> sampleList, double mean)
{
    double sampleVar;

    // interesante lectura
    // https://statistics.laerd.com/statistical-guides/measures-of-spread-standard-deviation.php

    // Loop to compute sample variance
    sampleVar = 0.0;
    for (int i=0; i<sampleList.size(); i++)
    {
        sampleVar = sampleVar + (pow((sampleList.at(i) - mean), 2.0) / (sampleList.size() - 1));
    }

      return sampleVar;
}

double MainWindow::computeConfidenceInterval(QList<double> sampleList, double mean)
{
    double sampleVariance = computeSampleVariance(sampleList, mean);

    // valor de la t de student para n-1 (29) grados de libertad para intervalo de 95%
    double tStudentScore = 1.6991;

    double confidenceInterval = tStudentScore * sqrt(sampleVariance/sampleList.size());
    return confidenceInterval;
}




void MainWindow::generateGnuplotFile(int sequences, int maxLatency)
{
    // crear archivo de salida
    QFile outputFile(resultsDirectory+"/gnuplotFile.gp");
    if (outputFile.exists())
    {
        outputFile.remove();
    }
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/.txt";
        Q_ASSERT_X(false, "MainWindow::generateGnuplotFile()", "no se pudo generar el archivo gnuplot");
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&outputFile);

    QString string = "#!/usr/bin/gnuplot \n";
    out << string ;

    string = "clear \n";
    out << string ;

    string = "reset\n";
    out << string ;

    string = "set xtics rotate out\n";
    out << string ;

    string = "set style data histogram\n";
    out << string ;

    string = "set style fill solid border\n";
    out << string ;

    string = "set style histogram errorbars linewidth 1\n";
    out << string ;

    string = "set style fill solid 0.3\n";
    out << string ;

    string = "set bars front\n";
    out << string ;

    string = "set title \"Intervalos de confianza 95% para cadenas\"\n";
    out << string ;

    // verificar el numero de cadenas + 5 de referencia
    //string = "set xrange [-1:15]\n";
    string = "set xrange [-1:";
    string.append(QString::number(sequences+5)+"]\n");
    out << string ;

    string = "set ylabel \"Numero de APs\"\n";
    out << string ;

    //string = "set yrange [1:30]\n";
    //out << string ;

    // verificar la latencia maxima
    // set yrange [1:70]
    string = "set yrange [1:";

    //int ymax =
    string.append(QString::number(((maxLatency+10)*0.1))+"]\n");
    out << string ;

    string = "set y2label \"Latencia *10 (ms)\"\n";
    out << string ;

    string = "set grid\n";
    out << string ;

    string = "set term png\n";
    out << string ;

    string = "set output \"grafico.png\"\n";
    out << string ;

    string = "plot \"" + resultsDirectory + "/dataToPlot.txt\" using 2:3:4:xticlabel(1) title \"APs\", \"" + resultsDirectory + "/dataToPlot.txt\" using ($5*0.1):($6*0.1):($7*0.1):xticlabel(1) title \"Latencia\"\n";
    out << string ;

    string = "replot\n";
    out << string ;


    return;
}

void MainWindow::generateGraphicOfSequences()
{
    // crear archivo de salida
    QFile outputFile(resultsDirectory+"/generateGraphic.sh");
    if (outputFile.exists())
    {
        outputFile.remove();
    }
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QString msg = "No se pudo crear el archivo /tmp/.txt";
        Q_ASSERT_X(false, "MainWindow::generateGraphicOfSequences()", "no se pudo generar el archivo gnuplot");
        qDebug(qPrintable(msg));
        return;
    }
    QTextStream out(&outputFile);

    QString string = "";

    string = "#!/bin/bash\n";
    out << string ;

    string = "cd "+resultsDirectory+"\n";
    out << string ;

    string = "./gnuplotFile.gp\n";
    out << string ;

    return;
}



void MainWindow::reportCTableHistory()
{

    simulation->reportCTableHistory(resultsDirectory);

    QString program;
    QString home = QDir::currentPath();

    if (individualSize == 1)
    {
        // copiar el archivo processCtableLogSize1.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize1.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize1.sh";
        system(qPrintable(program));

    }else if (individualSize == 2)
    {
        // copiar el archivo processCtableLogSize2.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize2.sh";
        system(qPrintable(program));

    } else if (individualSize == 3)
    {
        // copiar el archivo processCtableLogSize3.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize3.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize3.sh";
        system(qPrintable(program));

    } else if (individualSize == 4)
    {
        // copiar el archivo processCtableLogSize4.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize4.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize4.sh";
        system(qPrintable(program));
    } else if (individualSize == 5)
    {
        // copiar el archivo processCtableLogSize5.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize5.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize5.sh";
        system(qPrintable(program));
    } else if (individualSize == 6)
    {
        // copiar el archivo processCtableLogSize6.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize6.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize6.sh";
        system(qPrintable(program));
    } else if (individualSize == 7)
    {
        // copiar el archivo processCtableLogSize7.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize7.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize7.sh";
        system(qPrintable(program));
    } else if (individualSize == 8)
    {
        // copiar el archivo processCtableLogSize8.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize8.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize8.sh";
        system(qPrintable(program));
    } else if (individualSize == 9)
    {
        // copiar el archivo processCtableLogSize9.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize9.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processCtableLogSize9.sh";
        system(qPrintable(program));
    } else if (individualSize == 10)
    {
        // copiar el archivo processCtableLogSize10.sh al directorio de resultados
        program = "cp "+ home + "/processCtableLogSize10.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "processCtableLogSize10.sh";
        system(qPrintable(program));
    }else if (individualSize == 11)
    {
        QString script= "/processCtableLogSize11.sh ";

        // copiar el archivo processCtableLogSize11.sh al directorio de resultados
        program = "cp "+ home + script + resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + script;
        system(qPrintable(program));
    }
    else
    {
        Q_ASSERT_X(false, "MainWindow::reportCTableHistory()", "el tamano del individuo debe ser entre 1 y 11");
    }
}


void MainWindow::generateMutatedResultsTable()
{
    // copiar el script que genera las tablas en el subdirectorio de resultados
    // cp processResultsWithoutArrowMutatedSize1.sh .
    // ejecutar el script

    QString program;
    QString home = QDir::currentPath();

    if (individualSize == 1)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize1.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize1-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize1-v2.sh";
        system(qPrintable(program));

    }else if (individualSize == 2)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize2.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize2-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize2-v2.sh";
        system(qPrintable(program));

    } else if (individualSize == 3)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize3.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize3-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize3-v2.sh";
        system(qPrintable(program));

    } else if (individualSize == 4)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize4.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize4-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize4-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 5)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize5.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize5-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize5-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 6)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize6.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize6-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize6-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 7)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize7.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize7-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize7-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 8)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize8.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize8-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize8-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 9)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize9.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize9-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize9-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 10)
    {
        // copiar el archivo processResultsWithoutArrowMutatedSize10.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowMutatedSize10-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowMutatedSize10-v2.sh";
        system(qPrintable(program));
    }else if (individualSize == 11)
    {
        QString script;
        if (useSmartIndividual)
        {
            script = "/processResultsWithoutArrowMutatedSize11-v4.sh ";
        }
        else
        {
            script = "/processResultsWithoutArrowMutatedSize11-v2.sh ";
        }

        // copiar el archivo processResultsWithoutArrowMutatedSize11.sh al directorio de resultados
        program = "cp "+ home + script + resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + script;
        system(qPrintable(program));
    }
    else
    {
        Q_ASSERT_X(false, "MainWindow::generateMutatedResultsTable()", "el tamano del individuo debe ser entre 1 y 11");
    }

}


void MainWindow::generateResultsTable()
{
    // copiar el script que genera las tablas en el subdirectorio de resultados
    // cp processResultsWithoutArrowSize1.sh .
    // ejecutar el script

    QString program;
    QString home = QDir::currentPath();

    if (individualSize == 1)
    {
        // copiar el archivo processResultsWithoutArrowSize1.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize1-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize1-v2.sh";
        system(qPrintable(program));

    }else if (individualSize == 2)
    {
        // copiar el archivo processResultsWithoutArrowSize2.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize2-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize2-v2.sh";
        system(qPrintable(program));

    } else if (individualSize == 3)
    {
        // copiar el archivo processResultsWithoutArrowSize3.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize3-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize3-v2.sh";
        system(qPrintable(program));

    } else if (individualSize == 4)
    {
        // copiar el archivo processResultsWithoutArrowSize4.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize4-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize4-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 5)
    {
        // copiar el archivo processResultsWithoutArrowSize5.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize5-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize5-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 6)
    {
        // copiar el archivo processResultsWithoutArrowSize6.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize6-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize6-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 7)
    {
        // copiar el archivo processResultsWithoutArrowSize7.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize7-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize7-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 8)
    {
        // copiar el archivo processResultsWithoutArrowSize8.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize8-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize8-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 9)
    {
        // copiar el archivo processResultsWithoutArrowSize9.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize9-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize9-v2.sh";
        system(qPrintable(program));
    } else if (individualSize == 10)
    {
        // copiar el archivo processResultsWithoutArrowSize10.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize10-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize10-v2.sh";
        system(qPrintable(program));
    }else if (individualSize == 11)
    {
        // copiar el archivo processResultsWithoutArrowSize11.sh al directorio de resultados
        program = "cp "+ home + "/processResultsWithoutArrowSize11-v2.sh "+ resultsDirectory ;
        system(qPrintable(program));

        program = "cd " + resultsDirectory + "; " + home + "/processResultsWithoutArrowSize11-v2.sh";
        system(qPrintable(program));
    }
    else
    {
        Q_ASSERT_X(false, "MainWindow::generateResultsTable()", "el tamano del individuo debe ser entre 1 y 11");
    }

}


double MainWindow::askTopology(int channel, double min, double max)
{
    double aps = 0;
    for (int i=1; i<30; i++)
    {
        aps = aps + topologyEmulator->getAPs(channel, min, max);
    }
    double avgAPs = aps/30;
    return avgAPs;
}

QString MainWindow::askTopologyPerformanceSmartIndividual()
{
    QString smartIndividual = smartIndividualSequence;

    QString result;

    Individual * ind;
    ind = new Individual(true, smartIndividual);
    ind->getAverageOnFullScanning();

    for (int i=0; i<individualSize; i++)
    {
        result = ind->getIndividualAsQString();
    }
   return result;
}

bool MainWindow::getUseOF1Index()
{
    return useOF1Index;
}

double MainWindow::getSmartIndividualLatency()
{
    return smartIndividualLatency;
}
