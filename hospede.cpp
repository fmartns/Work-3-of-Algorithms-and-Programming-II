#include "hospede.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace tinyxml2;
using namespace std;

// Encontra o próximo Id disponível para atribuir ao hospede
int proximoIDDisponivel(string enderecoArquivo, string tagID) {

    // Carrega o arquivo XML
    XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.RootElement();

    vector<int> ids;

    for (XMLElement* elemento = raiz->FirstChildElement(); elemento; elemento = elemento->NextSiblingElement()) {
        const char* dadoId = elemento->FirstChildElement(tagID.c_str())->GetText();
        if (dadoId) {
            int id = atoi(dadoId);
            ids.push_back(id);
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

// Recupera todas as informações do arquivo XML e as coloca num vetor
vector<Hospede> recuperarHospedes(string enderecoArquivo) {

    // Cria um vetor de hospedes
    vector<Hospede> hospedes;

    // Carrega o arquivo XML
    XMLDocument arquivo;
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());

    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
        return {};
    }

    // Define a raiz do arquivo XML
    XMLElement* raiz = arquivo.FirstChildElement("Hotel");

    for (XMLElement* elementoHospede = raiz->FirstChildElement("Hospede"); elementoHospede; elementoHospede = elementoHospede->NextSiblingElement("Hospede")) {
        
        Hospede hospede;
        hospede.id = elementoHospede->FirstChildElement("Id")->IntText();
        hospede.nome = elementoHospede->FirstChildElement("Nome")->GetText();
        hospede.sobrenome = elementoHospede->FirstChildElement("Sobrenome")->GetText();
        hospede.cpf = elementoHospede->FirstChildElement("Cpf")->GetText();
        hospede.cep = elementoHospede->FirstChildElement("Cep")->GetText();
        hospede.logradouro = elementoHospede->FirstChildElement("Logradouro")->GetText();
        hospede.num = elementoHospede->FirstChildElement("Numero")->IntText();
        hospede.bairro = elementoHospede->FirstChildElement("Bairro")->GetText();
        hospede.cidade = elementoHospede->FirstChildElement("Cidade")->GetText();
        hospede.complemento = elementoHospede->FirstChildElement("Complemento")->GetText();
        hospede.email = elementoHospede->FirstChildElement("Email")->GetText();
        hospede.telefone = elementoHospede->FirstChildElement("Telefone")->GetText();

        hospedes.push_back(hospede);
    }

    return hospedes;
}

// Imprime as informações dos hospedes
void imprimirHospede(Hospede hospede) {

    cout << "ID: " << hospede.id << endl;
    cout << "Nome: " << hospede.nome << endl;
    cout << "Sobrenome: " << hospede.sobrenome << endl;
    cout << "CPF: " << hospede.cpf << endl;
    cout << "CEP: " << hospede.cep << endl;
    cout << "Logradouro: " << hospede.logradouro << endl;
    cout << "Número: " << hospede.num << endl;
    cout << "Bairro: " << hospede.bairro << endl;
    cout << "Cidade: " << hospede.cidade << endl;
    cout << "Complemento: " << hospede.complemento << endl;
    cout << "E-mail: " << hospede.email << endl;
    cout << "Telefone: " << hospede.telefone << endl;

}

// Coleta as informações para registro de um novo hospede
Hospede novoHospede() {

    // Verifica repetições e impede que seja feito a criação de mais de um hospede com o mesmo Id.
    int id = proximoIDDisponivel("./hospedes.xml", "Id");

    cout << "Nome: ";
    string nome;
    cin.ignore();
    getline(cin, nome);

    cout << "Sobrenome: ";
    string sobrenome;
    getline(cin, sobrenome);

    cout << "CPF: ";
    string cpf;
    cin >> cpf;
    cin.ignore();

    cout << "CEP: ";
    string cep;
    cin >> cep;
    cin.ignore();

    cout << "Logradouro: ";
    string logradouro;
    getline(cin, logradouro);

    cout << "Número do Logradouro: ";
    int num;
    cin >> num;
    cin.ignore();
    
    cout << "Bairro: ";
    string bairro;
    getline(cin, bairro);

    cout << "Cidade: ";
    string cidade;
    getline(cin, cidade);

    cout << "Complemento: ";
    string complemento;
    getline(cin, complemento);

    cout << "E-mail: ";
    string email;
    getline(cin, email);

    cout << "Telefone: ";
    string telefone;
    getline(cin, telefone);

    Hospede hospede = {id, nome, sobrenome, cpf, cep, logradouro, num, bairro, cidade, complemento, email, telefone};

    return hospede;
}

// Faz o registro das informações cadastradas dentro do arquivo XML.
void registrarHospede(string enderecoArquivo, Hospede hospede) {

    // Carrega o arquivo XML
    XMLDocument arquivo;

    // Verifica se o arquivo carregou corretamente e inclui as tags base.
    XMLError retorno = arquivo.LoadFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        arquivo.InsertEndChild(arquivo.NewDeclaration());
        XMLElement* raiz = arquivo.NewElement("Hotel");
        arquivo.InsertEndChild(raiz);
    }

    XMLElement* raiz = arquivo.FirstChildElement("Hotel");
    if (!raiz) {
        raiz = arquivo.NewElement("Hotel");
        arquivo.InsertEndChild(raiz);
    }

    XMLElement* elementoHospede = arquivo.NewElement("Hospede");

    XMLElement* id = arquivo.NewElement("Id");
    id->SetText(hospede.id);
    elementoHospede->InsertEndChild(id);

    XMLElement* nome = arquivo.NewElement("Nome");
    nome->SetText(hospede.nome.c_str());
    elementoHospede->InsertEndChild(nome);
    
    XMLElement* sobrenome = arquivo.NewElement("Sobrenome");
    sobrenome->SetText(hospede.sobrenome.c_str());
    elementoHospede->InsertEndChild(sobrenome);
    
    XMLElement* cpf = arquivo.NewElement("Cpf");
    cpf->SetText(hospede.cpf.c_str());
    elementoHospede->InsertEndChild(cpf);
    
    XMLElement* cep = arquivo.NewElement("Cep");
    cep->SetText(hospede.cep.c_str());
    elementoHospede->InsertEndChild(cep);
    
    XMLElement* logradouro = arquivo.NewElement("Logradouro");
    logradouro->SetText(hospede.logradouro.c_str());
    elementoHospede->InsertEndChild(logradouro);

    XMLElement* num = arquivo.NewElement("Numero");
    num->SetText(hospede.num);
    elementoHospede->InsertEndChild(num);
    
    XMLElement* bairro = arquivo.NewElement("Bairro");
    bairro->SetText(hospede.bairro.c_str());
    elementoHospede->InsertEndChild(bairro);
    
    XMLElement* cidade = arquivo.NewElement("Cidade");
    cidade->SetText(hospede.cidade.c_str());
    elementoHospede->InsertEndChild(cidade);
    
    XMLElement* complemento = arquivo.NewElement("Complemento");
    complemento->SetText(hospede.complemento.c_str());
    elementoHospede->InsertEndChild(complemento);

    XMLElement* email = arquivo.NewElement("Email");
    email->SetText(hospede.email.c_str());
    elementoHospede->InsertEndChild(email);
    
    XMLElement* telefone = arquivo.NewElement("Telefone");
    telefone->SetText(hospede.telefone.c_str());
    elementoHospede->InsertEndChild(telefone);

    // Adiciona o novo elemento ao final do XML
    raiz->InsertEndChild(elementoHospede);

    // Salva o arquivo
    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
}

// Possibilita a edição das principais informações de cada quarto.
void editarHospede(string enderecoArquivo) {

    // Recebe o número do ID do hospede a ser editado
    int idHospede;
    cout << "Informe o ID do hóspede a ser editado: ";
    cin >> idHospede;

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
    XMLElement* hospedeXML = nullptr;

    // Busca a posição do hospede de acordo com o Id e verifica se o mesmo foi encontrado
    for (XMLElement* posicao = raiz->FirstChildElement("Hospede"); posicao; posicao = posicao->NextSiblingElement("Hospede")) {
        int id;
        posicao->FirstChildElement("Id")->QueryIntText(&id);
        if (id == idHospede) {
            hospedeXML = posicao;
            break;
        }
    }

    if (!hospedeXML) {
        cout << "Hóspede com ID " << idHospede << " não encontrado." << endl;
        return;
    }

    string nome, sobrenome, cpf, cep, logradouro, bairro, cidade, complemento, email, telefone;

    cin.ignore();

    cout << "Informe o novo nome do hóspede: ";
    getline(cin, nome);

    cout << "Informe o novo sobrenome do hóspede: ";
    getline(cin, sobrenome);

    cout << "Informe o novo CPF do hóspede: ";
    getline(cin, cpf);

    cout << "Informe o novo CEP do hóspede: ";
    getline(cin, cep);

    cout << "Informe o novo logradouro do hóspede: ";
    getline(cin, logradouro);

    cout << "Informe o novo bairro do hóspede: ";
    getline(cin, bairro);

    cout << "Informe a nova cidade do hóspede: ";
    getline(cin, cidade);

    cout << "Informe o novo complemento do hóspede: ";
    getline(cin, complemento);

    cout << "Informe o novo e-mail do hóspede: ";
    getline(cin, email);

    cout << "Informe o novo telefone do hóspede: ";
    getline(cin, telefone);

    hospedeXML->FirstChildElement("Nome")->SetText(nome.c_str());
    hospedeXML->FirstChildElement("Sobrenome")->SetText(sobrenome.c_str());
    hospedeXML->FirstChildElement("Cpf")->SetText(cpf.c_str());
    hospedeXML->FirstChildElement("Cep")->SetText(cep.c_str());
    hospedeXML->FirstChildElement("Logradouro")->SetText(logradouro.c_str());
    hospedeXML->FirstChildElement("Bairro")->SetText(bairro.c_str());
    hospedeXML->FirstChildElement("Cidade")->SetText(cidade.c_str());
    hospedeXML->FirstChildElement("Complemento")->SetText(complemento.c_str());
    hospedeXML->FirstChildElement("Email")->SetText(email.c_str());
    hospedeXML->FirstChildElement("Telefone")->SetText(telefone.c_str());

    retorno = arquivo.SaveFile(enderecoArquivo.c_str());
    if (retorno != XML_SUCCESS) {
        cout << "Erro! Não foi possivel carregar o arquivo." << endl;
    } else {
        cout << "Hóspede atualizado com sucesso!" << endl;
    }
}