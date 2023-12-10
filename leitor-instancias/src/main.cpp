#include "Data.h"
#include <bits/stdc++.h>

using namespace std;

struct InsertionInfo {
        int noInserido; // no k a ser inserido
        int arestaRemovida; // aresta {i,j} na qual o no k sera inserido
        double custo; // delta ao inserir k na aresta {i,j}
};

typedef struct Solucao {
    vector<int> sequence;
    double valorObj;
} Solution;

vector<int> escolher3NosAleatorios(int n_vertices){
    vector <int> nos = {1, 1};
    int c=1;
    while (c <= 3){
        int no = rand() % n_vertices + 1; // escolhe um no aleatoriamente
        if (find(nos.begin(), nos.end(), no) == nos.end()){ // verifica se o no ja foi adicionado
            nos.insert(nos.begin() + c, no); // caso não tenha sido, o no é adicionado
            c++;
        }
    }

   return nos;
}

vector <int> nosRestantes (int n_verticies, vector <int> nos_escolhidos){
    vector <int> nos_restantes;
    for (int i=1; i <= n_verticies; i++){
        if (find(nos_escolhidos.begin(), nos_escolhidos.end(), i) == nos_escolhidos.end()){ // verifica se o no ja esta presente na solucao
            nos_restantes.push_back(i); // caso nao, adiciona os nos que faltam no vector
        }
    }
    return nos_restantes;
}

vector<InsertionInfo> calcularCustoInsercao(Solution& s, vector<int>& CL, vector<vector<int>>& c){
    vector<InsertionInfo> custoInsercao((s.sequence.size() - 1) * CL.size());
    int l = 0;

    for(int a = 0; a < s.sequence.size() - 1; a++) {
        int i = s.sequence[a];
        int j = s.sequence[a + 1];
        for (auto k : CL) {
            custoInsercao[l].custo = c[i-1][k-1] + c[j-1][k] - c[i-1][j-1];
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    }
    /*for (int i=0; i < custoInsercao.size(); i++){
            printf("CUSTO: %f    /    NO INSERIDO: %d    /   ARESTA REMOVIDA: %d\n", custoInsercao[i].custo, custoInsercao[i].noInserido, custoInsercao[i].arestaRemovida);
    }*/
    return custoInsercao;
}

bool CompararCusto (const InsertionInfo& a, const InsertionInfo& b){
    return a.custo < b.custo;
}

void ordenarEmOrdemCrescente(vector <InsertionInfo>& custoInsercao){
    sort(custoInsercao.begin(), custoInsercao.end(), CompararCusto); // ordena o vector com base no custo
}

void inserirNaSolucao(Solution& s, int noInserido, int arestaRemovida) {
    s.sequence.insert(s.sequence.begin() + arestaRemovida, noInserido);
}



Solution Construcao(int n_vertices, vector<vector<int>>& c){
    Solution s;

    s.sequence = escolher3NosAleatorios(n_vertices);

    vector<int> CL = nosRestantes(n_vertices, s.sequence);

    while(!CL.empty()) {
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, c);
        ordenarEmOrdemCrescente(custoInsercao);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao[selecionado].noInserido, custoInsercao[selecionado].arestaRemovida);
        CL.erase(remove(CL.begin(), CL.end(), custoInsercao[selecionado].noInserido), CL.end());
    }
    return s;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;

    vector<vector<int>> c(n, vector<int>(n, 0));
    for (int i=0; i < n; i++){
        for(int j=0; j < n; j++){
            c[i][j] = data.getDistance(i+1, j+1);
        }
    }

    Solution solucao = Construcao(n, c);

    cout << "\nSolucao Construida: ";
    for (int i = 0; i < solucao.sequence.size(); i++) {
        cout << solucao.sequence[i] << " -> ";
    }
    cout << solucao.sequence[0] << endl;

    return 0;
}
