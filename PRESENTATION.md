---
author: Tiago Dinis 1º IS / Nº 28
---
# snake-c

Um jogo da cobra em C puro sem bibliotecas que funciona no terminal

---
# Como funciona a entrada?

## Modo cru vs Modo cozinhado

Em linux, o `stdin` (stream de entrada padrão) em que recebemos input do utilizador, existe 2 modos:
- Modo **cozinhado**: O usuário precisa apertar enter pra receber a entrada
- Modo **cru**: O programa imediatamente recebe o caracter após a tecla ser pressionada

O `stdin` começa no modo canónico, então para este jogo precisei de mudar para o modo cru:
```cpp
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
```

---

# Como funciona a entrada?

## Leitura de input com o modo cru

Agora no modo cru podemos usar a função `read` para ler caracteres
```cpp

char c[1];
// A função read retorna o numero de carateres que conseguiu ler
int read_characters_count = read(STDIN_FILENO, 
  /* Lugar para guardar os caracteres */ &c, 
  /* Numero de caracteres a ler */ sizeof(c));

// Se conseguimos ler pelo menos 1 caracter printamos o caracter
if(read_characters_count > 0){
  printf("%c\n", c[0]);
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

**NOTA:** No caso de ter vários caracteres como nas setas, é preciso chamar o `read` mais vezes ou aumentar o numero de caracteres a ler

---

# Ponto de entrada e fluxo de atualização

## Função `main`:
- Inicialização do "raw mode"
- Mudança para a tela alternativa
- Enquanto o jogo não for fechado:
  - Se passou algum tempo desde o ultimo movimento automático
    - Move a cobra para a frente
  - Desenha o jogo
  - Verifica a entrada com a função `read`
    - Se apertou as setas, muda a cobra de direção e move a cobra
    - Sai do jogo se apertar `q` ou `Esc`
  - Espera algum tempo para a proxima atualização (para não fritar o pc)
- Coloca o terminal no estado normal
- Volta para a tela normal (restaurando o historico do terminal)

## Quando a cobra for movida:
- A cabeça colide com a frutinha?
  - Aumenta o tamanho da cobra
  - Acelera a cobra
  
---

# Bora ver isto a funcionar

```bash
///alacritty -e target/snake
```

