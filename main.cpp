#include <QCoreApplication>

#include <QSettings>
#include <QTime>

#include "mainwindow.h"
#include "individual.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainWindow mw;

    //qsrand((uint)QTime::currentTime().msec());

    if (mw.getDoComparationOfAlgorithms())
    {
        // ejecucion de la comparacion de los algoritmo original y modificado
        // el numero de ejecuciones de cada algoritmo se toma del archivo de
        // configuracion
        mw.compareAlgorithmRepeated();
    }
    else if (mw.getDoOriginalAlgorithm())
    {
        //mw.generateAPResultsOfOriginalMutation();
        //return 0;


        // se ejecuta el algoritmo cultural una vez de acuerdo a los parametros
        // establecidos en el archivo de configuracion
        // mw.executeAlgorithm();
        mw.executeAlgorithmRepeated();

        mw.generateAPResultsOfOriginalMutation();

        mw.generateResultsTable();
    }
    else // ejecutarMutacionDirigida
    {
        mw.executeModificatedAlgorithm();

        mw.generateAPResultsOfDirectedMutation();

        mw.reportCTableHistory();

        mw.generateMutatedResultsTable();
    }

    qDebug("return 0");
    return 0;

    //return a.exec();
}
