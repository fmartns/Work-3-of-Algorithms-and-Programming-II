#ifndef HOTEL_HPP
#define HOTEL_HPP
#include <iostream>
#include <string>
using namespace std;

struct Quarto{
  int numQuarto;
  string nomeQuarto;
  string fotoQuarto;
  int numCamasSolteiro;
  int numCamasCasal;
  bool temFrigobar;
  bool temArCondicionado;
  bool quartoDisponivel = true;
};

Quarto novoQuarto();
void imprimirQuarto(Quarto quarto);
void registrarQuarto(string enderecoArquivo, Quarto quarto);


#endif