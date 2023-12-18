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

    static std::set<std::string> usedWords; // Armazena palavras já utilizadas

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
        do {
            int themeIndex = rand() % wordBank.size();
            auto it = wordBank.begin();
            std::advance(it, themeIndex);
            theme = it->first;
            const std::vector<std::string>& words = it->second;
            currentWord = words[rand() % words.size()];
        } while (usedWords.find(currentWord) != usedWords.end()); // Verifica se a palavra já foi usada

        usedWords.insert(currentWord); // Adiciona a palavra atual ao conjunto de usadas

        displayWord = "";
        for (char c : currentWord) {
            displayWord += (c == ' ' ? "  " : "_ ");
        }
        displayWord.pop_back();
    }

    void displayState(int CurrentPlayerErrors) { 
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Tema: " << theme << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
        std::cout << "Palavra: ";
        for (char c : displayWord) {
            std::cout << c;  // Imprime a palavra com espaços
        }
        std::cout << std::endl;
        std::cout << "Erros: " << CurrentPlayerErrors << " de " << maxErrors << std::endl;
        std::cout << "Letras utilizadas: " << usedLetters << std::endl;
        std::cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    }

    // Método para verificar se o caractere é uma letra válida de A a Z
    bool isValidLetter(char letter) {
        return letter >= 'A' && letter <= 'Z';
    }

public:
    Game(std::string filename) : usedLettersCapacity(10) {
        loadWords(filename);
        srand(time(NULL));
        usedLetters = new char[usedLettersCapacity]; // Alocando memória
        usedLetters[0] = '\0'; // Inicializar com string vazia
    }
    virtual void play() = 0;

    virtual ~Game() {
        delete[] usedLetters; // Liberando memória alocada
    }

    
};

std::set<std::string> Game::usedWords;


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
            usedLetters[0] = '\0'; // Reinicializa o array
            chooseWord();  // Escolhe uma nova palavra e tema
            while (errors < maxErrors && displayWord.find('_') != std::string::npos) {
                displayState(errors);
                char guess;
                std::cout << "Escolha uma letra: ";
                std::cin >> guess;
                guess = toupper(guess);

                if (!isValidLetter(guess)) {
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(usedLetters).find(guess) != std::string::npos) {
                    std::cout << "Letra já utilizada. Tente outra.\n";
                    continue;
                }

                size_t len = strlen(usedLetters);
                if (len >= usedLettersCapacity - 1) {
                    usedLettersCapacity *= 2;
                    char* temp = new char[usedLettersCapacity];
                    strcpy(temp, usedLetters);
                    delete[] usedLetters;
                    usedLetters = temp;
                }

                usedLetters[len] = guess;
                usedLetters[len + 1] = '\0';

                bool found = false;
                for (size_t i = 0; i < currentWord.size(); i++) {
                    if (currentWord[i] == guess) {
                        displayWord[i * 2] = guess;
                        found = true;
                    }
                }

                if (!found) errors++;
            }

            displayState(errors);
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

// Classe MultiplayerGame
class MultiplayerGame : public Game {
public:
    MultiplayerGame(std::string filename) : Game(filename) {
        maxErrors = 6;
    }

    void play() override {
        std::string players[2];
        int scores[2] = {0, 0};
        int errorsPerPlayer[2] = {0, 0};
        std::cout << "Nome do Jogador 1: ";
        std::cin >> players[0];
        std::cout << "Nome do Jogador 2: ";
        std::cin >> players[1];

        bool playAgain = true;
        while (playAgain) {
            int currentPlayer = 0;
            errorsPerPlayer[0] = errorsPerPlayer[1] = 0;
            usedLetters[0] = '\0'; // Reinicializa o array
            chooseWord();

            while (errorsPerPlayer[0] < maxErrors && errorsPerPlayer[1] < maxErrors &&
                   displayWord.find('_') != std::string::npos) {
                displayState(errorsPerPlayer[currentPlayer]);
                std::cout << "Turno de " << players[currentPlayer] << std::endl;
                char guess;
                std::cout << "Escolha uma letra: ";
                std::cin >> guess;
                guess = toupper(guess);

                if (!isValidLetter(guess)) {
                std::cout << "Entrada inválida. Por favor, insira uma letra de A a Z.\n";
                continue;
                }

                if (std::string(usedLetters).find(guess) == std::string::npos) {
                    size_t len = strlen(usedLetters);
                    if (len >= usedLettersCapacity - 1) {
                        usedLettersCapacity *= 2;
                        char* temp = new char[usedLettersCapacity];
                        strcpy(temp, usedLetters);
                        delete[] usedLetters;
                        usedLetters = temp;
                    }

                    usedLetters[len] = guess;
                    usedLetters[len + 1] = '\0';
                }

                bool found = false;
                for (size_t i = 0; i < currentWord.size(); i++) {
                    if (currentWord[i] == guess) {
                        displayWord[i * 2] = guess;
                        found = true;
                    }
                }

                if (!found) {
                    errorsPerPlayer[currentPlayer]++;
                }

                currentPlayer = (currentPlayer + 1) % 2;
            }

            displayState(errorsPerPlayer[currentPlayer]);
            if (displayWord.find('_') == std::string::npos) {
                int winningPlayer = (currentPlayer + 1) % 2;
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
