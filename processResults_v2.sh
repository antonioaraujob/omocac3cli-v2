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

%\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}
\begin{tabularx}{\textwidth}{|XXXXXXXXXXX|m{1cm}|m{1cm}|}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
%\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\multicolumn{11}{|l|}{Ch\newline\$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$\newline(\#AP) & \$FO_2\$\newline(ms) \\""\\

\hline
" > /tmp/salida.tex


# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginal.txt
awk ' 

BEGIN { 
	FS=",";  
}  

{ 
	print "\\textbf{"$1"}\\newline$<$"$2","$3"$>\\rightarrow$"$4" & " "\\textbf{"$5"}\\newline$<$"$6","$7"$>\\rightarrow$"$8" & " "\\textbf{"$9"}\\newline$<$"$10","$11"$>\\rightarrow$"$12" & " \
	"\\textbf{"$13"}\\newline$<$"$14","$15"$>\\rightarrow$"$16" & " "\\textbf{"$17"}\\newline$<$"$18","$19"$>\\rightarrow$"$20" & " "\\textbf{"$21"}\\newline$<$"$22","$23"$>\\rightarrow$"$24" & " \
	"\\textbf{"$25"}\\newline$<$"$26","$27"$>\\rightarrow$"$28" & " "\\textbf{"$29"}\\newline$<$"$30","$31"$>\\rightarrow$"$32" & " "\\textbf{"$33"}\\newline$<$"$34","$35"$>\\rightarrow$"$36" & " \
	"\\textbf{"$37"}\\newline$<$"$38","$39"$>\\rightarrow$"$40" & " "\\textbf{"$41"}\\newline$<$"$42","$43"$>\\rightarrow$"$44" & " $45" & "$46 "\134\134" ;
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

\definecolor{light-gray}{gray}{0.95}
\rowcolors{1}{white}{light-gray}

%\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}
\begin{tabularx}{\textwidth}{|XXXXXXXXXXX|m{1cm}|m{1cm}|}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
%\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\multicolumn{11}{|l|}{Ch\newline\$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$\newline(\#AP) & \$FO_2\$\newline(ms) \\""\\
\hline
" >> /tmp/salida.tex

# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginalPorLatencia.txt
awk ' 

BEGIN { 
	FS=",";  
}  

{ 
#	print $1"$<$"$2","$3"$>\\rightarrow$"$4" & " $5"$<$"$6","$7"$>\\rightarrow$"$8" & " $9"$<$"$10","$11"$>\\rightarrow$"$12" & " \
#	$13"$<$"$14","$15"$>\\rightarrow$"$16" & " $17"$<$"$18","$19"$>\\rightarrow$"$20" & " $21"$<$"$22","$23"$>\\rightarrow$"$24" & " \
#	$25"$<$"$26","$27"$>\\rightarrow$"$28" & " $29"$<$"$30","$31"$>\\rightarrow$"$32" & " $33"$<$"$34","$35"$>\\rightarrow$"$36" & " \
#	$37"$<$"$38","$39"$>\\rightarrow$"$40" & " $41"$<$"$42","$43"$>\\rightarrow$"$44" & " $45" & "$46 "\134\134" ;

	print "$\\textbf{"$1"}^{"$2+$3"}$\\newline$<$"$2","$3"$>\\rightarrow$"$4" & " "$\\textbf{"$5"}^{"$2+$3+$6+$7"}$\\newline$<$"$6","$7"$>\\rightarrow$"$8" & " "$\\textbf{"$9"}^{"$2+$3+$6+$7+$10+$11"}$\\newline$<$"$10","$11"$>\\rightarrow$"$12" & " \
	"$\\textbf{"$13"}^{"$2+$3+$6+$7+$10+$11+$14+$15"}$\\newline$<$"$14","$15"$>\\rightarrow$"$16" & " "$\\textbf{"$17"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19"}$\\newline$<$"$18","$19"$>\\rightarrow$"$20" & " "$\\textbf{"$21"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23"}$\\newline$<$"$22","$23"$>\\rightarrow$"$24" & " \
	"$\\textbf{"$25"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27"}$\\newline$<$"$26","$27"$>\\rightarrow$"$28" & " "$\\textbf{"$29"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31"}$\\newline$<$"$30","$31"$>\\rightarrow$"$32" & " "$\\textbf{"$33"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35"}$\\newline$<$"$34","$35"$>\\rightarrow$"$36" & " \
	"$\\textbf{"$37"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35+$38+$39"}$\\newline$<$"$38","$39"$>\\rightarrow$"$40" & " "$\\textbf{"$41"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35+$38+$39+$42+$43"}$\\newline$<$"$42","$43"$>\\rightarrow$"$44" & " $45" & "$46 "\134\134" ;
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

