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

int main()
{
    menuPrincipal();
    return 0;
}

void encerrarMenu() {
    char op;
    cout << "Digite 'c' para continuar: ";
    cin >> op;
    if (op == 'c' || op == 'C') {
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
                menuQuartos();
                encerrarMenu();
                break;
            case 2:
                menuHospedes();
                encerrarMenu();
                break;
            case 3:
                menuReservas();
                encerrarMenu();
                break;
            case 0:
                cout << "Saindo..." << endl;
                return;
            default:
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
            Quarto novo = novoQuarto();
            registrarQuarto("./quartos.xml", novo);
            encerrarMenu();
            break;
        }
        case 2: {
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
            editarQuarto("./quartos.xml");  // Chama a função para editar quarto
            encerrarMenu();
            break;
        }
        case 0:
            return;
        default:
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
            Hospede novo = novoHospede();
            registrarHospede("./hospedes.xml", novo);
            encerrarMenu();
            break;
        }
        case 2: {
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
            editarHospede("./hospedes.xml");
            encerrarMenu();
            break;
        }
        case 0:
            return;
        default:
            cout << "Opção inválida!" << endl;
            encerrarMenu();
            break;
    }
}

void menuReservas() {
    int op;
    cout << "================================" << endl;
    cout << "           Reservas" << endl;
    cout << "================================" << endl;
    cout << "(1) Check-in" << endl;
    cout << "(2) Check-out" << endl;
    cout << "(3) Mostrar reservas" << endl;
    cout << "(0) Voltar" << endl;
    cout << "Escolha uma opção: ";
    cin >> op;
    switch(op) {
        case 1: {
            Reserva reserva = novaReserva();
            checkin("./reservas.xml", reserva);
            encerrarMenu();
            break;
        }
        case 2: {
            int idReserva;
            cout << "Id da reserva: ";
            cin >> idReserva;
            checkout("./reservas.xml", idReserva);
            encerrarMenu();
            break;
        }
        case 3: {
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
        case 0:
            return;
        default:
            cout << "Opção inválida!" << endl;
            encerrarMenu();
            break;
    }
}