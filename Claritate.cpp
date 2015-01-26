
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

/*
 *  Claritate
 *  <http://www.joseflavio.com/claritate/>
 *
 *     "Alia claritas solis,
 *      alia claritas lunae et
 *      alia claritas stellarum;
 *      stella enim a stella differt in claritate." (1Cor 15:41)
 *
 *  Implementação de Referência
 *  Reference Implementation
 */
int Claritate( int** matriz1, int** matriz2, int total, int L1, int L2, ofstream* saida ) {

	// Inicialização

	int**         adjacencia   = matriz1;
	int**         distancia    = matriz2;
	int*          grau_entrada = new int[total];
	Centralidade* centralidade = new Centralidade[total];

	for( int i = 0; i < total; i++ ){
		grau_entrada[i]         = 0;
		centralidade[i].posicao = i;
		centralidade[i].valor   = 0;
	}

	bool orientado =
	Inicializar( adjacencia, distancia, grau_entrada, centralidade, total );

	// Dispersão

	qsort( centralidade, total, sizeof(Centralidade), CompararCentralidade );

	int ordem[total];
	int posicao[total], posicao_atual, posicao_nova;

	for( int i = 0, pos; i < total; i++ ){
		pos = centralidade[i].posicao;
		ordem[i] = pos;
		posicao[pos] = i;
	}

	for( int i = 0, j, filhos, media; i < total; i++ ){

		filhos = 0;
		media = 0;

		for( j = 0; j < total; j++ ){
			if( adjacencia[i][j] != DESCONEXO && ( orientado || grau_entrada[j] <= grau_entrada[i] ) ){
				filhos++;
				media += posicao[j];
			}
		}

		if( filhos == 0 ) continue;
		
		posicao_atual = posicao[i];
		posicao_nova = ceil( (float) media / filhos );

		Deslocar( posicao_atual, posicao_nova, ordem, total );
		for( j = 0; j < total; j++ ) posicao[ordem[j]] = j;

	}

	// Compressão

	long dispersao, dispersao_nova;

	dispersao = Dispersao( adjacencia, ordem, total, orientado );

	int s1, s2, s3;
	int v1, v2, v3;
	int dist1, dist2;
	int real1, real2;
	int novo1, novo2;
	int pivo;

	int melhorias = 0;
	int ciclos = 0;

	while( dispersao > 0 ){

		s1 = Sortear(total-1);
		s3 = Sortear(total-1);
		if( abs(s3-s1) <= 1 ) continue;
		if( s1 > s3 ){
			s2 = s1;
			s1 = s3;
			s3 = s2;
		}
		s2 = s1 + Sortear(s3-s1-2) + 1;

		v1 = ordem[s1];
		v2 = ordem[s2];
		v3 = ordem[s3];

		dist1 = distancia[v1][v2];
		dist2 = distancia[v2][v3];

		if( dist1 == DESCONEXO || dist2 == DESCONEXO ) continue;

		real1 = s2 - s1;
		real2 = s3 - s2;

		novo1 = ceil( ( ((float)dist1) / ( dist1 + dist2 ) ) * ( real1 + real2 ) );
		novo2 = real1 + real2 - novo1;

		pivo = Sortear(100);

		if( pivo <= 33 ){

			posicao_atual = s1;
			posicao_nova  = s2 - novo1;
			if( posicao_nova < 0 ) posicao_nova = 0;

		}else if( pivo <= 66 ){

			posicao_atual = s2;
			posicao_nova  = s1 + novo1;

		}else{

			posicao_atual = s3;
			posicao_nova  = s2 + novo2;
			if( posicao_nova >= total ) posicao_nova = total - 1;

		}

		Deslocar( posicao_atual, posicao_nova, ordem, total );

		dispersao_nova = Dispersao( adjacencia, ordem, total, orientado );

		if( dispersao_nova < dispersao ){
			dispersao = dispersao_nova;
			melhorias++;
		}else{
			Deslocar( posicao_nova, posicao_atual, ordem, total );
		}

		ciclos++;

		if( melhorias == L1 || ciclos == L2 ){
			saida->seekp( 0, saida->beg );
			ImprimirResultado( ordem, total, saida );
			saida->flush();
			melhorias = 0;
			ciclos = 0;
		}

	}

	return 0;

}

/*
 *  Algoritmos conjugados para inicialização do Claritate.
 *  Floyd, R. W. (1962). Algorithm 97: Shortest path. Communications of the ACM, 5(6):345–.
 *  Hage, P. and Harary, F. (1995). Eccentricity and centrality in networks. Social Networks, 17:57–63.
 */
bool Inicializar( int** adjacencia, int** distancia, int* grau_entrada, Centralidade* centralidade, int total ) {

	bool orientado = false;
	int i, j, k, a, b, distanciaij;

	for( i = 0; i < total; i++ ){

		for( j = 0; j < total; j++ ){

			// Floyd-Warshall
			distanciaij = distancia[i][j];
			for( k = 0; k < total; k++ ){
				a = distancia[i][k];
				b = distancia[k][j];
				b = a == DESCONEXO || b == DESCONEXO ? DESCONEXO : a + b;
				if( b != DESCONEXO && ( distanciaij == DESCONEXO || b < distanciaij ) ) distanciaij = b;
			}
			distancia[i][j] = distanciaij;

			// Centralidade por excentricidade
			if( distanciaij > centralidade[i].valor ) centralidade[i].valor = distanciaij;

			// Grafo orientado?
			if( ! orientado && adjacencia[i][j] != adjacencia[j][i] ) orientado = true;

			// Grau de entrada
			if( adjacencia[j][i] != DESCONEXO ) grau_entrada[i]++;

		}

		if( centralidade[i].valor != 0 ) centralidade[i].valor = 1 / centralidade[i].valor;

	}

	return orientado;

}

/* Deslocamento intraordem */
void Deslocar( int posicao_atual, int posicao_nova, int* ordem, int total ) {

	int valor = ordem[posicao_atual];

	if( posicao_atual <= posicao_nova ){
		for( int i = posicao_atual; i < posicao_nova; i++ ) ordem[i] = ordem[i+1];
	}else{
		for( int i = posicao_atual; i > posicao_nova; i-- ) ordem[i] = ordem[i-1];
	}

	ordem[posicao_nova] = valor;

}

/* Volume do sistema multiestelar */
long Dispersao( int** adjacencia, int* ordem, int total, bool orientado ) {

	long dispersao = 0;
	int i, j;

	if( ! orientado ){
		for( i = 0; i < (total-1); i++ ){
			for( j = i+1; j < total; j++ ){
				if( adjacencia[ordem[i]][ordem[j]] != DESCONEXO ){
					dispersao += (j-i);
				}
			}
		}
	}else{
		for( i = 0; i < total; i++ ){
			for( j = 0; j < total; j++ ){
				if( adjacencia[ordem[i]][ordem[j]] != DESCONEXO ){
					dispersao += (i>=j) ? (i-j) : (j-i);
				}
			}
		}
	}

	return dispersao;

}

/* Impressão de Resultado */
void ImprimirResultado( int* ordem, int total, ostream* saida ) {
	*saida << ordem[0] + 1;
	for( int i = 1; i < total; i++ ) *saida << ',' << ordem[i] + 1;
}

int CompararCentralidade( const void* a, const void* b ) {
	return ( ((Centralidade*)a)->valor - ((Centralidade*)b)->valor );
}

/* Sorteio */
inline int Sortear( int maximo ) {
	return rand() % ( maximo + 1 );
}
