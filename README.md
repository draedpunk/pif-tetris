# CLI-Tetris 🧩  
![menutetris](https://github.com/user-attachments/assets/9c4723d6-bbff-4756-bb81-7771849a2f73)

<details>
<summary><strong>Ver detalhes do projeto</strong></summary>
<br>

## 01. Membros do Projeto
- Diego Xavier ([@elbedie](https://github.com/elbedie))
- Hyngrid Souza e Silva ([@Hyngras](https://github.com/Hyngras))
- Maria Gabriela Damásio Bezerra ([@gabidamasio](https://github.com/gabidamasio))
- Pamela Teixeira Rodrigues ([@draedpunk](https://github.com/draedpunk))

## 02. Disciplina
Programação Imperativa e Funcional - 2025.1

## 03. Instituição de Ensino
CESAR School

## 04. Mecânica do Jogo
Tetris é um jogo clássico onde o jogador deve posicionar peças chamadas tetraminós que caem do topo da área jogável. Cada vez que uma linha é completamente preenchida, ela é eliminada e o jogador ganha pontos.

Nesta versão em terminal, algumas peças ganharam uma funcionalidade especial: a capacidade de explodir blocos ao seu redor ao tocarem outras peças, tornando o jogo mais desafiador. Gerada aleatoriamente, a peça explosiva poderá dificultar ou também ajudar nos momentos em que o espaço da área jogável estiver menor.

<p align="center">
  <img src="https://github.com/user-attachments/assets/d84f1c2b-3d64-4749-8b22-9d7ceeaba754" width="45%"/>
  <img src="https://github.com/user-attachments/assets/067fcf19-8368-4818-ae8c-651bb3fc5050" width="45%"/>
</p>

## 05. Pontuação
- Cada peça posicionada concede **25 pontos**.
- Linhas completas eliminadas rendem **+75 pontos por linha**.
- Peça especial explosiva gera uma **penalidade de -50 pontos**, mesmo se eliminar linhas.
- A função de pontuação **prioriza a verificação da peça explosiva** antes de pontuar.
- A cada **5 linhas eliminadas**, o jogador **sobe de nível**.
- Níveis mais altos aumentam a **velocidade de queda das peças**, tornando o jogo mais desafiador.

## 06. Regras e Funcionamento 
- As peças caem uma de cada vez e podem ser movidas antes de tocar o fundo ou outras peças.
- O controle é feito pelas teclas WASD.
- Linhas completas são eliminadas e convertidas em pontos.
- A velocidade das peças aumenta com o tempo ou com o progresso do jogador.
- O jogo encerra quando não há mais espaço para novas peças.

## 07. Controles
- **W**: girar a peça  
- **A**: mover para a esquerda  
- **S**: acelerar a queda  
- **D**: mover para a direita  

## 08. Requisitos e Execução

Este jogo foi desenvolvido para sistemas baseados em Unix, como **Linux** e **macOS**, mas também pode ser executado no **Windows** utilizando o **WSL (Windows Subsystem for Linux)**.

### Requisitos:
- Ambiente Linux, macOS ou Windows com WSL
- Compilador C (como `gcc`)
- Utilitário `make` instalado

### Para compilar e executar o jogo:

1. Clone o repositório:
   ```bash
   git clone https://github.com/draedpunk/pif-tetris.git
   cd pif-tetris
   ```
2. Compilação e execução:
    ```bash
   make
   ./build/tetris
    ```
OBS.: Importante rodar no terminal Linux ou no WSL no caso de Windows.
   </details>
