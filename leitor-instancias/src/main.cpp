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
    double cost;
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
    return custoInsercao;
}

bool CompararCusto (const InsertionInfo& a, const InsertionInfo& b){
    return a.custo < b.custo;
}

void ordenarEmOrdemCrescente(vector <InsertionInfo>& custoInsercao){
    sort(custoInsercao.begin(), custoInsercao.end(), CompararCusto); // ordena o vector com base no custo
}

void inserirNaSolucao(Solution& s, int noInserido, int arestaRemovida) {
    s.sequence.insert(s.sequence.begin() + arestaRemovida + 1, noInserido);
}




Solution Construcao(int n_vertices, vector<vector<int>>& c){
    Solution s;

    s.cost = 0;
    s.sequence = escolher3NosAleatorios(n_vertices);

    vector<int> CL = nosRestantes(n_vertices, s.sequence);

    while(!CL.empty()) {
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, c);
        ordenarEmOrdemCrescente(custoInsercao);

        for (int i = 0; i < custoInsercao.size(); i++) {
    }

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao[selecionado].noInserido, custoInsercao[selecionado].arestaRemovida);
        CL.erase(remove(CL.begin(), CL.end(), custoInsercao[selecionado].noInserido), CL.end());
    }
    return s;
}


bool bestImprovementSwap(Solution *s, vector<vector<int>>& c) {

    double bestDelta = 0;
    int best_i, best_j;

    for (int i = 1; i < s->sequence.size() - 1; i++) {
        int vi = s->sequence[i] - 1;
        int vi_next = s->sequence[i + 1] - 1;
        int vi_prev = s->sequence[i - 1] - 1;

        for (int j = i + 1; j < s->sequence.size()-1; j++) {
            int vj = s->sequence[j] - 1;
            int vj_next = s->sequence[j + 1] - 1;
            int vj_prev = s->sequence[j - 1] - 1;
            double delta;

            if (j == i+1){
                delta = -c[vi_prev][vi] - c[vi][vi_next] + c[vi_prev][vj] + c[vj][vi_next] + c[vj_prev][vj] - c[vj][vj_next]
                            + c[vj_prev][vi] + c[vi][vj_next];
            } else{
                delta = -c[vi_prev][vi] - c[vi][vi_next] + c[vi_prev][vj] + c[vj][vi_next] - c[vj_prev][vj] - c[vj][vj_next]
                            + c[vj_prev][vi] + c[vi][vj_next];
            }

            if (delta < bestDelta) {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0) {
        swap(s->sequence[best_i], s->sequence[best_j]);
        s->cost += bestDelta;
        return true;
    }

    return false;
}



bool bestImprovement2Opt(Solution *s, vector<vector<int>>& c){
    double bestDelta = 0;
    int best_i, best_j;
    for (int i=1; i < s->sequence.size() - 2; i++){
        int vi = s->sequence[i]-1;
        int vi_prev = s->sequence[i-1]-1;
        int vi_next = s->sequence[i+1]-1;

        for (int j= i+2; j < s->sequence.size() - 2; j++){
            int vj = s->sequence[j] - 1;
            int vj_prev = s->sequence[j-1] - 1;
            int vj_next = s->sequence[j+1] - 1;

            double delta = -c[vi_prev][vi] -c[vj_prev][vj] + c[vi_prev][vj_prev] + c[vi][vj];

            if (delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0){
        reverse(s->sequence.begin() + best_i, s->sequence.begin() + best_j);
        s->cost += bestDelta;
        return true;
    }

    return false;
}

bool bestImprovementOrOpt(Solution *s, int k, vector<vector<int>>& c){
    double bestDelta=0;
    int best_i, best_j;
    for (int i=1; i < s->sequence.size() - k; i++){
        int vi = s->sequence[i] - 1;
        int vi_next = s->sequence[i + 1] - 1;
        int vi_prev = s->sequence[i - 1] - 1;
        for (int j= i + k; j < s->sequence.size() - 1; j++){
            int vj = s->sequence[j] - 1;
            int vj_next = s->sequence[j + 1] - 1;
            int vj_prev = s->sequence[j - 1] - 1;
            double delta;

            if (k==1){
                delta = -c[vi_prev][vi] - c[vi][vi_next] + c[vi_prev][vi_next] - c[vj][vj_next] + c[vj][vi] + c[vi][vj_next];
            } else if (k == 2){
                int vm = s->sequence[i + 2] - 1;
                delta = -c[vi_prev][vi] - c[vi_next][vm] - c[vj][vj_next] + c[vi_prev][vm] + c[vi][vj_next] + c[vi_next][vj];
            } else if (k == 3){
                int vm = s->sequence[i + 2] - 1;
                int vm2 = s->sequence[i + 3] - 1;
                delta = -c[vi_prev][vi] - c[vm][vm2] - c[vj][vj_next] + c[vi_prev][vm2] + c[vi][vj_next] + c[vm][vj];
            }

            if (delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0){
        reverse(s->sequence.begin() + best_i, s->sequence.begin() + best_i + k);
	    rotate(s->sequence.begin() + best_i, s->sequence.begin() + best_i + k, s->sequence.begin() + best_j + 1);
        s->cost += bestDelta;
        return true;  
    }
    return false;
}

void BuscaLocal(Solution *s,  vector<vector<int>>& c){
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;
    while (NL.empty() == false){
        int n = rand() % NL.size();

        switch (NL[n]){
            case 1: improved = bestImprovementSwap(s, c); break;
            case 2: improved = bestImprovement2Opt(s, c); break;
            case 3: improved = bestImprovementOrOpt(s, 1, c); break;
            case 4: improved = bestImprovementOrOpt(s, 2, c); break;
            case 5: improved = bestImprovementOrOpt(s, 3, c); break;
        }

        if (improved)
            NL = {1, 2, 3, 4, 5};
        else
            NL.erase(NL.begin() + n);
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();

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

    BuscaLocal(&solucao, c);

    printf("\nApos BuscaLocal(): \n\n");

    for (int i = 0; i < solucao.sequence.size(); i++) {
        cout << solucao.sequence[i] << " -> ";
    }

    return 0;
}
