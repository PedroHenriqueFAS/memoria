# Jogo de Memória com Raspberry Pi Pico W

## 📝 Sobre o Projeto

Este é um jogo de memória desenvolvido para o Raspberry Pi Pico W, onde o jogador deve repetir uma sequência de botões e adicionar um novo botão a cada rodada. O jogo utiliza um display SSD1306, uma matriz de LEDs, LEDs indicadores e um buzzer para fornecer feedback visual ao jogador.

## 🎯 Funcionalidades

- O jogo inicia com um padrão na matriz de LEDs e uma mensagem no display.

- O jogador deve repetir a sequência correta de botões.

- Cada rodada adiciona um novo botão à sequência.

- Se o jogador errar, o jogo exibe "Botão Errado" e reinicia.

- Um temporizador de 10 segundos é usado para reiniciar automaticamente caso não haja resposta.

- O brilho do LED azul pode ser ajustado dinamicamente.

## 🔓 Como Usar

1. Instale o SDK do Raspberry Pi Pico caso ainda não tenha.

2. Compile o código usando um ambiente compatível (ex: CMake com o SDK do Pico).

3. Carregue o programa para a placa via UF2 ou usando um debugger compatível.

4. Conecte os periféricos de acordo com o esquema de ligação acima.

5. Inicie o jogo! O display mostrará "Início de jogo", e a matriz de LEDs exibirá padrões conforme a jogabilidade.

## 📓 Estrutura do Código

- setup(): Configura os pinos GPIO, inicializa o display e os LEDs.

- game_over(): Exibe mensagem de erro e reinicia o jogo.

- next_round(): Aguarda a entrada do jogador e adiciona um novo botão à sequência.

- print_digit(): Mostra padrões na matriz de LEDs.

- wait_for_button_press(): Aguarda e identifica qual botão foi pressionado.

- exibir_botao_correto(): Exibe "Botão Correto" no display.

## 🛠️ Requisitos

- Raspberry Pi Pico W
- LEDs RGB (Vermelho, Verde e Azul)
- Matriz de LEDs (5x5)
- Display SSD1306 I2C
- Três botões conectados aos pinos GPIO (22, 4 e 5)

## ⚙️ Compilação e Execução

1. Clone o repositório do projeto:
   ```sh
   git clone https://github.com/PedroHenriqueFAS/memoria
   cd memoria
   ```
2. Crie um diretório de build e entre nele:
   ```sh
   mkdir build
   cd build
   ```
3. Execute o comando CMake para configurar a compilação:
   ```sh
   cmake ..
   ```
4. Compile o projeto:
   ```sh
   make
   ```
5. Faça o upload do binário gerado para a Raspberry Pi Pico.

## 👥 Colaboradores

1. **Pedro Henrique Ferreira Amorim da Silva** - [GitHub](https://github.com/PedroHenriqueFAS)

## 📜 Licença

Este projeto está licenciado sob a Licença MIT. Para mais detalhes, consulte o arquivo LICENSE.

