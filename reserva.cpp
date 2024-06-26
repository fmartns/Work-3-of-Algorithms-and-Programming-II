#include "reserva.hpp"
#include "hotel.hpp"
#include "hospede.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace tinyxml2;
using namespace std;

// Encontra o próximo Id disponível para atribuir a reserva
int proximoIDDisponivelReserva(string enderecoArquivo, string tagID) {

    // Carrega o arquivo XML
    XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.RootElement();

    vector<int> ids;
    for (XMLElement* elemento = raiz->FirstChildElement("Reserva"); elemento; elemento = elemento->NextSiblingElement("Reserva")) {
        XMLElement* elementoID = elemento->FirstChildElement(tagID.c_str());
        if (elementoID) {
            const char* dadoId = elementoID->GetText();
            if (dadoId) {
                int id = atoi(dadoId);
                ids.push_back(id);
            }
        }
    }

    sort(ids.begin(), ids.end());

    int proximoID = 1;

    for (int id : ids) {
        if (id == proximoID) {
            proximoID++;
        } else {
            break;
        }
    }

    return proximoID;
}

// Converte uma data do tipo time_t para string
string timeToString(time_t time) {
    struct tm *tm_info = localtime(&time);
    char buffer[11];
    strftime(buffer, 11, "%d/%m/%Y", tm_info);
    return string(buffer);
}

// Converte uma string para time_t
time_t stringToTime(string dateStr) {
    struct tm tm = {};
    if (strptime(dateStr.c_str(), "%d/%m/%Y", &tm) != nullptr) {
        return mktime(&tm);
    }
    return 0;
}

// Trata a data para o formato time_t
time_t tratarData(int dia, int mes, int ano){
    
    tm data = {0};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;
    time_t dataRetorno = mktime(&data);
    
    return dataRetorno;
}

// Busca um quarto pelo número
Quarto* buscarQuartoPorNumero(int idQuarto, vector<Quarto>& quartos) {
    // Busca o quarto pelo número
    for (auto& quarto : quartos) {
        if (quarto.numeroQuarto == idQuarto) {
            return &quarto;
        }
    }

    // Retorna nullptr caso não encontre o quarto
    return nullptr;
}

// Busca um hospede pelo ID
Hospede* buscarHospedePorId(int idHospede, vector<Hospede>& hospedes) {

    for (auto& hospede : hospedes) {
        if (hospede.id == idHospede) {
            return &hospede;
        }
    }

    // Retorna nullptr caso não encontre o quarto
    return nullptr;
}

// Calcula a diferença de dias entre duas datas
int calcularDiferencaDias(time_t dataEntrada, time_t dataSaida) {

    double diferenca = difftime(dataSaida, dataEntrada);
    int dias = diferenca / 86400;

    return dias;
}

// Recupera todas as reservas do arquivo XML
vector<Reserva> recuperarReservas(string enderecoArquivo) {

    // Vetor de reservas
    vector<Reserva> reservas;

    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;
    tinyxml2::XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    if (retorno != tinyxml2::XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
        return {};
    }

    // Define a raiz do arquivo XML
    tinyxml2::XMLElement* raiz = arquivo.FirstChildElement("Hotel");

    for (tinyxml2::XMLElement* reservaXML = raiz->FirstChildElement("Reserva"); reservaXML; reservaXML = reservaXML->NextSiblingElement("Reserva")) {
        
        int id = 0;
        Quarto quarto;
        Hospede hospede;
        time_t dataEntrada = 0;
        time_t dataSaida = 0;
        time_t dataCheckin = 0;
        time_t dataCheckout = 0;
        int numHospedes = 0;
        double valorTotal = 0.0;
        bool pago = false;

        reservaXML->FirstChildElement("Id")->QueryIntText(&id);

        XMLElement* elementoHospede = reservaXML->FirstChildElement("Hospede");
        if (elementoHospede) {
            hospede.id = elementoHospede->IntAttribute("id");
            hospede.nome = elementoHospede->Attribute("nome");
            hospede.sobrenome = elementoHospede->Attribute("sobrenome");
            hospede.cpf = elementoHospede->Attribute("cpf");
            hospede.cep = elementoHospede->Attribute("cep");
            hospede.logradouro = elementoHospede->Attribute("logradouro");
            hospede.num = elementoHospede->IntAttribute("num");
            hospede.bairro = elementoHospede->Attribute("bairro");
            hospede.cidade = elementoHospede->Attribute("cidade");
            hospede.complemento = elementoHospede->Attribute("complemento");
            hospede.email = elementoHospede->Attribute("email");
            hospede.telefone = elementoHospede->Attribute("telefone");
        }

        XMLElement* elementoQuarto = reservaXML->FirstChildElement("Quarto");
        if (elementoQuarto) {
            quarto.numeroQuarto = elementoQuarto->IntAttribute("numeroQuarto");
            quarto.tipo = elementoQuarto->Attribute("tipo");
            quarto.foto = elementoQuarto->Attribute("foto");
            quarto.descricao = elementoQuarto->Attribute("descricao");
            quarto.camasSolteiro = elementoQuarto->IntAttribute("camasSolteiro");
            quarto.camasCasal = elementoQuarto->IntAttribute("camasCasal");
            quarto.frigobar = elementoQuarto->BoolAttribute("frigobar");
            quarto.arCondicionado = elementoQuarto->BoolAttribute("arCondicionado");
            quarto.valor = elementoQuarto->DoubleAttribute("valor");
            quarto.disponivel = elementoQuarto->BoolAttribute("disponivel");
        }

        XMLElement* elementoDataEntrada = reservaXML->FirstChildElement("DataEntrada");
        if (elementoDataEntrada) {
            dataEntrada = stringToTime(elementoDataEntrada->GetText());
            if (dataEntrada == 0) {
                cout << "Erro ao converter DataEntrada: " << elementoDataEntrada->GetText() << endl;
            }
        }

        XMLElement* elementoDataSaida = reservaXML->FirstChildElement("DataSaida");
        if (elementoDataSaida) {
            dataSaida = stringToTime(elementoDataSaida->GetText());
            if (dataSaida == 0) {
                cout << "Erro ao converter DataSaida: " << elementoDataSaida->GetText() << endl;
            }
        }
        
        XMLElement* elementoDataCheckin = reservaXML->FirstChildElement("DataCheckin");
        if (elementoDataCheckin) {
            dataCheckin = stringToTime(elementoDataCheckin->GetText());
            if (dataCheckin == 0) {
                cout << "Erro ao converter Data Checkin: " << elementoDataCheckin->GetText() << endl;
            }
        }
        
        XMLElement* elementoDataCheckout = reservaXML->FirstChildElement("DataCheckout");
        if (elementoDataCheckout) {
            dataCheckout = stringToTime(elementoDataCheckout->GetText());
            if (dataCheckout == 0) {
                cout << "Erro ao converter Data Checkout: " << elementoDataCheckout->GetText() << endl;
            }
        }

        XMLElement* elementoNumHospedes = reservaXML->FirstChildElement("NumHospedes");
        if (elementoNumHospedes) {
            elementoNumHospedes->QueryIntText(&numHospedes);
        }

        XMLElement* elementoValorTotal = reservaXML->FirstChildElement("ValorTotal");
        if (elementoValorTotal) {
            elementoValorTotal->QueryDoubleText(&valorTotal);
        }

        XMLElement* elementoPago = reservaXML->FirstChildElement("Pago");
        if (elementoPago) {
            const char* text = elementoPago->GetText();
            pago = (text && strcmp(text, "true") == 0);
        }

        Reserva reserva = {id, quarto, hospede, dataEntrada, dataSaida, dataCheckin, dataCheckout, numHospedes, valorTotal, pago};
        reservas.push_back(reserva);
    }

    return reservas;
}

// Registra uma nova reserva
Reserva novaReserva() {
    Reserva reserva;

    int numeroQuarto;
    cout << "Digite o número do quarto: ";
    cin >> numeroQuarto;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<Quarto> quartos = recuperarQuartos("./quartos.xml");
    Quarto* quarto = buscarQuartoPorNumero(numeroQuarto, quartos);
    while (!quarto) {
        cout << "Quarto não encontrado! Digite o número do quarto: ";
        cin >> numeroQuarto;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        quarto = buscarQuartoPorNumero(numeroQuarto, quartos);
    }
    reserva.quarto = *quarto;

    int idHospede;
    cout << "Digite o id do hospede: ";
    cin >> idHospede;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<Hospede> hospedes = recuperarHospedes("./hospedes.xml");
    Hospede* hospede = buscarHospedePorId(idHospede, hospedes);
    while (!hospede) {
        cout << "Hospede não encontrado! Digite o id do hospede: ";
        cin >> idHospede;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        hospede = buscarHospedePorId(idHospede, hospedes);
    }
    reserva.hospede = *hospede;

    // Coleta da data de entrada
    int dia, mes, ano;
    cout << "Digite a data de entrada (DD MM AAAA): ";
    cin >> dia >> mes >> ano;
    // Limpa o buffer de entrada
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    reserva.dataEntrada = tratarData(dia, mes, ano);

    // Coleta da data de saída
    cout << "Digite a data de saída (DD MM AAAA): ";
    cin >> dia >> mes >> ano;
    // Limpa o buffer de entrada
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    reserva.dataSaida = tratarData(dia, mes, ano);

    // Coleta do número de hóspedes
    cout << "Digite o número de hóspedes: ";
    cin >> reserva.numHospedes;
    // Limpa o buffer de entrada
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Inicializa outros campos da reserva
    reserva.id = proximoIDDisponivelReserva("./reservas.xml", "Id");
    reserva.dataCheckin = 0; // Pode ser inicializado conforme necessário
    reserva.dataCheckout = 0; // Pode ser inicializado conforme necessário
    reserva.valorTotal = 0.0; // Pode ser inicializado conforme necessário
    reserva.pago = false;

    return reserva;
}

// Imprime as informações de uma reserva
void imprimirReserva(Reserva reserva) {

    cout << "ID: " << reserva.id << endl;
    cout << "Num Quarto: " << reserva.quarto.numeroQuarto << endl;
    cout << "Hospede: " << reserva.hospede.nome << " " << reserva.hospede.sobrenome << endl;
    cout << "Data de Entrada: " << timeToString(reserva.dataEntrada) << endl;
    cout << "Data de Saida: " << timeToString(reserva.dataSaida) << endl;
    if (reserva.dataCheckin != 0) {
        cout << "Data de Check-in: " << timeToString(reserva.dataCheckin) << endl;
    } else {
        cout << "Data de Check-in: Não disponível" << endl;
    }
    if (reserva.dataCheckout != 0) {
        cout << "Data de Check-out: " << timeToString(reserva.dataCheckout) << endl;
        cout << "Pago: " << (reserva.pago ? "Sim" : "Não") << endl;
        cout << "Valor Total: R$ " << fixed << setprecision(2) << reserva.valorTotal << endl;
    } else {
        cout << "Data de Check-out: Não disponível" << endl;
        cout << "Pago: Não dsiponivel" << endl;
        cout << "Valor Total: Não disponivel" << endl;
    }


}

// Registra uma nova reserva
void fazerReserva(string enderecoArquivo, Reserva reserva) {

    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;

    // Verifica se o arquivo existe
    tinyxml2::XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != tinyxml2::XML_SUCCESS) {
        arquivo.InsertEndChild(arquivo.NewDeclaration());
        tinyxml2::XMLElement* raiz = arquivo.NewElement("Hotel");
        arquivo.InsertEndChild(raiz);
    }

    // Define a raiz do arquivo XML
    tinyxml2::XMLElement* raiz = arquivo.FirstChildElement("Hotel");

    tinyxml2::XMLElement* elementoReserva = arquivo.NewElement("Reserva");

    tinyxml2::XMLElement* id = arquivo.NewElement("Id");
    id->SetText(reserva.id);
    elementoReserva->InsertEndChild(id);

    tinyxml2::XMLElement* elementoHospede = arquivo.NewElement("Hospede");
    elementoHospede->SetAttribute("id", reserva.hospede.id);
    elementoHospede->SetAttribute("nome", reserva.hospede.nome.c_str());
    elementoHospede->SetAttribute("sobrenome", reserva.hospede.sobrenome.c_str());
    elementoHospede->SetAttribute("cpf", reserva.hospede.cpf.c_str());
    elementoHospede->SetAttribute("cep", reserva.hospede.cep.c_str());
    elementoHospede->SetAttribute("logradouro", reserva.hospede.logradouro.c_str());
    elementoHospede->SetAttribute("num", reserva.hospede.num);
    elementoHospede->SetAttribute("bairro", reserva.hospede.bairro.c_str());
    elementoHospede->SetAttribute("cidade", reserva.hospede.cidade.c_str());
    elementoHospede->SetAttribute("complemento", reserva.hospede.complemento.c_str());
    elementoHospede->SetAttribute("email", reserva.hospede.email.c_str());
    elementoHospede->SetAttribute("telefone", reserva.hospede.telefone.c_str());
    elementoReserva->InsertEndChild(elementoHospede);

    tinyxml2::XMLElement* elementoQuarto = arquivo.NewElement("Quarto");
    elementoQuarto->SetAttribute("numeroQuarto", reserva.quarto.numeroQuarto);
    elementoQuarto->SetAttribute("tipo", reserva.quarto.tipo.c_str());
    elementoQuarto->SetAttribute("foto", reserva.quarto.foto.c_str());
    elementoQuarto->SetAttribute("descricao", reserva.quarto.descricao.c_str());
    elementoQuarto->SetAttribute("camasSolteiro", reserva.quarto.camasSolteiro);
    elementoQuarto->SetAttribute("camasCasal", reserva.quarto.camasCasal);
    elementoQuarto->SetAttribute("frigobar", reserva.quarto.frigobar ? "true" : "false");
    elementoQuarto->SetAttribute("arCondicionado", reserva.quarto.arCondicionado ? "true" : "false");
    elementoQuarto->SetAttribute("valor", reserva.quarto.valor);
    elementoQuarto->SetAttribute("disponivel", reserva.quarto.disponivel ? "true" : "false");
    elementoReserva->InsertEndChild(elementoQuarto);

    string dataEntradaStr = timeToString(reserva.dataEntrada);
    tinyxml2::XMLElement* dataEntrada = arquivo.NewElement("DataEntrada");
    dataEntrada->SetText(dataEntradaStr.c_str());
    elementoReserva->InsertEndChild(dataEntrada);
    
    string dataSaidaStr = timeToString(reserva.dataSaida);
    tinyxml2::XMLElement* dataSaida = arquivo.NewElement("DataSaida");
    dataSaida->SetText(dataSaidaStr.c_str());
    elementoReserva->InsertEndChild(dataSaida);

    tinyxml2::XMLElement* numHospedes = arquivo.NewElement("NumHospedes");
    numHospedes->SetText(reserva.numHospedes);
    elementoReserva->InsertEndChild(numHospedes);

    stringstream valorTotalStream;
    valorTotalStream << fixed << setprecision(2) << reserva.valorTotal;
    tinyxml2::XMLElement* valorTotal = arquivo.NewElement("ValorTotal");
    valorTotal->SetText(valorTotalStream.str().c_str());
    elementoReserva->InsertEndChild(valorTotal);

    tinyxml2::XMLElement* pago = arquivo.NewElement("Pago");
    pago->SetText("false");
    elementoReserva->InsertEndChild(pago);

    raiz->InsertEndChild(elementoReserva);

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != tinyxml2::XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
    }
}

// Faz o checkin de uma reserva
void checkin(string enderecoArquivo, int idReserva) {

    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possível carregar o arquivo." << endl;
        return;
    }

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.RootElement();

    // Busca a reserva pelo ID
    XMLElement* elementoReserva = raiz->FirstChildElement("Reserva");
    while (elementoReserva) {
        XMLElement* idElement = elementoReserva->FirstChildElement("Id");
        if (idElement && atoi(idElement->GetText()) == idReserva) {
            break;
        }
        elementoReserva = elementoReserva->NextSiblingElement("Reserva");
    }

    if (!elementoReserva) {
        cout << "Reserva não encontrada." << endl;
        return;
    }

    int dia, mes, ano;
    cout << "Digite a data de check-in (DD MM AAAA): ";
    cin >> dia >> mes >> ano;
    time_t dataCheckin = tratarData(dia, mes, ano);
    string dataCheckinStr = timeToString(dataCheckin);

    XMLElement* dataCheckinElement = elementoReserva->FirstChildElement("DataCheckin");
    if (!dataCheckinElement) {
        dataCheckinElement = arquivo.NewElement("DataCheckin");
        elementoReserva->InsertEndChild(dataCheckinElement);
    }
    dataCheckinElement->SetText(dataCheckinStr.c_str());

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possível salvar o arquivo." << endl;
    } else {
        cout << "Check-in realizado com sucesso!" << endl;
    }
}

// Faz o checkout de uma reserva
void checkout(string enderecoArquivo, int idReserva) {

    // Carrega o arquivo XML
    tinyxml2::XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
        return;
    }

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.RootElement();

    // Busca a reserva pelo ID
    XMLElement* elementoReserva = raiz->FirstChildElement("Reserva");
    while (elementoReserva) {
        XMLElement* idElement = elementoReserva->FirstChildElement("Id");
        if (idElement && atoi(idElement->GetText()) == idReserva) {
            break;
        }
        elementoReserva = elementoReserva->NextSiblingElement("Reserva");
    }

    if (!elementoReserva) {
        cout << "Reserva não encontrada." << endl;
        return;
    }

    int dia, mes, ano;
    cout << "Digite a data de check-out (DD MM AAAA): ";
    cin >> dia >> mes >> ano;
    time_t dataCheckout = tratarData(dia, mes, ano);
    string dataCheckoutStr = timeToString(dataCheckout);

    XMLElement* dataCheckoutElement = elementoReserva->FirstChildElement("DataCheckout");
    if (!dataCheckoutElement) {
        dataCheckoutElement = arquivo.NewElement("DataCheckout");
        elementoReserva->InsertEndChild(dataCheckoutElement);
    }
    dataCheckoutElement->SetText(dataCheckoutStr.c_str());

    XMLElement* dataCheckinElement = elementoReserva->FirstChildElement("DataCheckin");
    if (!dataCheckinElement) {
        cout << "Data de check-in não encontrada." << endl;
        return;
    }

    tm dataCheckinTm = {0};
    strptime(dataCheckinElement->GetText(), "%d/%m/%Y", &dataCheckinTm);
    time_t dataCheckin = mktime(&dataCheckinTm);

    int diasEstadia = calcularDiferencaDias(dataCheckin, dataCheckout);

    XMLElement* quartoElement = elementoReserva->FirstChildElement("Quarto");
    if (!quartoElement) {
        cout << "Dados do quarto não encontrados." << endl;
        return;
    }

    // Calcula o valor total da reserva
    double valorDiaria;
    quartoElement->QueryDoubleAttribute("valor", &valorDiaria);

    double valorTotal = (diasEstadia + 1) * valorDiaria;

    stringstream valorTotalStream;
    valorTotalStream << fixed << setprecision(2) << valorTotal;
    XMLElement* valorTotalElement = elementoReserva->FirstChildElement("ValorTotal");
    if (!valorTotalElement) {
        valorTotalElement = arquivo.NewElement("ValorTotal");
        elementoReserva->InsertEndChild(valorTotalElement);
    }
    valorTotalElement->SetText(valorTotalStream.str().c_str());

    // Marca a reserva como paga
    XMLElement* pagoElement = elementoReserva->FirstChildElement("Pago");
    if (!pagoElement) {
        pagoElement = arquivo.NewElement("Pago");
        elementoReserva->InsertEndChild(pagoElement);
    }
    pagoElement->SetText("true");

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
    }

    cout << "Valor total da reserva: R$ " << valorTotalStream.str() << endl;
}

// Verifica se dois períodos de tempo se cruzam ou se coincidem em algum ponto
bool dataCruzam(time_t start1, time_t end1, time_t start2, time_t end2) {
    return (start1 < end2 && start2 < end1);
}

// Verifica se o quarto estará disponivel na data
bool quartoDisponivel(Quarto quarto, time_t dataEntrada, time_t dataSaida, vector<Reserva> reservas) {
    for (const Reserva& reserva : reservas) {
        if (reserva.quarto.numeroQuarto == quarto.numeroQuarto &&
            dataCruzam(dataEntrada, dataSaida, reserva.dataEntrada, reserva.dataSaida)) {
            return false;
        }
    }
    return true;
}

// Encontra o quarto ideal de acordo com o numero de hospedes e data
Quarto acharQuartoIdeal(int numHospedes, time_t dataEntrada, time_t dataSaida, string quartosFile, string reservasFile) {
    vector<Quarto> quartos = recuperarQuartos(quartosFile);
    vector<Reserva> reservas = recuperarReservas(reservasFile);

    Quarto quartoIdeal;
    bool found = false;

    for (const Quarto& quarto : quartos) {
        if (quartoDisponivel(quarto, dataEntrada, dataSaida, reservas)) {
            int capacidade = quarto.camasSolteiro + (quarto.camasCasal * 2);
            if (capacidade >= numHospedes) {
                if (!found || quarto.valor < quartoIdeal.valor) {
                    quartoIdeal = quarto;
                    found = true;
                }
            }
        }
    }

    if (found) {
        return quartoIdeal;
    } else {
        throw runtime_error("Nenhum quarto disponível encontrado que atenda aos critérios.");
    }
}
