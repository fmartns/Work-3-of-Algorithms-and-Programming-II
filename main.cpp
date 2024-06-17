#include <iostream>
#include "hotel.hpp"

using namespace std;

int main()
{

    Quarto quarto = {
            101,
            "Suíte Luxo",
            "foto_suite_luxo.jpg",
            2,
            1,
            true,
            true,
            true
    };

    // Chama a função registrarQuarto para salvar os dados no arquivo XML
    registrarQuarto("quartosHotel.xml", quarto);

    // Informa que o arquivo XML foi salvo com sucesso
    cout << "Arquivo XML salvo com sucesso!" << endl;

    return 0;
}