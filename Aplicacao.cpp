
/*
 *  Copyright (C) 2015 José Flávio de Souza Dias Júnior
 *  
 *  This file is part of Claritate - <http://www.joseflavio.com/claritate/>.
 *  
 *  Claritate is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  Claritate is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Claritate. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  Direitos Autorais Reservados (C) 2015 José Flávio de Souza Dias Júnior
 * 
 *  Este arquivo é parte de Claritate - <http://www.joseflavio.com/claritate/>.
 * 
 *  Claritate é software livre: você pode redistribuí-lo e/ou modificá-lo
 *  sob os termos da Licença Pública Menos Geral GNU conforme publicada pela
 *  Free Software Foundation, tanto a versão 3 da Licença, como
 *  (a seu critério) qualquer versão posterior.
 * 
 *  Claritate é distribuído na expectativa de que seja útil,
 *  porém, SEM NENHUMA GARANTIA; nem mesmo a garantia implícita de
 *  COMERCIABILIDADE ou ADEQUAÇÃO A UMA FINALIDADE ESPECÍFICA. Consulte a
 *  Licença Pública Menos Geral do GNU para mais detalhes.
 * 
 *  Você deve ter recebido uma cópia da Licença Pública Menos Geral do GNU
 *  junto com Claritate. Se não, veja <http://www.gnu.org/licenses/>.
 */

#include "Claritate.h"

void CSV_Arquivo( ifstream* origem, int** destino1, int** destino2, int largura );
int  CSV_Largura( ifstream* origem );

/* Exemplo de aplicação do Claritate */
int main( int argc, char *argv[] ) {

	const char *nomeArquivoAdjac = argv[1];
	const char *nomeArquivoOrdem = strcat( strcpy( (char*)malloc(256), nomeArquivoAdjac ), ".ordem" );
	
	ifstream entrada ( nomeArquivoAdjac );
	ofstream saida   ( nomeArquivoOrdem, ofstream::trunc );

	const int total = CSV_Largura( &entrada );

	int** adjacencia1 = new int*[total];
	int** adjacencia2 = new int*[total];

	for( int i = 0; i < total; i++ ){
		adjacencia1[i] = new int[total];
		adjacencia2[i] = new int[total];
	}

	CSV_Arquivo( &entrada, adjacencia1, adjacencia2, total );
	entrada.close();

	return Claritate( adjacencia1, adjacencia2, total, 10, 1000, &saida );

}

/* Arquivo de entrada no formato CSV */
void CSV_Arquivo( ifstream* origem, int** destino1, int** destino2, int largura ) {
	
	char numero[50], c;
	int d = 0, i = 0, j = 0, v;

	while( ! (*origem).eof() ){

		c = (*origem).get();

		switch( c ){
			case ',' :
			case ';' :
			case '|' :
			case ' ' :
			case '\t' :
			case '\n' :
				numero[d++] = '\0';
				v = strtol( numero, NULL, 10 );
				destino1[i][j] = v;
				destino2[i][j] = v;
				j++;
				d = 0;
				break;
			case '"' :
			case '\'' :
			case '\r' :
				break;
			default :
				numero[d++] = c;
				break;
		}

		if( j == largura ){
			i++;
			j = 0;
		}

	}

	if( d > 0 && j > 0 && j < largura ){
		numero[d++] = '\0';
		v = strtol( numero, NULL, 10 );
		destino1[i][j] = v;
		destino2[i][j] = v;
	}

}

/* Ordem da Matriz de Adjacência */
int CSV_Largura( ifstream* origem ){
	int total = 1;
	char c;
	while( ! (*origem).eof() ){
		c = (*origem).get();
		if( c == '\n' ) break;
		if( c == ',' || c == ';' || c == '|' || c == ' ' || c == '\t' ) total++;
	}
	(*origem).seekg( 0, (*origem).beg );
	return total;
}
