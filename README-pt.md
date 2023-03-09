[Read this page in English](https://github.com/arielhrc/UnoJoyShifter/blob/master/README.md)

Pré-requisito: instalar o software FLIP-Installer, que esta na pasta UnoJoy. E o Arduino IDE (né?)
1: Conecta o seu Arduino UNO R3 no computador e faz o upload do codigo no UnoShifter.ino
2: Depois de upado, voce vai colocar o Arduino em modo DFU. Basta fazer contato nos dois pinos que ficam pertinho do conector USB.
3: Agora voce roda o TurnIntoAJoystick.bat, quando acabar o processo é só desconectar e reconectar o arduino que ele sera reconhecido como um joystick no windows

A LIGAÇAO DOS FIOS.


//  DB9  Cor    Cambio      Descriçao           Arduino
//  1    Roxo        1      Clock               pino 2
//  2    Cinza       7      Data                pino 3
//  3    Amarelo     5      Button !CS & !PL    pino 4
//  4    Laranja     3      Shifter X axis      pino A0
//  5    Branco      2      SPI input           não utilizado
//  6    Preto       8      GND                 GND
//  7    Vermelho    6      +5V                 VCC
//  8    Verde       4      Shifter Y axis      pino A1
//  9    Vermelho    9      +5V                 VCC

O codigo anterior incluia um freio de mao com sensor hall, que pra compilar o codigo eu tive que comentar tudo.
Codigo original em https://github.com/jssting/UnoJoyShifter
Caso queira utilizar o arduino novamente como arduino é só entrar em DFU novamente e rodar o TurnIntoAnArduino.bat
Contato: Ariel Carvalho Facebook no grupo Volantes caseiros

