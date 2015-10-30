#include "normativegrid.h"
#include "gridsubinterval.h"

//#include <qmessagebox.h>

#include<iostream>
using namespace std;


NormativeGrid::NormativeGrid(int subIntervals, NormativePhenotypicPart * nPhenotypicPart)
{

    lowerF1 = nPhenotypicPart->getLowerF1();

    upperF1 = nPhenotypicPart->getUpperF1();

    lowerF2 = nPhenotypicPart->getLowerF2();

    upperF2 = nPhenotypicPart->getUpperF2();

    subIntervalNumber = subIntervals;

/*
    ptrGrid = new int* [subIntervalNumber];
    for (int i = 0; i < subIntervalNumber; i++)
    {
        ptrGrid[i] = new int[subIntervalNumber];
    }
*/

    ptrGrid = new Cell* [subIntervalNumber];
    for (int i = 0; i < subIntervalNumber; i++)
    {
        //ptrGrid[i] = new int[subIntervalNumber];
        ptrGrid[i] = new Cell[subIntervalNumber];
    }

/*
    QString row;

    qDebug("Elementos de la Matriz : ");
    for (int i = 0; i < subIntervalNumber; i++)
    {
        for (int j = 0; j < subIntervalNumber; j++)
        {
            // inicializar el contador en 0
            ptrGrid[i][j] = 0;
            row.append(QString::number(ptrGrid[i][j]));
            row.append("-");
        }
        qDebug(qPrintable(row));
        row.clear();
    }
*/
    QString row;

    Cell * cell;

    //Individual * individual;
    qDebug("Elementos de la Matriz : ");
    for (int i = 0; i < subIntervalNumber; i++)
    {
        for (int j = 0; j < subIntervalNumber; j++)
        {
            cell = new Cell();
            //individual = new Individual();
            //individual->printIndividual();
            //cell->addIndividual(individual);

            // inicializar el contador en 0
            ptrGrid[i][j] = *cell;

            row.append("(");
            row.append(QString::number(ptrGrid[i][j].getCount()));
            row.append(")-");
        }
        qDebug(qPrintable(row));
        row.clear();
    }


    // construir los intervalos de F1 y de F2
    buildSubintervalsF1(nPhenotypicPart);
    buildSubintervalsF2(nPhenotypicPart);
}


NormativeGrid::~NormativeGrid()
{
    // borrar la matriz
    // Elimino cada vector de la matriz
        for (int i = 0; i < subIntervalNumber; i++) {
            delete[] ptrGrid[i];
        }

        // Elimino el vector principal
        delete[] ptrGrid;
}


int NormativeGrid::getSubIntervalNumber()
{
    return subIntervalNumber;
}


void NormativeGrid::addIndividualToGrid(Individual * ind)
{
    qDebug("...NormativeGrid::addIndividualToGrid");
    double f1 = ind->getPerformanceDiscovery();
    double f2 = ind->getPerformanceLatency();

    // encontrar el subintervalo de F1 y de F2
    int indexF1 = getF1SubintervalIndex(f1);
    Q_ASSERT_X( ((0 <= indexF1) && (indexF1 < subIntervalListF1.count())),
                "NormativeGrid::addIndividualToGrid",
               "Se obtuvo un indice de subintervalo fuera de rango");


    int indexF2 = getF2SubintervalIndex(f2);
    Q_ASSERT_X( ((0 <= indexF2) && (indexF2 < subIntervalListF2.count())),
                "NormativeGrid::getCountOfCell",
               "Se obtuvo un indice de subintervalo fuera de rango");

    if (indexF1 == -1)
    {
        //QMessageBox msg;
        //msg.setText("NormativeGrid::addIndividualToGrid: indexF1 == -1. individuo fuera de la grid");
        qDebug("NormativeGrid::addIndividualToGrid: indexF1 == -1. individuo fuera de la grid");
        //msg.exec();

    }
    if (indexF2 == -1)
    {
        //QMessageBox msg;
        //msg.setText("NormativeGrid::addIndividualToGrid: indexF2 == -1. individuo fuera de la grid");
        qDebug("NormativeGrid::addIndividualToGrid: indexF2 == -1. individuo fuera de la grid");
        //msg.exec();

    }


    // incrementar el contador de la celda en los subintervalos
    //int count = ptrGrid[indexF1][indexF2];
    //qDebug("...valor de count: %d",count);
    //grid[indexF1].append(indexF2) = increment+1;
    //ptrGrid[indexF1][indexF2] = count+1;

    ptrGrid[indexF1][indexF2].addIndividual(ind, indexF1, indexF2);

    // agregar la celda a la lista de celda solo cuando esta no se haya
    // agregado previamente
    if (!isCellInCellList(indexF1,indexF2))
    {
        populatedCellList.append(&ptrGrid[indexF1][indexF2]);
    }


    qDebug("...valor de ptrGrid[%d][%d]: %d",indexF1, indexF2, ptrGrid[indexF1][indexF2].getCount());

}


int NormativeGrid::getCount(int f1, int f2)
{
    return ptrGrid[f1][f2].getCount();
}


void NormativeGrid::buildSubintervalsF1(NormativePhenotypicPart *nPhenotypicPart)
{
    double widthF1 = (nPhenotypicPart->getUpperF1() - nPhenotypicPart->getLowerF1()) / subIntervalNumber;
    //qDebug("---ancho del intervaloF1: %f", widthF1);

    double l = nPhenotypicPart->getLowerF1();
    double u = 0;

    //qDebug("   %f",nPhenotypicPart->getLowerF1());
    for (int i = 1; i <= getSubIntervalNumber(); i++)
    {
        u = nPhenotypicPart->getLowerF1()+widthF1*i;
        //qDebug("%f - %f", l, u);
        GridSubInterval * subInterval = new GridSubInterval(l,u);
        subIntervalListF1.append(subInterval);

        //qDebug("   %f",nPhenotypicPart->getLowerF1()+widthF1*i);
        l = nPhenotypicPart->getLowerF1()+widthF1*i;
    }
}


void NormativeGrid::buildSubintervalsF2(NormativePhenotypicPart * nPhenotypicPart)
{
    double widthF2 = (nPhenotypicPart->getUpperF2() - nPhenotypicPart->getLowerF2()) / subIntervalNumber;
    //qDebug("---ancho del intervaloF2: %f", widthF2);

    double l = nPhenotypicPart->getLowerF2();
    double u = 0;

    //qDebug("   %f",nPhenotypicPart->getLowerF2());
    for (int i = 1; i <= getSubIntervalNumber(); i++)
    {
        u = nPhenotypicPart->getLowerF2()+widthF2*i;
        //qDebug("%f - %f", l, u);
        GridSubInterval * subInterval = new GridSubInterval(l,u);
        subIntervalListF2.append(subInterval);

        //qDebug("   %f",nPhenotypicPart->getLowerF2()+widthF2*i);
        l = nPhenotypicPart->getLowerF2()+widthF2*i;
    }
}

int NormativeGrid::getF1SubintervalIndex(double value)
{
    int index = -1;
    for (int i = 0; i < subIntervalListF1.count(); i++)
    {
        if (i == subIntervalListF1.count()-1){
            if (subIntervalListF1.at(i)->belongsToSubinterval(value, true))
            {
                index = i;
                return index;
            }
        }else
        {
            if (subIntervalListF1.at(i)->belongsToSubinterval(value))
            {
                index = i;
                return index;
            }
        }
    }
    Q_ASSERT_X( ((0 <= index) && (index < subIntervalListF1.count())),
                "NormativeGrid::getF1SubintervalIndex",
               "Se obtuvo un indice de subintervalo fuera de rango");

    if (index == -1)
        qDebug("XXXXXXX getF1SubintervalIndex(double %f): %d", value, index);

    return index;
}

int NormativeGrid::getF2SubintervalIndex(double value)
{
    int index = -1;
    for (int i = 0; i < subIntervalListF2.count(); i++)
    {
        if (i == subIntervalListF2.count()-1){
            if (subIntervalListF2.at(i)->belongsToSubinterval(value, true))
            {
                index = i;
                return index;
            }
        }else
        {
            if (subIntervalListF2.at(i)->belongsToSubinterval(value))
            {
                index = i;
                return index;
            }
        }
    }

    Q_ASSERT_X( ((0 <= index) && (index < subIntervalListF2.count())),
                "NormativeGrid::getF2SubintervalIndex",
               "Se obtuvo un indice de subintervalo fuera de rango");

    if (index == -1)
        qDebug("XXXXXXX getF2SubintervalIndex(double %f): %d", value, index);
    return index;
}

void NormativeGrid::printGrid()
{
    QString row;
    for (int i = 0; i < subIntervalNumber; i++) {
        for (int j = 0; j < subIntervalNumber; j++) {
            row.append(QString::number(ptrGrid[i][j].getCount()));
            row.append("-");
        }
        qDebug(qPrintable(row));
        row.clear();
    }
}


bool NormativeGrid::individualInsideGrid(Individual * individual)
{
    // lowerF1 <= individual->getPerformanceDiscovery() <= upperF1
    // lowerF2 <= individual->getPerformanceLatency() <= upperF2
    if ( ( (individual->getPerformanceDiscovery() >= lowerF1) && (individual->getPerformanceDiscovery() <= upperF1) ) &&
        ( (individual->getPerformanceLatency() >= lowerF2) && (individual->getPerformanceLatency() <= upperF2) ) )
    {
        return true;
    }
    else
    {
        return false;
    }

}

int NormativeGrid::getCountOfCell(Individual * individual)
{
    qDebug("...NormativeGrid::getCountOfCell");
    double f1 = individual->getPerformanceDiscovery();
    double f2 = individual->getPerformanceLatency();

    // encontrar el subintervalo de F1 y de F2
    int indexF1 = getF1SubintervalIndex(f1);
    Q_ASSERT_X( ((0 <= indexF1) && (indexF1 < subIntervalListF1.count())),
                "NormativeGrid::getCountOfCell",
               "Se obtuvo un indice de subintervalo fuera de rango");


    int indexF2 = getF2SubintervalIndex(f2);
    Q_ASSERT_X( ((0 <= indexF2) && (indexF2 < subIntervalListF2.count())),
                "NormativeGrid::getCountOfCell",
               "Se obtuvo un indice de subintervalo fuera de rango");

    return getCount(indexF1, indexF2);

    /*
    if ( (indexF1 == -1) || (indexF2 == -1) )
    {
        return 0;
    }
    else
    {
        return getCount(indexF1, indexF2);
    }
    */
}

QList<Cell*> NormativeGrid::getPopulatedCellList()
{
    return populatedCellList;
}


bool NormativeGrid::isCellInCellList(int indexF1, int indexF2)
{
    for (int i=0;i<populatedCellList.count(); i++)
    {
        if ( (populatedCellList.at(i)->getSubintervalF1() == indexF1) &&
            (populatedCellList.at(i)->getSubintervalF2() == indexF2) )
        {
            // la celda identificada con (indexF1,indexF2) ya existe en la lista
            return true;
        }
    }
    return false;
}





