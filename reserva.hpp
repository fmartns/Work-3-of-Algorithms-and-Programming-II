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
    int numHospedes;
    double valorTotal;
    bool pago;
};

// Encontra o próximo Id disponível para atribuir a reserva
int proximoIDDisponivelReserva(const string& enderecoArquivo, const string& tagID);
// Converte uma data do tipo time_t para string
string timeToString(time_t time);
// Converte uma string para time_t
time_t stringToTime(string dateStr);
// Converte uma data do tipo time_t para string
time_t tratarData(int dia, int mes, int ano);
// Busca um quarto pelo número
Quarto* buscarQuartoPorNumero(int idQuarto);
// Busca um hospede pelo ID
Hospede* buscarHospedePorId(int idHospede);
// Calcula a diferença de dias entre duas datas
int calcularDiferencaDias(time_t dataEntrada, time_t dataSaida);
// Recupera todas as reservas do arquivo XML
vector<Reserva> recuperarReservas(string enderecoArquivo);
// Imprime as informações de uma reserva
void imprimirReserva(Reserva reserva);
// Faz o checkin de uma reserva
void checkin(string enderecoArquivo, Reserva reserva);
// Faz o checkout de uma reserva
void checkout(string enderecoArquivo, int idReserva);
// Registra uma nova reserva
Reserva novaReserva();;

#endif