
/*
 *  Copyright (C) 2015 Jos� Fl�vio de Souza Dias J�nior
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
 *  Direitos Autorais Reservados (C) 2015 Jos� Fl�vio de Souza Dias J�nior
 * 
 *  Este arquivo � parte de Claritate - <http://www.joseflavio.com/claritate/>.
 * 
 *  Claritate � software livre: voc� pode redistribu�-lo e/ou modific�-lo
 *  sob os termos da Licen�a P�blica Menos Geral GNU conforme publicada pela
 *  Free Software Foundation, tanto a vers�o 3 da Licen�a, como
 *  (a seu crit�rio) qualquer vers�o posterior.
 * 
 *  Claritate � distribu�do na expectativa de que seja �til,
 *  por�m, SEM NENHUMA GARANTIA; nem mesmo a garantia impl�cita de
 *  COMERCIABILIDADE ou ADEQUA��O A UMA FINALIDADE ESPEC�FICA. Consulte a
 *  Licen�a P�blica Menos Geral do GNU para mais detalhes.
 * 
 *  Voc� deve ter recebido uma c�pia da Licen�a P�blica Menos Geral do GNU
 *  junto com Claritate. Se n�o, veja <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <fstream>

using namespace std;

#define DESCONEXO 0

struct Centralidade {
	int   posicao;
	float valor;
};

int  Claritate( int** matriz1, int** matriz2, int total, int L1, int L2, ofstream* saida );
bool Inicializar( int** adjacencia, int** distancia, int* grau_entrada, Centralidade* centralidade, int total );
void Deslocar( int posicao_atual, int posicao_nova, int* ordem, int total );
long Dispersao( int** adjacencia, int* ordem, int total, bool orientado );
void ImprimirResultado( int* ordem, int total, ostream* saida );
int  CompararCentralidade( const void* a, const void* b );
inline int Sortear( int maximo );

/*
 *
 *     "Alia claritas solis,
 *      alia claritas lunae et
 *      alia claritas stellarum;
 *      stella enim a stella differt in claritate." (1Cor 15:41)
 *
 */
