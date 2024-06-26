#ifndef RESERVA_HPP
#define RESERVA_HPP
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "hotel.hpp"
#include "hospede.hpp"
#include "reserva.hpp"

using namespace std;

struct Reserva{
    int id;
    Quarto quarto;
    Hospede hospede;
    time_t dataEntrada;
    time_t dataSaida;
    time_t dataCheckin;
    time_t dataCheckout;
    int numHospedes;
    double valorTotal;
    bool pago;
};

// Encontra o próximo Id disponível para atribuir a reserva
int proximoIDDisponivelReserva(string enderecoArquivo, string tagID);
// Converte uma data do tipo time_t para string
string timeToString(time_t time);
// Converte uma string para time_t
time_t stringToTime(string dateStr);
// Converte uma data do tipo time_t para string
time_t tratarData(int dia, int mes, int ano);
// Busca um quarto pelo número
Quarto* buscarQuartoPorNumero(int idQuarto, vector<Quarto>& quartos);
// Busca um hospede pelo ID
Hospede* buscarHospedePorId(int idHospede, vector<Hospede>& hospedes);
// Calcula a diferença de dias entre duas datas
int calcularDiferencaDias(time_t dataEntrada, time_t dataSaida);
// Recupera todas as reservas do arquivo XML
vector<Reserva> recuperarReservas(string enderecoArquivo);
// Registra uma nova reserva
Reserva novaReserva();
// Imprime as informações de uma reserva
void imprimirReserva(Reserva reserva);
// Registra uma nova reserva
void fazerReserva(string enderecoArquivo, Reserva reserva);
// Faz o checkin de uma reserva
void checkin(string enderecoArquivo, int idReserva);
// Faz o checkout de uma reserva
void checkout(string enderecoArquivo, int idReserva);
// Verifica se dois períodos de tempo se cruzam ou se coincidem em algum ponto
bool dataCruzam(time_t start1, time_t end1, time_t start2, time_t end2);
// Verifica se o quarto estará disponivel na data
bool quartoDisponivel(Quarto quarto, time_t dataEntrada, time_t dataSaida, vector<Reserva> reservas);
// Encontra o quarto ideal de acordo com o numero de hospedes e data
Quarto acharQuartoIdeal(int numHospedes, time_t dataEntrada, time_t dataSaida, string quartosFile, string reservasFile);

#endif