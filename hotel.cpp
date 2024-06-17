#include "hotel.hpp"
#include <string>
#include "tinyxml2.h"
#include <fstream>

using namespace std;

Quarto novoQuarto() {
    cout << "Informe o número do quarto: ";
    int numQuarto;
    cin >> numQuarto;
    cin.ignore();  // Ignorar o newline deixado por cin

    cout << "Informe o nome do quarto: ";
    string nomeQuarto;
    getline(cin, nomeQuarto);

    cout << "Cole o url com a foto do quarto: ";
    string fotoQuarto;
    cin >> fotoQuarto;

    cout << "Número de camas de solteiro: ";
    int numCamasSolteiro;
    cin >> numCamasSolteiro;

    cout << "Número de camas de casal: ";
    int numCamasCasal;
    cin >> numCamasCasal;

    cout << "O quarto tem frigobar? (s/n) ";
    char resposta;
    cin >> resposta;
    bool temFrigobar = (resposta == 's' || resposta == 'S');

    cout << "O quarto tem ar condicionado? (s/n) ";
    cin >> resposta;
    bool temArCondicionado = (resposta == 's' || resposta == 'S');

    Quarto quarto = {numQuarto, nomeQuarto, fotoQuarto, numCamasSolteiro, numCamasCasal, temFrigobar, temArCondicionado, true};

    return quarto;
}

void imprimirQuarto(Quarto quarto) {
    cout << "Número do quarto: " << quarto.numQuarto << endl;
    cout << "Nome quarto: " << quarto.nomeQuarto << endl;
    cout << "Número de camas de solteiro: " << quarto.numCamasSolteiro << endl;
    cout << "Numero de camas de casal: " << quarto.numCamasCasal << endl;
    cout << (quarto.temFrigobar ? "Possui frigobar" : "Não possui frigobar") << endl;
    cout << (quarto.temArCondicionado ? "Possui ar condicionado" : "Não possui ar condicionado") << endl;
    cout << (quarto.quartoDisponivel ? "Quarto disponível!" : "Quarto indisponível!") << endl;
}

void registrarQuarto(string enderecoArquivo, Quarto quarto) {
    tinyxml2::XMLDocument arquivo;

    tinyxml2::XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != tinyxml2::XML_SUCCESS) {
        arquivo.InsertEndChild(arquivo.NewDeclaration());
        tinyxml2::XMLElement* raiz = arquivo.NewElement("hotel");
        arquivo.InsertEndChild(raiz);
    }

    tinyxml2::XMLElement* raiz = arquivo.FirstChildElement("hotel");
    if (!raiz) {
        raiz = arquivo.NewElement("hotel");
        arquivo.InsertEndChild(raiz);
    }

    tinyxml2::XMLElement* elementoQuarto = arquivo.NewElement("quarto");

    tinyxml2::XMLElement* numQuarto = arquivo.NewElement("numQuarto");
    numQuarto->SetText(quarto.numQuarto);
    elementoQuarto->InsertEndChild(numQuarto);

    tinyxml2::XMLElement* nomeQuarto = arquivo.NewElement("nomeQuarto");
    nomeQuarto->SetText(quarto.nomeQuarto.c_str());
    elementoQuarto->InsertEndChild(nomeQuarto);

    tinyxml2::XMLElement* fotoQuarto = arquivo.NewElement("fotoQuarto");
    fotoQuarto->SetText(quarto.fotoQuarto.c_str());
    elementoQuarto->InsertEndChild(fotoQuarto);

    tinyxml2::XMLElement* numCamasSolteiro = arquivo.NewElement("numCamasSolteiro");
    numCamasSolteiro->SetText(quarto.numCamasSolteiro);
    elementoQuarto->InsertEndChild(numCamasSolteiro);

    tinyxml2::XMLElement* numCamasCasal = arquivo.NewElement("numCamasCasal");
    numCamasCasal->SetText(quarto.numCamasCasal);
    elementoQuarto->InsertEndChild(numCamasCasal);

    tinyxml2::XMLElement* temFrigobar = arquivo.NewElement("temFrigobar");
    temFrigobar->SetText(quarto.temFrigobar ? "true" : "false");
    elementoQuarto->InsertEndChild(temFrigobar);

    tinyxml2::XMLElement* temArCondicionado = arquivo.NewElement("temArCondicionado");
    temArCondicionado->SetText(quarto.temArCondicionado ? "true" : "false");
    elementoQuarto->InsertEndChild(temArCondicionado);

    tinyxml2::XMLElement* quartoDisponivel = arquivo.NewElement("quartoDisponivel");
    quartoDisponivel->SetText(quarto.quartoDisponivel ? "true" : "false");
    elementoQuarto->InsertEndChild(quartoDisponivel);

    raiz->InsertEndChild(elementoQuarto);

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != tinyxml2::XML_SUCCESS) {
        cout << "Erro ao salvar arquivo XML: " << retorno << endl;
    }
}
