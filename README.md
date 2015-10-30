# omocac3cli


omocac versión 3 con mutación dirigida desde espacio de creencias considerando canales más poblados

omocac3cli

Dependencias Qt>=4.8, libsqlite3

Para compilar: 

$ qmake
$ make

Para ejecutar:

Ajustar parámetros en el archivo settings.ini:


[CONF]
tamanoDeIndividuo=11
poblacion=20
numeroDeGeneraciones=1
desviacionEstandar=2
desviacionEstandarMinChannelTime=8
desviacionEstandarMaxChannelTime=20
tamanoArchivoExterno=100
numeroDeSubintervalosDeRejilla=10
frecuenciaDeActualizacion=10
numeroDeEncuentrosPorIndividuo=10
probabilidadDeMutacionDirigida=0.7
ejecutarComparacionDeAlgoritmos=false
ejecutarAlgoritmoOriginal=false
ejecutarMutacionDirigida=true
numeroDeRepeticionesDeSimulacion=1
emularScanning=true
usarIndividuosInteligentes=false
tamanoDeVentanaCTable=3
indiceParaOdernarTablaC=0
