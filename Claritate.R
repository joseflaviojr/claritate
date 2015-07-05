

#  Copyright (C) 2015 José Flávio de Souza Dias Júnior
#  
#  This file is part of Claritate - <http://www.joseflavio.com/claritate/>.
#  
#  Claritate is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  Claritate is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public License
#  along with Claritate. If not, see <http://www.gnu.org/licenses/>.


#  Direitos Autorais Reservados (C) 2015 José Flávio de Souza Dias Júnior
#  
#  Este arquivo é parte de Claritate - <http://www.joseflavio.com/claritate/>.
#  
#  Claritate é software livre: você pode redistribuí-lo e/ou modificá-lo
#  sob os termos da Licença Pública Menos Geral GNU conforme publicada pela
#  Free Software Foundation, tanto a versão 3 da Licença, como
#  (a seu critério) qualquer versão posterior.
#  
#  Claritate é distribuído na expectativa de que seja útil,
#  porém, SEM NENHUMA GARANTIA; nem mesmo a garantia implícita de
#  COMERCIABILIDADE ou ADEQUAÇÃO A UMA FINALIDADE ESPECÍFICA. Consulte a
#  Licença Pública Menos Geral do GNU para mais detalhes.
#  
#  Você deve ter recebido uma cópia da Licença Pública Menos Geral do GNU
#  junto com Claritate. Se não, veja <http://www.gnu.org/licenses/>.


library(igraph)
library(grid)


#  Claritate
#  <http://www.joseflavio.com/claritate/>
#  
#     "Alia claritas solis,
#      alia claritas lunae et
#      alia claritas stellarum;
#      stella enim a stella differt in claritate." (1Cor 15:41)
#  
#  Implementação de Referência
#  Reference Implementation
#
Claritate <- function( grafo, visualizar=TRUE, L1=10, L2=1000 ){

    # Inicialização

    adjacencia <- as.matrix(grafo[])
    total      <- nrow(adjacencia)
    orientado  <- is.directed(grafo)
    
    if( visualizar ) Claritate.visualizar(adjacencia)

    distancia  <- shortest.paths(grafo, mode="out")
    distancia  <- apply( distancia, c(1,2), function(x){ if(x==Inf) 0 else x } )
    posicao    <- eccentricity(grafo, mode="out")
    posicao    <- unlist(lapply( posicao, function(x){ if(x!=0) (1/x) else 0 } ))
    grau       <- degree(grafo, mode="in")

    # Dispersão

    ordem <- order(posicao)
    for( p in 1:total ) posicao[ordem[p]] <- p;

    for( i in 1:total ){
        
        filhos <- 0
        media <- 0
        
        for( j in 1:total ){
            if( adjacencia[i,j] != 0 && ( orientado || grau[j] <= grau[i] ) ){
                filhos <- filhos + 1
                media <- media + posicao[j]
            }
        }
        
        if( filhos == 0 ) next
        
        posicao_atual <- posicao[i]
        posicao_nova <- ceiling(media/filhos)

        ordem <- Claritate.deslocar( posicao_atual, posicao_nova, ordem )
        for( p in 1:total ) posicao[ordem[p]] <- p;

    }

    if( visualizar ) Claritate.visualizar(adjacencia[ordem,ordem])

    # Compressão

    dispersao <- Claritate.dispersao( adjacencia[ordem,ordem], total, orientado )

    melhorias <- 0
    ciclos    <- 0

    while( dispersao > 0 ){

        sorteio <- sample.int(total,size=3)
        sorteio_ordem <- order(sorteio)
        s1 <- sorteio[sorteio_ordem[1]]
        s2 <- sorteio[sorteio_ordem[2]]
        s3 <- sorteio[sorteio_ordem[3]]

        v1 <- ordem[s1]
        v2 <- ordem[s2]
        v3 <- ordem[s3]

        dist1 <- distancia[v1,v2]
        dist2 <- distancia[v2,v3]

        if( dist1 == 0 || dist2 == 0 ) next

        real1 <- s2 - s1
        real2 <- s3 - s2

        novo1 <- ceiling((dist1/(dist1+dist2))*(real1+real2))
        novo2 <- real1+real2-novo1

        pivo <- runif(1,0,1)

        if( pivo <= 0.33 ){
            ordem_nova <- Claritate.deslocar( s1, if((s2-novo1)<1) 1 else s2-novo1, ordem )

        }else if( pivo <= 0.66 ){
            ordem_nova <- Claritate.deslocar( s2, s1+novo1, ordem )

        }else{
            ordem_nova <- Claritate.deslocar( s3, if((s2+novo2)>total) total else s2+novo2, ordem )
        }

        dispersao_nova <- Claritate.dispersao( adjacencia[ordem_nova,ordem_nova], total, orientado )

        if( dispersao_nova < dispersao ){
            ordem <- ordem_nova
            dispersao <- dispersao_nova
            melhorias <- melhorias + 1
        }

        ciclos <- ciclos + 1

        if( melhorias == L1 || ciclos == L2 ){
            if( visualizar ) Claritate.visualizar(adjacencia[ordem,ordem])
            melhorias <- 0
            ciclos    <- 0
        }

    }

    ordem

}


# Deslocamento intraordem
# 
Claritate.deslocar <- function( posicao_atual, posicao_nova, ordem ){
    if( posicao_atual >= posicao_nova ){
        ordem <- append( ordem, ordem[posicao_atual], after=posicao_nova-1 )
        ordem[-(posicao_atual+1)]
    }else{
        ordem <- append( ordem, ordem[posicao_atual], after=posicao_nova )
        ordem[-posicao_atual]
    }
}

# Volume do sistema multiestelar
# 
Claritate.dispersao <- function( adjacencia, total, orientado ){
    
    dispersao <- 0

    if( ! orientado ){
        for( i in 1:(total-1) ){
            for( j in (i+1):total ){
                if( adjacencia[i,j] != 0 ){
                    dispersao <- dispersao + (j-i)
                }
            }
        }
    }else{
        for( i in 1:total ){
            for( j in 1:total ){
                if( adjacencia[i,j] != 0 ){
                    dispersao <- dispersao + abs(j-i)
                }
            }
        }
    }

    dispersao

}


Claritate.visualizar <- function( matriz ){
    base <- max(matriz)
    grid.raster( apply( matriz, c(1,2), function(x){ 1 - x / base } ), interpolate=FALSE )
}

