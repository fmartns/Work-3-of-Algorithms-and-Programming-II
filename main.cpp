#include <iostream>
#include "hotel.hpp"
#include "hospede.hpp"
#include "reserva.hpp"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

void menuPrincipal();
void menuQuartos();
void menuHospedes();
void menuReservas();
void encerrarMenu();
void limparTela();

int main()
{
    menuPrincipal();
    return 0;
}

void limparTela(){
    (void)system("clear");
}

void encerrarMenu() {
    char op;
    cout << "Digite 'c' para continuar: ";
    cin >> op;
    if (op == 'c' || op == 'C') {
        limparTela(); 
        return;
    } else {
        encerrarMenu();
    }
}

void menuPrincipal() {
    int op;
    while (true) {
        cout << "================================" << endl;
        cout << "        Sistema Hotel" << endl;
        cout << "================================" << endl;
        cout << "(1) Quartos" << endl;
        cout << "(2) Hospedes" << endl;
        cout << "(3) Reservas" << endl;
        cout << "(0) Sair" << endl;
        cout << "Escolha uma opção: ";
        cin >> op;

        switch(op) {
            case 1:
                limparTela(); 
                menuQuartos();
                encerrarMenu();
                break;
            case 2:
                limparTela();
                menuHospedes();
                encerrarMenu();
                break;
            case 3:
                limparTela();
                menuReservas();
                encerrarMenu();
                break;
            case 0:
                limparTela();
                cout << "Programa encerrado" << endl;
                return;
            default:
                limparTela();
                cout << "Opção inválida!" << endl;
                encerrarMenu();
        }
    }
}

void menuQuartos() {
    int op;
    cout << "================================" << endl;
    cout << "           Quartos" << endl;
    cout << "================================" << endl;
    cout << "(1) Cadastrar quarto" << endl;
    cout << "(2) Imprimir quartos existentes" << endl;
    cout << "(3) Editar quarto" << endl;
    cout << "(0) Voltar" << endl;
    cout << "Escolha uma opção: ";
    cin >> op;
    switch(op) {
        case 1: {
            limparTela();
            Quarto novo = novoQuarto();
            registrarQuarto("./quartos.xml", novo);
            encerrarMenu();
            break;
        }
        case 2: {
            limparTela();
            vector<Quarto> quartos = recuperarQuartos("./quartos.xml");
            cout << "Lista de quartos: " << endl;
            for(const Quarto& quarto : quartos) {
                imprimirQuarto(quarto);
                cout << "-------------------" << endl;
            }
            encerrarMenu();
            break;
        }
        case 3: {
            limparTela();
            editarQuarto("./quartos.xml");  // Chama a função para editar quarto
            encerrarMenu();
            break;
        }
        case 0:
            limparTela();
            menuPrincipal();
        default:
            limparTela();
            cout << "Opção inválida!" << endl;
            encerrarMenu();
            break;
    }
}

void menuHospedes() {
    int op;
    cout << "================================" << endl;
    cout << "           Hospedes" << endl;
    cout << "================================" << endl;
    cout << "(1) Cadastrar hóspede" << endl;
    cout << "(2) Imprimir hóspedes existentes" << endl;
    cout << "(3) Editar hóspedes" << endl;
    cout << "(0) Voltar" << endl;
    cout << "Escolha uma opção: ";
    cin >> op;
    switch(op) {
        case 1: {
            limparTela(); 
            Hospede novo = novoHospede();
            registrarHospede("./hospedes.xml", novo);
            encerrarMenu();
            break;
        }
        case 2: {
            limparTela(); 
            vector<Hospede> hospedes = recuperarHospedes("./hospedes.xml");
            cout << "Lista de hóspedes: " << endl;
            for(const Hospede& hospede : hospedes) {
                imprimirHospede(hospede);
                cout << "-------------------" << endl;
            }
            encerrarMenu();
            break;
        }
        case 3: {
            limparTela(); 
            editarHospede("./hospedes.xml");
            encerrarMenu();
            break;
        }
        case 0:
            limparTela();
            menuPrincipal();
        default:
            limparTela();   
            cout << "Opção inválida!" << endl;
            menuPrincipal();
    }
}

void menuReservas() {
    int op;
    cout << "================================" << endl;
    cout << "           Reservas" << endl;
    cout << "================================" << endl;
    cout << "(1) Fazer reserva" << endl;
    cout << "(2) Check-in" << endl;
    cout << "(3) Check-out" << endl;
    cout << "(4) Mostrar reservas" << endl;
    cout << "(5) Encontrar quarto ideal" << endl;
    cout << "(0) Voltar" << endl;
    cout << "Escolha uma opção: ";
    cin >> op;
    switch(op) {
        case 1: {
            limparTela(); 
            Reserva reserva = novaReserva();
            fazerReserva("./reservas.xml", reserva);
            encerrarMenu();
            break;
        }
        case 2: {
            limparTela(); 
            int idReserva;
            cout << "Id da reserva: ";
            cin >> idReserva;
            checkin("./reservas.xml", idReserva);
            encerrarMenu();
            break;
        }
        case 3: {
            limparTela(); 
            int idReserva;
            cout << "Id da reserva: ";
            cin >> idReserva;
            checkout("./reservas.xml", idReserva);
            encerrarMenu();
            break;
        }
        case 4: {
            limparTela(); 
            string enderecoArquivo = "./reservas.xml";
            vector<Reserva> reservas = recuperarReservas(enderecoArquivo);
            
            cout << "Lista de reservas: " << endl;
            for (const auto& reserva : reservas) {
                imprimirReserva(reserva);
                cout << endl;
            }
            encerrarMenu();
            break;
        }
        case 5: {
            limparTela(); 
            int numHospedes;
            int dia, mes, ano;
        
            cout << "Número de Hóspedes: ";
            cin >> numHospedes;
        
            cout << "Data de Entrada (DD MM AAAA): ";
            cin >> dia >> mes >> ano;
            time_t dataEntrada = tratarData(dia, mes, ano);
        
            cout << "Data de Saída (DD MM AAAA): ";
            cin >> dia >> mes >> ano;
            time_t dataSaida = tratarData(dia, mes, ano);
        
            string quartosFile = "quartos.txt";
            string reservasFile = "reservas.txt";
            
            try{
                Quarto quartoIdeal = acharQuartoIdeal(numHospedes, dataEntrada, dataSaida, "./quartos.xml", "./reservas.xml");
                cout << "Quarto Ideal Encontrado: " << endl;
                imprimirQuarto(quartoIdeal);
            } catch (const exception& e) {
                cerr << e.what() << endl;
            }
            encerrarMenu();
        }
        case 0:
            limparTela(); 
            menuPrincipal();
        default:
            limparTela(); 
            cout << "Opção inválida!" << endl;
            menuPrincipal;
    }
}