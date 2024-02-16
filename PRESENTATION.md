---
author: Tiago Dinis 1º IS / Nº 28
---
# snake-c

Um jogo da cobra em C puro sem bibliotecas que funciona no terminal

---
# Como funciona a entrada?

## Modo canónico vs Modo cozinhado

Em linux, o `stdin` (stream de entrada padrão) em que recebemos input do utilizador, existe 2 modos:
- Modo **canónico**: O usuário precisa apertar enter pra receber a entrada
- Modo **cru**: O programa imediatamente recebe o caracter após a tecla ser pressionada

O `stdin` começa no modo canónico, então para este jogo precisei de mudar para o modo cru:
```cpp
///#include <stdlib.h>
///#include <stdio.h>
///#include <termios.h>
///#include <unistd.h>
///int main(void) {
struct termios orig;
// Pegar a configuração atual para usar como base
tcgetattr(STDIN_FILENO, &orig);
struct termios raw = orig;
// Setar algumas coisas que nem faço ideia
// Não se preocupem que nem eu sei como isto funciona 😂
raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
raw.c_oflag &= ~(OPOST);
raw.c_cflag |= (CS8);
raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
raw.c_cc[VMIN] = 0;
raw.c_cc[VTIME] = 1;
tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
// Voltar a configuração original no final do programa
tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
///}
```

---

# Como funciona a entrada?

## Leitura de input com o modo cru

Agora no modo cru podemos usar a função `read` para ler caracteres
```cpp
char c;
// Fica `true` se conseguiu ler um caracter, se não `false`
bool has_character = read(STDIN_FILENO, &c, 1);
if(has_character){
  printf("%c\n", c);
}
```

`c` pode conter uma letra ou um caracter de escape `\e` que normalmente significa que
o utilizador apertou uma tecla especial como **as setas**.

---

# Como funciona a entrada?

## Teclas não ASCII

Quando apertamos as setas outras teclas especiais, estes caracteres são dados como entrada:
- `\e[A`: Seta para cima
- `\e[B`: Seta para baixo
- `\e[C`: Seta para direita
- `\e[D`: Seta para esquerda
- `\e`: Escape

**NOTA:** No caso de ter vários caracteres como nas setas, é preciso chamar o `read` várias vezes para puxar os proximos caracteres
---

# Ponto de entrada e fluxo de atualização

