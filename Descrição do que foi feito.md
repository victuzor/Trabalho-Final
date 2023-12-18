Descrição do Trabalho

Este projeto em C++ implementa um jogo da forca digital, empregando vários conceitos-chave da programação orientada a objetos e outras práticas fundamentais em C++.

Utilizamos alocação dinâmica de memória para o array letrasUsadas, que armazena as letras já escolhidas pelo jogador. Isso é feito através do uso do operador new[], permitindo flexibilidade no gerenciamento de memória durante a execução do jogo.

A função CarregarPalavras lê palavras de um arquivo de texto e as armazena em um map baseado no tema. Isso demonstra a habilidade de lidar com arquivos, essencial para programas que necessitam de dados persistentes ou de grande volume.

A classe base Jogo e suas subclasses JogoSolo e Multijogador possuem construtores para inicializar os objetos de forma apropriada. Além disso, o destrutor virtual na classe Jogo garante a correta liberação da memória dinamicamente alocada, evitando vazamentos de memória.

A variável palavrasUsadas é declarada como um membro estático da classe Jogo. Ela mantém um registro das palavras já usadas em todas as instâncias do jogo, demonstrando a utilidade de membros estáticos para compartilhar informações entre várias instâncias de uma classe.

Implementamos a sobrecarga na função Mostrar, que pode ser modificada ou expandida em subclasses, permitindo uma maior flexibilidade e customização de acordo com as necessidades específicas de cada tipo de jogo (solo ou multijogador).

Herança: A classe Jogo serve como uma classe base para as subclasses JogoSolo e Multijogador. Esta abordagem de herança permite reutilizar e estender funcionalidades da classe base, ao mesmo tempo em que se implementam comportamentos específicos nas subclasses.

Este trabalho exemplifica a aplicação de conceitos avançados de C++ de forma prática, abordando desde a estruturação de classes e herança até a gestão de recursos, crucial em aplicações modernas de software.
