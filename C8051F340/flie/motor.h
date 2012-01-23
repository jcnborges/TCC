#ifndef MOTOR
#define MOTOR
#define TAM 10
enum Qual {Maior,Menor,Igual};

//typedef bool boolean ; /*enum boolean {false,true};*/

typedef int TVetor[TAM];


class TTipoPremissa
{
    private:


    public:
		 char *atributo;
	 float valor;
	 int tamvp;
	 TVetor vPremissas;
	
         TTipoPremissa();
	 TTipoPremissa(char *newAtributo, float newV);
	 void setValor(float newValor);
	 TVetor *getVPremissas();
	 int getValorPremissa(int premissa);
	 int getTamvp();
	 void inserePremissa (int premissa);

	 void Notificacao();
};

/* CLASSE TCONDICAO */
class TCondicao
{
    private:

    public:
	       float valor;
       float valorRecebido;
	
      TCondicao(float newV, float newVR);
      void setValorRecebido(float newVR);
      bool Maior(); //tipo (1)
      bool Menor(); //tipo (2)
      bool Igual(); //tipo (3)
	~TCondicao();
};
/* FIM DA CLASSE TCODIÇÃO*/

class TPremissa
{
    private:


    public:
		 char *atributo;
	 TCondicao condicao;
	 bool status;
	 Qual tipo;
	 TVetor vRegras;
	 int tamvr;
	
	 TPremissa();
	 TPremissa (Qual newTipo, char *newAtributo, float newV, float newVR, bool newStatus);
	 void setStatus (float valorRecebido);
	 bool getStatus();
//	 char* getAtributo();
	 Qual getTipo();
	 TVetor *getVRegras();
	 void InsereRegras(int regra);
	 int getValorRegra(int regra);
	 int getTamvr();
	 void Notifica();
};


class TConclusao
{
    private:

    public:
		char *mensagem;
	TVetor vRegras;
	bool status;
	
	TConclusao(char *newMensagem, bool newStatus);
	char * getMensagem();
	TVetor *getVRegras();
	void setStatus (bool newStatus);
	bool getStatus();
};

class TRegra
{
    protected:


    public:
		 char *nome;
	 TVetor vPremissas;
	 TVetor conclusao;
	 int tamvp, tamc;
	
         TRegra();
	 TRegra (char * newNome);
	 char * getNome();
	 TVetor *getPremissas();
	 TVetor *getConclusao();
	 int getTamvp();
	 int getTamc();
	 void InserePremissa(int Premissa);
	 void InsereConclusao(int newConclusao);
	 int getValorPremissa (int premissa);
	 int getValorConclusao();
	 void Notifica();
};

/*
Vetores de premissa, regras e conclusao.
O controle esta sendo realizado por indice. Dessa forma
um tipo de premissa pode startar as premissas 0,1 e 2 por exemplo
e estas startarem as regras 1,2,3 exemplo
Deve ser pensado melhor a respeito disso
*/

typedef TPremissa *TVP[10];
typedef TRegra *TR[10];
typedef TConclusao *TC[10];

#endif
