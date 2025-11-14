# 🗺️ Sistema de Cadastro de Territórios — Linguagem C

Este repositório contém a implementação de um sistema simples de cadastro de territórios para fins acadêmicos, utilizando **linguagem C**, **structs** e vetores para organizar dados relacionados.  
O programa foi desenvolvido como parte de um desafio para reforçar o entendimento sobre estruturas compostas e manipulação de dados no contexto da programação estrutural.

---

# 📘 1. Introdução

Este projeto apresenta um programa em C que realiza o cadastro de **cinco territórios**, armazenando informações como nome, cor do exército e número de tropas.  
A atividade tem como objetivo aplicar conceitos fundamentais da linguagem, incluindo:

- Definição e uso de `structs`
- Manipulação de vetores compostos
- Entrada e saída de dados
- Organização lógica e documentada do código

---

# 🎯 2. Objetivos

### **Objetivo geral**
Implementar um sistema em C que utilize estruturas compostas (`struct`) para armazenar e exibir dados relacionados a territórios.

### **Objetivos específicos**
- Criar uma `struct` personalizada chamada `Territorio`.
- Utilizar um vetor de estruturas para armazenar múltiplos registros.
- Permitir que o usuário cadastre dados manualmente.
- Exibir os dados formatados após o registro.
- Comentar e organizar o código conforme boas práticas.

---

# 🧠 3. Conceitos Técnicos

## 3.1 Estrutura (`struct Territorio`)
A struct funciona como um **agrupador de dados**, permitindo combinar variáveis de tipos diferentes que se relacionam entre si.

```c
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


Exibição dos dados

Após o cadastro, outro laço percorre o vetor e exibe as informações.
Esse processo demonstra:

Iteração estruturada

Acesso aos campos da struct

Formatação de dados em C

Desenvolvimento do Programa

Fluxo de execução

Inicializa vetor de 5 territórios

Executa laço de entrada:

Pede nome, cor e tropas

Armazena na struct

Executa laço de saída:

Exibe os dados formatados

Encerra o programa
📊 Resultados

Ao final da execução, o sistema mostra uma listagem organizada com os cinco territórios cadastrados.
Esse resultado demonstra o uso correto de:

structs compostas

vetores multidimensionais de dados

entrada e saída estruturada

organização lógica de um programa simples em C

🧾 Conclusão

Este projeto cumpriu seu objetivo educacional ao demonstrar, de forma prática,
o uso de estruturas compostas (structs) para armazenar e manipular dados relacionados em C.
Além disso, o programa reforça habilidades essenciais:

Organização e modularidade do código

Manipulação de arrays e estruturas

Interação com o usuário

Documentação e boas práticas

A solução é simples, eficiente e serve como base para sistemas maiores que
envolvam cadastros, bancos de dados
em memória e gerenciamento de entidades relacionadas.

👩‍💻 Autora

Jaqueline Santos
Projeto acadêmico — Estruturas de Dados em C
