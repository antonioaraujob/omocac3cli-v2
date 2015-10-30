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

" > salida.tex



# ******************************************************************************
# agregar la tabla 4 correspondiente a los individuos con canales ordenados por AP descendente

echo "
\begin{table}[h]
\centering
\caption{Channel sequences ordered by descending APs discovered. If two channels have same APs discovered the lower latency in channels is used to order the sequence. }
\label{tab:sequencesorderbyapsdiscovereddescending}

\definecolor{light-gray}{gray}{0.95}
\rowcolors{1}{white}{light-gray}

%\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}
\begin{tabularx}{\textwidth}{|XX|m{1cm}|m{1cm}|}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
%\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\multicolumn{2}{|l|}{Ch\newline\$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$\newline(\#AP) & \$FO_2\$\newline(ms) \\""\\
\hline
" >> salida.tex


# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginalPorAPsDescendente.txt
awk ' 

BEGIN { 
	FS=","; gen1="";gen2="";
}  

{ 

	if ($4=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen1="\\textit{"$1"}";
		#gen1="\\textit{\\textbf{"$1"}\\newline$<$"$2","$3"$>$"$4"}";
		gen1="\\textit{$\\textbf{"$1"}^{"$2+$3"}$\\newline$<$"$2","$3"$>$"$4"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen1="\\textbf{"$1"}";
		#gen1="\\textbf{"$1"}\\newline$<$"$2","$3"$>$"$4;
		gen1="$\\textbf{"$1"}^{"$2+$3"}$\\newline$<$"$2","$3"$>$"$4;
	}
	if ($8=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen2="\\textit{"$5"}";
		gen2="\\textit{$\\textbf{"$5"}^{"$2+$3+$6+$7"}$\\newline$<$"$6","$7"$>$"$8"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen2="\\textbf{"$5"}";
		gen2="$\\textbf{"$5"}^{"$2+$3+$6+$7"}$\\newline$<$"$6","$7"$>$"$8;
	}

		
	print gen1" & "gen2 " & " $9" & "$10 "\134\134" ;

}' individuosFrenteParetoModificadoPorAPsDescendente.txt >> salida.tex

# finalizar la tabla de latex
echo "
\hline
%\end{tabular}

\end{tabularx}

\end{table}


" >> salida.tex


# ******************************************************************************
# agregar la tabla 5 correspondiente a los intervalos de confianza de 95% de 
# APs encontrados

echo "
\begin{table}[h]
\centering
\caption{95 Confidence Intervals}

\definecolor{light-gray}{gray}{0.95}
\rowcolors{1}{white}{light-gray}

\begin{tabularx}{\textwidth}{|X|X|X|X|X|}

\hline
\textbf{Cadena} & \textbf{APs} & \textbf{Min} & \textbf{Max} & \textbf{Latency (ms)} \\\\
\hline
" >> salida.tex


# procesar cada linea de secuencia de canales del archivo dataToPlot.txt
awk ' 
BEGIN { 
	FS=" "; gen1="";gen2="";gen3="";gen4="";gen5="";
}  
{ 
	print $1" & "$2" & "$3" & "$4" & "$5"\134\134";
}' dataToPlot.txt >> salida.tex

# finalizar la tabla de latex
echo "
\hline
\end{tabularx}
\end{table}

" >> salida.tex







# ******************************************************************************
# escribir la configuracion utilizada para la ejecucion
awk ' 

BEGIN {}  

{ 
	print $0 "\n";
}' settings.ini >> salida.tex


echo "

\end{document}" >> salida.tex


# *****************************************************************************
# compilar el archivo /salida.tex y generar el pdf

pdflatex salida.tex
