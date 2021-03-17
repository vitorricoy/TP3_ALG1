#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <sstream>

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
    vector<int> somaPrefixoTempo(n+1);

    // Lê os valores de desconto
    for(int I=0; I<d; I++) {
        cin >> descontoPercentual[I];
        // Converte o valor do desconto de porcentagem para decimal
        descontoPercentual[I]/=100;
    }

    // Lê o tempo e preço do bilhete de cada escala
    for(int I=0; I<n; I++) {
        cin >> tempoViagem[I] >> custoBilhete[I];
    }
    
    // Soma os valores do desconto com os anteriores a ele para cada  
    // valor de desconto indicar o desconto total ao ser acumulado
    for(int I=1; I<d; I++) {
        descontoPercentual[I] += descontoPercentual[I-1];
        // Caso um desconto chegaria a mais de 100%, o mantêm no 100% de desconto
        descontoPercentual[I] = min(descontoPercentual[I], 1.0);
    }


    // Calcula o vetor de soma de prefixos do tempo das viagens
    somaPrefixoTempo[0] = 0;
    for(int I=1; I<=n; I++) {
        somaPrefixoTempo[I] = tempoViagem[I-1] + somaPrefixoTempo[I-1];
    }

    // Declara a matriz usada para guardar os resultados da programação dinâmica
    // Foi usada uma otimização de espaço em que apenas os dados da última escala é guardado
    // Isso é possível pois é necessário apenas os dados da escala anterior para calcular os dados da seguinte
    // O caso base da equação de Bellman já está cobertos pela inicialização com 0.0
    vector<vector<double> > pd(2, vector<double>(n+1, 0.0));
	
    // Implementa a equação de Bellman da solução do problema
    for(int escalaAtual = n-1; escalaAtual >= 0; escalaAtual--) {
        // Iteração acontece de escala atual até a primeira escala, que pode ser onde se iniciou o desconto
        for(int escalaDesconto = escalaAtual; escalaDesconto >= 0; escalaDesconto--) {
            // Número de escalas com desconto entre a escala atual e a escala em que o desconto se iniciou
            int descontosJaUsados = escalaAtual-escalaDesconto;
            // O tempo passado desde o embarque na primeira escala do desconto até o embarque na escala atual
            // Ou seja, a soma dos tempos de viagem do intervalo [escalaDesconto, escalaAtual)
            int tempoUltimoDesconto = somaPrefixoTempo[escalaAtual]-somaPrefixoTempo[escalaDesconto];
            
            // Verifica se o tempo gasto entre escalaDesconto e escalaAtual permite que o desconto seja usado
            // e se não se atingiu o número máximo de descontos entre escalaDesconto e escalaAtual 
            if(tempoUltimoDesconto < t && descontosJaUsados < d) {    
                // Inicializa o valor dessa instância com o valor gasto na passagem da escala atual
                pd[escalaAtual%2][escalaDesconto] = (1.0-descontoPercentual[descontosJaUsados])*custoBilhete[escalaAtual];
                // Escolhe entre iniciar um novo ciclo de descontos ou continuar com o desconto atual
                pd[escalaAtual%2][escalaDesconto] += min(pd[(escalaAtual+1)%2][escalaAtual+1], pd[(escalaAtual+1)%2][escalaDesconto]);
            } else {
                // É impossível se ter um desconto para escalaAtual iniciado em escalaDesconto
                // Isso acontece pelo tempo de transporte entre essas escalas exceder o tempo máximo de um desconto
                // ou por se ter atingido o número máximo de descontos consecutivos
                pd[escalaAtual%2][escalaDesconto] = 1e20; // 1e20 usado como infinito
            }
        }
    }
    
    // Trunca o resultado para duas casas decimais
    double resultado = pd[0][0];
    stringstream ss;
    ss << fixed;
    ss.precision(3);
    ss << resultado;
    string resposta = ss.str().substr(0, ss.str().size()-1);
    // Exibe o resultado calculado
    cout << resposta << endl;
    return 0;
}