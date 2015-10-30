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

\title{Historico de CTABLE}
%\author{PGCOMP}
\date{}

\begin{document}
\maketitle

" > ctable.tex



# ******************************************************************************
# agregar la tabla 4 correspondiente a los individuos con canales ordenados por AP descendente

echo "
\begin{table}[h]
\centering
\caption{CTABLE LOG: each row represents the SIND evolution through the simulation }
\label{tab:sequencesorderbyapsdiscovereddescending}

\definecolor{light-gray}{gray}{0.95}
\rowcolors{1}{white}{light-gray}

%\begin{tabularx}{\textwidth}{XXXXXXXXXXXrr}
\begin{tabularx}{\textwidth}{|XXXXXXXXXXX|m{1cm}|m{1cm}|m{1cm}|}

%\begin{tabular}{lrrrrrrrrrrrr}
\hline
%\multicolumn{11}{l}{Ch$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$ & \$FO_2\$ \\""\\
\multicolumn{11}{|l|}{Ch\newline\$<\$min,max\$>\rightarrow\$AP} & \$FO_1\$\newline(\#AP) & \$FO_2\$\newline(ms) & FONC \\""\\
\hline
" >> ctable.tex


# procesar cada linea de secuencia de canales del archivo individuosFrenteParetoOriginalPorAPsDescendente.txt
awk ' 

BEGIN { 
	FS=","; gen1="";gen2="";gen3="";gen4="";gen5="";gen6="";gen7="";gen8="";gen9="";gen10="";gen11="";
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
		gen1="$\\textbf{"$1"}^{"$2+$3"}$\\newline$<$"$2","$3"$>$"$4"\\newline$"$5"$";
	}
	if ($9=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen2="\\textit{"$5"}";
		gen2="\\textit{$\\textbf{"$6"}^{"$2+$3+$6+$7"}$\\newline$<$"$6","$7"$>$"$8"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen2="\\textbf{"$5"}";
		gen2="$\\textbf{"$6"}^{"$2+$3+$7+$8"}$\\newline$<$"$7","$8"$>$"$9"\\newline$"$10"$";
	}

	if ($14=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen3="\\textit{"$9"}";
		#$\\textbf{"$9"}^{"$2+$3+$6+$7+$10+$11"}$\\newline$<$"$10","$11"$>$"$12" & " \
		gen3="\\textit{$\\textbf{"$9"}^{"$2+$3+$6+$7+$10+$11"}$\\newline$<$"$10","$11"$>$"$12"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen3="\\textbf{""$9}";
		gen3="$\\textbf{"$11"}^{"$2+$3+$7+$8+$12+$13"}$\\newline$<$"$12","$13"$>$"$14"\\newline$"$15"$";
	}
	if ($19=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen4="\\textit{"$13"}";
		gen4="\\textit{$\\textbf{"$13"}^{"$2+$3+$6+$7+$10+$11+$14+$15"}$\\newline$<$"$14","$15"$>$"$16"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen4="\\textbf{"$13"}";
		gen4="$\\textbf{"$16"}^{"$2+$3+$7+$8+$12+$13+$17+$18"}$\\newline$<$"$17","$18"$>$"$19"\\newline$"$20"$";
	}
	if ($24=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen5="\\textit{"$17"}";
		gen5="\\textit{$\\textbf{"$17"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19"}$\\newline$<$"$18","$18"$>$"$20"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen5="\\textbf{"$17"}";
		gen5="$\\textbf{"$21"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23"}$\\newline$<$"$22","$23"$>$"$24"\\newline$"$25"$";
	}
	if ($29=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen6="\\textit{"$21"}";
		gen6="\\textit{$\\textbf{"$21"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23"}$\\newline$<$"$22","$23"$>$"$24"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen6="\\textbf{"21"}";
		gen6="$\\textbf{"$26"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28"}$\\newline$<$"$27","$28"$>$"$29"\\newline$"$30"$";
	}
	if ($34=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen7="\\textit{"$25"}";
		gen7="\\textit{$\\textbf{"$25"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27"}$\\newline$<$"$26","$27"$>$"$28"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen7="\\textbf{"$25"}";
		gen7="$\\textbf{"$31"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28+$32+$33"}$\\newline$<$"$32","$33"$>$"$34"\\newline$"$35"$";
	}
	if ($39=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen8="\\textit{"$29"}";
		gen8="\\textit{$\\textbf{"$29"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31"}$\\newline$<$"$30","$31"$>$"$32"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen8="\\textbf{"$29"}";
		gen8="$\\textbf{"$36"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28+$32+$33+$37+$38"}$\\newline$<$"$37","$38"$>$"$39"\\newline$"$40"$";
	}
	if ($44=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen9="\\textit{"$33"}";
		gen9="\\textit{$\\textbf{"$33"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35"}$\\newline$<$"$34","$35"$>$"$36"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen9="\\textbf{"$33"}";
		gen9="$\\textbf{"$41"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28+$32+$33+$37+$38+$42+$43"}$\\newline$<$"$42","$43"$>$"$44"\\newline$"$45"$";
	}
	if ($49=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen10="\\textit{"$37"}";
		gen10="\\textit{$\\textbf{"$37"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35+$38+$39"}$\\newline$<$"$38","$39"$>$"$40"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen10="\\textbf{"$37"}";
		gen10="$\\textbf{"$46"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28+$32+$33+$37+$38+$42+$43+$47+$48"}$\\newline$<$"$47","$48"$>$"$49"\\newline$"$50"$";
	}
	if ($54=="0")
	{
		#gen1="$\textit{$\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5}";
		#gen11="\\textit{"$41"}";
		gen11="\\textit{$\\textbf{"$41"}^{"$2+$3+$6+$7+$10+$11+$14+$15+$18+$19+$22+$23+$26+$27+$30+$31+$34+$35+$38+$39+$42+$43"}$\\newline$<$"$42","$43"$>$"$44"}";
	}
	else
	{
		#gen1="$\\textbf{10}^{70}$\newline$<$7,63$>\rightarrow$5";
		#gen11="\\textbf{"$41"}";
		gen11="$\\textbf{"$51"}^{"$2+$3+$7+$8+$12+$13+$17+$18+$22+$23+$27+$28+$32+$33+$37+$38+$42+$43+$47+$48+$52+$53"}$\\newline$<$"$52","$53"$>$"$54"\\newline$"$55"$";
	}
	
		
        print gen1" & "gen2" & "gen3" & "gen4" & "gen5" & "gen6" & "gen7" & "gen8" & "gen9" & "gen10" & "gen11 " & " $56" & "$57 "&" $58"\134\134" ;
        #if (NR =="1")
        #{
        #    print "\\hline";
        #    print "\\hline";
        #}


}' ctableHistory.txt >> ctable.tex

# finalizar la tabla de latex
echo "
\hline
%\end{tabular}

\end{tabularx}

\end{table}


" >> ctable.tex




echo "

\end{document}" >> ctable.tex


# *****************************************************************************
# compilar el archivo /salida.tex y generar el pdf

pdflatex ctable.tex
