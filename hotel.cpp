#include "hotel.hpp"
#include <string>
#include "tinyxml2.h"
#include <fstream>
#include <vector>

using namespace tinyxml2;
using namespace std;

// Verifica se há repetição de derminado valor dentro de um arquivo XML.
bool verificarRepeticao(string enderecoArquivo,  string tag, string dado) {

    // Carrega o arquivo XML
    XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.RootElement();

    for (XMLElement* quartoXML = raiz->FirstChildElement("Quarto"); quartoXML; quartoXML = quartoXML->NextSiblingElement("Quarto")) {
        XMLElement* elemento = quartoXML->FirstChildElement(tag.c_str());
        if (elemento) {
            const char* comparativo = elemento->GetText();
            if (comparativo && dado == comparativo) {
                return true;
            }
        }
    }

    return false;
}

// Recupera todas as informações do arquivo XML e as coloca num vetor
vector<Quarto> recuperarQuartos(string enderecoArquivo) {
    // Cria um vetor de quartos
    vector<Quarto> quartos;

    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;
    tinyxml2::XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    // Verifica se foi carregado corretamente
    if (retorno != tinyxml2::XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
        return {};
    }

    // Define a raiz do arquivo XML
    tinyxml2::XMLElement* raiz = arquivo.FirstChildElement("Hotel");

    for (tinyxml2::XMLElement* quartoXML = raiz->FirstChildElement("Quarto"); quartoXML; quartoXML = quartoXML->NextSiblingElement("Quarto")) {
        Quarto quarto;
        
        quarto.numeroQuarto = quartoXML->FirstChildElement("NumeroQuarto")->IntText();
        quarto.tipo = quartoXML->FirstChildElement("Tipo")->GetText();
        quarto.foto = quartoXML->FirstChildElement("UrlFoto")->GetText();
        quarto.descricao = quartoXML->FirstChildElement("Descricao")->GetText();
        quarto.camasSolteiro = quartoXML->FirstChildElement("CamasSolteiro")->IntText();
        quarto.camasCasal = quartoXML->FirstChildElement("CamasCasal")->IntText();

        const char* dadoFrigobar = quartoXML->FirstChildElement("Frigobar")->GetText();
        quarto.frigobar = (dadoFrigobar != nullptr && strcmp(dadoFrigobar, "true") == 0);

        const char* dadoArCondicionado = quartoXML->FirstChildElement("ArCondicionado")->GetText();
        quarto.arCondicionado = (dadoArCondicionado != nullptr && strcmp(dadoArCondicionado, "true") == 0);
        
        quarto.valor = quartoXML->FirstChildElement("Valor")->DoubleText();

        const char* dadoDisponivel = quartoXML->FirstChildElement("Disponivel")->GetText();
        quarto.disponivel = (dadoDisponivel != nullptr && strcmp(dadoDisponivel, "true") == 0);

        quartos.push_back(quarto);
    }

    return quartos;
}

// Imprime as informações dos quartos
void imprimirQuarto(Quarto quarto) {
    
    cout << "Nro do quarto: " << quarto.numeroQuarto << endl;
    cout << "Nome quarto: " << quarto.tipo << endl;
    cout << "Descrição: " << quarto.descricao << endl;
    cout << "Nro camas solteiro: " << quarto.camasSolteiro << endl;
    cout << "Nro camas casal: " << quarto.camasCasal << endl;
    cout << "Possui frigobar: ";
    cout << (quarto.frigobar ? "Sim" : "Não") << endl;
    cout << "Ar condicionado: ";
    cout << (quarto.arCondicionado ? "Sim" : "Não") << endl;
    cout << "Valor diaria: R$ " << quarto.valor << endl;
    cout << "Disponivel: ";
    cout << (quarto.disponivel ? "Sim" : "Não") << endl;
    
}

// Recebe as informações para criação de um novo quarto de hotel
Quarto novoQuarto() {
    
    cout << "Informe o número do quarto: ";
    int numeroQuarto;
    cin >> numeroQuarto;
    
    // Verifica repetições e impede que seja feito a criação de mais de um quarto com o mesmo número.
    bool verificar = verificarRepeticao("./quartos.xml", "NumeroQuarto", to_string(numeroQuarto));
    if(verificar) {
        while(verificar){
            cout << "Número do quarto em uso. Escolha outro: ";
            cin >> numeroQuarto;
            verificar = verificarRepeticao("./quartos.xml", "NumeroQuarto", to_string(numeroQuarto));
        }
    }
    
    cin.ignore();

    cout << "Informe o nome do quarto: ";
    string tipo;
    getline(cin, tipo);

    cout << "Cole o url com a foto do quarto: ";
    string foto;
    cin >> foto;
    cin.ignore();

    cout << "Informe a descrição do quarto: ";
    string descricao;
    getline(cin, descricao);

    cout << "Número de camas de solteiro: ";
    int camasSolteiro;
    cin >> camasSolteiro;

    cout << "Número de camas de casal: ";
    int camasCasal;
    cin >> camasCasal;

    cout << "O quarto tem frigobar? (s/n) ";
    char resposta;
    cin >> resposta;
    bool frigobar = (resposta == 's' || resposta == 'S');

    cout << "O quarto tem ar condicionado? (s/n) ";
    cin >> resposta;
    bool arCondicionado = (resposta == 's' || resposta == 'S'); // Verifica se a resposta é "s" ou "S" e define como true, caso contrario é false.
    
    cout << "Valor da diária: ";
    double valor;
    cin >> valor;

    Quarto quarto = {numeroQuarto, tipo, foto, descricao, camasSolteiro, camasCasal, frigobar, arCondicionado, valor, true};

    return quarto;
}

// Faz o registro das informações cadastradas dentro do arquivo XML.
void registrarQuarto(string enderecoArquivo, Quarto quarto) {
    
    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;

    // Verifica se o arquivo carregou corretamente e inclui as tags base.
    tinyxml2::XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != tinyxml2::XML_SUCCESS) {
        arquivo.InsertEndChild(arquivo.NewDeclaration());
        tinyxml2::XMLElement* raiz = arquivo.NewElement("Hotel");
        arquivo.InsertEndChild(raiz);
    }
    
    tinyxml2::XMLElement* raiz = arquivo.FirstChildElement("Hotel");
    if (!raiz) {
        raiz = arquivo.NewElement("Hotel");
        arquivo.InsertEndChild(raiz);
    }

    tinyxml2::XMLElement* elementoQuarto = arquivo.NewElement("Quarto");

    tinyxml2::XMLElement* numeroQuarto = arquivo.NewElement("NumeroQuarto");
    numeroQuarto->SetText(quarto.numeroQuarto);
    elementoQuarto->InsertEndChild(numeroQuarto);

    tinyxml2::XMLElement* tipo = arquivo.NewElement("Tipo");
    tipo->SetText(quarto.tipo.c_str());
    elementoQuarto->InsertEndChild(tipo);

    tinyxml2::XMLElement* foto = arquivo.NewElement("UrlFoto");
    foto->SetText(quarto.foto.c_str());
    elementoQuarto->InsertEndChild(foto);

    tinyxml2::XMLElement* descricao = arquivo.NewElement("Descricao");
    descricao->SetText(quarto.descricao.c_str());
    elementoQuarto->InsertEndChild(descricao);

    tinyxml2::XMLElement* camasSolteiro = arquivo.NewElement("CamasSolteiro");
    camasSolteiro->SetText(quarto.camasSolteiro);
    elementoQuarto->InsertEndChild(camasSolteiro);

    tinyxml2::XMLElement* camasCasal = arquivo.NewElement("CamasCasal");
    camasCasal->SetText(quarto.camasCasal);
    elementoQuarto->InsertEndChild(camasCasal);

    tinyxml2::XMLElement* frigobar = arquivo.NewElement("Frigobar");
    frigobar->SetText(quarto.frigobar ? "true" : "false");
    elementoQuarto->InsertEndChild(frigobar);

    tinyxml2::XMLElement* arCondicionado = arquivo.NewElement("ArCondicionado");
    arCondicionado->SetText(quarto.arCondicionado ? "true" : "false");
    elementoQuarto->InsertEndChild(arCondicionado);

    tinyxml2::XMLElement* valor = arquivo.NewElement("Valor");
    valor->SetText(quarto.valor);
    elementoQuarto->InsertEndChild(valor);

    tinyxml2::XMLElement* disponivel = arquivo.NewElement("Disponivel");
    disponivel->SetText(quarto.disponivel ? "true" : "false");
    elementoQuarto->InsertEndChild(disponivel);

    // Adiciona o novo elemento ao final do XML
    raiz->InsertEndChild(elementoQuarto);

    // Salva o arquivo
    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
}

// Possibilita a edição das principais informações de cada quarto.
void editarQuarto(string enderecoArquivo) {
    
    // Recebe o número do quarto a ser editado
    int numeroQuarto;
    cout << "Informe o número do quarto a ser editado: ";
    cin >> numeroQuarto;

    // Carrega o arquivo XML
    XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro ao carregar o arquivo XML: " << retorno << endl;
        return;
    }

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.FirstChildElement("Hotel");
    if (!raiz) {
        cout << "Elemento raiz 'Hotel' não encontrado no XML." << endl;
        return;
    }

    // Limpa a posição do vetor de quarto
    XMLElement* quartoXML = nullptr;
    
    // Busca a posição do quarto de acordo com o número e verifica se o mesmo foi encontrado
    for (XMLElement* posicao = raiz->FirstChildElement("Quarto"); posicao; posicao = posicao->NextSiblingElement("Quarto")) {
        int numero;
        posicao->FirstChildElement("NumeroQuarto")->QueryIntText(&numero);
        if (numero == numeroQuarto) {
            quartoXML = posicao;
            break;
        }
    }

    if (!quartoXML) {
        cout << "Quarto " << numeroQuarto << " não encontrado." << endl;
        return;
    }

    string tipo;
    string foto;
    string descricao;
    int camasSolteiro;
    int camasCasal;
    char resposta;
    bool frigobar;
    bool arCondicionado;
    double valor;

    cin.ignore();

    cout << "Informe o novo nome do quarto: ";
    getline(cin, tipo);

    cout << "Cole o novo url com a foto do quarto: ";
    getline(cin, foto);

    cout << "Informe a nova descrição do quarto: ";
    getline(cin, descricao);

    cout << "Número de camas de solteiro: ";
    cin >> camasSolteiro;

    cout << "Número de camas de casal: ";
    cin >> camasCasal;
    
    cout << "Valor da diária: ";
    cin >> valor;

    cout << "O quarto tem frigobar? (s/n) ";
    cin >> resposta;
    frigobar = (resposta == 's' || resposta == 'S');

    cout << "O quarto tem ar condicionado? (s/n) ";
    cin >> resposta;
    arCondicionado = (resposta == 's' || resposta == 'S');

    quartoXML->FirstChildElement("Tipo")->SetText(tipo.c_str());
    quartoXML->FirstChildElement("UrlFoto")->SetText(foto.c_str());
    quartoXML->FirstChildElement("Descricao")->SetText(descricao.c_str());
    quartoXML->FirstChildElement("CamasSolteiro")->SetText(camasSolteiro);
    quartoXML->FirstChildElement("CamasCasal")->SetText(camasCasal);
    quartoXML->FirstChildElement("Frigobar")->SetText(frigobar ? "true" : "false");
    quartoXML->FirstChildElement("ArCondicionado")->SetText(arCondicionado ? "true" : "false");
    quartoXML->FirstChildElement("Valor")->SetText(valor);

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
    } else {
        cout << "Quarto atualizado com sucesso!" << endl;
    }
}
