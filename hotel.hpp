#ifndef HOTEL_HPP
#define HOTEL_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Quarto{
  int numeroQuarto;
  string tipo;
  string foto;
  string descricao;
  int camasSolteiro;
  int camasCasal;
  bool frigobar;
  bool arCondicionado;
  double valor;
  bool disponivel = true;
};

// Verifica se há repetição de derminado valor dentro de um arquivo XML.
bool verificarRepeticao(string enderecoArquivo, string tag, string dado);
// Recupera todas as informações do arquivo XML e as coloca num vetor
vector<Quarto> recuperarQuartos(string enderecoArquivo);
// Imprime as informações dos quartos
void imprimirQuarto(Quarto quarto);
// Recebe as informações para criação de um novo quarto de hotel
Quarto novoQuarto();
// Faz o registro das informações cadastradas dentro do arquivo XML.
void registrarQuarto(string enderecoArquivo, Quarto quarto);
// Possibilita a edição das principais informações de cada quarto.
void editarQuarto(string enderecoArquivo);

#endif