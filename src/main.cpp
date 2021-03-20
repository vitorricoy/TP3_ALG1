#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Retorna o valor da soma do intervalo [inicio, fim] de um vetor de soma de prefixos
// 'vetorSoma' é 1-based, já 'inicio' e 'fim' são 0-based
int calculaSomaIntervalo(int inicio, int fim, vector<int>& vetorSoma) {
    return vetorSoma[fim+1]-vetorSoma[inicio];
}

int main() {
    // Quantidade de escalas
    int n;
    // Quantidade máxima de escalas com descontos cumulativos 
    int d;
    // Período de tempo máximo da duração dos descontos
    int t;

    // Lê as três variáveis
    cin >> n >> d >> t;

    // Vetor que guarda os descontos para cada escala consecutiva
    vector<double> descontoPercentual(d);
    // Vetor que guarda o tempo que leva o translado de cada escala
    vector<int> tempoViagem(n);
    // Vetor que guarda o custo do bilhete de cada escala
    vector<double> custoBilhete(n);
    // Vetor de soma de prefixo para o tempo de viagem
    // Usado para calcular o tempo gasto para percorrer um intervalo de escalas
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
        // Caso um desconto chegaria a mais de 100%, o mantêm em 100%
        descontoPercentual[I] = min(descontoPercentual[I], 1.0);
    }


    // Calcula o vetor de soma de prefixos do tempo das viagens
    // Vetor 1-based para facilitar o cálculo da soma de um intervalo
    somaPrefixoTempo[0] = 0;
    for(int I=1; I<=n; I++) {
        somaPrefixoTempo[I] = tempoViagem[I-1] + somaPrefixoTempo[I-1];
    }

    // Declara a matriz usada para guardar os resultados da programação dinâmica
    // Foi usada uma otimização de espaço em que apenas os dados da escala atual e da seguinte são guardados
    // Isso é possível pois precisamos apenas dos dados da escala seguinte para calcular os dados da anterior
    // O caso base da equação de Bellman já está cobertos pela inicialização com 0.0
    vector<vector<double> > pd(2, vector<double>(d+1, 0.0));
	
    // Implementa a equação de Bellman da solução do problema
    // Itera pelas escalas que devem ser tomadas, em ordem inversa
    for(int escalaAtual = n-1; escalaAtual >= 0; escalaAtual--) {
        // Itera pelos possíveis números de descontos já utilizados no ciclo de descontos atual
        for(int descontosUsados = d; descontosUsados >= 0; descontosUsados--) {
            // Calcula a escala em que o desconto se iniciou
            int escalaDesconto = escalaAtual-descontosUsados;
            // Se o desconto teria se iniciado em uma escala inválida
            // Ou caso o desconto ultrapasse o limite de decontos ao ser utilizado em 'escalaAtual',
            // temos um caso inválido
            if(escalaDesconto < 0 || descontosUsados == d) {
                pd[escalaAtual%2][descontosUsados] = 1e20; // 1e20 usado como infinito
                continue;
            }
            // Calcula o tempo passado desde o embarque na primeira escala do desconto até o embarque na escala atual
            // Ou seja, a soma dos tempos de viagem do intervalo [escalaDesconto, escalaAtual)
            int tempoUltimoDesconto = calculaSomaIntervalo(escalaDesconto, escalaAtual-1, somaPrefixoTempo);
            
            // Verifica se o tempo gasto entre 'escalaDesconto' e 'escalaAtual' permite que o desconto seja usado
            if(tempoUltimoDesconto < t) {    
                // Inicializa o valor dessa instância com o valor gasto na passagem da escala atual
                pd[escalaAtual%2][descontosUsados] = (1.0-descontoPercentual[descontosUsados])*custoBilhete[escalaAtual];
                // Escolhe entre iniciar um novo ciclo de descontos ou continuar com o desconto atual
                pd[escalaAtual%2][descontosUsados] += min(pd[(escalaAtual+1)%2][0], pd[(escalaAtual+1)%2][descontosUsados+1]);
            } else {
                // É impossível que 'escalaAtual' tenha tido 'descontosUsados' descontos consecutivos
                // Isso acontece pelo tempo de transporte entre essas escalas exceder o tempo máximo de duração um desconto
                pd[escalaAtual%2][descontosUsados] = 1e20; // 1e20 usado como infinito
            }
        }
    }

    // Salva a resposta obtida pela programação dinâmica
    double resposta = pd[0][0];

    // Exibe o resultado calculado com duas casas decimais
    cout << fixed << setprecision(2) << resposta << endl;
    return 0;
}
