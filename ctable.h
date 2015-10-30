#ifndef CTABLE_H
#define CTABLE_H

#include <QList>

#include "individual.h"
#include "ctablegen.h"


/**
 * @brief Clase que abstrae la CTable del espacio de creencias que mantiene el conocimiento de
 * mejor individuo. Se usa para luego influenciar el proceso de mutacion de los
 * individuos.
 */
class CTable
{

private:
    /**
     * @brief Lista de los genes
     */
    QList<CTableGen *> genList;

    /**
     * @brief Lista de registro de los superindividuos utilizados durante la simulacion
     */
    QList< QList<CTableGen *> > historicSuperIndividualList;

    /**
     * @brief Tamano de la ventana de genes de la tabla.
     *
     * Representa el numero de genes que se deben retornar para influir el proceso de mutacion
     */
    int cTableWindow;

    /**
     * @brief indice que se usa para ordenar la tabla
     */
    int indexToSort;

public:
    /**
     * @brief Constructor de CTable
     * @param individualSize tamaño del individuo
     * @param ctWindow ventana de genes que se usaran para influir la mutacion
     * @param indexToSort especifica el tipo de indice que se utilizara para ordenar la tabla C
     *      0 = A
     *      1 = B
     *      2 = C
     */
    CTable(int individualSize, int ctWindow, int index);

    /**
     * Destructor de la clase
     */
    ~CTable();

    /**
     * @brief Agrega un super individuo a la tabla C del espacio de creencias
     * @param superIndividualGenList lista de genes del individuo
     */
    void addSuperIndividual(QList<CTableGen *> superIndividualGenList, int indexToSort);


    /**
     * @brief Agrega el individuo pasado como argumento a CTable como superindividuo
     * @param individual individuo a considerar como superindividuo
     */
    void addSuperIndividual(Individual * individual);


    /**
     * @brief Retorna lista de los genes de la ventana de la tabla C del espacio de creencias
     * @return lista de los genes de la ventana de la tabla C del espacio de creencias
     */
    QList<CTableGen *> getWindowGenes();

    /**
     * @brief Retorna la lista de los genes del superindividuo
     * @return lista de los genes del superindividuo
     */
    QList<CTableGen *> getGenes();

    /**
     * @brief Convierte un Individuo en una lista de CTableGen
     * @param ind invididuo a convertir
     * @return Individuo en la forma de una lista de CTableGen
     */
    QList<CTableGen*> convertIndividualToCTableGen(Individual* ind);

    /**
     * @brief Retorna el indice del gen en el cual se encuentra el canal pasado
     * como argumento
     * @param ch canal a buscar en la lista
     * @param list lista en el cual se busca el canal
     * @return
     */
    int searchChannelInList(int ch, QList<CTableGen *> list);

    /**
     * @brief Limpia la tabla C para actualizarla.
     *
     * Se agrega el super individuo actual a la lista de historicos y se limpia la lista genList
     * de los genes para esperar un nuevo super individuo
     * @param recordCurrent verdadero si se debe registrar el superindividuo en el historico
     */
    void clearCTable(bool recordCurrent=true);

    /**
     * @brief Genera un archivo con los individuos historicos de la CTable a lo largo de la
     * simulacion
     *
     * @param resultsDirectory directorio de resultados
     */
    void reportCTableHistory(QString resultsDirectory);

    /**
     * @brief Retorna un individuo creado a partir de la mutación del individuo pasado como argumento
     *
     * @param father individuo a mutar
     * @return un individuo creado a partir de la mutación del individuo pasado como argumento
     */
    Individual * getNewIndividualFromCTable(Individual *father);

};

#endif // CTABLE_H
