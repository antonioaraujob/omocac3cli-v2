#!/bin/bash

# script para procesar los resultados del programa omocac2cli y generar una salida
# en formato de tabla de LaTeX
#
#
# El formato de la tabla es como sigue
#
#
# 	| ch1<min,max>->AP|...|ch11<min,max>->AP|totalAP|totalLatencia|
#
#
#

# iniciar el archivo latex
echo "\documentclass{article}
\usepackage{fullpage}
\usepackage[a4paper,margin=10mm,landscape]{geometry}

\usepackage{tabularx}
\usepackage{tablefootnote}
\usepackage[table]{xcolor}

\title{Resultados de ejecucion}
%\author{PGCOMP}
\date{}

\begin{document}
\maketitle

\begin{table}[h]
\centering
\caption{Channel sequences}
\label{tab:sequences}

\definecolor{light-gray}{gray}{0.95}
\rowcolors{1}{white}{light-gray}

\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\hline
" > /tmp/salida.tex


# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginal.txt
awk ' 

BEGIN { 
	FS=",";  
}  

{ 
	print $1"$<$"$2","$3"$>\\rightarrow$"$4" & " $5"$<$"$6","$7"$>\\rightarrow$"$8" & " $9"$<$"$10","$11"$>\\rightarrow$"$12" & " \
	$13"$<$"$14","$15"$>\\rightarrow$"$16" & " $17"$<$"$18","$19"$>\\rightarrow$"$20" & " $21"$<$"$22","$23"$>\\rightarrow$"$24" & " \
	$25"$<$"$26","$27"$>\\rightarrow$"$28" & " $29"$<$"$30","$31"$>\\rightarrow$"$32" & " $33"$<$"$34","$35"$>\\rightarrow$"$36" & " \
	$37"$<$"$38","$39"$>\\rightarrow$"$40" & " $41"$<$"$42","$43"$>\\rightarrow$"$44" & " $45" & "$46 "\134\134" ;
}' /tmp/individuosFrenteParetoOriginal.txt >> /tmp/salida.tex

# finalizar la tabla de latex
echo "\hline
%\end{tabular}

\end{tabularx}

\end{table}

" >> /tmp/salida.tex

# agregar la tabla correspondiente a los individuos con canales ordenados por mayor latencia

echo "
\begin{table}[h]
\centering
\caption{Channel sequences ordered by channels latency}
\label{tab:sequencesorderbylatency}

\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\hline
" >> /tmp/salida.tex

# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginalPorLatencia.txt
awk ' 

BEGIN { 
	FS=",";  
}  

{ 
	print $1"$<$"$2","$3"$>\\rightarrow$"$4" & " $5"$<$"$6","$7"$>\\rightarrow$"$8" & " $9"$<$"$10","$11"$>\\rightarrow$"$12" & " \
	$13"$<$"$14","$15"$>\\rightarrow$"$16" & " $17"$<$"$18","$19"$>\\rightarrow$"$20" & " $21"$<$"$22","$23"$>\\rightarrow$"$24" & " \
	$25"$<$"$26","$27"$>\\rightarrow$"$28" & " $29"$<$"$30","$31"$>\\rightarrow$"$32" & " $33"$<$"$34","$35"$>\\rightarrow$"$36" & " \
	$37"$<$"$38","$39"$>\\rightarrow$"$40" & " $41"$<$"$42","$43"$>\\rightarrow$"$44" & " $45" & "$46 "\134\134" ;
}' /tmp/individuosFrenteParetoOriginalPorLatencia.txt >> /tmp/salida.tex

# finalizar la tabla de latex
echo "
\hline
%\end{tabular}

\end{tabularx}

\end{table}


" >> /tmp/salida.tex


# escribir la configuracion utilizada para la ejecucion
awk ' 

BEGIN {}  

{ 
	print $0 "\n";
}' /tmp/settings.ini >> /tmp/salida.tex


echo "

\end{document}" >> /tmp/salida.tex

