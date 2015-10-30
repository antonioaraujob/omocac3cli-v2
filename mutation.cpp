#include "mutation.h"

#include "mainwindow.h"
//#include "scan.h"

//#include "QMessageBox"

#include <random>
#include <chrono>

/**
 * @brief Funcion de comparacion de celdas con respecto al valor de individuos en la lista
 * @param p1 Celda 1 a comparar
 * @param p2 Celda 2 a comparar
 * @return Verdadero si p1 es menor que p2 con respecto a la funcion objetivo de descubrimiento
 */
inline static bool cell1LessThanCell2(Cell * cell1, Cell * cell2)
{
    return cell1->getCount() < cell2->getCount();
}

/**
 * @brief Funcion de comparacion de individuos con respecto al valor de F1
 * @param p1 individuo 1 a comparar
 * @param p2 individuo 2 a comparar
 * @return Verdadero si p1 es menor que p2 con respecto a la funcion objetivo de descubrimiento
 */
inline static bool individual1LessThanIndividual2(Individual * ind1, Individual * ind2)
{
    return ind1->getPerformanceDiscovery() < ind2->getPerformanceDiscovery();
}


Mutation::Mutation()
{
    newPopulation.clear();

    stdDeviation = 0;

    // inicializar el diccionario de canales utilizados en la mutacion en falso
    for (int i=1; i<=11;i++)
    {
        channelsUsedForMutation[i]=false;
    }

    //scanningCampaing = new ScanningCampaing("database.db", "full");
    //scanningCampaing->init();
    //scanningCampaing->prepareIRD();


}


Mutation::~Mutation()
{

}


void Mutation::doMutation(QList<Individual *> population, double std, double stdMin, double stdMax)
{
    newPopulation.clear();

    Individual * father;
    Individual * offspring;

    qDebug(" ----- doMutation: tamano inicial de newPopulation %d", newPopulation.count());

    int newParameterValue = 0;

    // recorrer la lista de poblacion
    for (int i=0; i<population.count(); i++)
    {
        father = population.at(i);
        offspring = new Individual();
        //qDebug("===== offspring id: %d", offspring->getIndividualId());

        // crear un individuo (offspring) y mutar todos sus parametros
        for (int i=0; i<father->getNumberOfParameters(); i++)
        {
            newParameterValue = mutateIndividualParameter(i, 0 ,std, stdMin, stdMax,
                                                          father->getParameter(i), offspring);
            offspring->setParameter(i, newParameterValue);
        }

        offspring->setNewNscansForMutation();

        offspring->printIndividual();
        offspring->getAverageOnFullScanning();
        offspring->printIndividual();

        // se muto el offspring ahora limpiar el diccionario de canales usados
        // asignar el diccionario de canales utilizados en la mutacion en falso
        for (int c=1; c<=11;c++)
        {
            channelsUsedForMutation[c]=false;
        }

        // evaluar el offspring con los nuevos valores de parametros
        offspring->calculateDiscoveryValue();
        offspring->calculateLatencyValue();

        // agregar el individuo padre y el individuo hijo a la lista newPopulation
        // newPopulation sera de tamano 2p
        newPopulation.append(father);
        newPopulation.append(offspring);

        //qDebug(" ----- domutation: tamano de newPopulation %d", newPopulation.count());
    }
    qDebug(" ----- doMutation: tamano final de newPopulation %d", newPopulation.count());
}


/*
// prueba de ejecutar nscans 3 para mejorar la FO1
void Mutation::doMutation(QList<Individual *> population, double std, double stdMin, double stdMax, int deployedAp)
{
    newPopulation.clear();

    Individual * father;
    Individual * offspring;

    qDebug(" ----- doMutation: tamano inicial de newPopulation %d", newPopulation.count());

    int newParameterValue = 0;

    // recorrer la lista de poblacion
    for (int i=0; i<population.count(); i++)
    {
        father = population.at(i);
        offspring = new Individual();
        //qDebug("===== offspring id: %d", offspring->getIndividualId());

        // antes de mutar
        Individual * ind1 = scanSequence(offspring);
        Individual * ind2 = scanSequence(offspring);
        Individual * ind3 = scanSequence(offspring);

        QList<Individual *> list;
        list.append(ind1);
        list.append(ind2);
        list.append(ind3);

        qSort(list.begin(), list.end(), individual1LessThanIndividual2);
        Individual * indivitualToReturn = list.at(list.size()-1);

        Individual * newOffspring = new Individual();
        int channel=0;
        int min=0;
        int max=0;
        int aps=0;

        for (int j=0;j<offspring->getIndividualSize(); j++)
        {
            channel = indivitualToReturn->getParameter(j*4);
            min = indivitualToReturn->getParameter((j*4)+1);
            max = indivitualToReturn->getParameter((j*4)+2);
            aps = indivitualToReturn->getParameter((j*4)+3);

            newOffspring->setParameter((j*4),channel);
            newOffspring->setParameter(((j*4)+1),min);
            newOffspring->setParameter(((j*4)+2),max);
            newOffspring->setParameter(((j*4)+3),aps);
        }

        // crear un individuo (offspring) y mutar todos sus parametros
        for (int i=0; i<father->getNumberOfParameters(); i++)
        {
            newParameterValue = mutateIndividualParameter(i, 0 ,std, stdMin, stdMax,
                                                          father->getParameter(i), offspring);
            //offspring->setParameter(i, newParameterValue);
            newOffspring->setParameter(i, newParameterValue);
        }

        //offspring->setNewNscansForMutation();
        newOffspring->setNewNscansForMutation();

        // se muto el offspring ahora limpiar el diccionario de canales usados
        // asignar el diccionario de canales utilizados en la mutacion en falso
        for (int c=1; c<=11;c++)
        {
            channelsUsedForMutation[c]=false;
        }

        // evaluar el offspring con los nuevos valores de parametros
        //offspring->calculateDiscoveryValue();
        //offspring->calculateLatencyValue();

        newOffspring->calculateDiscoveryValue();
        newOffspring->calculateLatencyValue();

        // agregar el individuo padre y el individuo hijo a la lista newPopulation
        // newPopulation sera de tamano 2p
        newPopulation.append(father);
        //newPopulation.append(offspring);
        newPopulation.append(newOffspring);

        //qDebug(" ----- domutation: tamano de newPopulation %d", newPopulation.count());
    }
    qDebug(" ----- doMutation: tamano final de newPopulation %d", newPopulation.count());
}
*/

void Mutation::doDirectedMutation(QList<Individual *> population, double std, double stdMin, double stdMax,
                                  double dMutationProbability, NormativeGrid * grid)
{
    qDebug("Mutation::doDirectedMutation con probabilidad %f", dMutationProbability);

    newPopulation.clear();

    double randomNumber = 0;

    Individual * father;

    // recorrer la lista de poblacion
    for (int i=0; i<population.count(); i++)
    {
        father = population.at(i);
        randomNumber = getRandomUniform();

        qDebug("Numero aleatorio: %f", randomNumber);
        qDebug("dMutationProbability: %f", dMutationProbability);

        if (randomNumber < dMutationProbability)
        {
            qDebug("--> directedMutation()");

            setStdDeviation(std);

            // hacer la mutacion dirigida
            directedMutation(grid, father, stdMin, stdMax);
        }
        else
        {
            // hacer la mutacion gausiana con el papa como patron
            // escribir una funcion
            originalMutation(father, std, stdMin, stdMax);
        }

    }

}


void Mutation::doDirectedMutation(QList<Individual *> population, double std, double stdMin, double stdMax,
                                  double dMutationProbability, CTable * ctable)
{
    // ejecutar el proceso de mutacion de acuerdo al conocimiento de la tabla C del espacio de creencias

    qDebug("Mutation::doDirectedMutation con probabilidad %f", dMutationProbability);

    newPopulation.clear();

    double randomNumber = 0;

    Individual * father;

    // recorrer la lista de poblacion
    for (int i=0; i<population.count(); i++)
    {
        father = population.at(i);
        randomNumber = getRandomUniform();

        qDebug("Numero aleatorio: %f", randomNumber);
        qDebug("dMutationProbability: %f", dMutationProbability);

        if (randomNumber < dMutationProbability)
        {
            qDebug("--> directedMutation()");

            setStdDeviation(std);

            // hacer la mutacion dirigida
            //directedMutation(ctable, father);

            // descomentar la siguiente linea para ejecutar la mutacion dirigida con modificacion
            // de min y max de los genes que estan fuera de la ventana
            directedMutation(ctable, father, stdMin, stdMax);
        }
        else
        {
            // hacer la mutacion gausiana con el papa como patron
            originalMutation(father, std, stdMin, stdMax);
        }

    }

}


QList<Individual *> Mutation::getNewPopulation()
{
    return newPopulation;
}

int Mutation::getRandom(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

double Mutation::getRandomUniform()
{
    return qrand()/double(RAND_MAX);
}

/* original

int Mutation::mutateIndividualParameter(int index, int mean, double std, double stdMin, double stdMax,
                                        double currentParameterValue, Individual * offspring)
{
    // mean representa el parametro sobre el cual se va a mutar
    // std la desviacion estandar de la distribucion normal

    // tomado de http://www.cplusplus.com/reference/random/normal_distribution/

    std::default_random_engine generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    std::normal_distribution<double>  distribution(mean,std);

    std::normal_distribution<double>  minChannelTimeDistribution(mean, stdMin);

    std::normal_distribution<double>  maxChannelTimeDistribution(mean, stdMax);

    double yi = distribution(generator);

    //qDebug("--Mutar parametro de individuo--");
    //qDebug("   valor de la distribucion normal: %d, %d", mean, std);
    //qDebug(qPrintable(QString::number(yi)));

    // redondear el yi
    int intYi = qRound(yi);

    //if (isThisParameterAChannel(index))
    if (isThisParameterAChannel(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAChannel(index)");
        intYi = getRandom(1,11);

        // verificar que el canal no se haya utilizado en mutaciones anteriores
        while (channelsUsedForMutation.value(intYi))
        {
            // seleccionar otro canal que no se haya seleccionado
            intYi = getRandom(1,11);
        }
        channelsUsedForMutation[intYi]=true;
        //qDebug(qPrintable("   channel despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAMinChannelTime(index))
    else if (isThisParameterAMinChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMinChannelTime(index)");
        int randomValue = qRound(minChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi <= 5)
        {
            //qDebug("   el minChannelTime mutado esta por debajo del limite (index %d)", index);
            while(intYi <=5)
            {
                yi = minChannelTimeDistribution(generator);
                intYi = qRound(yi);
            }
        }
        else if (intYi > 15)
        {
            intYi = 15;
            //qDebug("   el minChannelTime mutado esta por encima del limite (index %d)", index);
        }

        //qDebug(qPrintable("   minChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAMaxChannelTime(index))
    else if (isThisParameterAMaxChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMaxChannelTime(index)");
        int randomValue = qRound(maxChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi < 10)
        {
            intYi = 10;
            //qDebug("   el maxChannelTime mutado esta por debajo del limite (index %d)", index);
        }
        else if (intYi > 90)
        {
            intYi = 90;
            //qDebug("   el maxChannelTime mutado esta por encima del limite (index %d)", index);
        }

        //qDebug(qPrintable("   maxChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAPs(index))
    //else if (isThisParameterAPs(index, offspring->getIndividualSize()))
    else
    {
        //qDebug("   isThisParameterAPs(index)");
        //if (intYi<0)
        //{
        //    intYi = 0;
        //}

        intYi = getNewParameterAPs(offspring->getParameter(index-3),
                                   offspring->getParameter(index-2),
                                   offspring->getParameter(index-1),
                                   offspring->getNscanForMutation());

        if (intYi < 0)
        {
            qDebug(qPrintable("*********   APs despues de mutado NEGATIVO: "+QString::number(intYi)));
        }

        qDebug(qPrintable("   APs despues de mutado: "+QString::number(intYi)));
    }

    qDebug(qPrintable("   ----intYi justo antes de return: "+QString::number(intYi)));
    return intYi;
}
*/

double Mutation::mutateIndividualParameter(int index, int mean, double std, double stdMin, double stdMax,
                                        double currentParameterValue, Individual * offspring)
{
    // mean representa el parametro sobre el cual se va a mutar
    // std la desviacion estandar de la distribucion normal

    // tomado de http://www.cplusplus.com/reference/random/normal_distribution/

    std::default_random_engine generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    std::normal_distribution<double>  distribution(mean,std);

    std::normal_distribution<double>  minChannelTimeDistribution(mean, stdMin);

    std::normal_distribution<double>  maxChannelTimeDistribution(mean, stdMax);

    double yi = distribution(generator);

    //qDebug("--Mutar parametro de individuo--");
    //qDebug("   valor de la distribucion normal: %d, %d", mean, std);
    //qDebug(qPrintable(QString::number(yi)));

    // redondear el yi
    double intYi = qRound(yi);

    //if (isThisParameterAChannel(index))
    if (isThisParameterAChannel(index, offspring->getIndividualSize()))
    {

        //qDebug("   isThisParameterAChannel(index)");
        intYi = getRandom(1,11);

        // verificar que el canal no se haya utilizado en mutaciones anteriores
        while (channelsUsedForMutation.value(intYi))
        {
            // seleccionar otro canal que no se haya seleccionado
            intYi = getRandom(1,11);
        }
        channelsUsedForMutation[intYi]=true;
        //qDebug(qPrintable("   channel despues de mutado: "+QString::number(intYi)));


        // descomentar para no mutar el canal de forma aleatoria
        //intYi = currentParameterValue;

    }
    //else if (isThisParameterAMinChannelTime(index))
    else if (isThisParameterAMinChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMinChannelTime(index)");
        int randomValue = qRound(minChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi <= MainWindow::getLowerMinChannelTime())
        {
            //qDebug("   el minChannelTime mutado esta por debajo del limite (index %d)", index);
            while(intYi <= MainWindow::getLowerMinChannelTime())
            {
                yi = minChannelTimeDistribution(generator);
                intYi = qRound(yi);
            }
        }
        if (intYi > MainWindow::getUpperMinChannelTime())
        {
            intYi = MainWindow::getUpperMinChannelTime();
            //qDebug("   el minChannelTime mutado esta por encima del limite (index %d)", index);
        }
        Q_ASSERT_X( ((MainWindow::getLowerMinChannelTime()<=intYi) && (intYi<=MainWindow::getUpperMinChannelTime())),
                    "mutateIndividualParameter()", qPrintable(QString::number(intYi)));
        //qDebug(qPrintable("   minChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAMaxChannelTime(index))
    else if (isThisParameterAMaxChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMaxChannelTime(index)");
        int randomValue = qRound(maxChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi < MainWindow::getLowerMaxChannelTime())
        {
            intYi = MainWindow::getLowerMaxChannelTime();
            //qDebug("   el maxChannelTime mutado esta por debajo del limite (index %d)", index);
        }
        if (intYi > MainWindow::getUpperMaxChannelTime())
        {
            intYi = MainWindow::getUpperMaxChannelTime();
            //qDebug("   el maxChannelTime mutado esta por encima del limite (index %d)", index);
        }
        Q_ASSERT_X( ((MainWindow::getLowerMaxChannelTime()<=intYi) && (intYi<=MainWindow::getUpperMaxChannelTime())),
                    "mutateIndividualParameter()", qPrintable(QString::number(intYi)));
        //qDebug(qPrintable("   maxChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAPs(index))
    //else if (isThisParameterAPs(index, offspring->getIndividualSize()))
    else
    {
        //qDebug("   isThisParameterAPs(index)");
        //if (intYi<0)
        //{
        //    intYi = 0;
        //}

        intYi = getNewParameterAPs(offspring->getParameter(index-3),
                                   offspring->getParameter(index-2),
                                   offspring->getParameter(index-1),
                                   offspring->getNscanForMutation());

        Q_ASSERT_X( (intYi>= 0), "mutateIndividualParameter()", qPrintable(QString::number(intYi)) );

        if (intYi < 0)
        {
            qDebug(qPrintable("*********   APs despues de mutado NEGATIVO: "+QString::number(intYi)));
        }

        qDebug(qPrintable("   APs despues de mutado: "+QString::number(intYi)));
    }

    qDebug(qPrintable("   ----intYi justo antes de return: "+QString::number(intYi)));
    return intYi;
}

double Mutation::mutateIndividualParameterModified(int index, int mean, double std, double stdMin, double stdMax,
                                        double currentParameterValue, Individual * offspring, QList<int> busyChannels)
{
    // mean representa el parametro sobre el cual se va a mutar
    // std la desviacion estandar de la distribucion normal

    // tomado de http://www.cplusplus.com/reference/random/normal_distribution/

    std::default_random_engine generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    std::normal_distribution<double>  distribution(mean,std);

    std::normal_distribution<double>  minChannelTimeDistribution(mean, stdMin);

    std::normal_distribution<double>  maxChannelTimeDistribution(mean, stdMax);

    double yi = distribution(generator);

    //qDebug("--Mutar parametro de individuo--");
    //qDebug("   valor de la distribucion normal: %d, %d", mean, std);
    //qDebug(qPrintable(QString::number(yi)));

    // redondear el yi
    double intYi = qRound(yi);

    //if (isThisParameterAChannel(index))
    if (isThisParameterAChannel(index, offspring->getIndividualSize()))
    {

        // ocupar los canales de la ventana
        for (int i=0; i< busyChannels.size(); i++)
        {
            channelsUsedForMutation[busyChannels.at(i)]=true;
        }


        //qDebug("   isThisParameterAChannel(index)");
        intYi = getRandom(1,11);

        // verificar que el canal no se haya utilizado en mutaciones anteriores
        while (channelsUsedForMutation.value(intYi))
        {
            // seleccionar otro canal que no se haya seleccionado
            intYi = getRandom(1,11);
        }
        channelsUsedForMutation[intYi]=true;
        //qDebug(qPrintable("   channel despues de mutado: "+QString::number(intYi)));


        // descomentar para no mutar el canal de forma aleatoria
        //intYi = currentParameterValue;

    }
    //else if (isThisParameterAMinChannelTime(index))
    else if (isThisParameterAMinChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMinChannelTime(index)");
        int randomValue = qRound(minChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi <= MainWindow::getLowerMinChannelTime())
        {
            //qDebug("   el minChannelTime mutado esta por debajo del limite (index %d)", index);
            while(intYi <= MainWindow::getLowerMinChannelTime())
            {
                yi = minChannelTimeDistribution(generator);
                intYi = qRound(yi);
            }
        }
        if (intYi > MainWindow::getUpperMinChannelTime())
        {
            intYi = MainWindow::getUpperMinChannelTime();
            //qDebug("   el minChannelTime mutado esta por encima del limite (index %d)", index);
        }
        Q_ASSERT_X( ((MainWindow::getLowerMinChannelTime()<=intYi) && (intYi<=MainWindow::getUpperMinChannelTime())),
                    "mutateIndividualParameter()", qPrintable(QString::number(intYi)));
        //qDebug(qPrintable("   minChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAMaxChannelTime(index))
    else if (isThisParameterAMaxChannelTime(index, offspring->getIndividualSize()))
    {
        //qDebug("   isThisParameterAMaxChannelTime(index)");
        int randomValue = qRound(maxChannelTimeDistribution(generator));
        intYi = randomValue + currentParameterValue;
        if (intYi < MainWindow::getLowerMaxChannelTime())
        {
            intYi = MainWindow::getLowerMaxChannelTime();
            //qDebug("   el maxChannelTime mutado esta por debajo del limite (index %d)", index);
        }
        if (intYi > MainWindow::getUpperMaxChannelTime())
        {
            intYi = MainWindow::getUpperMaxChannelTime();
            //qDebug("   el maxChannelTime mutado esta por encima del limite (index %d)", index);
        }
        Q_ASSERT_X( ((MainWindow::getLowerMaxChannelTime()<=intYi) && (intYi<=MainWindow::getUpperMaxChannelTime())),
                    "mutateIndividualParameter()", qPrintable(QString::number(intYi)));
        //qDebug(qPrintable("   maxChannelTime despues de mutado: "+QString::number(intYi)));
    }
    //else if (isThisParameterAPs(index))
    //else if (isThisParameterAPs(index, offspring->getIndividualSize()))
    else
    {
        //qDebug("   isThisParameterAPs(index)");
        //if (intYi<0)
        //{
        //    intYi = 0;
        //}

        intYi = getNewParameterAPs(offspring->getParameter(index-3),
                                   offspring->getParameter(index-2),
                                   offspring->getParameter(index-1),
                                   offspring->getNscanForMutation());

        Q_ASSERT_X( (intYi>= 0), "mutateIndividualParameter()", qPrintable(QString::number(intYi)) );

        if (intYi < 0)
        {
            qDebug(qPrintable("*********   APs despues de mutado NEGATIVO: "+QString::number(intYi)));
        }

        qDebug(qPrintable("   APs despues de mutado: "+QString::number(intYi)));
    }

    qDebug(qPrintable("   ----intYi justo antes de return: "+QString::number(intYi)));
    return intYi;
}


bool Mutation::isThisParameterAChannel(int index)
{
    if ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) ||
         (index == 20) || (index == 24) || (index == 28) || (index == 32) || (index == 36) || (index == 40) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Mutation::isThisParameterAChannel(int index, int individualSize)
{
    bool isChannel = false;

    if ( (individualSize == 1) && (index == 0) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 2) && ( (index == 0) || (index == 4) ) )
    {
            isChannel = true;
    }
    else if ( (individualSize == 3) && ( (index == 0) || (index == 4) || (index == 8) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 4) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 5) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 6) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 7) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20)
                                         || (index == 24) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 8) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20)
                                         || (index == 24) || (index == 28) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 9) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20)
                                         || (index == 24) || (index == 28) || (index == 32) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 10) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20)
                                          || (index == 24) || (index == 28) || (index == 32) || (index == 36) ) )
    {
        isChannel = true;
    }
    else if ( (individualSize == 11) && ( (index == 0) || (index == 4) || (index == 8) || (index == 12) || (index == 16) || (index == 20)
                                          || (index == 24) || (index == 28) || (index == 32) || (index == 36) || (index == 40) ) )
    {
        isChannel = true;
    }
    return isChannel;
}

bool Mutation::isThisParameterAMinChannelTime(int index)
{
    if ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) ||
         (index == 21) || (index == 25) || (index == 29) || (index == 33) || (index == 37) || (index == 41) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
bool Mutation::isThisParameterAMinChannelTime(int index, int individualSize)
{
    bool isMin = false;

    if ( (individualSize == 1) && (index == 1) )
    {
        isMin = true;
    }
    else if ( (individualSize == 2) && ( (index == 1) || (index == 5) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 3) && ( (index == 1) || (index == 5) || (index == 9) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 4) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 5) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 6) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 7) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 8) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) || (index == 29) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 9) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) || (index == 29) || (index == 33) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 10) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                          || (index == 25) || (index == 29) || (index == 33) || (index == 37) ) )
    {
        isMin = true;
    }
    else if ( (individualSize == 11) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                          || (index == 25) || (index == 29) || (index == 33) || (index == 37) || (index == 41) ) )
    {
        isMin = true;
    }
    return isMin;
}
*/

bool Mutation::isThisParameterAMinChannelTime(int index, int individualSize)
{
    bool isMin = false;

    if ( (individualSize == 1) && (index == 1) )
    {
        return  true;
    }
    else if ( (individualSize == 2) && ( (index == 1) || (index == 5) ) )
    {
        return true;
    }
    else if ( (individualSize == 3) && ( (index == 1) || (index == 5) || (index == 9) ) )
    {
        return true;
    }
    else if ( (individualSize == 4) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) ) )
    {
        return true;
    }
    else if ( (individualSize == 5) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) ) )
    {
        return true;
    }
    else if ( (individualSize == 6) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21) ) )
    {
        return true;
    }
    else if ( (individualSize == 7) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) ) )
    {
        return true;
    }
    else if ( (individualSize == 8) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) || (index == 29) ) )
    {
        return true;
    }
    else if ( (individualSize == 9) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                         || (index == 25) || (index == 29) || (index == 33) ) )
    {
        return true;
    }
    else if ( (individualSize == 10) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                          || (index == 25) || (index == 29) || (index == 33) || (index == 37) ) )
    {
        return true;
    }
    else if ( (individualSize == 11) && ( (index == 1) || (index == 5) || (index == 9) || (index == 13) || (index == 17) || (index == 21)
                                          || (index == 25) || (index == 29) || (index == 33) || (index == 37) || (index == 41) ) )
    {
        return true;
    }
    else
    {
        return isMin;
        /*
        //Q_ASSERT();
        QString error("indice: ");
        error.append(QString::number(index));
        error.append(" tamano: ");
        error.append(QString::number(individualSize));

        Q_ASSERT_X(true, "isThisParameterAMinChannelTime(int index, int individualSize)", qPrintable(error));
        */
    }

}


bool Mutation::isThisParameterAMaxChannelTime(int index)
{
    if ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) ||
         (index == 22) || (index == 26) || (index == 30) || (index == 34) || (index == 38) || (index == 42) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
bool Mutation::isThisParameterAMaxChannelTime(int index, int individualSize)
{
    bool isMax = false;

    if ( (individualSize == 1) && (index == 2) )
    {
        isMax = true;
    }
    else if ( (individualSize == 2) && ( (index == 2) || (index == 6) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 3) && ( (index == 2) || (index == 6) || (index == 10) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 4) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 5) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 6) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 7) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 8) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) || (index == 30) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 9) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) || (index == 30) || (index == 34) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 10) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                          || (index == 26) || (index == 30) || (index == 34) || (index == 38) ) )
    {
        isMax = true;
    }
    else if ( (individualSize == 11) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                          || (index == 26) || (index == 30) || (index == 34) || (index == 38) || (index == 42) ) )
    {
        isMax = true;
    }
    return isMax;
}
*/

bool Mutation::isThisParameterAMaxChannelTime(int index, int individualSize)
{
    bool isMax = false;

    if ( (individualSize == 1) && (index == 2) )
    {
        return true;
    }
    else if ( (individualSize == 2) && ( (index == 2) || (index == 6) ) )
    {
        return true;
    }
    else if ( (individualSize == 3) && ( (index == 2) || (index == 6) || (index == 10) ) )
    {
        return true;
    }
    else if ( (individualSize == 4) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) ) )
    {
        return true;
    }
    else if ( (individualSize == 5) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) ) )
    {
        return true;
    }
    else if ( (individualSize == 6) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22) ) )
    {
        return true;
    }
    else if ( (individualSize == 7) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) ) )
    {
        return true;
    }
    else if ( (individualSize == 8) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) || (index == 30) ) )
    {
        return true;
    }
    else if ( (individualSize == 9) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                         || (index == 26) || (index == 30) || (index == 34) ) )
    {
        return true;
    }
    else if ( (individualSize == 10) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                          || (index == 26) || (index == 30) || (index == 34) || (index == 38) ) )
    {
        return true;
    }
    else if ( (individualSize == 11) && ( (index == 2) || (index == 6) || (index == 10) || (index == 14) || (index == 18) || (index == 22)
                                          || (index == 26) || (index == 30) || (index == 34) || (index == 38) || (index == 42) ) )
    {
        return true;
    }
    else
    {
        return isMax;
    }
    //return isMax;
}


bool Mutation::isThisParameterAPs(int index)
{
    if ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) ||
         (index == 23) || (index == 27) || (index == 31) || (index == 35) || (index == 39) || (index == 43) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
bool Mutation::isThisParameterAPs(int index, int individualSize)
{
    bool isAp = false;

    if ( (individualSize == 1) && (index == 3) )
    {
        isAp = true;
    }
    else if ( (individualSize == 2) && ( (index == 3) || (index == 7) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 3) && ( (index == 3) || (index == 7) || (index == 11) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 4) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 5) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 6) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 7) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 8) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) || (index == 31) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 9) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) || (index == 31) || (index == 35) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 10) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                          || (index == 27) || (index == 31) || (index == 35) || (index == 39) ) )
    {
        isAp = true;
    }
    else if ( (individualSize == 11) && ( (index == 3) || (index == 6) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                          || (index == 27) || (index == 31) || (index == 35) || (index == 39) || (index == 43) ) )
    {
        isAp = true;
    }
    return isAp;
}
*/

bool Mutation::isThisParameterAPs(int index, int individualSize)
{
    bool isAp = false;

    if ( (individualSize == 1) && (index == 3) )
    {
        return true;
    }
    else if ( (individualSize == 2) && ( (index == 3) || (index == 7) ) )
    {
        return true;
    }
    else if ( (individualSize == 3) && ( (index == 3) || (index == 7) || (index == 11) ) )
    {
        return true;
    }
    else if ( (individualSize == 4) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) ) )
    {
        return true;
    }
    else if ( (individualSize == 5) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) ) )
    {
        return true;
    }
    else if ( (individualSize == 6) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23) ) )
    {
        return true;
    }
    else if ( (individualSize == 7) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) ) )
    {
        return true;
    }
    else if ( (individualSize == 8) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) || (index == 31) ) )
    {
        return true;
    }
    else if ( (individualSize == 9) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                         || (index == 27) || (index == 31) || (index == 35) ) )
    {
        return true;
    }
    else if ( (individualSize == 10) && ( (index == 3) || (index == 7) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                          || (index == 27) || (index == 31) || (index == 35) || (index == 39) ) )
    {
        return true;
    }
    else if ( (individualSize == 11) && ( (index == 3) || (index == 6) || (index == 11) || (index == 15) || (index == 19) || (index == 23)
                                          || (index == 27) || (index == 31) || (index == 35) || (index == 39) || (index == 43) ) )
    {
        return true;
    }
    else
    {
        return isAp;
    }
    //return isAp;
}

void Mutation::printNewPopulation()
{
    for (int i = 0; i < newPopulation.count(); i++)
    {
        newPopulation.at(i)->printIndividual();
    }
}


double Mutation::getNewParameterAPs(int channel, double minChannelTime, double maxChannelTime, int nscans)
{
    //qDebug("Mutation::getNewParameterAPs(%d, %f, %f)", channel, minChannelTime, maxChannelTime);

    // base de datos de experimentos
    QString database("test_18.1.db");
    //QString database("database.db");


    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();

    // obtener el nuevo valor de AP de acuerdo al enfoque del mayor valor encontrado
    // en la repeticion de nscans veces
    double newparameterAPs = getAPsFromMaxNumberApproach(channel, minChannelTime, maxChannelTime, nscans);
    return newparameterAPs*1.0;

    //return scan.getAP(channel, minChannelTime, maxChannelTime);


/*
    //Scan::ScanResults results;
    ScanningCampaing::ScanResults results;

    //results = scan.execute(channel, minChannelTime, maxChannelTime);

    results = scan.randomScan(channel, minChannelTime, maxChannelTime);

    qDebug("** nuevo parametro AP: %d", results.size());
    return (int) results.size();
*/


    //return MainWindow::getAPs(channel, minChannelTime, maxChannelTime);
}


void Mutation::originalMutation(Individual * father, double std, double stdMin, double stdMax)
{
    qDebug("Mutation::originalMutation(Individual * father)");

    //QMessageBox msg;
    //msg.setText("*** MUTACION ORIGINAL***");
    //msg.exec();

    //father = population.at(i);

    Individual * offspring;
    offspring = new Individual();
    //qDebug("===== offspring id: %d", offspring->getIndividualId());

    double newParameterValue = 0;
    // crear un individuo (offspring) y mutar todos sus parametros
    for (int i=0; i<father->getNumberOfParameters(); i++)
    {
        newParameterValue = mutateIndividualParameter(i, 0 /*father->getParameter(i)*/,std, stdMin, stdMax,
                                                      father->getParameter(i), offspring);
        offspring->setParameter(i, newParameterValue);
    }

    offspring->getAverageOnFullScanning();

    // se muto el offspring ahora limpiar el diccionario de canales usados
    // asignar el diccionario de canales utilizados en la mutacion en falso
    for (int c=1; c<=11;c++)
    {
        channelsUsedForMutation[c]=false;
    }

    // evaluar el offspring con los nuevos valores de parametros
    offspring->calculateDiscoveryValue();
    offspring->calculateLatencyValue();

    // agregar el individuo padre y el individuo hijo a la lista newPopulation
    // newPopulation sera de tamano 2p
    newPopulation.append(father);
    newPopulation.append(offspring);
}


void Mutation::directedMutation(NormativeGrid *grid, Individual *father, double stdMin, double stdMax)
{
    //QMessageBox msg;
    //msg.setText("*** MUTACION DIRIGIDA***");
    //msg.exec();
    //return;


    //Identificar las celdas de la rejilla que tienen contadores mayores a cero.
    // Agregar las celdas en una lista de Cells.
    // lista de celdas pobladas
    QList<Cell *> cellList = grid->getPopulatedCellList();
    qDebug("   numero de celdas con individuos: %d", cellList.count());

    //Ordenar la lista de menor a mayor cantidad de individuos por celda.
    qSort(cellList.begin(), cellList.end(), cell1LessThanCell2);


    for (int i=0; i<cellList.count();i++)
    {
        qDebug("Celda (%d-%d) con: %d individuos", cellList.at(i)->getSubintervalF1(),
        cellList.at(i)->getSubintervalF2(), cellList.at(i)->getCount());
    }

    // no hay como ejercer influencia en la mutacion
    if (cellList.count() == 0)
    {
        originalMutation(father, getStdDeviation(), stdMin, stdMax);
        return;
    }
    else // existe al menos una celda con individuo en la rejilla
    {
        int randomCell = getRandom(0,cellList.count()-1);
        Cell * selectedCell = cellList.at(randomCell);

        // la celda tiene solo un individuo
        if (selectedCell->getCount() == 1)
        {
            qDebug("   la celda tiene un solo individuo");
            originalMutation(father, getStdDeviation(), stdMin, stdMax);
            return;
        }
        else // la celda tiene dos o mas individuos
        {
            qDebug("   la celda tiene dos o mas individuos");
            for (int i=0; i<selectedCell->getCount();i++)
            {
                selectedCell->getIndividual(i)->printIndividual();
            }
            qDebug("----------------------------------------------------------------");

            // TODO
            //QMessageBox msg;
            //msg.setText("*** MUTACION DIRIGIDA***\nLa celda tiene dos o mas individuos!!!\TODO!");
            //msg.exec();



            // ----------------------------------------------------------------
            // primera etapa de la mutacion dirigida

            // TODO: establecer el umbral como un parametro
            double stdDevThreshold = 0.1;

            int numberOfIndividuals = selectedCell->getCount();

            Individual * preOffspring = new Individual();

            qDebug("-----preoffspring------");
            preOffspring->printIndividual();
            qDebug("-----------------------");

            QList<double> minList;
            QList<double> maxList;

            QHash<QString,double> meanAndStdDevHash;

            // verlo primero por bloque y luego por individuo...
            // iterar por bloque
            // la cantidad de bloques viene dada por el tamano del individuo

            //for (int i=0; i<11;i++)
            for (int i=0; i<father->getIndividualSize(); i++)
            {
                // iterar por individuo
                for (int j=0; j<numberOfIndividuals;j++)
                {

                    //selectedCell->getIndividual(j)->printIndividual();
                    // obtener los min y max y agregarlos en las listas respectivas
                    minList.append(selectedCell->getIndividual(j)->getParameter((i*4)+1));
                    maxList.append(selectedCell->getIndividual(j)->getParameter((i*4)+2));
                }
                meanAndStdDevHash = calculateMeanAndStdDev(minList, maxList,0);
                minList.clear();
                maxList.clear();

                // verificar si la desviacion del min es menor que umbral
                if (meanAndStdDevHash.value("stdDevMinChannelTime") < stdDevThreshold)
                {
                    preOffspring->setParameter(((i*4)+1),meanAndStdDevHash.value("meanMinChannelTime"));
                }
                else
                {
                    // asignar el valor mutado del parametro del padre
                    preOffspring->setParameter(((i*4)+1),father->getParameter((i*4)+1));

                }
                // verificar si la desviacion del max es menor que umbral
                if (meanAndStdDevHash.value("stdDevMaxChannelTime") < stdDevThreshold)
                {
                    preOffspring->setParameter(((i*4)+2),meanAndStdDevHash.value("meanMaxChannelTime"));
                }
                else
                {
                    // asignar el valor mutado del parametro del padre
                    preOffspring->setParameter(((i*4)+2),father->getParameter((i*4)+2));
                }
                meanAndStdDevHash.clear();
            }
            qDebug("--MIN y MAX MUTADOS---");
            preOffspring->printIndividual();
            qDebug("-----");

            // ----------------------------------------------------------------
            // ----------------------------------------------------------------
            // segunda etapa de la mutacion dirigida
            // identificar la secuencia mas comun de escaneo


            QList <int> channelList;

            QList<int> patternSequence;
            int newChannel = 0;

            // verlo primero por bloque y luego por individuo...
            // iterar por bloque
            // la cantidad de bloques viene dada por el tamano del individuo

            //for (int i=0; i<11;i++)
            for (int i=0; i<father->getIndividualSize(); i++)
            {
                // iterar por individuo
                for (int j=0; j<numberOfIndividuals;j++)
                {
                    // obtener el canal de cada individuo
                    //selectedCell->getIndividual(j)->printIndividual();
                    channelList.append(selectedCell->getIndividual(j)->getParameter((i*4)));
                }
                // operar sobre channelList
                newChannel = getPatternSequence(channelList);
                //qDebug("canal %d del patron: %d",i+1, newChannel);
                channelList.clear();
                patternSequence.append(newChannel);
            }

            qDebug("PATRON DE SECUENCIA: --------------------");
            QString seq;
            for (int i=0;i<patternSequence.count();i++)
            {
                seq.append(QString::number(patternSequence.at(i)));
                seq.append("-");
            }
            qDebug(qPrintable(seq));
            qDebug("------------------------------------------");

            QList<int> finalSequence;

            QHash<int,bool> usedChannels;
            for (int i=1;i<=11;i++)
            {
                usedChannels.insert(i,false);
            }


            for (int i=0; i<patternSequence.count();i++)
            {
                if (usedChannels.value(patternSequence.at(i)))
                {
                    finalSequence.append(0);
                }
                else
                {
                    usedChannels.insert(patternSequence.at(i),true);
                    finalSequence.append(patternSequence.at(i));
                }

            }
            qDebug("Secuencia final con ceros:");
            QString seq2;
            for (int i=0;i<finalSequence.count();i++)
            {
                seq2.append(QString::number(finalSequence.at(i)));
                seq2.append("-");
            }
            qDebug(qPrintable(seq2));
            qDebug("------------------------------------------");

            for (int i=0;i<finalSequence.count();i++)
            {
                if (finalSequence.at(i) == 0)
                {
                    for (int j=0;j<usedChannels.count();j++)
                    {
                        if (usedChannels.value(j+1) == false)
                        {
                            finalSequence.replace(i,j+1);
                            usedChannels.insert(j+1,true);
                            break;
                        }
                    }
                }
            }
            qDebug("Secuencia final despues de quitar repetidos:");
            QString seq3;
            for (int i=0;i<finalSequence.count();i++)
            {
                seq3.append(QString::number(finalSequence.at(i)));
                seq3.append("-");
            }
            qDebug(qPrintable(seq3));
            qDebug("------------------------------------------");


            //for (int i=0; i<11; i++)
            for (int i=0; i<father->getIndividualSize(); i++)
            {
                preOffspring->setParameter(i*4,finalSequence.at(i));
            }
            qDebug("*******************OFFSPRING*******************************");
            preOffspring->printIndividual();
            qDebug("***********************************************************");

            // ----------------------------------------------------------------

            // TODO: agregar el offspring a newPopulation
            // agregar el individuo padre y el individuo hijo a la lista newPopulation
            // newPopulation sera de tamano 2p
            newPopulation.append(father);
            newPopulation.append(preOffspring);
        }


    }



}


void Mutation::directedMutation(CTable * ct, Individual * father)
{
    QList<CTableGen *> windowGenesList = ct->getWindowGenes();
    Individual * offspring = new Individual(*father);

    QList<CTableGen*> convertedIndividual = ct->convertIndividualToCTableGen(offspring);

    CTableGen * gen;
    CTableGen * tmpGen;

    int index = 0;

    for (int i=0; i< windowGenesList.size(); i++)
    {
        gen = windowGenesList.at(i);

        // gen temporal del hijo
        tmpGen = convertedIndividual.at(i);

        // indice del canal en el cual se debe colocar el nuevo canal
        index = ct->searchChannelInList(gen->getChannel(), convertedIndividual);

        // solo reemplazar
        if (index == -1)
        {
            convertedIndividual.replace(i, gen);
        }else{ // permutar
            convertedIndividual.replace(i,gen);
            convertedIndividual.replace(index, tmpGen);
        }
        index = 0;
    }

    // en este punto ya convertedIndividual esta completamente mutado, ahora devolver el cambio
    for (int i=0; i<offspring->getIndividualSize(); i++)
    {
        offspring->setParameter((i*4), convertedIndividual.at(i)->getChannel());
        offspring->setParameter(((i*4)+1), convertedIndividual.at(i)->getMinChannelTime());
        offspring->setParameter(((i*4)+2), convertedIndividual.at(i)->getMaxChannelTime());
        offspring->setParameter(((i*4)+3), convertedIndividual.at(i)->getAPs());
    }
    qDebug("revisa");

    // *******************************************************
/*
    double min = 0;
    double max = 0;

    // prueba de aumentar temporizador a los canales de w
    for (int i = 0; i < windowGenesList.size(); i++)
    {
        min = offspring->getParameter((i*4)+1);
        min = min + 1;
        if (min > 15)
        {
            min = 15;
        }
        max = offspring->getParameter((i*4)+2);
        max = max + 5;
        if (max > 90 )
        {
            max = 90;
        }

        offspring->setParameter(((i*4)+1), min);
        offspring->setParameter(((i*4)+2), max);

    }


    // prueba de disminuir los temporizadores a los canales 11-w
    // restar 5 a min y restar 10 a max
    for (int i = windowGenesList.size(); i < offspring->getIndividualSize() ; i++)
    {
        min = offspring->getParameter((i*4)+1) - 5 ;
        if (min <= 0)
        {
            min = 3;
        }
        max = offspring->getParameter((i*4)+2) - 10;
        if (max <= 0)
        {
            max = 3;
        }
        offspring->setParameter(((i*4)+1), min);
        offspring->setParameter(((i*4)+2), max);
    }
*/
    // *******************************************************

    // *******************************************************
/*
    // Definir una latencia máxima L
    // obtener l_w = L/4
    // Para cada gen de W
    //     asignar temporizadores al canal que sumen l_w

    // obtener l_t-w = l_w/8
    // Para cada gen de TamañoIndividuo-W
    //    asignar temporizadores al canal que sumen l_t-w

    double min = 0;
    double max = 0;

    double L = 601;
    double lw = 150;//L/11;
    for (int i = 0; i < windowGenesList.size(); i++)
    {
        min = 15;
        max = 135;

        offspring->setParameter(((i*4)+1), min);
        offspring->setParameter(((i*4)+2), max);

    }
    double ltw = 18;
    for (int i = windowGenesList.size(); i < offspring->getIndividualSize() ; i++)
        {
            min = 17;
            max = 1;
            offspring->setParameter(((i*4)+1), min);
            offspring->setParameter(((i*4)+2), max);
        }
*/
    // *******************************************************


    offspring->getAverageOnFullScanning();

    offspring->calculateDiscoveryValue();
    offspring->calculateLatencyValue();

    // agregar el individuo padre y el individuo hijo a la lista newPopulation para mantener tamano 2p
    newPopulation.append(father);
    newPopulation.append(offspring);
}


// mutación de los genes de W y los genes restantes se mutan min y max de acuerdo a la distribución normal
void Mutation::directedMutation(CTable * ct, Individual * father, double stdMin, double stdMax)
{
    QList<CTableGen *> windowGenesList = ct->getWindowGenes();
    Individual * offspring = new Individual(*father);

    QList<CTableGen*> convertedIndividual = ct->convertIndividualToCTableGen(offspring);

    CTableGen * gen;
    CTableGen * tmpGen;

    int index = 0;

    QList<int> busyChannels;

    for (int i=0; i< windowGenesList.size(); i++)
    {
        gen = windowGenesList.at(i);

        busyChannels.append(gen->getChannel());

        // gen temporal del hijo
        tmpGen = convertedIndividual.at(i);

        // indice del canal en el cual se debe colocar el nuevo canal
        index = ct->searchChannelInList(gen->getChannel(), convertedIndividual);

        // solo reemplazar
        if (index == -1)
        {
            convertedIndividual.replace(i, gen);
        }else{ // permutar
            convertedIndividual.replace(i,gen);
            convertedIndividual.replace(index, tmpGen);
        }
        index = 0;
    }

    // en este punto ya convertedIndividual esta completamente mutado, ahora devolver el cambio
    for (int i=0; i<offspring->getIndividualSize(); i++)
    {
        offspring->setParameter((i*4), convertedIndividual.at(i)->getChannel());
        offspring->setParameter(((i*4)+1), convertedIndividual.at(i)->getMinChannelTime());
        offspring->setParameter(((i*4)+2), convertedIndividual.at(i)->getMaxChannelTime());
        offspring->setParameter(((i*4)+3), convertedIndividual.at(i)->getAPs());
    }
    qDebug("revisa");

    // *******************************************************
    // ahora se deben mutar los min y max de los genes tamanoIndividuo-w

    int window = windowGenesList.size();
    int start = window*4;

    double newParameterValue = 0;

    for (int i=start; i<father->getNumberOfParameters(); i++)
    {
        // funcion de mutacion de parametros original
        //newParameterValue = mutateIndividualParameter(i, 0, 1, stdMin, stdMax, offspring->getParameter(i), offspring);

        // funcion utilizada para el caso de prueba de modficar min y max de los genes restantes despues de W
        newParameterValue = mutateIndividualParameterModified(i, 0, 1, stdMin, stdMax, offspring->getParameter(i), offspring, busyChannels);
        offspring->setParameter(i, newParameterValue);
    }

    // limpiar lista de canales utilizados por W
    busyChannels.clear();

    // se muto el offspring ahora limpiar el diccionario de canales usados
    for (int c=1; c<=11;c++)
    {
        channelsUsedForMutation[c]=false;
    }

    // *******************************************************

    offspring->getAverageOnFullScanning();

    offspring->calculateDiscoveryValue();
    offspring->calculateLatencyValue();

    // agregar el individuo padre y el individuo hijo a la lista newPopulation para mantener tamano 2p
    newPopulation.append(father);
    newPopulation.append(offspring);
}



/*
// prueba del 30/07/15 de tomar los min y max del sind en los canales fuera de la ventana
// cuando la descubierta del gen del offspring es <= 0.7
void Mutation::directedMutation(CTable * ct, Individual * father, double stdMin, double stdMax)
{
    QList<CTableGen *> windowGenesList = ct->getWindowGenes();
    Individual * offspring = new Individual(*father);

    QList<CTableGen*> convertedIndividual = ct->convertIndividualToCTableGen(offspring);

    CTableGen * gen;
    CTableGen * tmpGen;

    int index = 0;

    QList<int> busyChannels;

    for (int i=0; i< windowGenesList.size(); i++)
    {
        gen = windowGenesList.at(i);

        busyChannels.append(gen->getChannel());

        // gen temporal del hijo
        tmpGen = convertedIndividual.at(i);

        // indice del canal en el cual se debe colocar el nuevo canal
        index = ct->searchChannelInList(gen->getChannel(), convertedIndividual);

        // solo reemplazar
        if (index == -1)
        {
            convertedIndividual.replace(i, gen);
        }else{ // permutar
            convertedIndividual.replace(i,gen);
            convertedIndividual.replace(index, tmpGen);
        }
        index = 0;
    }

    // en este punto convertedIndividual esta completamente mutado en window, ahora viene el resto de genes
    QList<CTableGen *> sindGenesList = ct->getGenes();

    int window = windowGenesList.size();

    CTableGen * sindGen;
    double sindGenFONC = 0;

    CTableGen * offspringGen;
    double offspringGenAPs = 0;
    double offspringGenFONC = 0;

    for (int i=window; i<sindGenesList.size(); i++)
    {
        sindGen = sindGenesList.at(i);
        sindGenFONC = sindGen->getFONC();

        offspringGen = convertedIndividual.at(i);
        offspringGenAPs = offspringGen->getAPs();
        offspringGenFONC = offspringGen->getFONC();

        // se deben tomar los min y max del gen y colocarlos en el offspring
        if ((offspringGenAPs <= 0.7) && (sindGenFONC > offspringGenFONC))
        {
            offspringGen->setMinChannelTime(sindGen->getMinChannelTime());
            offspringGen->setMaxChannelTime(sindGen->getMaxChannelTime());

            // mutar el valor del canal
            int channelIndex = i*4;
            double currentChannel = offspringGen->getChannel();

            int newChannel = mutateIndividualParameterModified(channelIndex, 0, 1, stdMin, stdMax, currentChannel, father, busyChannels);
            offspringGen->setChannel(newChannel);

            // asignar el valor de AP
            int apsIndex = (i*4)+3;
            double currentAPs = offspringGen->getAPs();

            int newAPs = mutateIndividualParameterModified(apsIndex, 0, 1, stdMin, stdMax, currentAPs, father, busyChannels);
            offspringGen->setAPs(newAPs);


        }
        else // se hace la mutacion de min y max con ~ N(0,std)
        {
            // mutar el valor del canal
            int channelIndex = i*4;
            double currentChannel = offspringGen->getChannel();
            int newChannel = mutateIndividualParameterModified(channelIndex, 0, 1, stdMin, stdMax, currentChannel, father, busyChannels);
            offspringGen->setChannel(newChannel);

            // mutar el valor del min
            int minIndex = (i*4)+1;
            double currentMin = offspringGen->getMinChannelTime();
            int newMin = mutateIndividualParameterModified(minIndex, 0, 1, stdMin, stdMax, currentMin, father, busyChannels);
            offspringGen->setMinChannelTime(newMin);

            // mutar el valor del max
            int maxIndex = (i*4)+2;
            double currentMax = offspringGen->getMaxChannelTime();
            int newMax = mutateIndividualParameterModified(maxIndex, 0, 1, stdMin, stdMax, currentMax, father, busyChannels);
            offspringGen->setMaxChannelTime(newMax);

            // asignar el valor de AP
            int apsIndex = (i*4)+3;
            double currentAPs = offspringGen->getAPs();
            int newAPs = mutateIndividualParameterModified(apsIndex, 0, 1, stdMin, stdMax, currentAPs, father, busyChannels);
            offspringGen->setAPs(newAPs);
        }

        convertedIndividual.replace(i,offspringGen);
    }

    // limpiar lista de canales utilizados por W
    busyChannels.clear();

    // se muto el offspring ahora limpiar el diccionario de canales usados
    for (int c=1; c<=11;c++)
    {
        channelsUsedForMutation[c]=false;
    }

    // en este punto ya convertedIndividual esta completamente mutado, ahora devolver el cambio
    for (int i=0; i<offspring->getIndividualSize(); i++)
    {
        offspring->setParameter((i*4), convertedIndividual.at(i)->getChannel());
        offspring->setParameter(((i*4)+1), convertedIndividual.at(i)->getMinChannelTime());
        offspring->setParameter(((i*4)+2), convertedIndividual.at(i)->getMaxChannelTime());
        offspring->setParameter(((i*4)+3), convertedIndividual.at(i)->getAPs());
    }
    qDebug("revisa");

    offspring->getAverageOnFullScanning();

    offspring->calculateDiscoveryValue();
    offspring->calculateLatencyValue();

    // agregar el individuo padre y el individuo hijo a la lista newPopulation para mantener tamano 2p
    newPopulation.append(father);
    newPopulation.append(offspring);
}
*/


void Mutation::setStdDeviation(double std)
{
    stdDeviation = std;
}

double Mutation::getStdDeviation()
{
    return stdDeviation;
}


QHash<QString, double> Mutation::calculateMeanAndStdDev(QList<double> minChannelTimeList, QList<double> maxChannelTimeList, int genIndex)
{
    // media de minchanneltime

    double minChannelTimeSum = 0.0;
    double maxChannelTimeSum = 0.0;

    double meanMinChannelTime = 0.0;
    double meanMaxChannelTime = 0.0;

    double minChannelTimeStdSum = 0.0;
    double maxChannelTimeStdSum = 0.0;

    double stdDevMinChannelTime = 0.0;
    double stdDevMaxChannelTime = 0.0;

    QHash<QString, double> hash;

    for (int i=0;i<minChannelTimeList.count();i++)
    {
        minChannelTimeSum = minChannelTimeSum + minChannelTimeList.at(i);
    }
    meanMinChannelTime = minChannelTimeSum / minChannelTimeList.count();
    hash.insert("meanMinChannelTime", meanMinChannelTime);


    for (int j=0; j<minChannelTimeList.count(); j++)
    {
        minChannelTimeStdSum = minChannelTimeStdSum + pow((minChannelTimeList.at(j)-meanMinChannelTime),2);
    }
    stdDevMinChannelTime = (sqrt(minChannelTimeStdSum/minChannelTimeList.count()));
    hash.insert("stdDevMinChannelTime", stdDevMinChannelTime);


    maxChannelTimeSum = 0.0;
    for (int k=0;k<maxChannelTimeList.count();k++)
    {
        maxChannelTimeSum = maxChannelTimeSum + maxChannelTimeList.at(k);
    }
    meanMaxChannelTime = maxChannelTimeSum / maxChannelTimeList.count();
    hash.insert("meanMaxChannelTime", meanMaxChannelTime);

    for (int l=0; l<maxChannelTimeList.count(); l++)
    {
        maxChannelTimeStdSum = maxChannelTimeStdSum + pow((maxChannelTimeList.at(l)-meanMaxChannelTime),2);
    }
    stdDevMaxChannelTime = (sqrt(maxChannelTimeStdSum/maxChannelTimeList.count()));
    hash.insert("stdDevMaxChannelTime", stdDevMaxChannelTime);


    //qDebug("meanMin: %f, stdDevMin: %f, meanMax: %f, sdtDevMax: %f",
    //       hash.value("meanMinChannelTime"), hash.value("stdDevMinChannelTime"),
    //       hash.value("meanMaxChannelTime"), hash.value("stdDevMaxChannelTime"));

    return hash;
}


int Mutation::getPatternSequence(QList<int> channelList)
{


    QHash<int, int> channelMap;
    for (int i=1;i<=11;i++)
    {
        channelMap.insert(i,0);
    }



    int readChannel = 0;
    for (int i=0; i<channelList.count();i++)
    {
        readChannel = channelList.at(i);
        if (readChannel == 1)
        {
            channelMap.insert(1,channelMap.value(1)+1);
        }
        else if (readChannel == 2) {
          channelMap.insert(2,channelMap.value(2)+1);
        }
        else if (readChannel == 3) {
          channelMap.insert(3,channelMap.value(3)+1);
        }
        else if (readChannel == 4) {
          channelMap.insert(4,channelMap.value(4)+1);
        }
        else if (readChannel == 5) {
          channelMap.insert(5,channelMap.value(5)+1);
        }
        else if (readChannel == 6) {
          channelMap.insert(6,channelMap.value(6)+1);
        }
        else if (readChannel == 7) {
          channelMap.insert(7,channelMap.value(7)+1);
        }
        else if (readChannel == 8) {
          channelMap.insert(8,channelMap.value(8)+1);
        }
        else if (readChannel == 9) {
          channelMap.insert(9,channelMap.value(9)+1);
        }
        else if (readChannel == 10) {
          channelMap.insert(10,channelMap.value(10)+1);
        }
        else if (readChannel == 11) {
          channelMap.insert(11,channelMap.value(11)+1);
        }
    }

    QList<int> repeated;
    int indexMax = 0;

    int channelToReturn = 0;

    for (int i=1; i<=11;i++)
    {
        if (i==1)
        {
           indexMax = i;
           repeated.append(i);
           continue;
        }
        if (channelMap.value(i) >= channelMap.value(indexMax))
        {
            indexMax = i;
        }
    }
    channelToReturn = indexMax;

    if (channelMap.value(channelToReturn) >= channelList.count()/2)
    {
        return channelToReturn;
    }
    else
    {
        return 0;
    }


}



double Mutation::getAPsFromMaxNumberApproach(int channel, double min, double max, int nscans)
{
    //QString database("database.db");
    QString database("test_18.1.db");

    QString experiment("full");
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();
    //return scan.getAP(channel, minChannelTime, maxChannelTime);
/*
    int low = 1;
    int high = 8;
    int nscans = qrand() % ((high+1)-low)+low;
*/
    double tmpAPs = 0;
    double finalAPs = 0;

    for(int i=0; i<nscans; i++)
    //for(int i=0; i<100; i++)
    {
        tmpAPs = scan.getAPs(channel, min, max);
        //tmpAPs = scan.getAPs(11, 15, 60);

        if (tmpAPs > finalAPs)
        {
            finalAPs = tmpAPs;
        }
    }
    return finalAPs;
}




Individual * Mutation::scanSequence(Individual * ind)
{
    Individual * scannedIndividual = new Individual();

    //QString database("database.db");
    QString database("test_18.1.db");

    QString experiment("full");
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();

    int tmpAps = 0;
    int channel = 0;
    int min = 0;
    int max = 0;

    for (int i=0;i<ind->getIndividualSize(); i++)
    {
        channel = ind->getParameter(i*4);
        min = ind->getParameter((i*4)+1);
        max = ind->getParameter((i*4)+2);
        tmpAps = scan.getAPs(channel, min, max);

        scannedIndividual->setParameter((i*4), channel);
        scannedIndividual->setParameter(((i*4)+1), min);
        scannedIndividual->setParameter(((i*4)+2), max);
        scannedIndividual->setParameter(((i*4)+3), tmpAps);
    }

    scannedIndividual->calculateDiscoveryValue();
    scannedIndividual->calculateLatencyValue();

    return scannedIndividual;
}


Individual * Mutation::updateOffspring(Individual *offspring)
{

    Individual * ind1 = scanSequence(offspring);
    Individual * ind2 = scanSequence(offspring);
    Individual * ind3 = scanSequence(offspring);

    QList<Individual *> list;
    list.append(ind1);
    list.append(ind2);
    list.append(ind3);

    qSort(list.begin(), list.end(), individual1LessThanIndividual2);
    Individual * indivitualToReturn = list.at(list.size()-1);



    return indivitualToReturn;
}






