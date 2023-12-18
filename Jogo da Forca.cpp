#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// Classe Base
class Game {
protected:
    std::map<std::string, std::vector<std::string>> wordBank;
    std::string currentWord;
    std::string displayWord;
    int maxErrors;
    int errors;
    char* usedLetters; // Ponteiro para um array dinâmico
    int usedLettersCapacity; // Tamanho atual do array
    std::string theme;

    void loadWords(std::string filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            return;
        }

        std::string line, theme;
        while (getline(file, line)) {
            if (line.empty()) continue;
            if (line.back() == ':') {
                theme = line.substr(0, line.size() - 1);
                wordBank[theme] = std::vector<std::string>();
            } else {
                wordBank[theme].push_back(line);
            }
        }

        if (wordBank.empty()) {
            std::cerr << "Nenhuma palavra carregada do arquivo." << std::endl;
        }
    }

    void chooseWord() {
        int themeIndex = rand() % wordBank.size();
        auto it = wordBank.begin();
        std::advance(it, themeIndex);
        theme = it->first;
        const std::vector<std::string>& words = it->second;
        currentWord = words[rand() % words.size()];
        displayWord = "";
        for (char c : currentWord) {
            displayWord += (c == ' ' ? "  " : "_ ");  // Adiciona espaço para cada letra e dois espaços para espaços
        }
        displayWord.pop_back(); // Remove o último espaço extra
    }

    void displayState() {
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Tema: " << theme << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Palavra: ";
        for (char c : displayWord) {
            std::cout << c;  // Imprime a palavra com espaços
        }
        std::cout << std::endl;
        std::cout << "Erros: " << errors << " de " << maxErrors << std::endl;
        std::cout << "Letras utilizadas: " << usedLetters << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    }

public:
    Game(std::string filename) : usedLettersCapacity(10) {
        loadWords(filename);
        srand(time(NULL));
        usedLetters = new char[usedLettersCapacity]; // Alocando memória
        usedLetters[0] = '\0'; // Inicializar com string vazia
    }

    virtual ~Game() {
        delete[] usedLetters; // Liberando memória alocada
    }

// Classe SoloGame
class SoloGame : public Game {
public:
    SoloGame(std::string filename) : Game(filename) {
        maxErrors = 6;
    }

    void play() override {
        int score = 0;
        bool playAgain = true;
        while (playAgain) {
            errors = 0;
            usedLetters = "";
            chooseWord();  // Escolhe uma nova palavra e tema
            while (errors < maxErrors && displayWord.find('_') != std::string::npos) { // Verifica se ainda há underlines
                displayState();
                char guess;
                std::cout << "Escolha uma letra: ";
                std::cin >> guess;
                guess = toupper(guess); // Converter para maiúscula

                if (usedLetters.find(guess) != std::string::npos) {
                    std::cout << "Letra já utilizada. Tente outra.\n";
                    continue;
                }
                usedLetters += guess;
                 if (currentWord.find(guess) != std::string::npos) {
                    for (size_t i = 0; i < currentWord.size(); i++) {
                        if (currentWord[i] == guess) {
                            displayWord[i * 2] = guess; // Multiplica por 2 para ajustar a posição
                        }
                    }
                } else {
                    errors++;
                }
            }
            displayState();
            if (displayWord.find('_') == std::string::npos) {
                std::cout << "Parabéns! Você acertou a palavra: " << currentWord << std::endl;
                score++;
            } else {
                std::cout << "Você perdeu. A palavra era: " << currentWord << std::endl;
            }
            std::cout << "Deseja jogar novamente? (S/N): ";
            char response;
            std::cin >> response;
            playAgain = (toupper(response) == 'S');
        }
        std::cout << "Total de vitórias nesta sessão: " << score << std::endl;
    }
};

class MultiplayerGame : public Game {
public:
    MultiplayerGame(std::string filename) : Game(filename) {
        maxErrors = 6;
    }

 void play() override {
    std::string players[2];
    int scores[2] = {0, 0};
    int errorsPerPlayer[2] = {0, 0}; // Erros individuais dos jogadores
    std::cout << "Nome do Jogador 1: ";
    std::cin >> players[0];
    std::cout << "Nome do Jogador 2: ";
    std::cin >> players[1];

    bool playAgain = true;
    while (playAgain) {
        int currentPlayer = 0;
        errorsPerPlayer[0] = errorsPerPlayer[1] = 0; // Reiniciar contagem de erros
        usedLetters = "";
        chooseWord();

        while (errorsPerPlayer[0] < maxErrors && errorsPerPlayer[1] < maxErrors &&
               displayWord.find('_') != std::string::npos) {
            std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
            std::cout << "Tema: " << theme << std::endl;
            std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
            std::cout << "Palavra: ";
            for (char c : displayWord) {
                std::cout << c;
            }
            std::cout << std::endl;
            std::cout << "Erros de " << players[currentPlayer] << ": " << errorsPerPlayer[currentPlayer] << " de " << maxErrors << std::endl;
            std::cout << "Letras utilizadas: " << usedLetters << std::endl;
            std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;

            char guess;
            std::cout << "Turno de " << players[currentPlayer] << std::endl;
            std::cout << "Escolha uma letra: ";
            std::cin >> guess;
            guess = toupper(guess);

            if (usedLetters.find(guess) == std::string::npos) {
            if (!usedLetters.empty()) {
            usedLetters += "-"; // Adicionar um separador
            }
            usedLetters += guess;
            }

            if (currentWord.find(guess) != std::string::npos) {
                for (size_t i = 0; i < currentWord.size(); i++) {
                    if (currentWord[i] == guess) {
                        displayWord[i * 2] = guess; // Ajusta a posição
                    }
                }
            } else {
                errorsPerPlayer[currentPlayer]++;
            }

            currentPlayer = (currentPlayer + 1) % 2; // Alterna o jogador
        }

        if (displayWord.find('_') == std::string::npos) {
            int winningPlayer = (currentPlayer + 1) % 2; // Determina o vencedor
            std::cout << "Parabéns, " << players[winningPlayer] << "! Você acertou a palavra: " << currentWord << std::endl;
            scores[winningPlayer]++;
        } else {
            std::cout << "Nenhum jogador acertou. A palavra era: " << currentWord << std::endl;
        }
        std::cout << "Placar: " << players[0] << " " << scores[0] << ", " << players[1] << " " << scores[1] << std::endl;
        std::cout << "Deseja jogar novamente? (S/N): ";
        char response;
        std::cin >> response;
        playAgain = (toupper(response) == 'S');
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

    int choice;
    std::cin >> choice;

    switch(choice) {
        case 1: {
            SoloGame soloGame("wordlist.txt");
            soloGame.play();
            break;
        }
        case 2: {
            MultiplayerGame multiplayerGame("wordlist.txt");
            multiplayerGame.play();
            break;
        }
        default:
            std::cout << "Opção inválida." << std::endl;
    }

    return 0;
}
