#include "ctablegen.h"

#include <QString>

#include "scan.h"
#include "mainwindow.h"

CTableGen::CTableGen(int ch, double min, double max, double ap, double indA, double indB, double indC)
{
    channel = ch;
    minChannelTime = min;
    maxChannelTime = max;
    aps = ap;
    indexA = indA;
    indexB = indB;
    indexC = indC;
}

CTableGen::CTableGen(const CTableGen &g)
{
    channel = g.getChannel();
    minChannelTime = g.getMinChannelTime();
    maxChannelTime = g.getMaxChannelTime();
    aps = g.getAPs();
    indexA = g.getIndexA();
    indexB = g.getIndexB();
    indexC = g.getIndexC();
}

CTableGen::~CTableGen()
{
}

double CTableGen::getChannel() const
{
    return channel;
}

void CTableGen::setChannel(int ch)
{
    channel = ch;
}

double CTableGen::getMinChannelTime() const
{
    return minChannelTime;
}

void CTableGen::setMinChannelTime(double min)
{
    minChannelTime = min;
}

double CTableGen::getMaxChannelTime() const
{
    return maxChannelTime;
}

void CTableGen::setMaxChannelTime(double max)
{
    maxChannelTime = max;
}

double CTableGen::getAPs() const
{
    return aps;
}

void CTableGen::setAPs(double ap)
{
    aps = ap;
}

double CTableGen::getIndexA() const
{
    return indexA;
}

void CTableGen::setIndexA(double indA)
{
    indexA = indA;
}

void CTableGen::calculateIndexA()
{
    /*
    double foa = (aps/minChannelTime)*0.20 + (aps/maxChannelTime)*0.80;
    indexA = foa;
    */

    double foa = 0;

    // obtener APmin/min + APmax/max

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();


    // primer termino APmin/min
    double minAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, 0) corresponde a los APs encontrados con minchanneltime
        minAPsum = minAPsum + scan.getAPs(channel, minChannelTime, 0);
    }
    double APmin = minAPsum/30;

    // segundo termino APmax/max
    double maxAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, max) corresponde a los APs encontrados con maxchanneltime
        maxAPsum = maxAPsum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    double APmax = maxAPsum/30;

    // si maxChannelTime es cero no se suman los aps encontrados con max
    if (maxChannelTime ==0)
    {
        foa = (APmin/minChannelTime)*0.2;
    }
    else
    {
        foa = (APmin/minChannelTime)*0.2 + (std::abs(APmax-APmin)/maxChannelTime)*0.8;
    }

    indexA = foa;
}

double CTableGen::getIndexB() const
{
    return indexB;
}

void CTableGen::setIndexB(double indB)
{
    indexB = indB;
}

void CTableGen::calculateIndexB()
{
    /*
    double fob = (aps/minChannelTime)*0.40 + (aps/maxChannelTime)*0.60;
    indexB = fob;
    */

    double fob = 0;

    // obtener APmin/min + APmax/max

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();


    // primer termino APmin/min
    double minAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, 0) corresponde a los APs encontrados con minchanneltime
        minAPsum = minAPsum + scan.getAPs(channel, minChannelTime, 0);
    }
    double APmin = minAPsum/30;

    // segundo termino APmax/max
    double maxAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, max) corresponde a los APs encontrados con maxchanneltime
        maxAPsum = maxAPsum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    double APmax = maxAPsum/30;

    // si maxChannelTime es cero no se suman los aps encontrados con max
    if (maxChannelTime ==0)
    {
        fob = (APmin/minChannelTime)*0.4;
    }
    else
    {
        fob = (APmin/minChannelTime)*0.4 + (std::abs(APmax-APmin)/maxChannelTime)*0.6;
    }

    indexB = fob;
}

double CTableGen::getIndexC() const
{
    return indexC;
}

void CTableGen::setIndexC(double indC)
{
    indexC = indC;
}

void CTableGen::calculateIndexC()
{
    /*
    double foc = (aps/minChannelTime)*0.60 + (aps/maxChannelTime)*0.40;
    indexC = foc;
    */

    double foc = 0;

    // obtener APmin/min + APmax/max

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();


    // primer termino APmin/min
    double minAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, 0) corresponde a los APs encontrados con minchanneltime
        minAPsum = minAPsum + scan.getAPs(channel, minChannelTime, 0);
    }
    double APmin = minAPsum/30;

    // segundo termino APmax/max
    double maxAPsum = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, max) corresponde a los APs encontrados con maxchanneltime
        maxAPsum = maxAPsum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    double APmax = maxAPsum/30;

    // si maxChannelTime es cero no se suman los aps encontrados con max
    if (maxChannelTime ==0)
    {
        foc = (APmin/minChannelTime)*0.6;
    }
    else
    {
        foc = (APmin/minChannelTime)*0.6 + (std::abs(APmax-APmin)/maxChannelTime)*0.4;
    }

    indexC = foc;
}


double CTableGen::getFONC()
{
    // verificar el parametro indiceParaOrdenarTablaC que especifica el indice a utilizar
    int index = MainWindow::getIndexToSortCTable();

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();

    // sumatorio de APs en min
    double minAPsum = 0;

    // APs promedio encontrados con min
    double APmin = 0;

    // sumatorio de APs en min
    double maxAPsum = 0;

    // APs promedio encontrados con min
    double APmax = 0;

    // valor del indice para el gen
    double fonc = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, 0) corresponde a los APs encontrados con minchanneltime
        minAPsum = minAPsum + scan.getAPs(channel, minChannelTime, 0);
    }
    APmin = minAPsum/30;

    for (int i=0; i<30; i++)
    {
        // (ch, min, max) corresponde a los APs encontrados con maxchanneltime
        maxAPsum = maxAPsum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    APmax = maxAPsum/30;

    if (index == 0)
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            fonc = (APmin/minChannelTime)*0.2 + (std::abs(APmax-APmin)/maxChannelTime)*0.8;
        }
    }
    else if (index == 1)
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            fonc = (APmin/minChannelTime)*0.4 + (std::abs(APmax-APmin)/maxChannelTime)*0.6;
        }
    }
    else // index == 2
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            //fonc = (APmin/minChannelTime)*0.6 + (std::abs(APmax-APmin)/maxChannelTime)*0.4;

            // para prueba de simulation200gFONC 0.7 y 0.3
            fonc = (APmin/minChannelTime)*0.7 + (std::abs(APmax-APmin)/maxChannelTime)*0.3;
        }
    }
    return fonc;
}

double CTableGen::calculateAPs()
{

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();

    // sumatorio de APs
    double apSum = 0;

    double apsAvg = 0;

    for (int i=0; i<30; i++)
    {
        apSum = apSum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    apsAvg = apSum/30;
    return apsAvg;
}
