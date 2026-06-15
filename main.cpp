#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <cmath>
#include <ctime>
#include <fstream>
#include <unordered_map>

#define MIN_MAQUINAS 2
#define MAX_MAQUINAS 5
#define MIN_LOTES 10
#define MAX_LOTES 30
#define MIN_SAPATOS 10
#define MAX_SAPATOS 50
#define MIN_TEMPO 1
#define MAX_TEMPO 25

#define TAMANHO_TORNEIO 3
#define QTD_GERACOES 100
#define POPULACAO 10
#define TAXA_CROSSOVER 0.8
#define TAXA_MUTACAO 0.05

using namespace std;

// STRUCT
struct Lote
{
    int id;
    int qtd;
    int tempoUnit;
    int tempoTotal;
};

struct Hab
{
    // Vai conter o id das máquinas que irão trabalhar com cada lote
    vector<int> lotes;

    // Contém o maior valor gasto por uma máquina para o habitante
    int fitness;
};

vector<Lote> parametrosIniciais(int *numLotes, int *numMaquinas)
{
    vector<Lote> lotes;
    Lote lote;

    do
    {
        cout << "Digite a quantidade de lotes: ";
        cin >> *numLotes;
    } while (*numLotes < MIN_LOTES || *numLotes > MAX_LOTES);

    do
    {
        cout << "Digite a quantidade de maquinas: ";
        cin >> *numMaquinas;
    } while (*numMaquinas < MIN_MAQUINAS || *numMaquinas > MAX_MAQUINAS);

    for (int i = 0; i < *numLotes; i++)
    {
        do
        {
            cout << "Quantidade de itens do " << (i + 1) << "º lote: ";
            cin >> lote.qtd;
        } while (lote.qtd < MIN_SAPATOS || lote.qtd > MAX_SAPATOS);

        do
        {
            cout << "Tempo gasto em cada item do " << (i + 1) << "º lote: ";
            cin >> lote.tempoUnit;
        } while (lote.tempoUnit < MIN_TEMPO || lote.tempoUnit > MAX_TEMPO);

        lote.id = i;
        lote.tempoTotal = lote.qtd * lote.tempoUnit;

        lotes.push_back(lote);
    }

    return lotes;
}

vector<Lote> parametrosAleatorios(int *numLotes, int *numMaquinas)
{
    vector<Lote> lotes;

    *numLotes =
        MIN_LOTES +
        rand() % (MAX_LOTES - MIN_LOTES + 1);

    *numMaquinas =
        MIN_MAQUINAS +
        rand() % (MAX_MAQUINAS - MIN_MAQUINAS + 1);

    cout << "\n=== DADOS GERADOS ===\n";
    cout << "Lotes: " << *numLotes << endl;
    cout << "Maquinas: " << *numMaquinas << endl;

    for (int i = 0; i < *numLotes; i++)
    {
        Lote lote;

        lote.id = i;

        lote.qtd =
            MIN_SAPATOS +
            rand() % (MAX_SAPATOS - MIN_SAPATOS + 1);

        lote.tempoUnit =
            MIN_TEMPO +
            rand() % (MAX_TEMPO - MIN_TEMPO + 1);

        lote.tempoTotal =
            lote.qtd * lote.tempoUnit;

        lotes.push_back(lote);

        cout
            << "Lote " << i
            << " -> Qtd: " << lote.qtd
            << ", Tempo Unit: " << lote.tempoUnit
            << ", Tempo Total: " << lote.tempoTotal
            << endl;
    }

    return lotes;
}

vector<Hab> geracaoInicial(int numLotes, int numMaquinas, vector<Lote> lotes)
{
    vector<Hab> habitantes;
    int maquina;

    for (int i = 0; i < POPULACAO; i++)
    {
        Hab habitante;
        for (int j = 0; j < numLotes; j++)
        {
            maquina = rand() % numMaquinas;
            habitante.lotes.push_back(maquina);
        }
        habitantes.push_back(habitante);
        habitante.lotes.clear();
    }

    return habitantes;
};

int avaliarIndividuo(Hab &individuo, vector<Lote> &lotes, int numMaquinas)
{
    vector<int> carga(numMaquinas, 0);

    for (int i = 0; i < lotes.size(); i++)
    {
        int maquina = individuo.lotes[i];

        carga[maquina] += lotes[i].tempoTotal;
    }

    return *max_element(carga.begin(), carga.end());
};

vector<Hab> avaliarPopulacao(vector<Hab> populacao, vector<Lote> &lotes, int numMaquinas)
{
    for (auto &ind : populacao)
    {
        ind.fitness = avaliarIndividuo(ind, lotes, numMaquinas);
    }

    sort(populacao.begin(), populacao.end(),
         [](Hab &a, Hab &b)
         {
             return a.fitness < b.fitness;
         });

    return populacao;
}

Hab selecionarPai(vector<Hab> &populacao)
{
    int k = TAMANHO_TORNEIO;

    Hab melhor = populacao[rand() % populacao.size()];

    for (int i = 1; i < k; i++)
    {
        Hab candidato = populacao[rand() % populacao.size()];

        if (candidato.fitness < melhor.fitness)
        {
            melhor = candidato;
        }
    }

    return melhor;
};

vector<Hab> torneio(vector<Hab> &populacao)
{
    vector<Hab> pais;

    while (pais.size() < POPULACAO)
    {
        pais.push_back(
            selecionarPai(populacao));
    }

    return pais;
}

vector<Hab> crossover(vector<Hab> pais)
{
    vector<Hab> filhos;

    int tamanho = pais[0].lotes.size();

    while (filhos.size() < POPULACAO)
    {
        Hab p1 = pais[rand() % pais.size()];

        Hab p2 = pais[rand() % pais.size()];

        Hab f1, f2;

        if (((double)rand() / RAND_MAX) < TAXA_CROSSOVER)
        {
            int corte = rand() % tamanho;

            for (int i = 0; i < tamanho; i++)
            {
                if (i < corte)
                {
                    f1.lotes.push_back(p1.lotes[i]);

                    f2.lotes.push_back(p2.lotes[i]);
                }
                else
                {
                    f1.lotes.push_back(p2.lotes[i]);

                    f2.lotes.push_back(p1.lotes[i]);
                }
            }
        }
        else
        {
            f1 = p1;
            f2 = p2;
        }

        filhos.push_back(f1);

        if (filhos.size() < POPULACAO)
        {
            filhos.push_back(f2);
        }
    }

    return filhos;
};

vector<Hab> mutacao(vector<Hab> populacao, int numMaquinas)
{
    for (auto &ind : populacao)
    {
        for (auto &gene : ind.lotes)
        {
            if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
            {
                gene = rand() % numMaquinas;
            }
        }
    }

    return populacao;
};

vector<Hab> proximaGeracao(vector<Hab> populacao, vector<Lote> &lotes, int numMaquinas)
{
    auto pais = torneio(populacao);

    auto filhos = crossover(pais);
    filhos = mutacao(filhos, numMaquinas);
    filhos = avaliarPopulacao(filhos, lotes, numMaquinas);

    return filhos;
}

Hab algoritmoGenetico(int numLotes, int numMaquinas, vector<Lote> &lotes, vector<Hab> populacao)
{
    populacao = avaliarPopulacao(populacao, lotes, numMaquinas);

    for (int i = 0; i < QTD_GERACOES; i++)
    {
        auto filhos = proximaGeracao(populacao, lotes, numMaquinas);

        filhos.push_back(populacao[0]);

        filhos = avaliarPopulacao(filhos, lotes, numMaquinas);

        filhos.resize(POPULACAO);

        populacao = filhos;
    }

    return populacao[0];
}

void exibirResultado(const Hab &melhor, const vector<Lote> &lotes, int numMaquinas)
{
    vector<int> carga(numMaquinas, 0);

    cout << "\n===== MELHOR SOLUCAO =====\n";
    cout << "Makespan: "
         << melhor.fitness
         << "\n\n";

    for (int maq = 0; maq < numMaquinas; maq++)
    {
        cout << "Maquina "
             << maq
             << ":\n";

        bool possuiLote = false;

        for (int i = 0; i < lotes.size(); i++)
        {
            if (melhor.lotes[i] == maq)
            {
                possuiLote = true;

                cout
                    << "  Lote "
                    << lotes[i].id
                    << " (tempo "
                    << lotes[i].tempoTotal
                    << ")\n";

                carga[maq] +=
                    lotes[i].tempoTotal;
            }
        }

        if (!possuiLote)
            cout << "  Nenhum lote\n";

        cout
            << "  Tempo total: "
            << carga[maq]
            << "\n\n";
    }
}

// ================= JSON =================
void salvarJSON(
    const Hab &melhor,
    const vector<Lote> &lotes)
{
    ofstream file("dados.json");

    file << "{\n";

    file << "\"fitness\": "
         << melhor.fitness
         << ",\n";

    file << "\"alocacao\": [\n";

    for (int i = 0; i < lotes.size(); i++)
    {
        file
            << "{"
            << "\"lote\":"
            << lotes[i].id
            << ","
            << "\"maquina\":"
            << melhor.lotes[i]
            << "}";

        if (i < lotes.size() - 1)
            file << ",";

        file << "\n";
    }

    file << "]\n";

    file << "}\n";
}

//================= MAIN =================
int main()
{
    srand(time(nullptr));
    int numMaquinas, numLotes;

    int opcao;

    cout << "1 - Inserir manualmente\n";
    cout << "2 - Gerar aleatoriamente\n";
    cout << "Opcao: ";
    cin >> opcao;

    vector<Lote> lotes;

    if (opcao == 1)
    {
        lotes = parametrosIniciais(&numLotes, &numMaquinas);
    }
    else
    {
        lotes = parametrosAleatorios(&numLotes, &numMaquinas);
    }

    auto gInicial = geracaoInicial(numLotes, numMaquinas, lotes);

    auto resultado = algoritmoGenetico(numLotes, numMaquinas, lotes, gInicial);

    exibirResultado(resultado, lotes, numMaquinas);

    // salvarJSON(lotes, historico);
    //
    // cout << "OK - JSON GERADO\n";
    return 0;
}