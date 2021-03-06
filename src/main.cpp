#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

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
    // Soma tempo viagem
    vector<int> somaTempoViagem(n+1);

    // Lê os valores de desconto
    for(int I=0; I<d; I++) {
        //cin >> descontoPercentual[I];
        descontoPercentual[I] = 0;
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
        //cin >> tempoViagem[I] >> custoBilhete[I];
        tempoViagem[I] = custoBilhete[I] = 1;
    }
    somaTempoViagem[0] = 0;
    for(int I=1; I<=n; I++) {
        somaTempoViagem[I] = tempoViagem[I-1] + somaTempoViagem[I-1];
    }

    double dp[2][n+1];

    for(int ultPego = n-1; ultPego>=0; ultPego--) {
        dp[n%2][ultPego] = 0;
    }

    for(int at = n-1; at >= 0; at--) {
        for(int ultPego = at; ultPego >= 0; ultPego--) {

            int numeroDescontosConsecutivos = at-ultPego;
            int tempoUltimoDesconto = somaTempoViagem[at]-somaTempoViagem[ultPego];

            dp[at%2][ultPego] = (1.0-descontoPercentual[numeroDescontosConsecutivos])*custoBilhete[at];
            if(tempoUltimoDesconto+tempoViagem[at] >= t || numeroDescontosConsecutivos == d-1) {
                dp[at%2][ultPego] += dp[(at+1)%2][at+1];
            } else {
                dp[at%2][ultPego] += min(dp[(at+1)%2][at+1], dp[(at+1)%2][ultPego]);
            }
        }
    }
    
    // Trunca o resultado para duas casas decimais
    double resultado = dp[0][0];
    resultado*=100;
    resultado = trunc(resultado);
    resultado = resultado/100;

    // Exibe o resultado calculado
    cout << fixed << setprecision(2) << resultado << endl;
    return 0;
}