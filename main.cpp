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
#define QTD_GERACOES 10
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

// estruturas para o front
struct EventoTorneio
{
    vector<int> fitnessParticipantes;
    int fitnessVencedor;
};

struct EventoCrossover
{
    vector<int> pai1;
    vector<int> pai2;

    int corte;

    vector<int> filho1;
    vector<int> filho2;
};

struct EventoMutacao
{
    int individuo;
    int gene;

    int antes;
    int depois;
};

struct HistoricoGeracao
{
    vector<Hab> populacao;

    vector<EventoTorneio> torneios;
    vector<EventoCrossover> crossovers;
    vector<EventoMutacao> mutacoes;

    Hab melhor;
};

vector<HistoricoGeracao> historico;

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

Hab selecionarPai(vector<Hab> &populacao, HistoricoGeracao &hist)
{
    int k = TAMANHO_TORNEIO;

    EventoTorneio evento;

    Hab melhor = populacao[rand() % populacao.size()];

    evento.fitnessParticipantes.push_back(
        melhor.fitness);

    for (int i = 1; i < k; i++)
    {
        Hab candidato = populacao[rand() % populacao.size()];

        evento.fitnessParticipantes.push_back(
            candidato.fitness);

        if (candidato.fitness < melhor.fitness)
        {
            melhor = candidato;
        }
    }

    evento.fitnessVencedor = melhor.fitness;

    hist.torneios.push_back(evento);

    return melhor;
};

vector<Hab> torneio(vector<Hab> &populacao, HistoricoGeracao &hist)
{
    vector<Hab> pais;

    while (pais.size() < POPULACAO)
    {
        pais.push_back(
            selecionarPai(populacao, hist));
    }

    return pais;
}

vector<Hab> crossover(vector<Hab> pais, HistoricoGeracao &hist)
{
    vector<Hab> filhos;

    int tamanho = pais[0].lotes.size();

    while (filhos.size() < POPULACAO)
    {
        Hab p1 = pais[rand() % pais.size()];

        Hab p2 = pais[rand() % pais.size()];

        Hab f1, f2;

        EventoCrossover evento;

        evento.pai1 = p1.lotes;
        evento.pai2 = p2.lotes;

        if (((double)rand() / RAND_MAX) < TAXA_CROSSOVER)
        {
            int corte = rand() % tamanho;

            evento.corte = corte;

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
            evento.corte = -1;

            f1 = p1;
            f2 = p2;
        }

        evento.filho1 = f1.lotes;
        evento.filho2 = f2.lotes;

        hist.crossovers.push_back(evento);

        filhos.push_back(f1);

        if (filhos.size() < POPULACAO)
        {
            filhos.push_back(f2);
        }
    }

    return filhos;
};

vector<Hab> mutacao(
    vector<Hab> populacao,
    int numMaquinas,
    HistoricoGeracao &hist)
{
    for (int i = 0; i < populacao.size(); i++)
    {
        for (int j = 0; j < populacao[i].lotes.size(); j++)
        {
            if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
            {
                EventoMutacao evento;

                evento.individuo = i;
                evento.gene = j;

                evento.antes = populacao[i].lotes[j];

                populacao[i].lotes[j] =
                    rand() % numMaquinas;

                evento.depois = populacao[i].lotes[j];

                hist.mutacoes.push_back(evento);
            }
        }
    }

    return populacao;
}

vector<Hab> proximaGeracao(vector<Hab> populacao, vector<Lote> &lotes, int numMaquinas, HistoricoGeracao &hist)
{
    hist.populacao = populacao;

    auto pais = torneio(populacao, hist);

    auto filhos = crossover(pais, hist);
    filhos = mutacao(filhos, numMaquinas, hist);
    filhos = avaliarPopulacao(filhos, lotes, numMaquinas);

    hist.melhor = filhos[0];

    return filhos;
}

Hab algoritmoGenetico(int numLotes, int numMaquinas, vector<Lote> &lotes, vector<Hab> populacao)
{
    populacao = avaliarPopulacao(populacao, lotes, numMaquinas);

    for (int i = 0; i < QTD_GERACOES; i++)
    {
        HistoricoGeracao geracao;

        auto filhos = proximaGeracao(populacao, lotes, numMaquinas, geracao);

        filhos.push_back(populacao[0]);

        filhos = avaliarPopulacao(filhos, lotes, numMaquinas);

        filhos.resize(POPULACAO);

        populacao = filhos;

        historico.push_back(geracao);
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
void salvarJSON(const vector<HistoricoGeracao> &historico, const vector<Lote> &lotes, int numLotes, int numMaquinas)
{
    ofstream file("dados.json");

    file << "{\n";

    file << "\"configuracao\": {\n";

    file << "\"numLotes\": "
         << numLotes
         << ",\n";

    file << "\"numMaquinas\": "
         << numMaquinas
         << "\n";

    file << "},\n";

    file << "\"lotes\": [\n";

    for (int i = 0; i < lotes.size(); i++)
    {
        file << "{";

        file << "\"id\": "
             << lotes[i].id
             << ",";

        file << "\"qtd\": "
             << lotes[i].qtd
             << ",";

        file << "\"tempoUnit\": "
             << lotes[i].tempoUnit
             << ",";

        file << "\"tempoTotal\": "
             << lotes[i].tempoTotal;

        file << "}";

        if (i < lotes.size() - 1)
            file << ",";

        file << "\n";
    }

    file << "],\n";

    for (int g = 0; g < historico.size(); g++)
    {
        const auto &geracao = historico[g];

        file << "\"geracao_" << (g + 1) << "\": {\n";

        // ================= POPULACAO =================
        file << "\"populacao\": [\n";

        for (int i = 0; i < geracao.populacao.size(); i++)
        {
            const auto &ind = geracao.populacao[i];

            file << "{";

            file << "\"fitness\": "
                 << ind.fitness
                 << ",";

            file << "\"cromossomo\": [";

            for (int j = 0; j < ind.lotes.size(); j++)
            {
                file << ind.lotes[j];

                if (j < ind.lotes.size() - 1)
                    file << ",";
            }

            file << "]";

            file << "}";

            if (i < geracao.populacao.size() - 1)
                file << ",";

            file << "\n";
        }

        file << "],\n";

        // ================= TORNEIO =================
        file << "\"torneio\": [\n";

        for (int i = 0; i < geracao.torneios.size(); i++)
        {
            const auto &t = geracao.torneios[i];

            file << "{";

            file << "\"participantes\":[";

            for (int j = 0; j < t.fitnessParticipantes.size(); j++)
            {
                file << t.fitnessParticipantes[j];

                if (j < t.fitnessParticipantes.size() - 1)
                    file << ",";
            }

            file << "],";

            file << "\"vencedor\":"
                 << t.fitnessVencedor;

            file << "}";

            if (i < geracao.torneios.size() - 1)
                file << ",";

            file << "\n";
        }

        file << "],\n";

        // ================= CROSSOVER =================
        file << "\"crossover\": [\n";

        for (int i = 0; i < geracao.crossovers.size(); i++)
        {
            const auto &c = geracao.crossovers[i];

            file << "{";

            file << "\"pai1\": [";
            for (int j = 0; j < c.pai1.size(); j++)
            {
                file << c.pai1[j];
                if (j < c.pai1.size() - 1)
                    file << ",";
            }
            file << "],";

            file << "\"pai2\": [";
            for (int j = 0; j < c.pai2.size(); j++)
            {
                file << c.pai2[j];
                if (j < c.pai2.size() - 1)
                    file << ",";
            }
            file << "],";

            file << "\"corte\": "
                 << c.corte
                 << ",";

            file << "\"filho1\": [";
            for (int j = 0; j < c.filho1.size(); j++)
            {
                file << c.filho1[j];
                if (j < c.filho1.size() - 1)
                    file << ",";
            }
            file << "],";

            file << "\"filho2\": [";
            for (int j = 0; j < c.filho2.size(); j++)
            {
                file << c.filho2[j];
                if (j < c.filho2.size() - 1)
                    file << ",";
            }
            file << "]";

            file << "}";

            if (i < geracao.crossovers.size() - 1)
                file << ",";

            file << "\n";
        }

        file << "],\n";

        // ================= MUTAÇÃO =================
        file << "\"mutacao\": [\n";

        for (int i = 0; i < geracao.mutacoes.size(); i++)
        {
            const auto &m = geracao.mutacoes[i];

            file << "{";
            file << "\"individuo\":" << m.individuo << ",";
            file << "\"gene\":" << m.gene << ",";
            file << "\"antes\":" << m.antes << ",";
            file << "\"depois\":" << m.depois;
            file << "}";

            if (i < geracao.mutacoes.size() - 1)
                file << ",";

            file << "\n";
        }

        file << "],\n";

        // ================= MELHOR =================
        file << "\"melhor\": {\n";

        file << "\"fitness\": "
             << geracao.melhor.fitness
             << ",\n";

        file << "\"cromossomo\": [";

        for (int i = 0; i < geracao.melhor.lotes.size(); i++)
        {
            file << geracao.melhor.lotes[i];

            if (i < geracao.melhor.lotes.size() - 1)
                file << ",";
        }

        file << "]\n";

        file << "}\n";

        file << "}";

        if (g < historico.size() - 1)
            file << ",";

        file << "\n";
    }

    file << "}\n";

    file.close();
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

    salvarJSON(historico, lotes,
               numLotes,
               numMaquinas);

    cout << "OK - JSON GERADO\n";

    return 0;
}