#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    // Quantidade de escalas
    int n;
    // Quantidade máxima de escalas com descontos cumulativos no intervalo T 
    int d;
    // Tempo máximo para aplicação de descontos
    int t;
    // Lê as três variáveis
    cin >> n >> d >> t;
    // Declara o vetor que guarda os descontos para cada escala consecutiva
    vector<double> descontoPercentual(d);
    // Declara o vetor que guarda o tempo que leva o translado de cada escala
    vector<int> tempoViagem(n);
    // Declara o vetor que guarda o custo do bilhete de cada escala
    vector<double> custoBilhete(n);

    // Lê os valores de desconto
    for(int I=0; I<d; I++) {
        cin >> descontoPercentual[I];
        descontoPercentual[I]/=100;
    }

    // Soma os valores do desconto com os anteriores a ele para cada  
    // valor de desconto indicar o desconto total ao ser acumulado
    for(int I=1; I<d; I++) {
        descontoPercentual[I] += descontoPercentual[I-1];
        descontoPercentual[I] = min(descontoPercentual[I], 1.0);
    }

    // Lê o tempo e preço do bilhete de cada escala
    for(int I=0; I<n; I++) {
        cin >> tempoViagem[I] >> custoBilhete[I];
    }

    // Declara a matriz da programação dinâmica
    double dp[2][d][t];

    // Inicializa os valores do caso base
    for(int J=d-1; J>=0; J--) {
        for(int K=t-1; K>=0; K--) {
            dp[n%2][J][K] = 0;
        }
    }

    // Implementa a equação de bellman para preencher a matriz dp
    // Vale notar que, para utilizar menos espaço, apenas os valores 
    // da escala anterior são salvos
    for(int escala=n-1; escala>=0; escala--) {
        for(int desconto=d-1; desconto>=0; desconto--) {
            for(int tempoDesconto=t-1; tempoDesconto>=0; tempoDesconto--) {
                double precoBilhete = (1.0-descontoPercentual[desconto])*custoBilhete[escala];
                if(tempoDesconto+tempoViagem[escala] >= t || desconto == d-1) {
                    dp[escala%2][desconto][tempoDesconto] = precoBilhete + dp[(escala+1)%2][0][0];
                } else {
                    dp[escala%2][desconto][tempoDesconto] = precoBilhete + min(dp[(escala+1)%2][0][0], dp[(escala+1)%2][desconto+1][tempoDesconto+tempoViagem[escala]]);
                }
            }
        }
    }
    
    // Exibe o resultado calculado
    cout << fixed << setprecision(2) << dp[0][0][0] << endl;
    return 0;
}