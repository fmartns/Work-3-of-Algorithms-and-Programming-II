#ifndef HOSPEDE_HPP
#define HOSPEDE_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Hospede{
    int id;
    string nome;
    string sobrenome;
    string cpf;
    string cep;
    string logradouro;
    int num;
    string bairro;
    string cidade;
    string complemento;
    string email;
    string telefone;
};

// Encontra o próximo Id disponível para atribuir ao hospede
int proximoIDDisponivel(string enderecoArquivo, string tagID);
// Recupera todas as informações do arquivo XML e as coloca num vetor
vector<Hospede> recuperarHospedes(string enderecoArquivo);
// Imprime as informações dos quartos
void imprimirHospede(Hospede hospede);
// Recebe as informações para criação de um novo hospede
Hospede novoHospede();
// Faz o registro das informações cadastradas dentro do arquivo XML.
void registrarHospede(string enderecoArquivo, Hospede hospede);
// Possibilita a edição das principais informações de cada quarto.
void editarHospede(string enderecoArquivo);

#endif