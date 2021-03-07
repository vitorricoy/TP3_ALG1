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
    // Vetor de soma de prefixo para o cálculo do valor da soma de um intervalo em tempo constante
    vector<int> somaTempoViagem(n+1);

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

    // Calcula o vetor de soma de prefixos do tempo das viagens
    somaTempoViagem[0] = 0;
    for(int I=1; I<=n; I++) {
        somaTempoViagem[I] = tempoViagem[I-1] + somaTempoViagem[I-1];
    }

    // Declara a matriz usada para guardar os resultados da programação dinâmica
    // Foi usada uma otimização de espaço em que apenas os dados da última escala é guardado
    // Isso é possível pois é necessário apenas os dados da escala anterior para calcular os dados da seguinte
    // Os casos base da equação de Bellman já estão cobertos pela inicialização com 0.0
    vector<vector<double> > dp(2, vector<double>(n+1, 0.0));
	
    // Implementa a equação de Bellman da solução do problema
    for(int escalaAtual = n-1; escalaAtual >= 0; escalaAtual--) {
        // Iteração acontece de escala atual até a primeira escala que pode ter um efeito na escala atual
        int primeiraEscalaComEfeito = max(0, escalaAtual-d+1);
        for(int primeiraEscalaDesconto = escalaAtual; primeiraEscalaDesconto >= primeiraEscalaComEfeito; primeiraEscalaDesconto--) {
            // Número de escalas com desconto entre a escala atual e a escala em que o desconto se iniciou
            int numeroDescontosConsecutivos = escalaAtual-primeiraEscalaDesconto;
            // O tempo passado desde o embarque na primeira escala do desconto
            int tempoUltimoDesconto = somaTempoViagem[escalaAtual]-somaTempoViagem[primeiraEscalaDesconto];
            // Inicializa o custo da dp com o valor gasto na passagem da escala atual
            dp[escalaAtual%2][primeiraEscalaDesconto] = (1.0-descontoPercentual[numeroDescontosConsecutivos])*custoBilhete[escalaAtual];
            if(tempoUltimoDesconto+tempoViagem[escalaAtual] >= t || numeroDescontosConsecutivos >= d-1) {
                // Caso não seja possível continuar o desconto acumulado para a próxima escala
                // Adiciona o custo a partir da próxima escala iniciando um novo ciclo de descontos
                dp[escalaAtual%2][primeiraEscalaDesconto] += dp[(escalaAtual+1)%2][escalaAtual+1];
            } else {
                // Caso seja possível continuar o desconto acumulado para a próxima escala
                // Escolhe entre iniciar um novo ciclo de descontos ou continuar com o desconto atual
                dp[escalaAtual%2][primeiraEscalaDesconto] += min(dp[(escalaAtual+1)%2][escalaAtual+1], dp[(escalaAtual+1)%2][primeiraEscalaDesconto]);
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