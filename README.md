# RFID-KIRBY

Este é um projeto que envolve a leitura e gravação de cartões RFID usando o M5Cardputer e o módulo RFID2.

## Sobre

Este código foi criado por Mateus Fonseca em 2024 para o projeto RFID-KIRBY.

## Dependências

- **Wire.h**: Biblioteca para comunicação I2C.
- **MFRC522_I2C.h**: Biblioteca para o módulo RFID2.
- **kirbybmp.h**: Arquivo de bitmap para a tela inicial.
- **M5Cardputer.h**: Biblioteca para o M5Cardputer.
- **SD.h**: Biblioteca para comunicação com o cartão SD.

## Funcionalidades

- **Modo de Leitura**: Permite ler informações de cartões RFID.
- **Modo de Escrita**: Permite gravar informações em cartões RFID.
- **Botão TAB**: Alterna entre os modos de leitura e escrita.

## Configuração Inicial

- A tela é inicializada com uma imagem de boot do Kirby.
- O programa exibe a versão e o nome do aplicativo.
- Inicializa o módulo RFID2.
- Inicializa o cartão SD.
- 
## Como Usar

- No modo de leitura, segure um cartão RFID próximo ao leitor para visualizar as informações do cartão.
- No modo de escrita, as informações lidas anteriormente serão gravadoas no cartão, segure o cartão próximo ao leitor para gravá-las.
- Use o botão TAB para alternar entre os modos de leitura e escrita.

## Notas

- Certifique-se de que o modulo RFID2 esteja devidamente inserido.
- Este código é específico para o M5Cardputer e o módulo RFID2.
