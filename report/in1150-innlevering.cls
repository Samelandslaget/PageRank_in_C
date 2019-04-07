\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{in1150-innlevering}[2018/09/14 Innlevering for IN1150 -- Logiske metoder]
\LoadClass[12pt,english,a4paper]{article}

% Programeringssitat
\RequirePackage{listings}

% Språk og tegn
\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage[english]{babel}
\RequirePackage{tikz}

% Fonter
\RequirePackage{tgpagella}
\RequirePackage[small,euler-digits]{eulervm}

% Lister
\RequirePackage{enumitem}
\newlist{deloppgaver}{enumerate}{1}
\setlist[deloppgaver]{label={\textbf{(\alph*)}},leftmargin=*}

% Matematikk
\RequirePackage{amsmath,amssymb}
\RequirePackage{upgreek}

% Noen nyttige forkortelser
\newcommand{\set}[1]{\ensuremath{\{#1\}}}
\newcommand{\tuple}[1]{\ensuremath{\langle #1 \rangle}}
\newcommand{\imp}{\ensuremath{\rightarrow}}
\newcommand{\union}{\ensuremath{\cup}}
\newcommand{\snitt}{\ensuremath{\cap}}