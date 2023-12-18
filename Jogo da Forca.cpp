#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <set>

// Classe Base
class Jogo {
protected:
    std::map<std::string, std::vector<std::string>> banco_palavras;
    std::string palavra_atual;
    std::string mostrar_palavra;
    int maxErros;
    int erros;
    char* letrasUsadas; // Ponteiro para um array dinâmico
    int capacidadeLetrasUsadas; // Tamanho atual do array
    std::string tema;

    static std::set<std::string> palavrasUsadas; // Armazena palavras já utilizadas

    void CarregarPalavras(std::string filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            return;
        }

        std::string linha, tema;
        while (getline(file, linha)) {
            if (linha.empty()) continue;
            if (linha.back() == ':') {
                tema = linha.substr(0, linha.size() - 1);
                banco_palavras[tema] = std::vector<std::string>();
            } else {
                banco_palavras[tema].push_back(linha);
            }
        }

        if (banco_palavras.empty()) {
            std::cerr << "Nenhuma palavra carregada do arquivo." << std::endl;
        }
    }

    void escolherPalavra() {
        do {
            int indiceTema = rand() % banco_palavras.size();
            auto iterador = banco_palavras.begin();
            std::advance(iterador, indiceTema);
            tema = iterador->first;
            const std::vector<std::string>& palavras = iterador->second;
            palavra_atual = palavras[rand() % palavras.size()];
        } while (palavrasUsadas.find(palavra_atual) != palavrasUsadas.end()); // Verifica se a palavra já foi usada

        palavrasUsadas.insert(palavra_atual); // Adiciona a palavra atual ao conjunto de usadas

        mostrar_palavra = "";
        for (char c : palavra_atual) {
            mostrar_palavra += (c == ' ' ? "  " : "_ ");
        }
        mostrar_palavra.pop_back();
    }

    void Mostrar(int ErrosdoJogadorAtual) { 
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Tema: " << tema << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Palavra: ";
        for (char c : mostrar_palavra) {
            std::cout << c;  // Imprime a palavra com espaços
        }
        std::cout << std::endl;
        std::cout << "Erros: " << ErrosdoJogadorAtual << " de " << maxErros << std::endl;
        std::cout << "Letras utilizadas: " << letrasUsadas << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    }

    // Método para verificar se o caractere é uma letra válida de A a Z
    bool ValidarLetra(char letra) {
        return letra >= 'A' && letra <= 'Z';
    }

public:
    Jogo(std::string filename) : capacidadeLetrasUsadas(10) {
        CarregarPalavras(filename);
        srand(time(NULL));
        letrasUsadas = new char[capacidadeLetrasUsadas]; // Alocando memória
        letrasUsadas[0] = '\0'; // Inicializar com string vazia
    }
    virtual void jogar() = 0;

    virtual ~Jogo() {
        delete[] letrasUsadas; // Liberando memória alocada
    }

    
};

std::set<std::string> Jogo::palavrasUsadas;


// Classe JogoSolo
class JogoSolo : public Jogo {
public:
    JogoSolo(std::string filename) : Jogo(filename) {
        maxErros = 6;
    }

    void jogar() override {
        int pontos = 0;
        bool Jogardenovo = true;
        while (Jogardenovo) {
            erros = 0;
            letrasUsadas[0] = '\0'; // Reinicializa o array
            escolherPalavra();  // Escolhe uma nova palavra e tema
            while (erros < maxErros && mostrar_palavra.find('_') != std::string::npos) {
                Mostrar(erros);
                char tentativa;
                std::cout << "Escolha uma letra: ";
                std::cin >> tentativa;
                tentativa = toupper(tentativa);

                if (!ValidarLetra(tentativa)) {
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(letrasUsadas).find(tentativa) != std::string::npos) {
                    std::cout << "Letra já utilizada. Tente outra.\n";
                    continue;
                }

                size_t tamanho = strlen(letrasUsadas);
                if (tamanho >= capacidadeLetrasUsadas - 1) {
                    capacidadeLetrasUsadas *= 2;
                    char* temp = new char[capacidadeLetrasUsadas];
                    strcpy(temp, letrasUsadas);
                    delete[] letrasUsadas;
                    letrasUsadas = temp;
                }

                letrasUsadas[tamanho] = tentativa;
                letrasUsadas[tamanho + 1] = '\0';

                bool letraCerta = false;
                for (size_t i = 0; i < palavra_atual.size(); i++) {
                    if (palavra_atual[i] == tentativa) {
                        mostrar_palavra[i * 2] = tentativa;
                        letraCerta = true;
                    }
                }

                if (!letraCerta) erros++;
            }

            Mostrar(erros);
            if (mostrar_palavra.find('_') == std::string::npos) {
                std::cout << "Parabéns! Você acertou a palavra: " << palavra_atual << std::endl;
                pontos++;
            } else {
                std::cout << "Você perdeu. A palavra era: " << palavra_atual << std::endl;
            }

            std::cout << "Deseja jogar novamente? (S/N): ";
            char resposta;
            std::cin >> resposta;
            Jogardenovo = (toupper(resposta) == 'S');
        }

        std::cout << "Total de viteradorórias nesta sessão: " << pontos << std::endl;
    }
};

// Classe Multijogador
class Multijogador : public Jogo {
public:
    Multijogador(std::string filename) : Jogo(filename) {
        maxErros = 6;
    }

    void jogar() override {
        std::string jogadores[2];
        int pontos_jogadores[2] = {0, 0};
        int errosPorJogadores[2] = {0, 0};
        std::cout << "Nome do Jogador 1: ";
        std::cin >> jogadores[0];
        std::cout << "Nome do Jogador 2: ";
        std::cin >> jogadores[1];

        bool Jogardenovo = true;
        while (Jogardenovo) {
            int JogadorAtual = 0;
            errosPorJogadores[0] = errosPorJogadores[1] = 0;
            letrasUsadas[0] = '\0'; // Reinicializa o array
            escolherPalavra();

            while (errosPorJogadores[0] < maxErros && errosPorJogadores[1] < maxErros &&
                   mostrar_palavra.find('_') != std::string::npos) {
                Mostrar(errosPorJogadores[JogadorAtual]);
                std::cout << "Turno de " << jogadores[JogadorAtual] << std::endl;
                char tentativa;
                std::cout << "Escolha uma letra: ";
                std::cin >> tentativa;
                tentativa = toupper(tentativa);

                if (!ValidarLetra(tentativa)) {
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(letrasUsadas).find(tentativa) == std::string::npos) {
                    size_t tamanho = strlen(letrasUsadas);
                    if (tamanho >= capacidadeLetrasUsadas - 1) {
                        capacidadeLetrasUsadas *= 2;
                        char* temp = new char[capacidadeLetrasUsadas];
                        strcpy(temp, letrasUsadas);
                        delete[] letrasUsadas;
                        letrasUsadas = temp;
                    }

                    letrasUsadas[tamanho] = tentativa;
                    letrasUsadas[tamanho + 1] = '\0';
                }

                bool letraCerta = false;
                for (size_t i = 0; i < palavra_atual.size(); i++) {
                    if (palavra_atual[i] == tentativa) {
                        mostrar_palavra[i * 2] = tentativa;
                        letraCerta = true;
                    }
                }

                if (!letraCerta) {
                    errosPorJogadores[JogadorAtual]++;
                }

                JogadorAtual = (JogadorAtual + 1) % 2;
            }

            Mostrar(errosPorJogadores[JogadorAtual]);
            if (mostrar_palavra.find('_') == std::string::npos) {
                int winningjogarer = (JogadorAtual + 1) % 2;
                std::cout << "Parabéns, " << jogadores[winningjogarer] << "! Você acertou a palavra: " << palavra_atual << std::endl;
                pontos_jogadores[winningjogarer]++;
            } else {
                std::cout << "Nenhum jogador acertou. A palavra era: " << palavra_atual << std::endl;
            }

            std::cout << "Placar: " << jogadores[0] << " " << pontos_jogadores[0] << ", " << jogadores[1] << " " << pontos_jogadores[1] << std::endl;
            std::cout << "Deseja jogar novamente? (S/N): ";
            char resposta;
            std::cin >> resposta;
            Jogardenovo = (toupper(resposta) == 'S');
        }
    }
};

int main() {
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "            Jogo da Forca" << std::endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "Digiteradore 1 para jogar o modo solo." << std::endl;
    std::cout << "Digiteradore 2 para jogar o modo multijogador." << std::endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "Digiteradore aqui: ";

    int escolha;
    std::cin >> escolha;

    switch(escolha) {
        case 1: {
            JogoSolo JogoSolo("wordlist.txt");
            JogoSolo.jogar();
            break;
        }
        case 2: {
            Multijogador Multijogador("wordlist.txt");
            Multijogador.jogar();
            break;
        }
        default:
            std::cout << "Opção inválida." << std::endl;
    }

    return 0;
}
