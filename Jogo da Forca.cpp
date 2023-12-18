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
protected: // interface protegida
    std::map<std::string, std::vector<std::string>> banco_palavras; // Cria um map que guarda as palavras 
    std::string palavra_atual; // Cria string da palavra atual do jogo
    std::string mostrar_palavra; // Cria string que mostra a palavra
    int maxErros; // Cria variavel para o maximo de erros
    int erros; // Cria variavel para erros
    char* letrasUsadas; // Ponteiro para um array dinâmico
    int capacidadeLetrasUsadas; // Tamanho atual do array
    std::string tema; // Criação de string para o tema

    static std::set<std::string> palavrasUsadas; // Armazena palavras já utilizadas

    void CarregarPalavras(std::string filename) { // Função para carregar as palavras / Argumento: arquivo de texto
        std::ifstream file(filename); // Abrir arquivo
        if (!file) { // Verifica se o arquivo está presente no mesmo diretorio, etc
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            return;
        }

        std::string linha, tema; // Cria string de linha e coluna para as palavras e os temas
        while (getline(file, linha)) { // Loop que percorre todas as linhas do arquivo
            if (linha.empty()) continue; // Verifica se a linha está vazia
            if (linha.back() == ':') { // Vericia se há um ponto final no fim da palavra
                tema = linha.substr(0, linha.size() - 1); // Retira os dois pontos do final e põe a palavra na variavel tema
                banco_palavras[tema] = std::vector<std::string>(); // Coloca o tema no vetor de temas
            } else { // Caso não tenha dois pontos no final, então é uma palavra
                banco_palavras[tema].push_back(linha); // Poe a palavra no vetor de acordo com o tema
            }
        }

        if (banco_palavras.empty()) { // Verifica se após o processo, caso o vetor estiver vazio, talvez tenha um problema
            std::cerr << "Nenhuma palavra carregada do arquivo." << std::endl;
        }
    }

    void escolherPalavra() { // Função para escolher uma palavra
        do {
            int indiceTema = rand() % banco_palavras.size(); // Escolhe um numero aleatorio de acordo com a quantidade de palavras no banco
            auto iterador = banco_palavras.begin(); // Cria um ponteiro que aponta para o primeiro elemento do banco de palavras
            std::advance(iterador, indiceTema); // Avança o iterador para o indice especificado 
            tema = iterador->first; // Acessando o primeiro elemento "Tema" do banco
            const std::vector<std::string>& palavras = iterador->second; // Acessando o segundo elemento "palavras" do banco
            palavra_atual = palavras[rand() % palavras.size()]; // Seleciona uma palavra aleatória do vetor relacionado ao tema escolhido
        } while (palavrasUsadas.find(palavra_atual) != palavrasUsadas.end()); // Verifica se a palavra já foi usada

        palavrasUsadas.insert(palavra_atual); // Adiciona a palavra atual ao conjunto de usadas

        mostrar_palavra = ""; // Inicializa a string vazia
        for (char c : palavra_atual) { // Iteração que percorre cada caracter da string
            mostrar_palavra += (c == ' ' ? "  " : "_ "); // Substitui os caracteres com espaço por dois espaços e os sem espaço por underline e espaço
        }
        mostrar_palavra.pop_back(); // Remove o ultimo espaço desnecessario
    }

    void Mostrar(int ErrosdoJogadorAtual) {  // Função responsavel por mostrar o jogo
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Tema: " << tema << std::endl; // mostra o tema
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Palavra: ";
        for (char c : mostrar_palavra) { // Loop para mostrar o atual momento da adivinhação
            std::cout << c;  // Imprime a palavra com espaços
        }
        std::cout << std::endl;
        std::cout << "Erros: " << ErrosdoJogadorAtual << " de " << maxErros << std::endl; // mostra erros do jogador
        std::cout << "Letras utilizadas: " << letrasUsadas << std::endl; // mostra letras utilizadas
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    }

    // Método para verificar se o caractere é uma letra válida de A a Z
    bool ValidarLetra(char letra) { // Função responsavel por validar se a letra está presente no alfabeto
        return letra >= 'A' && letra <= 'Z';
    }

public: // interface publica
    Jogo(std::string filename) : capacidadeLetrasUsadas(10) { // Construtor da classe jogo que inicia com capacidade de 10 letras utilizadas
        CarregarPalavras(filename); // Carrega palavras do arquivo
        srand(time(NULL)); // Inicializa o gerador de números aleatórios
        letrasUsadas = new char[capacidadeLetrasUsadas]; // Alocando memória
        letrasUsadas[0] = '\0'; // Inicializar com string vazia
    }
    virtual void jogar() = 0; // torna jogo uma classe abstrata e virtual

    virtual ~Jogo() { // Destrutor da classe jogo que garante que cada subclasse tenha um destrutor correto chamado
        delete[] letrasUsadas; // Liberando memória alocada
    }

    
};

std::set<std::string> Jogo::palavrasUsadas; // declara variavel estatica que armazena palavras ja utilizadas no jogo


// Classe JogoSolo
class JogoSolo : public Jogo { // Declara a classe jogosolo que herda da classe jogo
public:
    JogoSolo(std::string filename) : Jogo(filename) { // construtor da classe jogosolo que recebe o nome do arquivo e passa para o construtor da classe jogo
        maxErros = 6; // inicializa a variavel de erros maximos com valor 6
    }

    void jogar() override { // faz parte da funcao virtual pré implementada na classe base jogo
        int pontos = 0; // Pontuaçao do jogador 
        bool Jogardenovo = true; // controle para repetir o jogo
        while (Jogardenovo) { // laço que continua enquanto o jogador quiser jogar
            erros = 0; // variavel que inicia os erros com valor 0
            letrasUsadas[0] = '\0'; // Reinicializa o array letrasUsadas
            escolherPalavra();  // Escolhe uma nova palavra e tema
            while (erros < maxErros && mostrar_palavra.find('_') != std::string::npos) { // laço que continua o jogo enquanto o numero de erros for menor que o maximo e tiver underlines ainda
                Mostrar(erros); // mostra o estado atual do jogo
                char tentativa; // variavel da tentativa
                std::cout << "Escolha uma letra: ";
                std::cin >> tentativa; // recebe a resposta da letra da tentativa
                tentativa = toupper(tentativa); // transforma a letra em maiuscula

                if (!ValidarLetra(tentativa)) { // Valida a letra e verifica se ela está no alfabeto
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(letrasUsadas).find(tentativa) != std::string::npos) { // Verifica se a letra ja foi utilizada
                    std::cout << "Letra já utilizada. Tente outra.\n";
                    continue;
                }

                size_t tamanho = strlen(letrasUsadas); // variavel que armazena o tamanho do vetor letrasusadas
                if (tamanho >= capacidadeLetrasUsadas - 1) { // verifica se o veto está quase cheio
                    capacidadeLetrasUsadas *= 2; // dobra a capacidade do vetor
                    char* temp = new char[capacidadeLetrasUsadas]; // aloca espaço e cria um novo vetor com capacidade dobrada
                    strcpy(temp, letrasUsadas); // copia o conteudo do vetor antigo para um temporario
                    delete[] letrasUsadas; // desaloca o espaço do vetor antigo
                    letrasUsadas = temp; // transfere para o vetor novo o conteudo do vetor temporario
                }

                letrasUsadas[tamanho] = tentativa; // adiciona a letra tentada ao vetor de letras usadas
                letrasUsadas[tamanho + 1] = '\0'; // atualiza o caracter final

                bool letraCerta = false; // controle para verificacao da letra
                for (size_t i = 0; i < palavra_atual.size(); i++) { // percorre a palavra
                    if (palavra_atual[i] == tentativa) { // verifica se a letra tentada corresponde a alguma da palavra
                        mostrar_palavra[i * 2] = tentativa; // coloca a letra tentada no espaço correspondente a sua underline
                        letraCerta = true; // define que a letra está certa
                    }
                }

                if (!letraCerta) erros++; // se a letra está errada, aumenta os erros
            }

            Mostrar(erros); // mostra o estado do jogo 
            if (mostrar_palavra.find('_') == std::string::npos) { // verifica se todos os underlines estao preenchidos
                std::cout << "Parabéns! Você acertou a palavra: " << palavra_atual << std::endl;
                pontos++; // incrementa a quantidade de pontos
            } else { // se nao estiverem preenchidos e os erros forem maiores que o maximo, perde
                std::cout << "Você perdeu. A palavra era: " << palavra_atual << std::endl;
            }

            std::cout << "Deseja jogar novamente? (S/N): ";
            char resposta; // variavel resposta
            std::cin >> resposta; // recebe a resposta
            Jogardenovo = (toupper(resposta) == 'S'); // transforma a resposta em maiusculo
        }

        std::cout << "Total de vitórias nesta sessão: " << pontos << std::endl; // mostra a quantidade de acertos
    }
};

// Classe Multijogador
class Multijogador : public Jogo { // Declara a classe multijogador que herda da classe jogo
public:
    Multijogador(std::string filename) : Jogo(filename) { // construtor da classe jogosolo que recebe o nome do arquivo e passa para o construtor da classe jogo
        maxErros = 6; // inicializa a variavel de erros maximos com valor 6
    }

    void jogar() override { // faz parte da funcao virtual pré implementada na classe base jogo
        std::string jogadores[2]; // armazena o nome dos jogadores
        int pontos_jogadores[2] = {0, 0}; // armazena os pontos dos jogadores
        int errosPorJogadores[2] = {0, 0}; // armazena os erros dos jogadores
        std::cout << "Nome do Jogador 1: ";
        std::cin >> jogadores[0]; // recebe o nome do jogador 1
        std::cout << "Nome do Jogador 2: ";
        std::cin >> jogadores[1]; // recebe o nome do jogador 2

        bool Jogardenovo = true; // controle para repetir o jogo
        while (Jogardenovo) { // laço que continua enquanto o jogador quiser jogar
            int JogadorAtual = 0; // variavel que define o jogador atual
            errosPorJogadores[0] = errosPorJogadores[1] = 0; // inicializa os erros de cara jogador
            letrasUsadas[0] = '\0'; // Reinicializa o array
            escolherPalavra(); // inicia a funcao que escolhe a palavra

            while (errosPorJogadores[0] < maxErros && errosPorJogadores[1] < maxErros &&
                   mostrar_palavra.find('_') != std::string::npos) { // laço que continua o jogo enquanto o numero de erros for menor que o maximo e tiver underlines ainda
                Mostrar(errosPorJogadores[JogadorAtual]); // mostra o estado do jogo para o jogador atual
                std::cout << "Turno de " << jogadores[JogadorAtual] << std::endl;
                char tentativa; // variavel que armazena a tentativa
                std::cout << "Escolha uma letra: ";
                std::cin >> tentativa; // recebe a tentativa
                tentativa = toupper(tentativa); // transforma a tentativa em maiusculo

                if (!ValidarLetra(tentativa)) { // verifica se a letra tentada está no alfabeto
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(letrasUsadas).find(tentativa) == std::string::npos) {
                    size_t tamanho = strlen(letrasUsadas); // variavel que armazena o tamanho do vetor letrasusadas
                    if (tamanho >= capacidadeLetrasUsadas - 1) { 
                        capacidadeLetrasUsadas *= 2; // dobra a capacidade do vetor
                        char* temp = new char[capacidadeLetrasUsadas]; // aloca espaço e cria um novo vetor com capacidade dobrada
                        strcpy(temp, letrasUsadas); // copia o conteudo do vetor antigo para um temporario
                        delete[] letrasUsadas; // desaloca o espaço do vetor antigo
                        letrasUsadas = temp; // transfere para o vetor novo o conteudo do vetor temporario
                    }

                    letrasUsadas[tamanho] = tentativa; // adiciona a letra tentada ao vetor de letras usadas
                    letrasUsadas[tamanho + 1] = '\0'; // atualiza o caracter final
                }

                bool letraCerta = false; // controle para verificacao da letra
                for (size_t i = 0; i < palavra_atual.size(); i++) { // percorre a palavra
                    if (palavra_atual[i] == tentativa) { // verifica se a letra tentada corresponde a alguma da palavra
                        mostrar_palavra[i * 2] = tentativa; // coloca a letra tentada no espaço correspondente a sua underline
                        letraCerta = true; // define que a letra está certa
                    }
                }

                if (!letraCerta) { // se a letra está errada aumenta os erros do jogador atual
                    errosPorJogadores[JogadorAtual]++;
                }

                JogadorAtual = (JogadorAtual + 1) % 2; // troca o jogador 
            }

            Mostrar(errosPorJogadores[JogadorAtual]); // mostra o estado do jogo pro jogador atual
            if (mostrar_palavra.find('_') == std::string::npos) { // verifica se todos os underlines foram preenchidos
                int JogadorVencedor = (JogadorAtual + 1) % 2; // da a vitoria pro jogador 
                std::cout << "Parabéns, " << jogadores[JogadorVencedor] << "! Você acertou a palavra: " << palavra_atual << std::endl;
                pontos_jogadores[JogadorVencedor]++; // aumenta os pontos do jogador venccedor
            } else {
                std::cout << "Nenhum jogador acertou. A palavra era: " << palavra_atual << std::endl;
            }

            std::cout << "Placar: " << jogadores[0] << " " << pontos_jogadores[0] << ", " << jogadores[1] << " " << pontos_jogadores[1] << std::endl;
            std::cout << "Deseja jogar novamente? (S/N): ";
            char resposta; // variavel da resposta 
            std::cin >> resposta; // recebe resposta 
            Jogardenovo = (toupper(resposta) == 'S'); // transforma em maiusculo a resposta
        }
    }
};

int main() {
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "            Jogo da Forca" << std::endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "Digite 1 para jogar o modo solo." << std::endl;
    std::cout << "Digite 2 para jogar o modo multijogador." << std::endl;
    std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "Digite aqui: ";

    int escolha; // variavel de escolha de modo de jogo
    std::cin >> escolha; // recebe a escolha de modo de jogo

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
