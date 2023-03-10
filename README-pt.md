[Read this page in English](https://github.com/arielhrc/UnoJoyShifter/blob/master/README.md)

Adaptação do cambio do G27 para USB utilizando um Arduino UNO R3

Pré-requisito: instalar o software [FLIP](https://www.microchip.com/en-us/development-tool/flip) E o Arduino IDE (né?)

1. Conecta o seu Arduino UNO R3 no computador e faz o upload do codigo no UnoShifter.ino
2. Depois de upado, voce vai colocar o Arduino em modo DFU. Basta fazer contato nos dois pinos que ficam pertinho do conector USB.
3. Agora voce roda o TurnIntoAJoystick.bat
4. Quando acabar o processo é só desconectar e reconectar o arduino que ele sera reconhecido como um joystick no windows (confere no painel de controle, dispositivos e impressoras)

A LIGAÇAO DOS FIOS.
--------------------


|DB9 | Cor        |Cambio  |Descriçao            |Arduino      |
|----|------------|--------|---------------------|-------------|
|  1 | Roxo       | 1      | Clock               |pino 2       |
|  2 | Cinza      | 7      | Data                |pino 3       |
|  3 | Amarelo    | 5      | Button !CS & !PL    |pino 4       |
|  4 | Laranja    | 3      | Shifter X axis      |pino A0      |
|  5 | Branco     | 2      | SPI input           |não utilizado|
|  6 | Preto      | 8      | GND                 |GND          |
|  7 | Vermelho   | 6      | +5V                 |VCC          |
|  8 | Verde      | 4      | Shifter Y axis      |pino A1      |
|  9 | Vermelho   | 9      | +5V                 |VCC          |


Caso queira utilizar o arduino novamente como arduino é só entrar em DFU novamente e rodar o TurnIntoAnArduino.bat
Codigo original em https://github.com/jssting/UnoJoyShifter
Editado por Ariel Carvalho, contato no Facebook, grupo Volantes caseiros

